#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "BoxSpreads.h"
#include "Structures.h"
#include "Spliner.h"


uint8_t compareOptions(OptionData * opt1, OptionData * opt2) {
	// printf("%lf, %lf, %d\n", opt1->STRIKE, opt2->STRIKE, (opt1->EXPIRE_UNIX == opt2->EXPIRE_UNIX));
	return (fabs(opt1->STRIKE - opt2->STRIKE) < 0.1) && (opt1->EXPIRE_UNIX == opt2->EXPIRE_UNIX);
}

uint64_t calcSecondsToExpiry(OptionData * opt) {
	return (opt->EXPIRE_UNIX - opt->QUOTE_UNIXTIME);
}

uint8_t hasCallBidAsk(OptionData * opt) {
	return opt->C_BID > 0.01 && opt->C_ASK > 0.01;
}

uint8_t hasPutBidAsk(OptionData * opt) {
	return opt->P_BID > 0.01 && opt->P_ASK > 0.01;
}


double getImpliedInterest(OptionData * opts) {
	uint64_t expireTime = opts[0].EXPIRE_UNIX;
	int optionCount = 0;
	double strikeMean = 0;
	double putMinusCallMean = 0;
	for(int i = 0; opts[i].EXPIRE_UNIX == expireTime; i++) {
		if(hasCallBidAsk(&opts[i]) && hasPutBidAsk(&opts[i])) {
			double mid = (opts[i].P_BID + opts[i].P_ASK) / 2.0 - (opts[i].C_BID + opts[i].C_ASK) / 2.0;

			optionCount++;
			strikeMean += opts[i].STRIKE;
			putMinusCallMean += mid;
		}
	}

	strikeMean /= (double) optionCount;
	putMinusCallMean /= (double) optionCount;
	// printf("(x, y): (%lf, %lf)\n", xMean, yMean);

	double numerator = 0;
	double denominator = 0;
	expireTime = opts[0].EXPIRE_UNIX;
	for(int i = 0; opts[i].EXPIRE_UNIX == expireTime; i++) {
		if(hasCallBidAsk(&opts[i]) && hasPutBidAsk(&opts[i])) {
			double mid = (opts[i].P_BID + opts[i].P_ASK) / 2.0 - (opts[i].C_BID + opts[i].C_ASK) / 2.0;

			numerator += (mid - putMinusCallMean) * (opts[i].STRIKE - strikeMean);
			denominator += (opts[i].STRIKE - strikeMean) * (opts[i].STRIKE - strikeMean);
		}
	}

	// double b = 0;
	double m = numerator / denominator;
	// printf("M: %lf\n", m);
	// double b = putMinusCallMean - strikeMean * m;

	// printf("(num, dem): (%lf, %lf)\n", numerator, denominator);
	// printf("S: %lf\n", opts[0].UNDERLYING_LAST);
	// printf("m: %lf\n", m);
	// printf("b: %lf\n", b);

	// double SSR = 0;
	// double TSS = 0;
	// for(int i = 0; opts[i].EXPIRE_UNIX == expireTime; i++) {
	// 	double mid = (opts[i].P_BID + opts[i].P_ASK) / 2.0 - (opts[i].C_BID + opts[i].C_ASK) / 2.0;
	// 	SSR += (mid - (opts[i].STRIKE * m + b)) * (mid - (opts[i].STRIKE * m + b));
	// 	TSS += (mid - putMinusCallMean) * (mid - putMinusCallMean);
	// }

	// printf("RSquared: %lf\n", 1.0 - SSR / TSS);
	// printf("DAYS: %lf, %lf\n", (double) opts[0].DTE, (double) opts[0].DTE / ((double) 365));
	// printf("TEST: %lf\n", -(log(0.93778) / ((double) 824 / ((double) 365))));

	// return pow((-b / m) / opts[0].UNDERLYING_LAST, opts[0].DTE / 252) - 1;

	double output;
	if(opts[0].DTE < 0.01) {
		output = 0;
	} else {
		output = -(log(m) / ((double) opts[0].DTE / ((double) 365)));
	}

	return output;
}

void writeHeaderBoxSpread(FILE * output) {
	fprintf(output, "Date, BoxRate1M, BoxRate2M, BoxRate3M, BoxRate6M, BoxRate1Y, BoxRate2Y, BoxRate3Y, BoxRate5Y, BoxRate10Y, BoxRate20Y\n");
}

void analyzeBoxSpread(FILE * output, OptionData * opts, int uniqueDateCount, int * dateIndices) {
	int nextDayIndex;
	int startingNextIndex;
	int currDayIndex;


	int optionIndex;

	double totalProfit = 0;
	double start = 1;
	for(int i = 0; i < uniqueDateCount; i++) {
		uint64_t expireTime = opts[dateIndices[i]].EXPIRE_UNIX;
		int dateCount = 1;
		for(int j = dateIndices[i]; j < dateIndices[i + 1]; j++) {
			if(opts[j].EXPIRE_UNIX != expireTime) {
				dateCount++;
				expireTime = opts[j].EXPIRE_UNIX;
			}
		}

		double * rates = (double *) malloc(dateCount * sizeof(double));
		double * DTE = (double *) malloc(dateCount * sizeof(double));
		dateCount = 0;
		for(int j = dateIndices[i]; j < dateIndices[i + 1]; j++) {
			if(opts[j].EXPIRE_UNIX != expireTime) {
				rates[dateCount] = getImpliedInterest(opts + j);
				DTE[dateCount] = opts[j].DTE;

				if(fabs(DTE[dateCount]) < 0.01) {
					rates[dateCount] = 0;
				}

				if(!(isnan(rates[dateCount]) || isinf(rates[dateCount]))) {
					dateCount++;
				}

				expireTime = opts[j].EXPIRE_UNIX;
			}
		}
		// printf("DATE COUNT: %d\n", dateCount);

		int hour = opts[dateIndices[i]].QUOTE_UNIXTIME % (60 * 60);
		int minute = opts[dateIndices[i]].QUOTE_UNIXTIME % 60;

		time_t rawtime = opts[dateIndices[i]].QUOTE_UNIXTIME;
	    struct tm ts;
	    char buf[80];

	    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
	    ts = *localtime(&rawtime);
	    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
	    fprintf(output, "%s", buf);

		fprintf(output, ", %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n",
			interpolate(DTE, rates, dateCount, 30),
			interpolate(DTE, rates, dateCount, 60),
			interpolate(DTE, rates, dateCount, 91),
			interpolate(DTE, rates, dateCount, 182),
			interpolate(DTE, rates, dateCount, 365),
			interpolate(DTE, rates, dateCount, 730),
			interpolate(DTE, rates, dateCount, 1065),
			interpolate(DTE, rates, dateCount, 1795),
			interpolate(DTE, rates, dateCount, 3650),
			interpolate(DTE, rates, dateCount, 7300)
		);

		free(rates);
	}
}
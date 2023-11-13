#include <stdio.h>

#include "LEAPS.h"
#include "Structures.h"

void writeHeaderLeaps(FILE * output) {
	fprintf(output, "Date, 0.0-0.1, Date, STRIKE, 0.1-0.2, Date, STRIKE, 0.2-0.3, Date, STRIKE, 0.3-0.4, Date, STRIKE, 0.4-0.5, Date, STRIKE, 0.5-0.6, Date, STRIKE, 0.6-0.7, Date, STRIKE, 0.7-0.8, Date, STRIKE, 0.8-0.9, Date, STRIKE, 0.9-1.0, Date, STRIKE,\n");
	// fprintf(output, "Date, Price\n");
}


int findOption(OptionData * opts, int * dateIndices, double delta, int index) {
	double minDTE = 300;
	for(int i = dateIndices[index + 1] - 1; i >= dateIndices[i]; i--) {
		if(opts[i].DTE > minDTE && opts[i].C_BID > 0 && opts[i].C_ASK > 0 && opts[i].C_DELTA >= delta && opts[i].C_DELTA <= delta + 0.1) {
			return i;
		}
	}

	return -1;
}

void analyzeLeaps(FILE * output, OptionData * opts, int uniqueDateCount, int * dateIndices) {
	for(int i = 0; i < uniqueDateCount; i++) {

		printf("CURR DATE: %d/%d/%d\n", opts[dateIndices[i]].QUOTE_DATE.year, opts[dateIndices[i]].QUOTE_DATE.month, opts[dateIndices[i]].QUOTE_DATE.day);
		fprintf(output, "%d/%d/%d, ", opts[dateIndices[i]].QUOTE_DATE.year, opts[dateIndices[i]].QUOTE_DATE.month, opts[dateIndices[i]].QUOTE_DATE.day);
		// fprintf(output, "%lf\n", opts[dateIndices[i]].UNDERLYING_LAST);
		for(double delta = 0.0; delta < 0.99; delta += 0.1) {
			int optionIndex = findOption(opts, dateIndices, delta, i);

			if(optionIndex != -1) {
				double price = (opts[optionIndex].C_BID + opts[optionIndex].C_ASK) / 2;
				fprintf(output, "%lf, ", price);
				fprintf(output, "%d/%d/%d, ", opts[optionIndex].EXPIRE_DATE.date.year, opts[optionIndex].EXPIRE_DATE.date.month, opts[optionIndex].EXPIRE_DATE.date.day);
				fprintf(output, "%lf, ", opts[optionIndex].STRIKE);
			} else {
				fprintf(output, ",,, ");
			}
		}
		fprintf(output, "\n");
	}
}
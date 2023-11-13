// #include <math.h>
// #include <stdio.h>

// #include "Structures.h"
// #include "Spliner.h"

// #define E_CONST ((double) 2.71828182845904)

// // Equations sourced from:
// // https://cdn.cboe.com/api/global/us_indices/governance/Cboe_Volatility_Index_Mathematics_Methodology.pdf

// // 100 * (sigma) ^ (1/2)
// double calcSigma(double stDevIn) {
// 	return 100 * sqrt(stDevIn);
// }

// double calcSingleTerm(
// 	double T,
// 	double delKOTM,
// 	double KOTM,
// 	double R,
// 	double mid,
// 	double Knaught) {

// 	double strikeMultiplier = delKOTM / (KOTM * KOTM);

// 	double riskFreeMultiplier = pow(E_CONST, R * T);

// 	double mainTerm = strikeMultiplier * riskFreeMultiplier * mid;

// 	return mainTerm;
// }

// double timeToExpiry(double secondsToExpiry) {
// 	return secondsToExpiry / (double) (60 * 60 * 24 * 365);
// }

// double calcOptionDataTimeToExpiry(OptionData * option, int count) {
// 	return timeToExpiry(option->EXPIRE_UNIX - option->QUOTE_UNIXTIME);
// }

// double calculateVixFromSelection(
// 	OptionData * options, 
// 	int optionCount, 
// 	double T, 
// 	double R, 
// 	double Knaught, 
// 	double F) {

// 	double total = 0;
// 	for(int i = 0; i < optionCount; i++) {
// 		if(options[i].STRIKE == Knaught) {
// 			// printf("Begining single-term calculation1...\n");
// 			double putVal = 0;
// 			double callVal = 0;
// 			double prevStrike;
// 			double nextStrike;
// 			for(int j = i-1; j >= 0; j--) {
// 				if(options[j].P_BID > 0) {
// 					prevStrike = options[j].STRIKE;
// 					// printf("Prev Strike Found...\n");
// 					break;
// 				}
// 			}

// 			for(int j = i+1; j < optionCount; j++) {
// 				if(options[j].C_BID > 0) {
// 					// printf("Next Strike Found...\n");
// 					nextStrike = options[j].STRIKE;
// 					break;
// 				}
// 			}

// 			// printf("Strike dist %.02lf %.02lf %.02lf\n", nextStrike, prevStrike, (nextStrike - prevStrike) / 2 );
// 			if(options[i].P_BID > 0) {
// 				putVal = calcSingleTerm(T,
// 								(nextStrike - prevStrike) / 2, 
// 								options[i].STRIKE, 
// 								R, 
// 								(options[i].P_BID + options[i].P_ASK) / 2,
// 								Knaught);
// 			}
// 			if(options[i].C_BID > 0) {
// 				callVal = calcSingleTerm(T,
// 							(nextStrike - prevStrike) / 2, 
// 							options[i].STRIKE, 
// 							R, 
// 							(options[i].C_BID + options[i].C_ASK) / 2,
// 							Knaught);
// 			}

// 			if(prevStrike > 0 && nextStrike > 0) {
// 				total += (putVal + callVal) / 2;
// 			}
// 			// printf("%.0lf Put/Call Average %.3lf %.0lf %.10lf\n", options[i].STRIKE, (options[i].C_BID + options[i].C_ASK) / 2, (options[i+1].STRIKE - options[i - 1].STRIKE) / 2, (putVal + callVal) / 2);

// 		} else if(options[i].STRIKE >= Knaught) {
// 			double prevStrike = 0;
// 			double nextStrike = 0;

// 			// printf("Begining single-term calculation2...\n");
// 			if(options[i].C_BID > 0) {
// 				for(int j = i-1; j >= 0; j--) {
// 					if(options[j].C_BID > 0) {
// 						// printf("Prev Strike Found...\n");
// 						prevStrike = options[j].STRIKE;
// 						break;
// 					}
// 				}

// 				for(int j = i+1; j < optionCount; j++) {
// 					if(options[j].C_BID > 0) {
// 						// printf("Next Strike Found...\n");
// 						nextStrike = options[j].STRIKE;
// 						break;
// 					}
// 				}

// 				// printf("Strike dist %.02lf %.02lf %.02lf\n", nextStrike, prevStrike, (nextStrike - prevStrike) / 2 );
// 				double callVal = calcSingleTerm(T,
// 								(nextStrike - prevStrike) / 2, 
// 								options[i].STRIKE, 
// 								R, 
// 								(options[i].C_BID + options[i].C_ASK) / 2,
// 								Knaught);

// 				if(prevStrike > 0 && nextStrike > 0) {
// 					total += callVal;
// 				}
// 				// printf("%.0lf Call %.3lf %.0lf %.10lf\n", options[i].STRIKE, (options[i].C_BID + options[i].C_ASK) / 2, (options[i+1].STRIKE - options[i - 1].STRIKE) / 2, callVal);
// 			}
// 		} else if(options[i].STRIKE <= Knaught) {
// 			double prevStrike = 0;
// 			double nextStrike = 0;
// 			// printf("Begining single-term calculation3...\n");
// 			if(options[i].P_BID > 0) {
// 				for(int j = i-1; j >= 0; j--) {
// 					if(options[j].P_BID > 0) {
// 						// printf("Prev Strike Found...\n");
// 						prevStrike = options[j].STRIKE;
// 						break;
// 					}
// 				}

// 				for(int j = i+1; j < optionCount; j++) {
// 					if(options[j].P_BID > 0) {
// 						// printf("Next Strike Found...\n");
// 						nextStrike = options[j].STRIKE;
// 						break;
// 					}
// 				}

// 				// printf("Strike dist %.02lf %.02lf %.02lf\n", nextStrike, prevStrike, (nextStrike - prevStrike) / 2 );
// 				double putVal = calcSingleTerm(T,
// 								(nextStrike - prevStrike) / 2, 
// 								options[i].STRIKE, 
// 								R, 
// 								(options[i].P_BID + options[i].P_ASK) / 2,
// 								Knaught);
// 				// printf("%.0lf Put %.3lf %.0lf %.10lf\n", options[i].STRIKE, (options[i].P_BID + options[i].P_ASK) / 2, (options[i+1].STRIKE - options[i - 1].STRIKE) / 2, putVal);
// 				if(prevStrike > 0 && nextStrike > 0) {
// 					total += putVal;
// 				}
// 			}
// 		}
// 	}
// 	double oneOverT = 1.0 / T;
// 	double FOverKnaught = F / Knaught - 1;
// 	double rightTerm = oneOverT * (FOverKnaught * FOverKnaught);

// 	double twoOverT = 2.0 / T;

// 	return twoOverT * total - rightTerm;
// }

// double interpolateVix(OptionData * optionNear, OptionData * optionNext, double VIXNear, double VIXNext) {
// 	double MT1 = (double) (optionNear[0].EXPIRE_UNIX - optionNear[0].QUOTE_UNIXTIME) / (double) 60.0;
// 	double MT2 = (double) (optionNext[0].EXPIRE_UNIX - optionNext[0].QUOTE_UNIXTIME) / (double) 60.0;


// 	double MCM = 30 * (60 * 24);
// 	double M365 = 365 * (60 * 24);
// 	double T1 = MT1 / M365;
// 	double T2 = MT2 / M365;
// 	printf("Near Days: %lf, Next Days: %lf, MCM: %f M365: %lf, T1: %lf, T2: %lf\n", MT1 / (60.0 * 24.0), MT2 / (60.0 * 24.0), MCM / (60.0 * 24.0), M365 / (60.0 * 24.0), T1, T2);
// 	return calcSigma(
// 			T1 * VIXNear * (MT2 - MCM) / (MT2 - MT1) * (M365 / MCM) +
// 			(T2 * VIXNext * (MCM - MT1) / (MT2 - MT1)) * (M365 / MCM));
// }

// double calcBondRate(double timeToExpiry, BondCurve * curve) {
// 	return interpolateBondCurve(curve, timeToExpiry * 365) / 100.0;
// }

// double findF(OptionData * options, int optionCount, BondCurve * curve) {
// 	double dist = 1000000000;
// 	double currStrike = 0;
// 	int index = -1;
// 	// printf("OptionCount: %d\n", optionCount); fflush(stdout);
// 	for(int i = 0; i < optionCount; i++) {
// 		// printf("%d: %lf %lf %lf\n", i, fabs(options[i].C_BID - options[i].P_BID), options[i].C_BID, options[i].P_BID); fflush(stdout);
// 		if(fabs(options[i].C_BID - options[i].P_BID) <= fabs(dist) || 
// 			(
// 				fabs(options[i].C_BID - options[i].P_BID) == dist &&
// 				options[i].STRIKE < currStrike
// 			) || (
// 				index == -1 && 
// 				options[i].C_BID > 0 &&
// 				options[i].C_ASK > 0 &&
// 				options[i].P_BID > 0 &&
// 				options[i].P_ASK > 0 &&
// 				options[i].C_BID < options[i].C_ASK &&
// 				options[i].P_BID < options[i].P_ASK)) {

// 			dist = options[i].P_BID - options[i].C_BID;
// 			currStrike = options[i].STRIKE;
// 			index = i;
// 		}
// 	}

// 	// printf("Exiting loop...\n"); fflush(stdout);
// 	// printf("CurrStrike: %lf, BondRate: %lf, TTM: %lf, Dist: %lf\n", currStrike, calcBondRate(calcOptionDataTimeToExpiry(options), curve), calcOptionDataTimeToExpiry(options), dist);
// 	// printf("EPOW: %lf\n", pow(E_CONST,
// 	// 		calcBondRate(calcOptionDataTimeToExpiry(options), curve) *
// 	// 		calcOptionDataTimeToExpiry(options)));
// 	return currStrike + pow(E_CONST,
// 			calcBondRate(calcOptionDataTimeToExpiry(options, optionCount), curve) *
// 			calcOptionDataTimeToExpiry(options, optionCount)) *
// 			(-dist);
// }

// double findKnaught(OptionData * options, int optionCount, double F) {
// 	double highestStrike = -1;

// 	for(int i = 0; i < optionCount; i++) {
// 		// // printf("Testing 1 : %d, 2 : %d, 3 : %d, 4 : %d, 5 : %d, 6 : %d, 7 : %d, 8 : %d\n",
// 		// 	options[i].STRIKE > highestStrike,
// 		// 	options[i].STRIKE <= F,
// 		// 	options[i].C_BID > 0,
// 		// 	options[i].C_ASK > 0,
// 		// 	options[i].P_BID > 0,
// 		// 	options[i].P_ASK > 0,
// 		// 	options[i].C_BID < options[i].C_ASK,
// 		// 	options[i].P_BID < options[i].P_ASK);
// 		if(options[i].STRIKE > highestStrike &&
// 			options[i].STRIKE <= F &&
// 			options[i].C_BID > 0 &&
// 			options[i].C_ASK > 0 &&
// 			options[i].P_BID > 0 &&
// 			options[i].P_ASK > 0 &&
// 			options[i].C_BID < options[i].C_ASK &&
// 			options[i].P_BID < options[i].P_ASK) {

// 			highestStrike = options[i].STRIKE;
// 		}
// 	}

// 	return highestStrike;
// }


// // These might be wrong.
// void findNearTerm(OptionData * options, 
// 		int count,
// 		OptionData ** output,
// 		int * outputCount,
// 		double dayMean) {

// 	// printf("DAYMIN: %lf\n", dayMin); fflush(stdout);
// 	int prevDateIndex = 0;
// 	uint64_t EXPIRE_TIME = options[0].EXPIRE_UNIX;
// 	int i;
// 	double timeToExpiry = dayMean / 365;

// 	// printf("count: %d\n", count);
// 	for(i = 0; i < count; i++) {
// 		if(options[i].EXPIRE_UNIX != EXPIRE_TIME) {
// 			if(calcOptionDataTimeToExpiry(&options[i], 1) < timeToExpiry) {
// 				// printf("SETTING PREV DATE\n");
// 				prevDateIndex = i;
// 				EXPIRE_TIME = options[i].EXPIRE_UNIX;
// 			}

// 		}
// 		// printf("HERE IS DTE: %lf BOOL: %d\n", options[i].DTE, calcOptionDataTimeToExpiry(&options[i], 1) > timeToExpiry);
// 		// printf("TESTING: %d/%d/%d\n", options[i].EXPIRE_DATE.year, options[i].EXPIRE_DATE.month, options[i].EXPIRE_DATE.day);
// 		if(calcOptionDataTimeToExpiry(&options[i], 1) > timeToExpiry && calcOptionDataTimeToExpiry(&options[i], 1) > 0) {
// 			// printf("BREAKING... %d, %lf, %lf\n", i, calcOptionDataTimeToExpiry(&options[i]), timeToExpiry); fflush(stdout);
// 			// printf("BREAKING\n");
// 			break;
// 		}
// 		if(!(calcOptionDataTimeToExpiry(&options[i], 1) > 0)) {
// 			// printf("DOING DUMB...\n");
// 			prevDateIndex = i;
// 			EXPIRE_TIME = options[i].EXPIRE_UNIX;
// 		}
// 	}

// 	// printf("PrevDate: %d, i: %d\n", prevDateIndex, i);
// 	for( ; EXPIRE_TIME == options[i].EXPIRE_UNIX; i++) {}

// 	*output = options + prevDateIndex;
// 	*outputCount = i - prevDateIndex;
// }

// void findNextTerm(OptionData * options, 
// 		OptionData * nearTerm,
// 		int count,
// 		OptionData ** output,
// 		int * outputCount,
// 		double dayMean) {

// 	// printf("DAYMIN: %lf\n", dayMin); fflush(stdout);
// 	int prevDateIndex = 0;
// 	double prevDTE;
// 	int i;
// 	double timeToExpiry = dayMean / 365;
// 	// printf("count: %d\n", count);
// 	for(i = 0; i < count; i++) {
// 		// printf("BREAKING... %d, %lf, %lf, %d/%d/%d\n", i, calcOptionDataTimeToExpiry(&options[i], 1), timeToExpiry, options[i].EXPIRE_DATE.year, options[i].EXPIRE_DATE.month, options[i].EXPIRE_DATE.day); fflush(stdout);
// 		if(calcOptionDataTimeToExpiry(&options[i], 1) > timeToExpiry && nearTerm->EXPIRE_UNIX != options[i].EXPIRE_UNIX) {
// 			prevDateIndex = i;
// 			prevDTE = calcOptionDataTimeToExpiry(&options[i], 1);
// 			break;
// 		}
// 	}

// 	for(i = prevDateIndex; i < count; i++) {
// 		if(!(fabs(prevDTE - calcOptionDataTimeToExpiry(&options[i], 1)) < 0.00001)) {
// 			break;
// 		}
// 	}

// 	*output = options + prevDateIndex;
// 	*outputCount = i - prevDateIndex;
// }

// void refineTerm(OptionData ** options, int * termCount, double F) {

// 	int optionIndex;
// 	for(optionIndex = 0; optionIndex < *termCount && (*options)[optionIndex].STRIKE < F; optionIndex++) {
// 	// 	printf("Strike: %lf\n", (*options)[optionIndex].STRIKE);
// 	}

// 	// printf("RUNNNNING REFINE TERM %d %d. %lf\n", optionIndex, *termCount, F);
// 	int maxIndex;
// 	char prevZero = 0;
// 	for(maxIndex = optionIndex; maxIndex < *termCount; maxIndex++) {
// 		// printf("Testing %d %lf: ", maxIndex, (*options)[maxIndex].STRIKE);
// 		if((*options)[maxIndex].C_BID < 0.01) {
// 			// printf("BAD STRIKE: %lf\n", (*options)[maxIndex].STRIKE);
// 			if(prevZero) {
// 				// printf("\tBreaking\n");
// 				maxIndex -= 1;
// 				break;
// 			} else {
// 				// printf("\tSetting prev\n");
// 				prevZero = 1;
// 			}
// 		} else {
// 			// printf("STRIKE: %lf\n", (*options)[maxIndex].STRIKE);
// 			prevZero = 0;
// 		}
// 	}

// 	// printf("\n\n\n\n\n\n");
// 	int minIndex;
// 	prevZero = 0;
// 	for(minIndex = optionIndex; minIndex >= 0; minIndex--) {
// 		// printf("Testing %d %lf: ", minIndex, (*options)[minIndex].STRIKE);
// 		if((*options)[minIndex].P_BID < 0.01) {
// 			// printf("BAD STRIKE: %lf\n", (*options)[minIndex].STRIKE);
// 			if(prevZero) {
// 				// printf("\tBreaking\n");
// 				minIndex += 1;
// 				break;
// 			} else {
// 				// printf("\tSetting prev\n");
// 				prevZero = 1;
// 			}
// 		} else {
// 			// printf("STRIKE: %lf\n", (*options)[minIndex].STRIKE);
// 			prevZero = 0;
// 		}
// 	}

// 	// printf("Min: %d, Max: %d\n", minIndex, maxIndex);
// 	*options = ((*options) + minIndex);
// 	*termCount = (maxIndex - minIndex) + 1;
// 	// printf("Outputting...\n"); fflush(stdout);
// 	// printf("CURRENT\n");
// 	// for(int i = 0; i < *termCount; i++) {
// 	// 	printf("U: %.02lf, $%.02lf: %.02lf %.02lf\n", (*options)[i].UNDERLYING_LAST, (*options)[i].STRIKE, (*options)[i].P_BID, (*options)[i].C_BID);
// 	// }
// }

// double calcVix(OptionData * option, int * dateIndices, int index, double dayMean, BondCurve * curve) {
// 	// printf("option: %p, dI: %p, i: %d, DAYMIN: %lf, DAYMAX: %lf\n", option, dateIndices, index, dayMin, dayMax); fflush(stdout);

// 	OptionData * nearTerm;
// 	OptionData * nextTerm;
// 	int nearTermCount;
// 	int nextTermCount;

// 	double prevDTE = 0;
// 	for(int i = dateIndices[index]; i < dateIndices[index + 1]; i++) {
// 		if(prevDTE != option[i].DTE) {
// 			prevDTE = option[i].DTE;
// 			// printf("DTE: %lf\n", prevDTE);
// 		}
// 	}

// 	// printf("Finding near term... DAYMIN: %lf\n", dayMin); fflush(stdout);
// 	findNearTerm(option + dateIndices[index], 
// 		dateIndices[index + 1] - dateIndices[index],
// 		&nearTerm,
// 		&nearTermCount,
// 		dayMean);

// 	// for(int i = 1; i < nearTermCount; i++) {
// 	// 	printf("$%lf, %lf\n", nearTerm[i].STRIKE, nearTerm[i].DTE);
// 	// }

// 	// printf("Date index 1: %d, Date index 2: %d\n", dateIndices[index], dateIndices[index + 1]);

// 	// printf("All:\n");
// 	// for(int i = dateIndices[index]; i < dateIndices[index + 1]; i++) {
// 	// 	printf("%d/%d/%d: ", option[i].EXPIRE_DATE.year, option[i].EXPIRE_DATE.month, option[i].EXPIRE_DATE.day);
// 	// 	printf("$%lf, ", option[i].STRIKE);
// 	// 	printf("CBID: %lf, CASK: %lf, PBID: %lf, PASK: %lf\n", option[i].C_BID, option[i].C_ASK, option[i].P_BID, option[i].P_ASK);
// 	// }

// 	// printf("Finding next term... With TTE: %lf, %d/%d/%d\n", calcOptionDataTimeToExpiry(nearTerm, 1) * 365, nearTerm->EXPIRE_DATE.year, nearTerm->EXPIRE_DATE.month, nearTerm->EXPIRE_DATE.day); fflush(stdout);
// 	findNextTerm(option + dateIndices[index], 
// 		nearTerm,
// 		dateIndices[index + 1] - dateIndices[index] + 1,
// 		&nextTerm,
// 		&nextTermCount,
// 		calcOptionDataTimeToExpiry(nearTerm, 1) * 365);

// 	// printf("Finding TTE...\n"); fflush(stdout);
// 	// printf("Near Term:\n");
// 	// for(int i = 0; i < nearTermCount; i++) {
// 	// 	printf("%d/%d/%d: ", nearTerm[i].EXPIRE_DATE.year, nearTerm[i].EXPIRE_DATE.month, nearTerm[i].EXPIRE_DATE.day);
// 	// 	printf("%llu %llu, ", nearTerm[i].QUOTE_UNIXTIME, nearTerm[i].EXPIRE_UNIX);
// 	// 	printf("$%lf, ", nearTerm[i].STRIKE);
// 	// 	printf("CBID: %lf, CASK: %lf, PBID: %lf, PASK: %lf\n", nearTerm[i].C_BID, nearTerm[i].C_ASK, nearTerm[i].P_BID, nearTerm[i].P_ASK);
// 	// }

// 	double TNear = calcOptionDataTimeToExpiry(nearTerm, nearTermCount);

// 	// Requires bond spline to be calculated first
// 	// printf("Finding F...\n"); fflush(stdout);
// 	double FNear = findF(nearTerm, nearTermCount, curve);
// 	// printf("FNear: %lf\n", FNear);

// 	// printf("Finding Knaught...\n"); fflush(stdout);
// 	double KnaughtNear = findKnaught(nearTerm, nearTermCount, FNear);
// 	// printf("KnaughtNear: %lf\n", KnaughtNear);
	
// 	// printf("Finding refineTerm...\n"); fflush(stdout);
// 	refineTerm(&nearTerm, &nearTermCount, FNear);

// 	// printf("Near Term:\n");
// 	// for(int i = 0; i < nearTermCount; i++) {
// 	// 	printf("%d/%d/%d: ", nearTerm[i].EXPIRE_DATE.year, nearTerm[i].EXPIRE_DATE.month, nearTerm[i].EXPIRE_DATE.day);
// 	// 	printf("$%lf, ", nearTerm[i].STRIKE);
// 	// 	printf("CBID: %lf, CASK: %lf, PBID: %lf, PASK: %lf\n", nearTerm[i].C_BID, nearTerm[i].C_ASK, nearTerm[i].P_BID, nearTerm[i].P_ASK);
// 	// }
// 	// printf("VIXNear debug: TNear: %lf, BondRate: %lf, Knaught: %lf, FNear: %lf\n", TNear, calcBondRate(TNear, curve), KnaughtNear, FNear);

// 	// printf("Finding VIXNear...\n"); fflush(stdout);
// 	double VIXNear = calculateVixFromSelection(nearTerm,
// 		nearTermCount,
// 		TNear,
// 		calcBondRate(TNear, curve),
// 		KnaughtNear,
// 		FNear);




// 	// printf("Finding TTE next...\n"); fflush(stdout);
// 	double TNext = calcOptionDataTimeToExpiry(nextTerm, nextTermCount);

// 	// Requires bond spline to be calculated first
// 	// printf("Finding F next...\n"); fflush(stdout);
// 	double FNext = findF(nextTerm, nextTermCount, curve);
// 	// printf("FNext: %lf\n", FNext);

// 	// printf("Finding Knaught next...\n"); fflush(stdout);
// 	double KnaughtNext = findKnaught(nextTerm, nextTermCount, FNext);
// 	// printf("KnaughtNext: %lf\n", KnaughtNext);

// 	// printf("Finding Knaught next...\n"); fflush(stdout);
// 	refineTerm(&nextTerm, &nextTermCount, FNext);

// 	// printf("Next Term:\n");
// 	// for(int i = 0; i < nextTermCount; i++) {
// 	// 	printf("%d/%d/%d: ", nextTerm[i].EXPIRE_DATE.year, nextTerm[i].EXPIRE_DATE.month, nextTerm[i].EXPIRE_DATE.day);
// 	// 	printf("$%lf, ", nextTerm[i].STRIKE);
// 	// 	printf("CBID: %lf, CASK: %lf, PBID: %lf, PASK: %lf\n", nextTerm[i].C_BID, nextTerm[i].C_ASK, nextTerm[i].P_BID, nextTerm[i].P_ASK);
// 	// }

// 	// printf("Finding VIX next...\n"); fflush(stdout);
// 	double VIXNext = calculateVixFromSelection(nextTerm,
// 		nextTermCount,
// 		TNext,
// 		calcBondRate(TNext, curve),
// 		KnaughtNext,
// 		FNext);

// 	printf("VIXNear: %lf\n", 100 * sqrt(VIXNear));
// 	printf("VIXNext: %lf\n", 100 * sqrt(VIXNext));
// 	printf("NearDist: %lf, NextDist: %lf\n", nearTerm->DTE, nextTerm->DTE);
// 	printf("QUOTEDATE: %d/%d/%d\n", option[dateIndices[index]].QUOTE_DATE.year, option[dateIndices[index]].QUOTE_DATE.month, option[dateIndices[index]].QUOTE_DATE.day);
// 	printf("VIX: %lf\n\n\n", interpolateVix(nearTerm, nextTerm, VIXNear, VIXNext));

// 	if(VIXNear < 0.01) {
// 		printf("Returning VIXNext\n");
// 		return VIXNext;
// 	} else if(VIXNext < 0.01) {
// 		return VIXNear;
// 	}
// 	return interpolateVix(nearTerm, nextTerm, VIXNear, VIXNext);
// }
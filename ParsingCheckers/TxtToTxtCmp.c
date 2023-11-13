#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../Structures.h"
#include "../ParsingFunctions.h"

int readLongNumRaw(char ** iterator, void * output) {
	uint64_t * outputPtr = (uint64_t *) output;
	*outputPtr = 0;

	if(!(**iterator >= '0' && **iterator <= '9')) {return 0;}

	while(**iterator != ',') {
		*outputPtr *= 10;
		*outputPtr += **iterator - '0';
		(*iterator)++;
	}
	(*iterator)++;
	(*iterator) += **iterator == ' ';

	return 1;
}

// Repeated whiles but I don't want to fix it.
int readDateAndTimeRaw(char ** iterator, void * output) {
	DateAndTime * outputPtr = (DateAndTime *) output;
	Date * date = &outputPtr->date;
	Time * time = &outputPtr->time;
	date->year = 0;

	if(!(**iterator >= '0' && **iterator <= '9')) {return 0;}
	while(**iterator != '-') {
		date->year *= 10;
		date->year += **iterator - '0';
		(*iterator)++;
	}
	(*iterator)++;

	date->month = 0;
	if(!(**iterator >= '0' && **iterator <= '9')) {return -1;}
	while(**iterator != '-') {
		date->month *= 10;
		date->month += **iterator - '0';
		(*iterator)++;
	}
	(*iterator)++;

	date->day = 0;
	if(!(**iterator >= '0' && **iterator <= '9')) {return -2;}
	while(**iterator != ' ') {
		date->day *= 10;
		date->day += **iterator - '0';
		(*iterator)++;
	}
	(*iterator)++;

	uint16_t tempMinutes = 0;

	tempMinutes = 0;
	if(!(**iterator >= '0' && **iterator <= '9')) {return -3;}
	while(**iterator != ':') {
		tempMinutes *= 10;
		tempMinutes += **iterator - '0';
		(*iterator)++;
	}
	(*iterator)++;

	tempMinutes *= 60;
	if(!(**iterator >= '0' && **iterator <= '9')) { return -4;}
	while(**iterator != ',') {
		time->minutes *= 10;
		time->minutes += **iterator - '0';
		(*iterator)++;
	}
	(*iterator)++;
	(*iterator) += **iterator == ' ';

	time->minutes += tempMinutes;

	return 1;
}

int readDateRaw(char ** iterator, void * output) {
	Date * outputPtr = (Date *) output;

	outputPtr->year = 0;
	if(!(**iterator >= '0' && **iterator <= '9')) {return 0;}
	while(**iterator != '-') {
		outputPtr->year *= 10;
		outputPtr->year += **iterator - '0';
		(*iterator)++;
	}
	(*iterator)++;

	outputPtr->month = 0;

	if(!(**iterator >= '0' && **iterator <= '9')) {return -1;}
	while(**iterator != '-') {
		outputPtr->month *= 10;
		outputPtr->month += **iterator - '0';
		(*iterator)++;
	}
	(*iterator)++;

	outputPtr->day = 0;
	if(!(**iterator >= '0' && **iterator <= '9')) {return -2;}
	while(**iterator != ',') {
		outputPtr->day *= 10;
		outputPtr->day += **iterator - '0';
		(*iterator)++;
	}
	(*iterator)++;
	(*iterator) += **iterator == ' ';

	return 1;
}

int readDoubleRaw(char ** iterator, void * output) {
	double * outputPtr = (double *) output;
	*outputPtr = 0;

	double negative = 1;

	if(**iterator == '-') {
		negative = -1;
		(*iterator)++;
	}

	if(!(**iterator >= '0' && **iterator <= '9')) { return 0;}
	while(**iterator != '.') {
		*outputPtr *= 10;
		*outputPtr += (double) ((**iterator) - '0');
		(*iterator)++;
	}
	(*iterator)++;

	double divider = 10;
	if(!(**iterator >= '0' && **iterator <= '9')) { return -1;}
	while(**iterator >= '0' && **iterator <= '9') {
		*outputPtr += (double) ((**iterator) - '0') / divider;
		divider *= 10;
		(*iterator)++;
	}
	(*iterator)++;
	(*iterator) += **iterator == ' ';

	*outputPtr *= negative;

	return 1;
}

int readSizeRaw(char ** iterator, void * output) {
	BidAskSize * outputPtr = (BidAskSize *) output;
	outputPtr->first = 0;
	outputPtr->second = 0;

	// printf("Reading first value...\n"); fflush(stdout);
	if(!(**iterator >= '0' && **iterator <= '9')) { return 0;}
	while(**iterator != ' ') {
		outputPtr->first *= 10;
		outputPtr->first += **iterator - '0';
		(*iterator)++;
	}

	(*iterator)++;
	if(!(**iterator == 'x')) { return -1;}
	(*iterator)++;
	if(!(**iterator == ' ')) { return -2;}
	(*iterator)++;

	// printf("Reading second value...\n"); fflush(stdout);
	if(!(**iterator >= '0' && **iterator <= '9')) { return -3;}
	while(**iterator != ',') {
		outputPtr->second *= 10;
		outputPtr->second += **iterator - '0';
		(*iterator)++;
	}
	(*iterator)++;
	(*iterator) += **iterator == ' ';

	return 1;
}

void printLongNumRaw(void * output) {
	uint64_t * print = (uint64_t *) output;
	printf("%llu\n", *print);
}

void printDateAndTimeRaw(void * output) {
	DateAndTime * outputPtr = (DateAndTime *) output;
	Date * date = &outputPtr->date;
	Time * time = &outputPtr->time;
	printf("%d-%d-%d %d\n", date->year, date->month, date->day, time->minutes);
}

void printDateRaw(void * output) {
	Date * date = (Date *) output;
	printf("%d-%d-%d\n", date->year, date->month, date->day);
}

void printDoubleRaw(void * output) {
	double * print = (double *) output;
	printf("%lf\n", *print);
}

void printSizeRaw(void * output) {
	BidAskSize * print = (BidAskSize *) output;
	printf("%d x %d\n", print->first, print->second);
}

int cmpLongNum(void * output, void * output2) {
	uint64_t * cmp1 = (uint64_t *) output;
	uint64_t * cmp2 = (uint64_t *) output2;
	return cmp1 == cmp2;
}

int cmpDateAndTime(void * output, void * output2) {
	DateAndTime * outputPtr1 = (DateAndTime *) output;
	Date * date1 = &outputPtr1->date;
	Time * time1 = &outputPtr1->time;

	DateAndTime * outputPtr2 = (DateAndTime *) output;
	Date * date2 = &outputPtr2->date;
	Time * time2 = &outputPtr2->time;
	
	return date1->year == date2->year &&
			date1->month == date2->month &&
			date1->day == date2->day &&
			time1->minutes == time2->minutes;
}

int cmpDate(void * output, void * output2) {
	Date * date1 = (Date *) output;
	Date * date2 = (Date *) output2;
	return date1->year == date2->year &&
			date1->month == date2->month &&
			date1->day == date2->day;
}

int cmpDouble(void * output, void * output2) {
	double * cmp1 = (double *) output;
	double * cmp2 = (double *) output2;
	return fabs(*cmp1 - *cmp2) < 0.01;
}

int cmpSize(void * output, void * output2) {
	BidAskSize * size1 = (BidAskSize *) output;
	BidAskSize * size2 = (BidAskSize *) output;
	return size1->first == size2->first &&
			size1->second == size2->second;
}



typedef int (*CmpFunction)(void *, void *);
CmpFunction cmpFunctionList[FUNCTION_READ_COUNT] = 
	{cmpLongNum, cmpDateAndTime, cmpDate, cmpDouble, cmpSize};

typedef int (*ReadFunctionRaw)(char **, void *);

ReadFunctionRaw readFunctionRawList[FUNCTION_READ_COUNT] = 
	{readLongNumRaw, readDateAndTimeRaw, readDateRaw, readDoubleRaw, readSizeRaw};

typedef void (*PrintFunction)(void *);

PrintFunction printFunctionList[FUNCTION_READ_COUNT] = 
	{printLongNumRaw, printDateAndTimeRaw, printDateRaw, printDoubleRaw, printSizeRaw};

int main() {
	// FILE * file = fopen()

	FILE * fp;
	FILE * fp2;
	uint64_t lSize;
	uint64_t lSize2;
	char *buffer;
	char *buffer2;

	printf("Opening input file...\n"); fflush(stdout);
	fp = fopen("OptionsData.txt" , "rb");
	if(!fp) {
		printf("Error opening input file...\n");
		return 1;
	}

	fseek(fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	/* allocate memory for entire content */
	printf("Allocating memory...\n"); fflush(stdout);
	buffer = calloc(sizeof(char), lSize+1);
	if(!buffer) {
		fclose(fp);
		printf("Memory allocation failed...\n");
		return 1;
	}

	/* copy the file into the buffer */
	printf("Reading from disk to memory...\n"); fflush(stdout);
	if(1 != fread(buffer, lSize, 1, fp)) {
		fclose(fp);
		free(buffer);
		printf("File read failed...\n");
		return 1;
	}


	printf("Opening input file2...\n"); fflush(stdout);
	fp2 = fopen("NewOptionsData.txt" , "rb");
	if(!fp2) {
		printf("Error opening input file...\n");
		return 1;
	}

	fseek(fp2, 0L, SEEK_END);
	lSize2 = ftell(fp2);
	rewind(fp2);

	/* allocate memory for entire content */
	printf("Allocating memory2...\n"); fflush(stdout);
	buffer2 = calloc(sizeof(char), lSize2+1);
	if(!buffer2) {
		fclose(fp2);
		printf("Memory allocation failed...\n");
		return 1;
	}

	/* copy the file into the buffer */
	printf("Reading from disk to memory...\n"); fflush(stdout);
	if(1 != fread(buffer2, lSize2, 1, fp2)) {
		fclose(fp2);
		free(buffer2);
		printf("File read failed...\n");
		return 1;
	}

	char * iterator = buffer;
	char * endIterator = buffer + lSize;

	char * iterator2 = buffer2;
	char * endIterator2 = buffer2 + lSize2;

	OptionData option;
	void * optionPtrList[] = {
		&(option.QUOTE_UNIXTIME),
		&(option.QUOTE_READTIME),
		&(option.QUOTE_DATE),
		&(option.QUOTE_TIME_HOURS),
		&(option.UNDERLYING_LAST),
		&(option.EXPIRE_DATE),
		&(option.EXPIRE_UNIX),
		&(option.DTE),
		&(option.C_DELTA),
		&(option.C_GAMMA),
		&(option.C_VEGA),
		&(option.C_THETA),
		&(option.C_RHO),
		&(option.C_IV),
		&(option.C_VOLUME),
		&(option.C_LAST),
		&(option.C_SIZE),
		&(option.C_BID),
		&(option.C_ASK),
		&(option.STRIKE),
		&(option.P_BID),
		&(option.P_ASK),
		&(option.P_SIZE),
		&(option.P_LAST),
		&(option.P_DELTA),
		&(option.P_GAMMA),
		&(option.P_VEGA),
		&(option.P_THETA),
		&(option.P_RHO),
		&(option.P_IV),
		&(option.P_VOLUME),
		&(option.STRIKE_DISTANCE),
		&(option.STRIKE_DISTANCE_PCT)
	};

	OptionData option2;
	void * optionPtrList2[] = {
		&(option.QUOTE_UNIXTIME),
		&(option.QUOTE_READTIME),
		&(option.QUOTE_DATE),
		&(option.QUOTE_TIME_HOURS),
		&(option.UNDERLYING_LAST),
		&(option.EXPIRE_DATE),
		&(option.EXPIRE_UNIX),
		&(option.DTE),
		&(option.C_DELTA),
		&(option.C_GAMMA),
		&(option.C_VEGA),
		&(option.C_THETA),
		&(option.C_RHO),
		&(option.C_IV),
		&(option.C_VOLUME),
		&(option.C_LAST),
		&(option.C_SIZE),
		&(option.C_BID),
		&(option.C_ASK),
		&(option.STRIKE),
		&(option.P_BID),
		&(option.P_ASK),
		&(option.P_SIZE),
		&(option.P_LAST),
		&(option.P_DELTA),
		&(option.P_GAMMA),
		&(option.P_VEGA),
		&(option.P_THETA),
		&(option.P_RHO),
		&(option.P_IV),
		&(option.P_VOLUME),
		&(option.STRIKE_DISTANCE),
		&(option.STRIKE_DISTANCE_PCT)
	};

	int optionCounter = 0;
	int failedTests = 0;


	printf("File lengths: %llu %llu\n", lSize, lSize2);
	printf("Beginning to parse memory...\n"); fflush(stdout);
	while(iterator < endIterator && iterator2 < endIterator2) {
		// printf("Printing current read:");
		// for(int i = 0; !(*((iterator) + i) == '\n'); i++) {
		// 	printf("%c", *((iterator) + i));
		// }
		// printf("\n"); fflush(stdout);
		// printf("Printing current read:");
		// for(int i = 0; i < 661; i++) {
		// 	printf("%c", *((iterator2) + i));
		// }
		// printf("\n"); fflush(stdout);
		// printf("1\n");
		// printf("1 Iterator: %llu Iterator2: %llu\n", iterator - buffer, iterator2 - buffer2);
		for(int i = 0; i < sizeof(valueOrder)/sizeof(VALUE_ORDER_E); i++) {
			int eCode = readFunctionRawList[valueOrder[i]](&iterator, optionPtrList[i]);
			if(eCode != 1) {
				while(*iterator != ',') {
					iterator++;
				}
				iterator++;
				iterator += *iterator == ' ';
			}
		}

		// printf("2\n");
		// printf("2 Iterator: %llu Iterator2: %llu\n", iterator - buffer, iterator2 - buffer2);
		for(int i = 0; i < sizeof(valueOrder)/sizeof(VALUE_ORDER_E); i++) {
			int eCode = readFunctionRawList[valueOrder[i]](&iterator2, optionPtrList2[i]);
			if(eCode != 1) {
				while(*iterator2 != ',') {
					iterator2++;
				}
				iterator2++;
				iterator2 += *iterator2 == ' ';
			}
		}

		// printf("3\n");
		// printf("3 Iterator: %llu Iterator2: %llu\n", iterator - buffer, iterator2 - buffer2);
		for(int i = 0; i < sizeof(valueOrder)/sizeof(VALUE_ORDER_E); i++) {
			if(!cmpFunctionList[valueOrder[i]](optionPtrList[i], optionPtrList2[i])) {
				printf("Failed test %s.\n", nameList[i]);
				printFunctionList[valueOrder[i]](optionPtrList[i]);
				printFunctionList[valueOrder[i]](optionPtrList2[i]);
				printf("\n");

				failedTests++;
				break;
			}
		}

		// printf("4\n");
		optionCounter++;
		if(optionCounter % 1000 == 0) {
			printf("%lf/100 done... %d FAILED: %d, SUCCESS: %d\n", 
										(double) (iterator - buffer) / 
										(double) (endIterator - buffer) * 
										(double) 100, iterator > endIterator,
										failedTests,
										optionCounter);
			printf("%lf/100 done... %d FAILED: %d, SUCCESS: %d\n", 
										(double) (iterator2 - buffer2) / 
										(double) (endIterator2 - buffer2) * 
										(double) 100, iterator2 > endIterator2,
										failedTests,
										optionCounter);

			fflush(stdout);
		}
		// printf("5 %d\n", iterator < endIterator && iterator2 < endIterator2);
		// printf("4 Iterator: %llu Iterator2: %llu\n", iterator - buffer, iterator2 - buffer2);
	}
	for(int i = 0; i < sizeof(valueOrder)/sizeof(VALUE_ORDER_E); i++) {
		printFunctionList[valueOrder[i]](optionPtrList[i]);
		printFunctionList[valueOrder[i]](optionPtrList2[i]);
	}
	printf("File lengths: %ld %ld %llu %llu\n", endIterator - iterator, endIterator2 - iterator2, lSize, lSize2);
	printf("Memory parsed...\n"); fflush(stdout);
	printf("%d/%d tests failed. That is a %lf%% success rate.\n", failedTests, optionCounter,
			(double) 100 * ((double) 1 - (double) failedTests / (double) optionCounter));
	printf("Exiting...\n"); fflush(stdout);

	fclose(fp);
	free(buffer);
}


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "Structures.h"
#include "ParsingFunctions.h"

static int compareDate(Date a, Date b) {
	return a.year == b.year && a.month == b.month && a.day == b.day;
}

typedef void (*ReadFunctionParsed)(FILE *, void *);
ReadFunctionParsed readFunctionParsedList[FUNCTION_READ_COUNT] = 
	{readLongNumParsed, readDateAndTimeParsed, readDateParsed, readDoubleParsed, readSizeParsed};

void readData(FILE * fp, OptionData ** optionArray, int * optionCounter, int * uniqueChainCount) {
	uint64_t lSize;
	char *buffer;

	fseek(fp, 0, SEEK_END);
	long end = ftell(fp);
	rewind(fp);

	uint32_t optionCount;
	fread(&optionCount, sizeof(uint32_t), 1, fp);

	*optionArray = (OptionData *) malloc(optionCount * sizeof(OptionData));

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

	uint64_t currExpiry = 0xFFFFFFFFFFFFFFFF;
	while((end - ftell(fp)) >= sizeof(OptionData)) {
		fread(&option, sizeof(OptionData), 1, fp);
		// for(int i = 0; i < sizeof(valueOrder)/sizeof(VALUE_ORDER_E); i++) {
		// 	readFunctionParsedList[valueOrder[i]](fp, optionPtrList[i]);
		// }
		
		(*optionArray)[*optionCounter] = option;
		(*optionCounter)++;

		(*uniqueChainCount) += (currExpiry != option.QUOTE_UNIXTIME);
		currExpiry = option.QUOTE_UNIXTIME;
	}
}

void freeData(OptionData * optionArray) {
	free(optionArray);
}
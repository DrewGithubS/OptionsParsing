#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>


#include "Structures.h"
#include "ParsingFunctions.h"

typedef int (*ReadFunctionRaw)(char *, void *);

ReadFunctionRaw readFunctionRawList[FUNCTION_READ_COUNT] = 
	{readLongNumRaw, readDateAndTimeRaw, readDateRaw, readDoubleRaw, readSizeRaw};

typedef void (*PrintFunction)(void *);

PrintFunction printFunctionList[FUNCTION_READ_COUNT] = 
	{printLongNumRaw, printDateAndTimeRaw, printDateRaw, printDoubleRaw, printSizeRaw};

typedef void (*WriteFunction)(FILE *, void *);

WriteFunction writeFunctionList[FUNCTION_READ_COUNT] = 
	{writeLongNumParsed, writeDateAndTimeParsed, writeDateParsed, writeDoubleParsed, writeSizeParsed};

void convertFile(FILE * fp, FILE * fp_out) {
	uint64_t lSize;
	char *buffer;

	fseek(fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	/* allocate memory for entire content */
	buffer = calloc(sizeof(char), lSize+1);
	if(!buffer) {
		fclose(fp);
		printf("Memory allocation failed...\n");
		return;
	}

	/* copy the file into the buffer */
	if(1 != fread(buffer, lSize, 1, fp)) {
		fclose(fp);
		free(buffer);
		printf("File read failed...\n");
		return;
	}

	char * iterator = buffer;
	char * endIterator = buffer + lSize;

	uint32_t finalOptionCount = 1;

	for(char * i = iterator; i < endIterator; i++) {
		finalOptionCount += *i == '\n';
	}

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

	int optionCounter = 1;

	fwrite(&finalOptionCount, sizeof(uint32_t), 1, fp_out);

	// Throwaway first line
	for( ; *iterator != '\n'; (iterator)++) {}
	iterator++;
	
	char prev[500];
	int error = 0;
	while(iterator + 100 < endIterator) {
		// printf("Full: ");
		// for(int j = 0; iterator[j] != '\n'; j++) {
		// 	printf("%c", iterator[j]);
		// }
		// printf("\n");
		for(int i = 0; i < sizeof(valueOrder)/sizeof(VALUE_ORDER_E); i++) {
			// printf("Curr: ");
			// for(int j = 0; iterator[j] != '\n'; j++) {
			// 	printf("%c", iterator[j]);
			// }
			// printf("\n");
			int eCode = readFunctionRawList[valueOrder[i]](iterator, optionPtrList[i]);

			// writeFunctionList[valueOrder[i]](fp_out, optionPtrList[i]);
			// printFunctionList[valueOrder[i]](optionPtrList[i]);
			// printf(", ");
			if(eCode != 1) {

				printf("Error CODE!!!!!\n");
				printf("Function: %s\n", nameList[i]);
				printf("\n");
				error = 1;
			}

			for( ; *iterator != ',' && *iterator != '\n'; iterator++) {}
			iterator++;
			iterator += *iterator == ' ';

			// printf("%s: ", nameList[i]);
			// printFunctionList[valueOrder[i]](optionPtrList[i]);
			// printf("\n\n");

		}
		if(error) {
			error = 0;
		} else {
			fwrite(&option, sizeof(OptionData), 1, fp_out);
		}
		// printf("\n");

		optionCounter++;
		// if(optionCounter % 1000 == 0) {
		// 	// printf("%lf/100 done... %d \n", (double) (iterator - buffer) / 
		// 	// 							(double) (endIterator - buffer) * 
		// 	// 							(double) 100, iterator > endIterator);
		// 	// fflush(stdout);
		// }
	}

	fclose(fp);
	free(buffer);
}

uint8_t fileExists(char *filename) {
	struct stat buffer;   
	return (stat (filename, &buffer) == 0);
}

int parseRawFiles(int argc, char ** argv) {
	if(argc != 2) {
		printf("Please run the command ./main <ticker>\n");
		return 1;
	}

	char ticker[10];
	char tickerLower[10];

	for(int i = 0; argv[1][i] != '\0'; i++) {
		ticker[i] = argv[1][i];
		ticker[i] -= ticker[i] >= 'a' ? ('a' - 'A') : 0;

		tickerLower[i] = argv[1][i];
		tickerLower[i] += tickerLower[i] <= 'Z' ? ('a' - 'A') : 0;
	}

	char inpFile[150];
	char outFile[150];
	

	for(int i = 2010; i < 2030; i++) {
		for(int j = 1; j < 13; j++) {
			sprintf(inpFile, "./Parsing/SPX30/RawData/spx_30x_%.04d%.02d.txt", i, j);
			sprintf(outFile, "./Parsing/SPX30/PsdData/spx_30x_%.04d%.02d.bin", i, j);

			if(!fileExists(outFile) && fileExists(inpFile)) {
				printf("Parsing: %s...\n", inpFile);
				FILE * fp_out = fopen(outFile, "wb");

				FILE * fp = fopen(inpFile, "r");
				if(!fp) {
					return 1;
				}

				if(!fp_out) {
					return 1;
				}

				convertFile(fp, fp_out);
			}
		}
	}

	return 0;
}
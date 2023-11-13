#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "ReadData.h"
#include "RunAnalysis.h"
#include "AnalysisFunctions.h"
#include "Structures.h"

void doAnalysis() {
	char buf[150];

	FILE * outputFile = fopen("output.csv", "w");
	writeHeader(outputFile);

	for(int i = 2010; i < 2030; i++) {
		for(int j = 1; j < 13; j++) {
			int optionCounter = 0;
			int uniqueChainCount = 0;
			OptionData * optionArray;
			sprintf(buf, "./Parsing/SPX/PsdData/spx_eod_%.04d%.02d.bin", i, j);

			FILE * file = fopen(buf, "rb");

			if(!file) {
				continue;
			}

			// Read the data from the file.
			readData(
				file,
				&optionArray, 
				&optionCounter, 
				&uniqueChainCount);

			printf("Running analysis on: %s...\n", buf);
			// printf("Unique chain count: %d\n", uniqueChainCount);


			// Gets the indices for the start of each date
			int * chainIndices = (int *) malloc((uniqueChainCount+1) * sizeof(int));
			// Index of first date
			chainIndices[0] = 0;
			uint64_t tempQuote = optionArray[0].QUOTE_UNIXTIME;

			int chainIndex = 0;
			for(int i = 0; i < optionCounter; i++) {
				// When a new date is found, add it to the date indices
				if(optionArray[i].QUOTE_UNIXTIME != tempQuote) {
					chainIndex++;
					chainIndices[chainIndex] = i;
					tempQuote = optionArray[i].QUOTE_UNIXTIME;
				}
			}

			// Set the last index to the option count
			chainIndices[uniqueChainCount] = optionCounter;

			analyze(outputFile, optionArray, uniqueChainCount, chainIndices);

			fclose(file);
			freeData(optionArray);
			free(chainIndices);
		}
	}
}


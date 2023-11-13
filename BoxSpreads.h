#ifndef BOXSPREADS_H
#define BOXSPREADS_H

#include <stdio.h>
#include "Structures.h"

void writeHeaderBoxSpread(FILE * output);
void analyzeBoxSpread(FILE * output, OptionData * opts, int uniqueDateCount, int * dateIndices);

#endif
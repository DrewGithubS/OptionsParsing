#ifndef VIX_H
#define VIX_H

#include "Structures.h"


void analyzeVix(FILE * output, OptionData * opts, int uniqueDateCount, int * dateIndices);
void writeHeaderVix(FILE * output);

#endif
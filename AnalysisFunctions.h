#ifndef ANALYSISFUNCTIONS_H
#define ANALYSISFUNCTIONS_H

#include <stdio.h>
#include "Structures.h"

void writeHeader(FILE * output);
void analyze(FILE * output, OptionData * opts, int uniqueDateCount, int * dateIndices);

#endif
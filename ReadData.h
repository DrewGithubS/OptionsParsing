#ifndef READDATA_H
#define READDATA_H

#include "Structures.h"

void readData(FILE * fp, OptionData ** optionArray, int * optionCounter, int * uniqueDateCount);
void freeData(OptionData * optionArray);

#endif
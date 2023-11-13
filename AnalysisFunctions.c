#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "AnalysisFunctions.h"
#include "Structures.h"
#include "Spliner.h"

#include "BoxSpreads.h"
#include "Vix.h"
#include "LEAPS.h"

void writeHeader(FILE * output) {
	writeHeaderLeaps(output);
}

void analyze(FILE * output, OptionData * opts, int uniqueDateCount, int * dateIndices) {
	analyzeLeaps(output, opts, uniqueDateCount, dateIndices);
}
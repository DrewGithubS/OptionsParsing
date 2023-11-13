#include "RunAnalysis.h"
#include "TxtToBin.h"

int main(int argc, char ** argv) {
	if(parseRawFiles(argc, argv) != 0) {
		return 1;
	}

	doAnalysis();
}
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Spliner.h"

// Math from https://www.desmos.com/calculator/axfvhtc8n7?lang=de
double interpolateRaw(double dataX[4], double dataY[4], double x) {
	// Assuming that dataX is sorted.
	double x1 = dataX[0];
	double x2 = dataX[1];
	double x3 = dataX[2];
	double x4 = dataX[3];
	// printf("x1: %.10lf\n", x1);
	// printf("x2: %.10lf\n", x2);
	// printf("x3: %.10lf\n", x3);
	// printf("x4: %.10lf\n", x4);

	double y1 = dataY[0];
	double y2 = dataY[1];
	double y3 = dataY[2];
	double y4 = dataY[3];
	// printf("\n");
	// printf("y1: %.10lf\n", y1);
	// printf("y2: %.10lf\n", y2);
	// printf("y3: %.10lf\n", y3);
	// printf("y4: %.10lf\n", y4);

	// Slopes of linear interpolation
	double m1 = (y2 - y1) / (x2 - x1);
	double m2 = (y3 - y2) / (x3 - x2);
	double m3 = (y4 - y3) / (x4 - x3);
	// printf("\n");
	// printf("m1: %.10lf\n", m1);
	// printf("m2: %.10lf\n", m2);
	// printf("m3: %.10lf\n", m3);

	// L1 = m1 * (x - x1) + y1
	// L2 = m2 * (x - x2) + y2
	// L3 = m3 * (x - x3) + y3


	// Second derivative of spline
	double z2 = 6 * (m3 * x2 + m2 * x3 - m3 * x3 + 2 * m2 * x4 + 2 * m1 * x2 - 2 * m1 * x4 - 3 * m2 * x2);
	z2 /= (4 * (x1 * x2 + x3 * x4 - x1 * x4)) - (x2 + x3) * (x2 + x3);

	double z3 = 6 * (m2 * x2 + m1 * x3 - m1 * x2 + 2 * m2 * x1 + 2 * m3 * x3 - 2 * m3 * x1 - 3 * m2 * x3);
	z3 /= (4 * (x1 * x2 + x3 * x4 - x1 * x4)) - (x2 + x3) * (x2 + x3);

	// printf("\n");
	// printf("z2: %.10lf\n", z2);
	// printf("z3: %.10lf\n", z3);

	// Part of third derivative
	double a1 = z2 / (6 * (x1 - x2));
	double b1 = 2 * z2 / (6 * (x2 - x1));

	double a2 = (2 * z2 + z3) / (6 * (x2 - x3));
	double b2 = (2 * z3 + z2) / (6 * (x3 - x2));

	double a3 = z3 / (3 * (x3 - x4));
	double b3 = z3 / (6 * (x4 - x3));

	// printf("\n");
	// printf("a1: %.10lf\n", a1);
	// printf("b1: %.10lf\n", b1);
	// printf("a2: %.10lf\n", a2);
	// printf("b2: %.10lf\n", b2);
	// printf("a3: %.10lf\n", a3);
	// printf("b3: %.10lf\n", b3);

	// Final Equations:
	// C1 = a1 * (x - x2) * (x - x2) * (x - x1) + b1 * (x - x2) * (x - x1) * (x - x1)
	// C2 = a2 * (x - x3) * (x - x3) * (x - x2) + b2 * (x - x3) * (x - x2) * (x - x2)
	// C2 = a3 * (x - x4) * (x - x4) * (x - x3) + b3 * (x - x4) * (x - x3) * (x - x3)

	// Spline:
	// S1 = C1 + L1   x E [x1, x2]
	// S2 = C2 + L2   x E [x2, x3]
	// S3 = C3 + L3   x E [x3, x4]

	if(x >= x1 && x <= x2) {
		double L1 = m1 * (x - x1) + y1;
		double C1 = a1 * (x - x2) * (x - x2) * (x - x1) + b1 * (x - x2) * (x - x1) * (x - x1);

		return L1 + C1;
	} else if(x <= x3) {
		double L2 = m2 * (x - x2) + y2;
		double C2 = a2 * (x - x3) * (x - x3) * (x - x2) + b2 * (x - x3) * (x - x2) * (x - x2);

		return L2 + C2;
	} else if(x <= x4) {
		double L3 = m3 * (x - x3) + y3;
		double C3 = a3 * (x - x4) * (x - x4) * (x - x3) + b3 * (x - x4) * (x - x3) * (x - x3);

		return L3 + C3;
	} else {
		return -1;
	}
}

double interpolate(double * dataX, double * dataY, int dataCount, double testX) {
	static double dataXParsed[4];
	static double dataYParsed[4];

	if(dataCount < 4 || testX < dataX[0] || testX > dataX[dataCount - 1]) {
		return -1;
	} else if(dataCount == 4) {
		for(int i = 0; i < 4; i++) {
			dataXParsed[i] = dataX[i];
			dataYParsed[i] = dataY[i];
		}
	} else {
		int tempIndex = 0;
		for(tempIndex = 0; tempIndex < dataCount - 1; tempIndex++) {
			if(testX < dataX[tempIndex + 1]) {
				break;
			}
		}

		tempIndex -= 2;

		if(tempIndex > dataCount - 4) {
			tempIndex = dataCount - 4;
		}
		if(tempIndex < 0) {
			tempIndex = 0;
		}

		for(int i = tempIndex; i < tempIndex + 4; i++) {
			dataXParsed[i - tempIndex] = dataX[i];
			dataYParsed[i - tempIndex] = dataY[i];
		}
	}


	return interpolateRaw(dataXParsed, dataYParsed, testX);
}
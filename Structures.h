#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "stdint.h"

typedef enum {
	LONGNUM,
	DATEANDTIME,
	DATE,
	DOUBLENUM,
	SIZE,
	FUNCTION_READ_COUNT
} VALUE_ORDER_E;

// 1262638800, 2010-01-04 16:00, 2010-01-04, 16.000000, 1132.990000, 2010-01-07, 1262898000, 3.000000, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000, , , 0.000000, 101 x 101, 205.990000, 208.990000, 925.000000, 0.000000, 0.060000, 0 x 420, 0.050000, -0.000770, 0.000040, 0.004390, -0.021160, -0.000490, 0.640130, 550.000000, 208.000000, 0.184000
static const VALUE_ORDER_E valueOrder[] = {
	LONGNUM,     // QUOTE_UNIXTIME      1262638800
	DATEANDTIME, // QUOTE_READTIME      2010-01-04 16:00
	DATE,        // QUOTE_DATE          2010-01-04
	DOUBLENUM,   // QUOTE_TIME_HOURS    16.000000
	DOUBLENUM,   // UNDERLYING_LAST     1132.990000
	DATE,		 // EXPIRE_DATE         2010-01-07
	LONGNUM,     // EXPIRE_UNIX         1262898000
	DOUBLENUM,   // DTE                 3.000000

	DOUBLENUM,   // C_DELTA             1.000000
	DOUBLENUM,   // C_GAMMA             0.000000
	DOUBLENUM,   // C_VEGA              0.000000
	DOUBLENUM,   // C_THETA             0.000000
	DOUBLENUM,   // C_RHO               0.000000
	DOUBLENUM,   // C_IV                0.640130
	DOUBLENUM,   // C_VOLUME            550.000000
	DOUBLENUM,   // C_LAST              0.000000
	SIZE,        // C_SIZE              101 x 101
	DOUBLENUM,   // C_BID               205.990000
	DOUBLENUM,   // C_ASK               208.990000

	DOUBLENUM,   // STRIKE              925.000000

	DOUBLENUM,   // P_BID               0.000000
	DOUBLENUM,   // P_ASK               0.060000
	SIZE,        // P_SIZE              0 x 420
	DOUBLENUM,   // P_LAST              0.050000
	DOUBLENUM,   // P_DELTA             -0.000770
	DOUBLENUM,   // P_GAMMA             0.000040
	DOUBLENUM,   // P_VEGA              0.004390
	DOUBLENUM,   // P_THETA             -0.021160
	DOUBLENUM,   // P_RHO               -0.000490
	DOUBLENUM,   // P_IV                0.640130
	DOUBLENUM,   // P_VOLUME            550.000000
	DOUBLENUM,   // STRIKE_DISTANCE     208.000000
	DOUBLENUM,   // STRIKE_DISTANCE_PCT 0.184000
};

/*
Unique Data Types:
LONGNUM     - Done
DATEANDTIME - Done
DATE        - Done
DOUBLENUM   - Done
SIZE        - 
*/

typedef struct {
	uint16_t year;
	uint8_t month;
	uint8_t day;
} Date;

typedef struct {
	uint16_t minutes;
} Time;

typedef struct {
	Date date;
	Time time;
} DateAndTime;

typedef struct {
	uint32_t first;
	uint32_t second;
} BidAskSize;

typedef struct {
	uint64_t QUOTE_UNIXTIME;
	DateAndTime QUOTE_READTIME;
	Date QUOTE_DATE;
	double QUOTE_TIME_HOURS;
	double UNDERLYING_LAST;
	DateAndTime EXPIRE_DATE;
	uint64_t EXPIRE_UNIX;
	double DTE;

	double C_DELTA;
	double C_GAMMA;
	double C_VEGA;
	double C_THETA;
	double C_RHO;
	double C_IV;
	uint64_t C_VOLUME;
	double C_LAST;
	BidAskSize C_SIZE;
	double C_BID;
	double C_ASK;

	double STRIKE;

	double P_BID;
	double P_ASK;
	BidAskSize P_SIZE;
	double P_LAST;
	double P_DELTA;
	double P_GAMMA;
	double P_VEGA;
	double P_THETA;
	double P_RHO;
	double P_IV;
	double P_VOLUME;
	double STRIKE_DISTANCE;
	double STRIKE_DISTANCE_PCT;
} OptionData;

static char * nameList[] = {
	"QUOTE_UNIXTIME",
	"QUOTE_READTIME",
	"QUOTE_DATE",
	"QUOTE_TIME_HOURS",
	"UNDERLYING_LAST",
	"EXPIRE_DATE",
	"EXPIRE_UNIX",
	"DTE",
	"C_DELTA",
	"C_GAMMA",
	"C_VEGA",
	"C_THETA",
	"C_RHO",
	"C_IV",
	"C_VOLUME",
	"C_LAST",
	"C_SIZE",
	"C_BID",
	"C_ASK",
	"STRIKE",
	"P_BID",
	"P_ASK",
	"P_SIZE",
	"P_LAST",
	"P_DELTA",
	"P_GAMMA",
	"P_VEGA",
	"P_THETA",
	"P_RHO",
	"P_IV",
	"P_VOLUME",
	"STRIKE_DISTANCE",
	"STRIKE_DISTANCE_PCT"
};

#endif
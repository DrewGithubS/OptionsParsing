#include <stdint.h>
#include <stdio.h>

#include "ParsingFunctions.h"
#include "Structures.h"

void printLongNumRaw(void * output) {
	uint64_t * print = (uint64_t *) output;
	printf("%llu", *print);
}

void printDateAndTimeRaw(void * output) {
	DateAndTime * outputPtr = (DateAndTime *) output;
	Date * date = &outputPtr->date;
	Time * time = &outputPtr->time;
	printf("%d-%d-%d %d", date->year, date->month, date->day, time->minutes);
}

void printDateRaw(void * output) {
	Date * date = (Date *) output;
	printf("%d-%d-%d", date->year, date->month, date->day);
}

void printDoubleRaw(void * output) {
	double * print = (double *) output;
	printf("%lf", *print);
}

void printSizeRaw(void * output) {
	BidAskSize * print = (BidAskSize *) output;
	printf("%d x %d", print->first, print->second);
}

int readLongNumRaw(char * iterator, void * output) {
	uint64_t * outputPtr = (uint64_t *) output;
	*outputPtr = 0;

	if(!(*iterator >= '0' && *iterator <= '9')) {return 0;}

	while(*iterator != ',') {
		*outputPtr *= 10;
		*outputPtr += *iterator - '0';
		iterator++;
	}

	return 1;
}

// Repeated whiles but I don't want to fix it.
int readDateAndTimeRaw(char * iterator, void * output) {
	DateAndTime * outputPtr = (DateAndTime *) output;
	Date * date = &outputPtr->date;
	Time * time = &outputPtr->time;
	date->year = 0;

	if(!(*iterator >= '0' && *iterator <= '9')) {return 0;}
	while(*iterator != '-') {
		date->year *= 10;
		date->year += *iterator - '0';
		iterator++;
	}
	iterator++;

	date->month = 0;
	if(!(*iterator >= '0' && *iterator <= '9')) {return -1;}
	while(*iterator != '-') {
		date->month *= 10;
		date->month += *iterator - '0';
		iterator++;
	}
	iterator++;

	date->day = 0;
	if(!(*iterator >= '0' && *iterator <= '9')) {return -2;}
	while(*iterator != ' ') {
		date->day *= 10;
		date->day += *iterator - '0';
		iterator++;
	}
	iterator++;

	uint16_t tempMinutes = 0;

	tempMinutes = 0;
	if(!(*iterator >= '0' && *iterator <= '9')) {return -3;}
	while(*iterator != ':') {
		tempMinutes *= 10;
		tempMinutes += *iterator - '0';
		iterator++;
	}
	iterator++;

	tempMinutes *= 60;
	if(!(*iterator >= '0' && *iterator <= '9')) { return -4;}
	while(*iterator != ',') {
		time->minutes *= 10;
		time->minutes += *iterator - '0';
		iterator++;
	}

	time->minutes += tempMinutes;

	return 1;
}

int readDateRaw(char * iterator, void * output) {
	Date * outputDate = (Date *) output;
	outputDate->year = 0;

	if(!(*iterator >= '0' && *iterator <= '9')) {return 0;}
	while(*iterator != '-') {
		outputDate->year *= 10;
		outputDate->year += *iterator - '0';
		iterator++;
	}
	iterator++;

	outputDate->month = 0;
	if(!(*iterator >= '0' && *iterator <= '9')) {return -1;}
	while(*iterator != '-') {
		outputDate->month *= 10;
		outputDate->month += *iterator - '0';
		iterator++;
	}
	iterator++;

	outputDate->day = 0;
	if(!(*iterator >= '0' && *iterator <= '9')) {return -2;}
	while(*iterator != ',') {
		outputDate->day *= 10;
		outputDate->day += *iterator - '0';
		iterator++;
	}
	iterator++;

	return 1;
}

int readDoubleRaw(char * iterator, void * output) {
	double * outputPtr = (double *) output;
	*outputPtr = 0;

	double negative = 1;

	while(*iterator == ' ') {(*iterator)++;}
	if(*iterator == ',') {return 1;}

	if(*iterator == '-') {
		negative = -1;
		iterator++;
	}

	if(!(*iterator >= '0' && *iterator <= '9')) { return 0;}
	while(*iterator != '.') {
		*outputPtr *= 10;
		*outputPtr += (double) (*iterator - '0');
		iterator++;
	}
	iterator++;

	double divider = 10;
	if(!(*iterator >= '0' && *iterator <= '9')) { return -1;}
	while(*iterator >= '0' && *iterator <= '9') {
		*outputPtr += (double) ((*iterator) - '0') / divider;
		divider *= 10;
		iterator++;
	}

	*outputPtr *= negative;

	return 1;
}

int readSizeRaw(char * iterator, void * output) {
	BidAskSize * outputPtr = (BidAskSize *) output;
	outputPtr->first = 0;
	outputPtr->second = 0;

	// printf("Reading first value...\n"); fflush(stdout);
	if(!(*iterator >= '0' && *iterator <= '9')) { return 0;}
	while(*iterator != ' ') {
		outputPtr->first *= 10;
		outputPtr->first += *iterator - '0';
		iterator++;
	}

	iterator++;
	if(!(*iterator == 'x')) { return -1;}
	iterator++;
	if(!(*iterator == ' ')) { return -2;}
	iterator++;

	// printf("Reading second value...\n"); fflush(stdout);
	if(!(*iterator >= '0' && *iterator <= '9')) { return -3;}
	while(*iterator != ',') {
		outputPtr->second *= 10;
		outputPtr->second += *iterator - '0';
		iterator++;
	}

	return 1;
}

void writeLongNumParsed(FILE * out, void * data) {
	uint64_t * dataToWrite = (uint64_t *) data;
	fwrite(data, sizeof(uint64_t), 1, out);
}

void writeDateAndTimeParsed(FILE * out, void * data) {
	DateAndTime * dataToWrite = (DateAndTime *) data;
	Date * date = &dataToWrite->date;
	Time * time = &dataToWrite->time;
	fwrite(date, sizeof(Date), 1, out);
	fwrite(time, sizeof(Time), 1, out);
}

void writeDateParsed(FILE * out, void * data) {
	Date * dataToWrite = (Date *) data;
	fwrite(dataToWrite, sizeof(Date), 1, out);
}

void writeDoubleParsed(FILE * out, void * data) {
	double * dataToWrite = (double *) data;
	fwrite(dataToWrite, sizeof(double), 1, out);
}

void writeSizeParsed(FILE * out, void * data) {
	BidAskSize * dataToWrite = (BidAskSize *) data;
	fwrite(&(dataToWrite->first), sizeof(uint32_t), 1, out);
	fwrite(&(dataToWrite->second), sizeof(uint32_t), 1, out);
}

void readLongNumParsed(FILE * in, void * data) {
	uint64_t * dataToRead = (uint64_t *) data;
	fread(data, sizeof(uint64_t), 1, in);
}

void readDateAndTimeParsed(FILE * in, void * data) {
	DateAndTime * dataToRead = (DateAndTime *) data;
	Date * date = &dataToRead->date;
	Time * time = &dataToRead->time;
	fread(date, sizeof(Date), 1, in);
	fread(time, sizeof(Time), 1, in);
}

void readDateParsed(FILE * in, void * data) {
	Date * dataToRead = (Date *) data;
	fread(dataToRead, sizeof(Date), 1, in);
}

void readDoubleParsed(FILE * in, void * data) {
	double * dataToRead = (double *) data;
	fread(dataToRead, sizeof(double), 1, in);
}

void readSizeParsed(FILE * in, void * data) {
	BidAskSize * dataToRead = (BidAskSize *) data;
	fread(&(dataToRead->first), sizeof(uint32_t), 1, in);
	fread(&(dataToRead->second), sizeof(uint32_t), 1, in);
}

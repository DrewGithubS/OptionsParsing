#ifndef PARSINGFUNCTIONS_H
#define PARSINGFUNCTIONS_H

#include "Structures.h"

void printLongNumRaw(void * output);
void printDateAndTimeRaw(void * output);
void printDateRaw(void * output);
void printDoubleRaw(void * output);
void printSizeRaw(void * output);

int readLongNumRaw(char * iterator, void * output);
int readDateAndTimeRaw(char * iterator, void * output);
int readDateRaw(char * iterator, void * output);
int readDoubleRaw(char * iterator, void * output);
int readSizeRaw(char * iterator, void * output);

void writeLongNumParsed(FILE * out, void * data);
void writeDateAndTimeParsed(FILE * out, void * data);
void writeDateParsed(FILE * out, void * data);
void writeDoubleParsed(FILE * out, void * data);
void writeSizeParsed(FILE * out, void * data);

void readLongNumParsed(FILE * in, void * data);
void readDateAndTimeParsed(FILE * in, void * data);
void readDateParsed(FILE * in, void * data);
void readDoubleParsed(FILE * in, void * data);
void readSizeParsed(FILE * in, void * data);

#endif
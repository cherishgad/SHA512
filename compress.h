#ifndef _compress
#define _compress

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#define LIMITULL 64
#define WORDSIZE 64
#define ORIGINWORDNUMBER 16
#define EXPANSIONWORDNUMBER 80
#define COMPRESSROUNDWORD 8
#define ROUNDNUMBER 80
void compress(unsigned long long int * hash, unsigned long long int * message, unsigned long long int * key);
void roundCompress(unsigned long long int * hash, unsigned long long int  word, unsigned long long int  k);
void wordExpansion(unsigned long long int *message, unsigned  long long int * expansion_message);
unsigned long long int rotShiftUnsignedLongInt(unsigned long long int value, int l, int m, int n);
unsigned long long int rotLeftUnsignedLongInt(unsigned long long int value, int shift);
unsigned long long int rotRightUnsignedLongInt(unsigned long long int value, int shift);
unsigned long long int majority(unsigned long long int x, unsigned long long int y, unsigned long long int z);
unsigned long long int rotateA(unsigned long long int x);
unsigned long long int rotateB(unsigned long long int x);
unsigned long long int conditional(unsigned long long int x, unsigned long long int y, unsigned long long int z);
void printUnsignedLongInt(unsigned long long int * long_ary, unsigned long long int size);
#endif
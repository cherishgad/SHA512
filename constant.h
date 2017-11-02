#ifndef _constant
#define _constant
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#define INITIALHASHSIZE 8
#define ROUNDCONSTANTS 80
void initialVector(unsigned long long int * initial_vector);
void roundConstants(unsigned long long int * round_constants);
#endif
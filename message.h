#ifndef _message
#define _message
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#define LENGTHBIT 128
#define MESSAGEBLOCKWORDS 16
#define MESSAGEBLOCKBITS 1024
#define WORDBITS 64
#define COUNTERSIZE 2
int messageBlockGenerator(unsigned long long int * message_block, int  message_block_size_bit, unsigned long long int * counter);
int counterIncreaser(unsigned long long int * counter, int increase_number);
int firstPartZeroPadding(unsigned long long int * target, int message_size_bit);
#endif
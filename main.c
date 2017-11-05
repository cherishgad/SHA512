#include "compress.h"
#include "constant.h"
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include "message.h"
#include <string.h>
#define MESSAGE_LENGTH_FIELD_LENGTH_4BYTES 4
#define BLOCK_SIZE_BYTES 1
#define MESSAGE_READ "%800s"
#define MAXIMUM_USER_STRING_WORD 100
#define COUNTERSIZE 2
#define CHARSIZE 8
//This program assume word size is 8 byte so 64 bits
void read_directly();
void excute(unsigned long long int * message, unsigned long long int message_size_bit);
int main(){
	int user_select = 0;
	printf("SHA512!! Please select mode !!\n");
	printf("1. Enter plain text directly\n");
	printf("Etc Exit the program\n");
	printf("Give me the one of the numbers above\n");
	scanf("%d", &user_select);
	if (user_select == 1){
		read_directly();
		printf("\n");
	}
	printf("\nGOOD BYE!!\n");
	return 0;
}
void read_directly(){
	unsigned long long int text_length = 0;
	char * message = (char*)malloc((MAXIMUM_USER_STRING_WORD)*sizeof(unsigned long long int)+(sizeof(char)));
	unsigned long long int * message_long = (unsigned long long int *)malloc((MAXIMUM_USER_STRING_WORD)*sizeof(unsigned long long int));
	int i  =0;
	int j = 0;
	if(message == NULL | message_long == NULL){  
		printf("Out of memory at during preparing scaning the plain text\n");  
		exit(-1);  
	}
	printf("Give me plain text\n");
	scanf(MESSAGE_READ, message);
	printf("plain text:\n%s", message);
	text_length = strlen(message);
	text_length = text_length<<3;
	printf("\nmessage size: %d bits\n", text_length);
	for(i = 0; i < (text_length + WORDSIZE -1)/WORDSIZE; i++){
		message_long[i] = 0;
		for(j =0; j< WORDSIZE/CHARSIZE; j++){
			message_long[i] = (message_long[i]<<CHARSIZE) | ( ((unsigned long long int)(message[i*CHARSIZE + j])) & ((unsigned long long int)(1<<CHARSIZE) -1));
		}
	}
	excute(message_long, text_length);
	free(message);
	free(message_long);
}
void excute(unsigned long long int * message, unsigned long long int message_size_bit){
	long long int i = 0;
	long long int j = 0;
	int one_more = 0;
	unsigned long long int * hash = (unsigned long long int *)malloc(INITIALHASHSIZE*sizeof(unsigned long long int));
	unsigned long long int * round_constant = (unsigned long long int *)malloc(ROUNDCONSTANTS*sizeof(unsigned long long int));
	unsigned long long int * message_block = (unsigned long long int *)malloc(ORIGINWORDNUMBER*sizeof(unsigned long long int));
	unsigned long long int * counter = (unsigned long long int *)malloc(COUNTERSIZE*sizeof(unsigned long long int));
	unsigned int * printer = NULL; 
	if(hash == NULL || round_constant == NULL || message_block == NULL || counter == NULL){  
		printf("Out of memory during excuting the hash function start point \n");  
		exit(-1);  
	}
	for(i = 0; i<COUNTERSIZE;i++)
		counter[i] = 0;
	roundConstants(round_constant);
	initialVector(hash);
	
	//read Message block bit message
	for ( i = 0; i<message_size_bit/MESSAGEBLOCKBITS; i++){
		for(j = 0; j< MESSAGEBLOCKWORDS; j++)
			message_block[j] = message[i*MESSAGEBLOCKWORDS + j];
		messageBlockGenerator(message_block, MESSAGEBLOCKBITS, counter);
		compress(hash, message_block, round_constant);
	}
	//read rest part
	for(i = message_size_bit/MESSAGEBLOCKBITS; i< (message_size_bit + MESSAGEBLOCKBITS -1)/MESSAGEBLOCKBITS; i++){
		for(j = 0; j< MESSAGEBLOCKWORDS && i*MESSAGEBLOCKBITS + j <(message_size_bit + WORDBITS -1)/WORDBITS; j++)
			message_block[j] = message[i*MESSAGEBLOCKWORDS + j];
	}
	one_more = messageBlockGenerator(message_block, (int)(message_size_bit%MESSAGEBLOCKBITS), counter);
	compress(hash, message_block, round_constant);
	
	if(one_more!=0){//rest part do
		messageBlockGenerator(message_block, MESSAGEBLOCKBITS - (int)(message_size_bit%MESSAGEBLOCKBITS), counter);
		compress(hash, message_block, round_constant);
	}

	for(i = 0; i< INITIALHASHSIZE; i++){
		printf("0x%08x", (hash[i]>>32));
		printf("%08x", ((hash[i]<<32)>>32));
		printf("\n");
	}
	free(hash);
	free(round_constant);
	free(message_block);
	free(counter);
}
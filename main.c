#include "compress.h"
#include "constant.h"
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include "message.h"
#define MESSAGE_LENGTH_FIELD_LENGTH_4BYTES 4
#define BLOCK_SIZE_BYTES 1
#define MESSAGE_READ "%1s"
#define MAXIMUM_USER_STRING
#define COUNTERSIZE 2
//This program assume integer size is 4 byte so 32 bits
void read_directly();
void read_file();
void excute(unsigned long long int * message, long int message_size_bit);
int messageBlockGenerator(unsigned long long int * message_block, unsigned long long int * counter);
int main(){
	unsigned int* text_length = NULL;
	int user_select = 0;
	text_length = (unsigned int*)malloc(sizeof(int)*MESSAGE_LENGTH_FIELD_LENGTH_4BYTES);
	printf("SHA512!! Please select mode !!\n");
	printf("1. Enter plain text directly\n");
	printf("2. Enter plain text file\n");
	printf("Etc Exit the program\n");
	printf("Give me the one of the numbers above\n");
	scanf("%d", &user_select);
	printf("%d", user_select);
	if (user_select == 1){
		read_directly();
	}
	else if(user_select == 2){
		read_file();
	}
	printf("GOOD BYE!!\n");
	free(text_length);
	return 0;
}
void read_directly(){
	char * message = (char*)malloc((BLOCK_SIZE_BYTES+1)*sizeof(char));
	int a;
	printf("Give me plain text\n");
	a = scanf(MESSAGE_READ, message);
	printf("%d", a);
	printf("%s", message);
	printf("Give me plain text\n");
	a = scanf(MESSAGE_READ, message);
	printf("%d", a);
	printf("%s", message);
	free(message);
}

void read_file(){
	unsigned long long int why = (unsigned long long int)((int)'1');
	why = why <<32;
	excute(&why, 0);


}
void excute(unsigned long long int * message, long int message_size_bit){
	int i = 0;
	int j = 0;
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
			message_block[j] = message[i*MESSAGEBLOCKBITS + j];
		messageBlockGenerator(message_block, MESSAGEBLOCKBITS, counter);
		compress(hash, message_block, round_constant);
	}
	//read rest part
	for(i = message_size_bit/MESSAGEBLOCKBITS; i< (message_size_bit + MESSAGEBLOCKBITS -1)/MESSAGEBLOCKBITS; i++){
		for(j = 0; j< MESSAGEBLOCKWORDS && i*MESSAGEBLOCKBITS + j <(message_size_bit + WORDBITS -1)/WORDBITS; j++)
			message_block[j] = message[i*MESSAGEBLOCKBITS + j];
	}
	one_more = messageBlockGenerator(message_block, message_size_bit%MESSAGEBLOCKBITS, counter);
	compress(hash, message_block, round_constant);
	
	if(one_more!=0){//rest part do
		messageBlockGenerator(message_block, MESSAGEBLOCKBITS - (message_size_bit%MESSAGEBLOCKBITS), counter);
		compress(hash, message_block, round_constant);
	}

	printer = (unsigned int *)hash;
	for(i = 0; i< INITIALHASHSIZE*2; i++)
		printf("%x", printer[i]);
	free(hash);
	free(round_constant);
	free(message_block);
	free(counter);
}
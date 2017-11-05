#include "compress.h"
void compress(unsigned long long int * hash, unsigned long long int * message, unsigned long long int * key){
	unsigned long long int * expansion_message = (unsigned long long int*)malloc(EXPANSIONWORDNUMBER*sizeof(unsigned long long int));
	unsigned long long int * round_result = (unsigned long long int*)malloc(COMPRESSROUNDWORD*sizeof(unsigned long long int));
	int i = 0;
	if(expansion_message == NULL || round_result == NULL){  
		printf("Out of memory during excuting the compress function start point \n");  
		exit(-1);  
	}
	for(i = 0; i< COMPRESSROUNDWORD; i++){
		printf("0x%x", (hash[i]>>32));
		printf("%x", ((hash[i]<<32)>>32));
		printf("\n");
	}
	printf("\n inital hash\n");
	for(i = 0; i< ORIGINWORDNUMBER; i++){
		printf("0x%x", (message[i]>>32));
		printf("%x", ((message[i]<<32)>>32));
		printf("\n");
	}
	printf("\nmessage block\n");
	//Expand messsage
	wordExpansion(message,expansion_message);
	for(i = 0; i< EXPANSIONWORDNUMBER; i++){
		printf("0x%08x", (expansion_message[i]>>32));
		printf("%08x", ((expansion_message[i]<<32)>>32));
		printf("\n");
	}
	printf("\nexpansion result\n");
	//copy the hash
	for(i = 0; i<COMPRESSROUNDWORD; i++){
		round_result[i] = hash[i];
	}
	//round function
	for(i = 0; i<ROUNDNUMBER; i++){
		roundCompress(round_result, expansion_message[i], key[i]);	
	}
	//addition word
	for(i =0; i<COMPRESSROUNDWORD; i++){
		hash[i] = hash[i] + round_result[i];
	}
	for(i = 0; i< COMPRESSROUNDWORD; i++){
		printf("0x%08x", (round_result[i]>>32));
		printf("%08x", ((round_result[i]<<32)>>32));
		printf("\n");
	}
	printf("\nround result\n");
	for(i = 0; i< COMPRESSROUNDWORD; i++){
		printf("0x%08x", (hash[i]>>32));
		printf("%08x", ((hash[i]<<32)>>32));
		printf("\n");
	}
	printf("\nhash result\n");


	free(expansion_message);
	free(round_result);
}

void roundCompress(unsigned long long int * hash, unsigned long long int  word, unsigned long long int  k){
	int i = 0;
	unsigned long long int mixer1 = 0;
	unsigned long long int mixer2 = 0;
	unsigned long long int * temp = NULL;
	unsigned long long int * result_hash = (unsigned long long int *)malloc(COMPRESSROUNDWORD*sizeof(unsigned long long int));
	if(result_hash == NULL){  
		printf("Out of memory at during excute the round compress function\n");  
		exit(-1);  
	}
	//compute
	mixer1 = majority(hash[0], hash[1], hash[2]) + rotate(hash[0]);
	mixer2 = hash[7] + conditional(hash[4],hash[5],hash[6]) + rotate(hash[4]) + word + k; 
	result_hash[0] = mixer1 + mixer2;
	for(i = 1; i<4; i++)
		result_hash[i] = hash[i-1];
	result_hash[4] = mixer2 + hash[3];
	for(i = 5; i<COMPRESSROUNDWORD; i++)
		result_hash[i] = hash[i-1];
	//copy the result
	for(i = 0; i<COMPRESSROUNDWORD; i++)
		hash[i] = result_hash[i];

	free(result_hash);
}
void wordExpansion(unsigned long long int *message, unsigned  long long int * expansion_message){
	int i = 0;
	for(i = 0; i < ORIGINWORDNUMBER; i++)
		expansion_message[i] = message[i];
	for(i = ORIGINWORDNUMBER; i< EXPANSIONWORDNUMBER; i++)
		expansion_message[i] = expansion_message[i- 16]^rotShiftUnsignedLongInt(expansion_message[i - 15],1,8,7)^expansion_message[i - 7]^rotShiftUnsignedLongInt(expansion_message[i - 2],19,61,6);
}
unsigned long long int rotShiftUnsignedLongInt(unsigned long long int value, int l, int m, int n){

	return rotRightUnsignedLongInt(value, l)^rotRightUnsignedLongInt(value,m)^(value<<n);
}

//rotate (circular) left shift
unsigned long long int rotLeftUnsignedLongInt(unsigned long long int value, int shift){
	shift = (shift + LIMITULL) % LIMITULL;
	return (value << shift)|(value >> (LIMITULL - shift));
}
//rotate (circular) right shift
unsigned long long int rotRightUnsignedLongInt(unsigned long long int value, int shift){
	shift = (shift + LIMITULL) % LIMITULL;
	return (value >> shift)|(value << (LIMITULL - shift));
}
unsigned long long int majority(unsigned long long int x, unsigned long long int y, unsigned long long int z){
	return (x & y)^(y&z)^(z&x);
}
unsigned long long int rotate(unsigned long long int x){
	return rotRightUnsignedLongInt(x, 28)^rotRightUnsignedLongInt(x, 34)^rotRightUnsignedLongInt(x, 39);
}
unsigned long long int conditional(unsigned long long int x, unsigned long long int y, unsigned long long int z){
	return (x&y) ^ ((~x)&z);
}
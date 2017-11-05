#include "message.h"
int messageBlockGenerator(unsigned long long int * message_block, int  message_block_size_bit, unsigned long long int * counter){
	int i = 0, j =0; 
	//message padding and count
	if(message_block_size_bit<0){//last block continue padding
		for(i = 0; i< MESSAGEBLOCKWORDS-COUNTERSIZE; i++){//normal 0 pedding
			message_block[i] = 0;
		}
		for(i= MESSAGEBLOCKWORDS-COUNTERSIZE,  j=0; i< MESSAGEBLOCKWORDS && j<COUNTERSIZE;i++,j++){
			message_block[i] = counter[j];
		}
		return 0;//finish
	}
	else if(message_block_size_bit>=0 && message_block_size_bit <= (MESSAGEBLOCKBITS - LENGTHBIT)){//have a space for length
		counterIncreaser(counter, message_block_size_bit);//counter increase
		firstPartZeroPadding(&message_block[message_block_size_bit/WORDBITS], message_block_size_bit%WORDBITS);//zero padding first part		
		for(i = message_block_size_bit/WORDBITS + 1; i < MESSAGEBLOCKWORDS-COUNTERSIZE; i++){//normal zero pedding
			message_block[i] = 0;
		}
		for(i= MESSAGEBLOCKWORDS-COUNTERSIZE,  j=0; i< MESSAGEBLOCKWORDS && j<COUNTERSIZE;i++,j++){//length part
			message_block[i] = counter[j];
		}
		return 0;//finish
	}
	else if(message_block_size_bit>(MESSAGEBLOCKBITS - LENGTHBIT) && message_block_size_bit<MESSAGEBLOCKBITS){
		firstPartZeroPadding(&message_block[message_block_size_bit/WORDBITS], message_block_size_bit%WORDBITS);
		for(i = message_block_size_bit/WORDBITS + 1; i < MESSAGEBLOCKWORDS-COUNTERSIZE; i++){//normal zero pedding
			message_block[i] = 0;
		}
		return 1;
	}
	else if(message_block_size_bit == MESSAGEBLOCKBITS){//full message
		counterIncreaser(counter, message_block_size_bit);//counter increase
		return 1;
	}
	else if(message_block_size_bit > MESSAGEBLOCKBITS){//inpossible case
		return -1;
	}

	//truely error case
	return -1;
}
int counterIncreaser(unsigned long long int * counter, int increase_number){
	int i = 1;
	unsigned long long int old_counter = counter[COUNTERSIZE-i];
	counter[COUNTERSIZE-i] += (unsigned long long int)increase_number;
	if (counter[COUNTERSIZE-i]< old_counter){//overflow
		i += 1;
		if(i>COUNTERSIZE)
			return -1;
		counter[COUNTERSIZE - i] +=1;//increase
		while(counter[COUNTERSIZE - i] == 0){//overflow case
			i += 1;
			if(i>COUNTERSIZE)//error case
				return -1;
			counter[COUNTERSIZE-i] += 1;
		}
	}
	return 1;
}
int firstPartZeroPadding(unsigned long long int * target, int message_size_bit){
	unsigned long long int pedding = (unsigned long long int)((long long int)(-1));
	if(message_size_bit < 0)
		return -1;
	*target = *target & (pedding<<(WORDBITS-message_size_bit));//erase the do not need	
	pedding = ((unsigned long long int)(1)) <<(WORDBITS-message_size_bit-1);
	*target = *target | pedding;
	return 1;
}
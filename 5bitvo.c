/*
 * -Program made by: Thien Vo
 * -Program name: 5bit
 * -EID: thv92
 *
 *
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 FILE *input;
 typedef char *STRING;
 int encryptionValue = 65;
 
//Boolean definitions
 typedef short Boolean;
#define TRUE 1
#define FALSE 0

//Function prototypes
 void encoder();
 void decoder();
 void printEncryptedCharacter(int character);
 int printBreak(int breaker);

 int main(int argc, char **argv){

	//Rough coding time

	Boolean instruction = TRUE;  //1 = encode 0 = decode
	Boolean fileFound = FALSE;

	while(argc > 1 && !fileFound){
		argc--, argv++;

		if( **argv== '-'){ 	
			instruction = FALSE;
		}else{

			input = fopen(*argv,"r");
			if(input == NULL){
				printf("Error opening file\n");
				return 0;
			}
			fileFound = TRUE;
		}
	}


	if(instruction){
		encoder();
		fclose(input);
	}else{
		decoder();
		fclose(input);
	}

	return 0;
}



void encoder(){

	/*Use while loop till EOF to get chars
      
	*/

	int letter;
	int lineBreak = 0;

	int arrEight[8];   		//8 bit array
	int eightCount =0; 		//8 bit container index

	int arrFive[5];    		//5 bit container
	int fiveCount = 0; 		//5bit container index
	int fiveShift = 4;

	int index; 				//temporary index for 8 bit container
	int shifter;   			//Shifts right bit by bit to put inside 8bit container
	int bitTransfer;  		//In preparation for sending over to 5 bit container
	int outActual;  		//Result after combining filled 5 bit container
	int tempEightIndex; 	//Index tracks position in 8 bit container
	
	int i, j, k;          	//Default index for for loops

	while((letter = getc(input)) != EOF){
		//8 bits from letter inserted into arrEight
		index = 0;

		while(index < 8){
			//shifter allows for inserting individual bits inside 8bit array
			shifter = 7-index;
			
			//Stores individual bits
			arrEight[index] = letter >> shifter;
			arrEight[index] &= 0x1;

			eightCount ++;
			index++;
		}

  		//8 bit array filled, transfering to 5 bit array.
		tempEightIndex =0;
		while(eightCount > 0){

  			//invidivual bits within the 8 bit array
			bitTransfer = arrEight[tempEightIndex];
			
			arrFive[fiveCount] = bitTransfer;
			fiveCount++;
			
			//Once 5bit array fills up
			if(fiveCount == 5){
				outActual = 0;
				lineBreak++;   					//Wraps long prints | Max = 72
		  		
		  		//Use for loop to pour those 5bits into a single int variable.
				for(i = 0; i < 5; i++){
					fiveCount--;
					outActual |= arrFive[i] << (fiveShift-i) ;
				}

				printEncryptedCharacter(outActual);
			}
			
			tempEightIndex++;
			eightCount --;
			
		}
	
	//Break line
	lineBreak = printBreak(lineBreak);
	}

	//Remaining Bits
	if(fiveCount > 0){
		lineBreak++;
		outActual = 0;
		
		for(i = 0; i < fiveCount; i++)
			outActual |= arrFive[i] << (fiveShift-i);
			
			//4-fiveCount
		for(j = fiveShift-fiveCount; j > 0; j--)
			outActual |= 0 << (j);
		
		printBreak(lineBreak);
		printEncryptedCharacter(outActual);
		printf("\n");
	}
}

void printEncryptedCharacter(int character){
	if((character+encryptionValue) > 'Z'){
		character -= 26;
		printf("%d",character);
	}else{
	    printf("%c",character+encryptionValue );			
	}
}

int printBreak(int breaker){
	if(breaker > 71){
		printf("\n");
		return 0;
	}
	return breaker;
}


void decoder(){

	int letter;
	int lineBreak = 0;
	int newlineConst = 10;

	int arrFive[5];   			//5 bit array
	int fiveCount =0; 			//5 bit container index


	int arrEight[8];    		//8 bit container
	int eightCount = 0; 		//8bit container index
	int fiveShift = 4;

	int index; 					//temporary index for 5 bit container
	int outActual;  			//Result after 8 bit container is full
	int tempFiveIndex; 			//Index tracks position in 8 bit container
	
	int tempVal;  	            //Temporary Values
	int i;          			//Default index for for loops

	while((letter = getc(input)) != EOF){

		//Incase of new Line
	    if(letter != newlineConst){

			if(letter >= encryptionValue)
		   		letter -= encryptionValue; 
			else     
				letter -=22; 		//Value needed to get actual int

		//Poured targeted 5 bits into the container
		index = 0;
		while(index < 5){
			tempVal = fiveShift - index;
			arrFive[index] = (letter >> tempVal) & 1;

			fiveCount++;
			index++;
		}

		// Pour from 5bit container to 8bit container
		tempFiveIndex = 0;
		while(fiveCount > 0){

			arrEight[eightCount] = arrFive[tempFiveIndex];
			eightCount++;

			//Cond: if 8bit container is full
			if(eightCount == 8){
				outActual = 0;

				for(i = 0; i < eightCount; i++){
					outActual |= (arrEight[i] << (7-i));

			    }

			    eightCount = 0;
			    printf("%c",outActual );

			}
			tempFiveIndex++;
			fiveCount--;
		}
	  }

	}


}










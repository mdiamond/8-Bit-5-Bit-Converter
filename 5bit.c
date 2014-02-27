/*

Student/Assignment Information

CS 429 Lab 2
5bit.c

Name: Matthew Diamond
UTEID: med2425
Unique #: 53870

*/

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

/*Includes and defined data types or variables*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef short Boolean;
#define TRUE 1
#define FALSE 0

typedef char *STRING;

FILE *input;

Boolean d_option = FALSE; /*decode*/

/*function prototypes*/
void writeChar(int ch);
int newLine(int charsOnLine);

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

void scanargs(char *s){
  /* check each character of the option list for its meaning. */
  while (*s != '\0')
    switch (*s++){
    case 'd': /*decode option*/
      d_option = TRUE;
      break;
    }
}

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

void encode(FILE *input){
  int letter; //the current 8 bit chunk being worked with from the file
  int letter5; //the current 5 bit chunk to be written out

  int bitArray8[8]; //temporary storage for a 8 bit or smaller chunk
  int bitArray5[5]; //temporary storage for a 5 bit or smaller chunk
  int size8 = 0; //# of bits in bitArray8
  int size5 = 0; //# of bits in bitArray5


  int index1; //will reach 7 when bitArray8 is full
  int index2; //will reach 4 when bitArray5 is full

  int mask = 0x1; //use to mask a single bit at a time

  int numCharsOut = 0; //use to track how many characters have been written out
  int charsOnLine = 0;

  int shift; //use to shift the current letter for masking
  int shift5 = 4; //use to shift the current bit back into position after masking

  int i; //generic index for loops

  Boolean empty = TRUE; //marker for empty file

  while((letter = getc(input)) != EOF){ //store next 8 bit chunk that is not an EOF
    empty = FALSE;
    index1 = 0; //reset the # of bits currently in bitArray8
    while(index1 < 8){ //while bitArray8 is not full, place a single bit into bitArray8
      shift = 7-index1; //set the shift amount
      bitArray8[index1] = (letter>>shift)&mask; //transfer the result of the mask to bitArray8 in its appropriate location
      size8 ++;
      index1 ++;
    }
    index2 = 0; //reset the # of bits currenty in bitArray5
    while(size8 > 0){ //while there are bits left to transfer to bitArray5
      bitArray5[size5] = bitArray8[index2]; //while bitArray8 is not empty, place a single bit into bitArray5 per time
      size5 ++;
      size8 --;
      index2 ++;
      if(size5 == 5){ //if bitArray 5 has been filled, write out
        letter5 = 0;
        for(i = 0; i < 5; i++){
          letter5 = letter5 | (bitArray5[i] << (shift5 - i));
          size5 --;
        }
        writeChar(letter5);
        charsOnLine ++;
      }
    }
    charsOnLine = newLine(charsOnLine);
  }
  if(size5 > 0){ //if there are leftover bits, write them out
    charsOnLine ++;
    letter5 = 0;
    for(i = 0; i < size5; i++){
        letter5 = letter5 | (bitArray5[i] << (shift5 - i));
    }
    for(i = shift5-size5; i > 0; i--){
      letter5 = letter5 | (0 << i);
    }
    newLine(charsOnLine);
    writeChar(letter5);
  }
  if(empty == FALSE) {
    if(charsOnLine != 0) {
      printf("\n");
    }
  }
}

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

void decode(){
  int letter; //the current 5 bit chunk being worked with from the file
  int letter8; //the current 8 bit chunk to be written out

  int bitArray8[8]; //temporary storage for a 8 bit or smaller chunk
  int bitArray5[5]; //temporary storage for a 5 bit or smaller chunk
  int size8 = 0; //# of bits in bitArray8
  int size5 = 0; //# of bits in bitArray5

  int shift5 = 4; //use to shift the current bit back into position after masking

  int index1; //will reach 4 when bitArray5 is full
  int index2; //will reach 7 when bitArray8 is full

  int i; //generic index for loops

  int temp; //temporary container for a shift amount

  while((letter = getc(input)) != EOF){ //store next 5 bit chunk that is not an EOF
    if(letter != 10){ //ignore new lines
      if(letter >= 65){ //if/else to correct the transformation from encode
        letter = letter - 65;
      }
      else{
        letter = letter - 22;
      }
      index1 = 0; //reset the # of bits currently in bitArray5
      while(index1 < 5){ //while bitArray5 is not full, fill it with bits
        size5 ++;
        bitArray5[index1] = (letter >> (shift5 - index1)) & 1;
        index1 ++;
      }
      index2 = 0; //reset the # of bits in bitArray8
      while(size5 > 0){ //while bitArray5 is not empty, move bits over to bitArray8
        bitArray8[size8] = bitArray5[index2];
        size8 ++;
        if(size8 == 8){ //if bitArray8 is full, write out
          letter8 = 0;
          for(i = 0; i < size8; i++){
            letter8 = letter8 | (bitArray8[i] << (7-i));
          }
          size8 = 0;
          printf("%c", letter8);
        }
        index2 ++;
        size5 --;
      }
    }
  }
}


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

//helper method to decide whether or not to write out a new line as well as possibly reset the tracker for how many chars have been written out
int newLine(int charsOnLine){
  if(charsOnLine == 72){
    printf("\n");
    return 0;
  }
  return charsOnLine;
}

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

//helper method to transform and write out an encoded character
void writeChar(int ch){
  if(ch + 65 > 'Z'){
    printf("%d", ch - 26);
  }
  else{
    printf("%c", ch + 65);
  }
}



int main(int numArgs, STRING *args){
  Boolean filenamenotgiven = TRUE;

  if(numArgs == 1){
    input = stdin;
    encode(input);
  }

  while (numArgs > 1){
    numArgs--, args++;
    if (**args == '-'){
      scanargs(*args);
    }
    else{
      filenamenotgiven = FALSE;
      input = fopen(*args,"r");
      if (input == NULL){
        fprintf (stderr, "Can't open %s\n",*args);
      }
      else{
        if(d_option) {
          decode();
        }
        else{
          encode(input);
        }
        fclose(input);
      }
    }
  }
  if (filenamenotgiven){
    input = stdin;
    if(d_option) {
      decode();
    }
    else{
     encode(input);
    }
  }
  exit(0);
}

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

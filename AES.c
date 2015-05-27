/* itoa example */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "AES.h"

int i,j,k;






void Add_key(unsigned char state[4][4], unsigned char temp_key[4][4])
{
  for(i = 0; i < 4; i++) {
    for(j = 0; j < 4; j++) {
      state[i][j] = state[i][j] ^ temp_key[i][j];
    }
  }
}

void key_generate(unsigned char key[4][4], unsigned char expan_key[4][44])
{

  int rloop = 0;
  printf("\n RCON \n");
  for(i = 0; i < 44; i++)
  {
    if(i < 4)
    {
      for(j = 0; j < 4; j++)
      {
        expan_key[j][i] = key[j][i];
      }
    }

    else if(i % 4 == 0)
    {

      unsigned char temp[4] = {expan_key[1][i-1], expan_key[2][i-1], expan_key[3][i-1], expan_key[0][i-1]};
      //	printf("%#X  %#X  %#X  %#X  \n",temp[0],temp[1],temp[2],temp[3]);
      int index;
      for(index = 0; index < 4; index++)
      {
        temp[index] = sbox[temp[index]];
      }
      //	printf("%#X  %#X  %#X  %#X  \n",temp[0],temp[1],temp[2],temp[3]);
      index =0;
     
          expan_key[index][i] = temp[index] ^ Rcon[rloop] ^ expan_key[index][i-4];
          expan_key[index+1][i] = temp[index+1] ^ expan_key[index+1][i-4];
          expan_key[index+2][i] = temp[index+2] ^ expan_key[index+2][i-4];
		  expan_key[index+3][i] = temp[index+3] ^ expan_key[index+3][i-4];
     
      rloop++;
    }
    else
    {
      unsigned char temp[4] = {expan_key[0][i-1], expan_key[1][i-1], expan_key[2][i-1], expan_key[3][i-1]};
      int index;
      for(index = 0; index < 4; index++)
        temp[index] ^= expan_key[index][i-4];

      for(index = 0; index < 4; index++)
        expan_key[index][i] = temp[index];
    }
  }

  printf("\n");

  printf("\n Key expansion \n");

  printf("\n");

  for(i = 0; i < 4; i++){
    for(j = 0; j < 44; j++){
      printf("%#X",expan_key[i][j]);
      printf("\t");
    }
    printf("\n");
  }
}


void subByte(unsigned char state[4][4])
{

  for(i=0;i<4;i++){
    for(j=0;j<4;j++){

      state[i][j]=sbox[state[i][j]];
    }
  }


  //	 printf("location value %#x ",Sb[num]);
}

void shiftRaw(unsigned char state[4][4])
{
  char tmp;
  tmp=state[1][3] ;
  state[1][3]=state[1][0];
  state[1][0]=state[1][1];
  state[1][1]=state[1][2];
  state[1][2]=tmp;

  tmp=state[2][0] ;
  state[2][0]=state[2][2];
  state[2][2]=tmp;
  tmp=state[2][3];
  state[2][3]=state[2][1];
  state[2][1]=tmp;

  tmp=state[3][0] ;
  state[3][0]=state[3][3];
  state[3][3]=state[3][2];
  state[3][2]=state[3][1];
  state[3][1]=tmp;

}

void mixcolumn(unsigned char state[4][4], unsigned char temp[4][4])
{



  unsigned char coumn[4][4]={{0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}};

  for(i=0;i<4;i++)
    for(j=0;j<4;j++)
      temp[i][j]=0x00;

  unsigned  char tem=0x00;
  // unsigned char tem1=0x00;
  for(j=0;j<4;j++)
  {

    for (i=0;i<4;i++)
    {
      for(k=0;k<4;k++)
      {
        if(coumn[j][k]==0x01)
          tem =state[k][i] ;
        if(coumn[j][k]==0x02)
        {
          
            tem =(state[k][i]<<1)^((state[k][i]/0x80)*0x11b)  ;
          
        }
        if(coumn[j][k]==0x03)
        {
          
            tem =((state[k][i]<<1)^state[k][i])^((state[k][i]/0x80)*0x11b)  ;
          
        }
        //   tem1 ^=(state[k][i]*coumn[j][k])%(0x11B);
        temp[j][i] = temp[j][i]^(tem);
        //   printf("%#X  %#X ",state[k][i],coumn[j][k]);
        // printf("%#X  ",tem);
        // printf("\t");
      }

      //printf("%#X  %#X\n",temp[j][i]);
    }
  }


}

void print(unsigned char print[4][4])
{
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      //  state[i][j]=state1[j][i];
      printf("%#X",print[i][j]);
      printf("\t");

    }
    printf("\n");
  }
  printf("\n");
}

//int main ()
//{
//
//
//  unsigned char state[4][4] = {{0x32,0x88,0x31,0xE0},{0x43,0x5A,0x31,0x37},{0xF6,0x30,0x98,0x07},{0xa8,0x8D,0xA2,0x34}};
//  unsigned char key[4][4]={{0x2B,0x28,0xAB,0x09},{0x7E,0xAE,0xF7,0xCF},{0x15,0xD2,0x15,0x4F},{0x16,0xA6,0x88,0x3C}};
//  unsigned char expan_key[4][44];
//  unsigned char temp[4][4];
//  //============1. Read user text into matrix=================
//
//
//  //Each letter is 1 byte, text should be <= 16 byte (128 bit). TextNofBits%128 =0 ?
//  //What if user entered less than 128 bits?
//
//  /*printf("Enter Plain Text:  ");
//    scanf("%s",*state);
//
//  //State length condition (16 letters only)
//  printf("\n TEXT IN HEX \n");
//
//  //PRINT IN HEX
//  unsigned char state1[4][4]={{0x00, 0x00, 0x00, 0x00},
//  {0x00, 0x00, 0x00, 0x00},
//  {0x00, 0x00, 0x00, 0x00},
//  {0x00, 0x00, 0x00, 0x00}};
//  for(i=0;i<4;i++){
//  for(j=0;j<4;j++){
//  state1[i][j]=state[i][j];
//  }
//  }
//  */
//
//  print(state);
//  printf("\n KEY IN HEX \n");
//
//
//
//
//  print(key);
//
//  printf("\n");
//  printf("\n");
//  key_generate(key,expan_key);
//
//  printf("\n");
//  printf("\n");
//  //=================2. Add round key Correct ================
//
//
//
//  Add_key(state,key);
//
//  printf("\n AFTER ADDING ROUND KEY \n");
//
//  print(state);
//
//  int start=4;
//  int loop;
//  unsigned char temp_key[4][4];
//  //=================3. n round ================
//
//  for(loop=1;loop<11;loop++)
//  {
//    printf("\n State %d \n",loop);
//
//    print(state);
//
//
//
//
//    for(i=0;i<4;i++)
//    {
//      for(j=0;j<4;j++)
//      {
//        temp_key[j][i]=expan_key[j][loop*start+i];
//      }
//
//    }
//
//
//    printf("\n key %d \n",loop);
//
//
//    print(temp_key);
//    //=================3.a. SubByte ================
//
//
//    subByte(state);
//
//    printf("\n AFTER SubByte\n");
//
//    print(state);
//
//    printf("\n");
//
//
//    //=================3.b. Shift raw Correct ================
//
//    shiftRaw(state);
//
//
//    printf("\n AFTER shift raw \n");
//
//    print(state);
//
//    printf("\n");
//
//
//    //=================3.c. mix Column Correct ================
//    if(loop!=10)
//    {
//      mixcolumn(state,temp);
//
//      printf("\n AFTER MIX COLUMN \n");
//
//      for(j=0;j<4;j++)
//      {
//        for (i=0;i<4;i++)
//        {
//          state[j][i]=temp[j][i];
//
//        }
//      }
//    }
//
//    print(state);
//
//    //=================3.d. Add round key ================
//
//
//    Add_key(state,temp_key);
//    /*for(i=0;i<4;i++){
//      for(j=0;j<4;j++){
//      state[i][j]= state[i][j] ^ temp_key[i][j];
//      }
//      }
//      */
//
//    printf("\n AFTER ADDING ROUND KEY %d\n",loop);
//
//    print(state);
//
//    printf("\n");
//
//
//
//
//  }
//
//  return 0;
//
//
//
//}

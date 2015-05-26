/* itoa example */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "AES.h"

/*
//Optimization To be discussed: 

//1. Doing 1D array to avoid nested loop
char s[16]={0x32,0x88,0x31,0xE0,0x43,0x5A,0x31,0x37,0xF6,0x30,0x98,0x07,0xa8,0x8D,0xA2,0x34};
char k[16]= {0x2B,0x28,0xAB,0x09,0x7E,0xAE,0xF7,0xCF,0x15,0xD2,0x15,0x4F,0x16,0xA6,0x88,0x3C};
//- Add round key: 
int i;
for(i=0;i<16;i++)
s[i] ^=k[i];
//-subbyte: 
int i;
for(i=0;i<16;i++)
s[i] =sbox[s[i]];
//-shift row:
int temp;
temp=s[7]; s[7]=s[4]; s[4]=s[5]; s[5]=s[6]; s[6]=temp;
temp=s[10]; s[10]=s[8]; s[8]=temp; temp=s[11]; s[11]=s[9]; s[9]=temp;
temp=[s13]; s[13]=s[12]; s[12]=s[15]; s[15]=s[14]; s[14]=temp;
 
//-mix columns

// 01 don't change 
//02 if left most bit is 1 >> 1 bit shift left then xor with 0x1B
//03 multiply by 2, then XOR result with original
//XOR results
//copy column
//Q: Does applying & change the number in the memory?
char c[4]={s[0],s[4],s[8],s[12]}
char t;
if( c[0]&0x80)
s[0] = (s[0]<<=1)^0x1B;
t = c[1]
if(c[0]&0x80)
s[0] ^=(((t<<=1)^0x1b)^c[1])^c[2]^c[3];
else 
s[0] ^=c[1]^c[2]^c[3];

//===================






*/



void Add_key(unsigned char state[4][4], unsigned char temp_key[4][4])
{
	int i,j,k;

  for(i = 0; i < 4; i++) {
    for(j = 0; j < 4; j++) {
      state[i][j] = state[i][j] ^ temp_key[i][j];
    }
  }
}

void key_generate(unsigned char key[4][4], unsigned char expan_key[4][44])
{
	int i,j,k;
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
	int i,j,k;
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){

      state[i][j]=sbox[state[i][j]];
    }
  }


  //	 printf("location value %#x ",Sb[num]);
}

void shiftRaw(unsigned char state[4][4])
{
	int i,j,k;

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


	int i,j,k;

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
          if (state[k][i]&0x80)
          {
            //	printf("%#X jj ",state[k][i]);
            tem =(state[k][i]<<1)^0x1b ;
          }
          else
            tem =(state[k][i]<<1);
        }
        if(coumn[j][k]==0x03)
        {
          if (state[k][i]&0x80)
            tem =((state[k][i]<<1)^state[k][i])^0x1b ;
          else
            tem =((state[k][i]<<1)^state[k][i]);
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
	int i,j,k;
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


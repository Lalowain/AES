/* itoa example */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "AES.h"








void Add_key(unsigned char state[4][4], unsigned char temp_key[4][4])
{
	int i,j;

  for(i = 0; i < 4; i++) {
    for(j = 0; j < 4; j++) {
      state[i][j] = state[i][j] ^ temp_key[i][j];
    }
  }
}

void key_generate(unsigned char key[4][4], unsigned char expan_key[4][44])
{
	int i,j;
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
      int index;
      for(index = 0; index < 4; index++)
      {
        temp[index] = sbox[temp[index]];
      }

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
	int i,j;
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){

      state[i][j]=sbox[state[i][j]];
    }
  }



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


	int i,j,k;

  unsigned char coumn[4][4]={{0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}};

  for(i=0;i<4;i++)
    for(j=0;j<4;j++)
      temp[i][j]=0x00;

  unsigned  char tem=0x00;

  for(j=0;j<4;j++)
  {

    for (i=0;i<4;i++)
    {
      for(k=0;k<4;k++)
      {

          tem =((state[k][i]<< (coumn[j][k]>>1))^(state[k][i]*(coumn[j][k]/0x03)))^(((state[k][i]/0x80)*0x1b )*(coumn[j][k]/0x02)) ;



        temp[j][i] = temp[j][i]^(tem);

      }

    }
  }


}

void print(unsigned char print[4][4])
{
	int i,j;
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){

      printf("%#X",print[i][j]);
      printf("\t");

    }
    printf("\n");
  }
  printf("\n");
}


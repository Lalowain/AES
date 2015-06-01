/* itoa example */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <Math.h>
#include "AES.h"



void Add_key(unsigned char state[4][4], unsigned char temp_key[4][44],int index)
{
	int i,j;

  for(i = 0; i < 4; i++) {
    for(j = 0; j < 4; j++) {
      state[i][j] = state[i][j] ^ temp_key[i][index+j];
    }
  }
}


void key_generate(unsigned char key[4][4], unsigned char expan_key[4][44])
{
	int i,j;
  int rloop = 0;

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


void rev_subbytes(unsigned char state[4][4])
{
	int i,j;
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){

      state[i][j]=rsbox[state[i][j]];
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

void rev_shiftrows(unsigned char state[4][4])
{


  char tmp;
  tmp=state[3][3] ;
   state[3][3]=state[3][0];
   state[3][0]=state[3][1];
   state[3][1]=state[3][2];
   state[3][2]=tmp;

   tmp=state[2][0] ;
   state[2][0]=state[2][2];
   state[2][2]=tmp;
   tmp=state[2][3];
   state[2][3]=state[2][1];
   state[2][1]=tmp;

   tmp=state[1][0] ;
   state[1][0]=state[1][3];
   state[1][3]=state[1][2];
   state[1][2]=state[1][1];
   state[1][1]=tmp;

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

          tem =((state[k][i]<< (coumn[j][k]>>1))^(state[k][i]*((coumn[j][k]>>1)&coumn[j][k])))^(((state[k][i]>>7)*0x1b )*(coumn[j][k]>>1)) ;


        temp[j][i] = temp[j][i]^(tem);

      }

    }
  }



  for(i=0;i<4;i++)
      for(j=0;j<4;j++)
        state[i][j]=temp[i][j];

}


void rev_mixcolumn(unsigned char state[4][4], unsigned char temp[4][4])
{


	int i,j,k;

  unsigned char coumn[4][4]={{0x0E, 0x0B, 0x0D, 0x09},
    {0x09, 0x0E, 0x0B, 0x0D},
    {0x0D, 0x09, 0x0E, 0x0B},
    {0x0B, 0x0D, 0x09, 0x0E}};

  unsigned  char BYTE_LEFT_BIT = 0x80;
  unsigned  char BYTE_Right_BIT = 0x01;


  		 unsigned  char a,b=0x00;
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
    	  tem=0x00;
            b=state[k][i];
            a=coumn[j][k];
	tem ^=(b*(a & BYTE_Right_BIT));
                       a>>=1;
    	  			(((b1 =((((b<<1)^(0x1b*((b & BYTE_LEFT_BIT)/BYTE_LEFT_BIT))))))));
    	  			tem ^=(b1*(a & BYTE_Right_BIT));
    	  			a>>=1;
	            	(((b2 =((((b1<<1)^(0x1b*((b1 & BYTE_LEFT_BIT)/BYTE_LEFT_BIT))))))));
	            	tem ^=(b2*(a & BYTE_Right_BIT));
	            	a>>=1;
	            	(((b3 =((((b2<<1)^(0x1b*((b2 & BYTE_LEFT_BIT)/BYTE_LEFT_BIT))))))));
	            	tem ^=(b3*(a & BYTE_Right_BIT));


        temp[j][i] = temp[j][i]^(tem);

      }

    }
  }


  for(i=0;i<4;i++)
      for(j=0;j<4;j++)
        state[i][j]=temp[i][j];

}

void encryption(unsigned char state[4][4], unsigned char expan_key[4][44])
{

	Add_key(state,expan_key,0);


	int start=4;
	int loop;
	unsigned char temp[4][4];

	for(loop=1;loop<10;loop++)
	{

	subByte(state);

	shiftRaw(state);

	mixcolumn(state,temp);


	Add_key(state,expan_key,(int)(loop*start));

	}

	subByte(state);
	shiftRaw(state);
	Add_key(state,expan_key,(int)(loop*start));


}

void decryption(unsigned char state[4][4], unsigned char expan_key[4][44])
{
	int start=4;
		int loop;
		unsigned char temp[4][4];

		Add_key(state,expan_key,(40));


		for(loop=9;loop>0;loop--)
		{

			rev_shiftrows(state);
		    rev_subbytes(state);

			Add_key(state,expan_key,(loop*start));

		    rev_mixcolumn(state,temp);

			}

		rev_shiftrows(state);
		rev_subbytes(state);
		Add_key(state,expan_key,(0));
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


void prints(unsigned char print[4][44],int index)
{
	int i,j;
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){

      printf("%#X",print[i][index+j]);
      printf("\t");

    }
    printf("\n");
  }
  printf("\n");
}

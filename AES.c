#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
   tmp=state[1][0] ;
   state[1][0]=state[1][3];
   state[1][3]=state[1][2];
   state[1][2]=state[1][1];
   state[1][1]=tmp;

   tmp=state[2][0] ;
   state[2][0]=state[2][2];
   state[2][2]=tmp;
   tmp=state[2][3];
   state[2][3]=state[2][1];
   state[2][1]=tmp;
   
   tmp=state[3][3] ;
   state[3][3]=state[3][0];
   state[3][0]=state[3][1];
   state[3][1]=state[3][2];
   state[3][2]=tmp;
   

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

          tem =((state[k][i]<< (coumn[j][k]>>1))^(state[k][i]&-((coumn[j][k]>>1)&coumn[j][k])))^((0x1b & -(state[k][i]>>7) )&-(coumn[j][k]>>1)) ;


        temp[j][i] = temp[j][i]^(tem);

      }
     
    }
	
  }

    for(i=0;i<4;i++)
      for(j=0;j<4;j++)
        state[i][j]=temp[i][j];
        

}

 void mixColumns2 (unsigned char state[4][4]) {
 int c , i;
    for ( c=0; c<4; c++) {
        unsigned char b1[4]  ;  // 'b1' is a copy of the current column from 'state'
        unsigned char b2[4];  // 'b2' is a•{02} in GF(2^8)

        
            b1[0] = state[0][c];
            b2[0] = b1[0]<<1 ^ (0x1b &-(b1[0]>>7) );
			
            b1[1] = state[1][c];
            b2[1] = b1[1]<<1 ^ (0x1b &-(b1[1]>>7) );
			
			b1[2] = state[2][c];
            b2[2] = b1[2]<<1 ^ (0x1b &-(b1[2]>>7) );
			
			b1[3] = state[3][c];
            b2[3] = b1[3]<<1 ^ (0x1b &-(b1[3]>>7) );
			
			
		 // b1[n] ^ b2[n] is a•{03} , b2[n] is a•{02} , b1[n]  is a•{01}in GF(2^8)
        state[0][c] = b2[0] ^ (b1[1] ^ b2[1]) ^ b1[2] ^ b1[3]; // {02}•a0 + {03}•a1 + a2 + a3
        state[1][c] = b1[0] ^ b2[1] ^ (b1[2] ^ b2[2]) ^ b1[3]; // a0 • {02}•a1 + {03}•a2 + a3
        state[2][c] = b1[0] ^ b1[1] ^ b2[2] ^ (b1[3] ^ b2[3]); // a0 + a1 + {02}•a2 + {03}•a3
        state[3][c] = (b1[0] ^ b2[0]) ^ b1[1] ^ b1[2] ^ b2[3]; // {03}•a0 + a1 + a2 + {02}•a3
		

    }
    
}
void mixColumns3 (unsigned char state[4][4]) {
     unsigned char raw[4];

	// raw 1
	raw[0] = state[0][0];
	raw[1] = state[1][0];
	raw[2] = state[2][0];
	raw[3] = state[3][0];
	state[0][0] = mixcolumn_lookup[raw[0]][0];
	state[0][0] ^= mixcolumn_lookup[raw[1]][1];
	state[0][0] ^= raw[2];
	state[0][0] ^= raw[3];
	state[1][0] = raw[0];
	state[1][0] ^= mixcolumn_lookup[raw[1]][0];
	state[1][0] ^= mixcolumn_lookup[raw[2]][1];
	state[1][0] ^= raw[3];
	state[2][0] = raw[0];
	state[2][0] ^= raw[1];
	state[2][0] ^= mixcolumn_lookup[raw[2]][0];
	state[2][0] ^= mixcolumn_lookup[raw[3]][1];
	state[3][0] = mixcolumn_lookup[raw[0]][1];
	state[3][0] ^= raw[1];
	state[3][0] ^= raw[2];
	state[3][0] ^= mixcolumn_lookup[raw[3]][0];
	// raw 2
	raw[0] = state[0][1];
	raw[1] = state[1][1];
	raw[2] = state[2][1];
	raw[3] = state[3][1];
	state[0][1] = mixcolumn_lookup[raw[0]][0];
	state[0][1] ^= mixcolumn_lookup[raw[1]][1];
	state[0][1] ^= raw[2];
	state[0][1] ^= raw[3];
	state[1][1] = raw[0];
	state[1][1] ^= mixcolumn_lookup[raw[1]][0];
	state[1][1] ^= mixcolumn_lookup[raw[2]][1];
	state[1][1] ^= raw[3];
	state[2][1] = raw[0];
	state[2][1] ^= raw[1];
	state[2][1] ^= mixcolumn_lookup[raw[2]][0];
	state[2][1] ^= mixcolumn_lookup[raw[3]][1];
	state[3][1] = mixcolumn_lookup[raw[0]][1];
	state[3][1] ^= raw[1];
	state[3][1] ^= raw[2];
	state[3][1] ^= mixcolumn_lookup[raw[3]][0];
	// raw 3
	raw[0] = state[0][2];
	raw[1] = state[1][2];
	raw[2] = state[2][2];
	raw[3] = state[3][2];
	state[0][2] = mixcolumn_lookup[raw[0]][0];
	state[0][2] ^= mixcolumn_lookup[raw[1]][1];
	state[0][2] ^= raw[2];
	state[0][2] ^= raw[3];
	state[1][2] = raw[0];
	state[1][2] ^= mixcolumn_lookup[raw[1]][0];
	state[1][2] ^= mixcolumn_lookup[raw[2]][1];
	state[1][2] ^= raw[3];
	state[2][2] = raw[0];
	state[2][2] ^= raw[1];
	state[2][2] ^= mixcolumn_lookup[raw[2]][0];
	state[2][2] ^= mixcolumn_lookup[raw[3]][1];
	state[3][2] = mixcolumn_lookup[raw[0]][1];
	state[3][2] ^= raw[1];
	state[3][2] ^= raw[2];
	state[3][2] ^= mixcolumn_lookup[raw[3]][0];
	// raw 4
	raw[0] = state[0][3];
	raw[1] = state[1][3];
	raw[2] = state[2][3];
	raw[3] = state[3][3];
	state[0][3] = mixcolumn_lookup[raw[0]][0];
	state[0][3] ^= mixcolumn_lookup[raw[1]][1];
	state[0][3] ^= raw[2];
	state[0][3] ^= raw[3];
	state[1][3] = raw[0];
	state[1][3] ^= mixcolumn_lookup[raw[1]][0];
	state[1][3] ^= mixcolumn_lookup[raw[2]][1];
	state[1][3] ^= raw[3];
	state[2][3] = raw[0];
	state[2][3] ^= raw[1];
	state[2][3] ^= mixcolumn_lookup[raw[2]][0];
	state[2][3] ^= mixcolumn_lookup[raw[3]][1];
	state[3][3] = mixcolumn_lookup[raw[0]][1];
	state[3][3] ^= raw[1];
	state[3][3] ^= raw[2];
	state[3][3] ^= mixcolumn_lookup[raw[3]][0];
		

    
    
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


  		 unsigned  char a,b,b1,b2,b3=0x00;
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
    	  			(((b1 =((((b<<1)^(0x1b &-((b & BYTE_LEFT_BIT)/BYTE_LEFT_BIT))))))));
    	  			tem ^=(b1 &-(a & BYTE_Right_BIT));
    	  			a>>=1;
	            	(((b2 =((((b1<<1)^(0x1b &-((b1 & BYTE_LEFT_BIT)/BYTE_LEFT_BIT))))))));
	            	tem ^=(b2 &-(a & BYTE_Right_BIT));
	            	a>>=1;
	            	(((b3 =((((b2<<1)^(0x1b &-((b2 & BYTE_LEFT_BIT)/BYTE_LEFT_BIT))))))));
	            	tem ^=(b3 &-(a & BYTE_Right_BIT));


        temp[j][i] = temp[j][i]^(tem);

      }

    }
  }


  for(i=0;i<4;i++)
      for(j=0;j<4;j++)
        state[i][j]=temp[i][j];

}

 void revmixColumns2 (unsigned char state[4][4]) {
 int c , i;
    for ( c=0; c<4; c++) {
        unsigned char b1[4] ;    // 'b1' is a copy of the current column from 'state'
        unsigned char b2[4];   // 'b2' is a•{02} in GF(2^8)
		unsigned char b4[4];  // 'b4' is a•{04} in GF(2^8)
		unsigned char b8[4]; // 'b8' is a•{08} in GF(2^8)

        
            b1[0] = state[0][c];
            b2[0] = b1[0] <<1 ^ (0x1b &-(b1[0]>>7) );
			b4[0] = b2[0] <<1 ^ (0x1b &-(b2[0]>>7) );
			b8[0] = b4[0] <<1 ^ (0x1b &-(b4[0]>>7) );
			
			b1[1] = state[1][c];
            b2[1] = b1[1] <<1 ^ (0x1b &-(b1[1]>>7) );
			b4[1] = b2[1] <<1 ^ (0x1b &-(b2[1]>>7) );
			b8[1] = b4[1] <<1 ^ (0x1b &-(b4[1]>>7) );
			
			b1[2] = state[2][c];
            b2[2] = b1[2] <<1 ^ (0x1b &-(b1[2]>>7) );
			b4[2] = b2[2] <<1 ^ (0x1b &-(b2[2]>>7) );
			b8[2] = b4[2] <<1 ^ (0x1b &-(b4[2]>>7) );
			
			b1[3] = state[3][c];
            b2[3] = b1[3] <<1 ^ (0x1b &-(b1[3]>>7) );
			b4[3] = b2[3] <<1 ^ (0x1b &-(b2[3]>>7) );
			b8[3] = b4[3] <<1 ^ (0x1b &-(b4[3]>>7) );
			
        
		// b8[n] ^ b4[n] ^ b2[n] is a•{0e} , b8[n] ^ b2[n] ^ b1[n] is  a•{0b} , b8[n] ^ b4[n] ^ b1[n] is  a•{0d} , b8[n]  b1[n] is  a•{09}in GF(2^8)
        state[0][c] = ( b8[0] ^ b4[0] ^ b2[0] )^ ( b8[1] ^ b2[1] ^ b1[1] )^ ( b8[2] ^ b4[2] ^ b1[2]) ^ ( b8[3] ^ b1[3]); // {0e}•a0 + {0b}•a1 + {0d}•a2 + {09}•a3
        state[1][c] = ( b8[0] ^ b1[0]) ^ ( b8[1] ^ b4[1] ^ b2[1] ) ^ ( b8[2] ^ b2[2] ^ b1[2] )  ^ ( b8[3] ^ b4[3] ^ b1[3]); // {09}•a0 • {0e}•a1 + {0b}•a2 + {0d}•a3
        state[2][c] = ( b8[0] ^ b4[0] ^ b1[0])  ^ ( b8[1] ^ b1[1]) ^ ( b8[2] ^ b4[2] ^ b2[2] ) ^ ( b8[3] ^ b2[3] ^ b1[3] ); // {0d}•a0 + {09}•a1 + {0e}•a2 + {0b}•a3
        state[3][c] = ( b8[0] ^ b2[0] ^ b1[0] ) ^ ( b8[1] ^ b4[1] ^ b1[1])  ^ ( b8[2] ^ b1[2]) ^ ( b8[3] ^ b4[3] ^ b2[3] ) ; // {0b}•a0 + {0d}•a1 + {09}•a2 + {0e}•a3
	
    }
    
}

void revmixColumns3 (unsigned char state[4][4]) {
     unsigned char raw[4];


	// raw 1
	raw[0] = state[0][0];
	raw[1] = state[1][0];
	raw[2] = state[2][0];
	raw[3] = state[3][0];
	state[0][0] = mixcolumn_lookup[raw[0]][5];
	state[0][0] ^= mixcolumn_lookup[raw[1]][3];
	state[0][0] ^= mixcolumn_lookup[raw[2]][4];
	state[0][0] ^= mixcolumn_lookup[raw[3]][2];
	state[1][0] = mixcolumn_lookup[raw[0]][2];
	state[1][0] ^= mixcolumn_lookup[raw[1]][5];
	state[1][0] ^= mixcolumn_lookup[raw[2]][3];
	state[1][0] ^= mixcolumn_lookup[raw[3]][4];
	state[2][0] = mixcolumn_lookup[raw[0]][4];
	state[2][0] ^= mixcolumn_lookup[raw[1]][2];
	state[2][0] ^= mixcolumn_lookup[raw[2]][5];
	state[2][0] ^= mixcolumn_lookup[raw[3]][3];
	state[3][0] = mixcolumn_lookup[raw[0]][3];
	state[3][0] ^= mixcolumn_lookup[raw[1]][4];
	state[3][0] ^= mixcolumn_lookup[raw[2]][2];
	state[3][0] ^= mixcolumn_lookup[raw[3]][5];
	// raw 2
	raw[0] = state[0][1];
	raw[1] = state[1][1];
	raw[2] = state[2][1];
	raw[3] = state[3][1];
	state[0][1] = mixcolumn_lookup[raw[0]][5];
	state[0][1] ^= mixcolumn_lookup[raw[1]][3];
	state[0][1] ^= mixcolumn_lookup[raw[2]][4];
	state[0][1] ^= mixcolumn_lookup[raw[3]][2];
	state[1][1] = mixcolumn_lookup[raw[0]][2];
	state[1][1] ^= mixcolumn_lookup[raw[1]][5];
	state[1][1] ^= mixcolumn_lookup[raw[2]][3];
	state[1][1] ^= mixcolumn_lookup[raw[3]][4];
	state[2][1] = mixcolumn_lookup[raw[0]][4];
	state[2][1] ^= mixcolumn_lookup[raw[1]][2];
	state[2][1] ^= mixcolumn_lookup[raw[2]][5];
	state[2][1] ^= mixcolumn_lookup[raw[3]][3];
	state[3][1] = mixcolumn_lookup[raw[0]][3];
	state[3][1] ^= mixcolumn_lookup[raw[1]][4];
	state[3][1] ^= mixcolumn_lookup[raw[2]][2];
	state[3][1] ^= mixcolumn_lookup[raw[3]][5];
	// raw 3
	raw[0] = state[0][2];
	raw[1] = state[1][2];
	raw[2] = state[2][2];
	raw[3] = state[3][2];
	state[0][2] = mixcolumn_lookup[raw[0]][5];
	state[0][2] ^= mixcolumn_lookup[raw[1]][3];
	state[0][2] ^= mixcolumn_lookup[raw[2]][4];
	state[0][2] ^= mixcolumn_lookup[raw[3]][2];
	state[1][2] = mixcolumn_lookup[raw[0]][2];
	state[1][2] ^= mixcolumn_lookup[raw[1]][5];
	state[1][2] ^= mixcolumn_lookup[raw[2]][3];
	state[1][2] ^= mixcolumn_lookup[raw[3]][4];
	state[2][2] = mixcolumn_lookup[raw[0]][4];
	state[2][2] ^= mixcolumn_lookup[raw[1]][2];
	state[2][2] ^= mixcolumn_lookup[raw[2]][5];
	state[2][2] ^= mixcolumn_lookup[raw[3]][3];
	state[3][2] = mixcolumn_lookup[raw[0]][3];
	state[3][2] ^= mixcolumn_lookup[raw[1]][4];
	state[3][2] ^= mixcolumn_lookup[raw[2]][2];
	state[3][2] ^= mixcolumn_lookup[raw[3]][5];
	// raw 4
	raw[0] = state[0][3];
	raw[1] = state[1][3];
	raw[2] = state[2][3];
	raw[3] = state[3][3];
	state[0][3] = mixcolumn_lookup[raw[0]][5];
	state[0][3] ^= mixcolumn_lookup[raw[1]][3];
	state[0][3] ^= mixcolumn_lookup[raw[2]][4];
	state[0][3] ^= mixcolumn_lookup[raw[3]][2];
	state[1][3] = mixcolumn_lookup[raw[0]][2];
	state[1][3] ^= mixcolumn_lookup[raw[1]][5];
	state[1][3] ^= mixcolumn_lookup[raw[2]][3];
	state[1][3] ^= mixcolumn_lookup[raw[3]][4];
	state[2][3] = mixcolumn_lookup[raw[0]][4];
	state[2][3] ^= mixcolumn_lookup[raw[1]][2];
	state[2][3] ^= mixcolumn_lookup[raw[2]][5];
	state[2][3] ^= mixcolumn_lookup[raw[3]][3];
	state[3][3] = mixcolumn_lookup[raw[0]][3];
	state[3][3] ^= mixcolumn_lookup[raw[1]][4];
	state[3][3] ^= mixcolumn_lookup[raw[2]][2];
	state[3][3] ^= mixcolumn_lookup[raw[3]][5];
		

    
    
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

     //mixcolumn(state,temp);
	 //mixColumns2(state);
       mixColumns3(state);

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

		  //rev_mixcolumn(state,temp);
		  //revmixColumns2(state);
            revmixColumns3(state);
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

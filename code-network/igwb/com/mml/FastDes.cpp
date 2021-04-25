/*****************************************************************************
**
**	FileName:	FastDes.c
**
**	Project:	Electronic Funds Transfer (EFT)
**
**	Purpose:	An improvement of DES
**
**	Public Functions: des_crypt
**			  set_key
**
**	Note:		Used in case of 32bits long integer.
**
**	Modified History:
**		12/9/95		created		by JinMing
**
*****************************************************************************/

#include "FastDes.h"

/*----------------------------------------------------------------------------
//	Function:	set_key
//
//	Description:	preprocess of key, make subkeys for the later 16 round
//
//	Parameters:	key	--	INPUT, key 64bits des key
//			ks      --	OUTPUT, point to table of subkeys
----------------------------------------------------------------------------*/
int set_key ( unsigned char * key, des_keys ks)
{
	register unsigned long c, d, temp, temp1, temp2 ;
	register unsigned char * in ;
	register unsigned long * reg_ks ;
	register int i ;

	reg_ks = (unsigned long *) ks ;
	in = key;

	/*
	**	First, convert key into two 32bits unsigned long.
	*/
	c2l(in,c);
	c2l(in,d);

	/*
	**	Initialize permute of key, get 56bits key in c and d
	*/
	OP_TWO (c,d,temp,4,0x0f0f0f0fL);
	OP_ONE (c,temp,18,0xcccc0000L);
	OP_ONE (d,temp,18,0xcccc0000L);
	OP_ONE (c,temp,9,0xAA00AA00L);
	OP_ONE (d,temp,9,0xAA00AA00L);
	temp1 = ( (d & 0x000000ffL) << 24 )
		|( (d & 0x0000ff00L) << 8 )
		|( (d & 0x00ff0000L) >> 8 )
		|( (d & 0xff000000L) >> 24 ) ;
	temp2 = c ;
	c = temp1 >> 4 ;
	d = ( (temp2 & 0x00ffffff) << 4 ) | ( temp1 & 0x0000000f ) ;
	
	/*
	**	Now get the subkeys of 16 rounds
	*/
	for (i=0; i<ROUNDS; i++)
	{
		if (left_shift[i])
			{ c=((c<<2)|(c>>26)); d=((d<<2)|(d>>26)); }
		else
			{ c=((c<<1)|(c>>27)); d=((d<<1)|(d>>27)); }
		c &= 0x0fffffffL ;
		d &= 0x0fffffffL ;

		temp1 =	sbtrans[0][(c>>22L)&0x3f] |
			sbtrans[1][((c>> 16L)&0x30) | ((c>>15L)&0x0f)] |
			sbtrans[2][((c>>9L)&0x3c) | ((c>>8L)&0x3) ] |
			sbtrans[3][((c>>2L)&0x20) | ((c>>1)&0x18) | ((c )&0x7) ] ;
			
		temp2 =	sbtrans[4][ (d>>22L)&0x3f                 ]|
			sbtrans[5][((d>>15L)&0x30)|((d>>14L)&0x0f)]|
			sbtrans[6][ (d>> 7L)&0x3f                 ]|
			sbtrans[7][((d>> 1L)&0x3c)|((d )&0x3)];
			
		*(reg_ks++) = (temp1 & 0xff000000L) | ((temp1 & 0x0000ff00L) << 8)
			|((temp2 & 0xff000000L) >> 16) | ((temp2 & 0x0000ff00L) >> 8) ;
		*(reg_ks++) = ((temp1 & 0x00ff0000L) << 8) | ((temp1 & 0x000000ffL) << 16)
			|((temp2 & 0x00ff0000L) >> 8) | ( temp2 & 0x000000ffL) ; 

	}
	
	return(0);
}


/*----------------------------------------------------------------------------
//	Function:	des_crypt
//
//	Description:	crypt plant text into cipher using DES
//
//	Parameters:	data_in	--	INPUT, data to be crypted
//			data_out--	OUTPUT, data after process
//			ks	--	INPUT, key schedule 
//			encrypt	--	indicate ENCRYPT or DECRYPT
----------------------------------------------------------------------------*/
void des_crypt ( unsigned char * data_in, unsigned char * data_out, 
		 des_keys ks, int encrypt_flag )
{
	register unsigned long l, r, temp1, temp2 ;
	register int i ;
	register unsigned long * reg_ks ;

	c2l(data_in, l) ;
	c2l(data_in, r) ;

	/*
	**	Initialize permution of data
	*/
	IP(l,r) ;

	/* clear the top bits on machines with 8byte longs, such as DEC 3300 */
	l &= 0xffffffffL ;
	r &= 0xffffffffL;

	reg_ks = (unsigned long *) ks ;
	
	/*
	**	Encrypt
	*/
	if (encrypt_flag)
	{
		for (i=0; i<32; i+=4)
		{
			ENCRYPT_R(l,r,temp1,temp2,reg_ks,i+0);
			ENCRYPT_R(r,l,temp1,temp2,reg_ks,i+2);
		}
	}
	else
	/*
	**	Decrypt
	*/
	{
		for (i=30; i>0; i-=4)
		{
			ENCRYPT_R(l,r,temp1,temp2,reg_ks,i-0); 
			ENCRYPT_R(r,l,temp1,temp2,reg_ks,i-2);
		}
	}

	/* clear the top bits on machines with 8byte longs */
	l&=0xffffffffL;
	r&=0xffffffffL;

	/*
	**	Reverse permution of data
	*/
	FP(r,l);
	l2c(r, data_out) ;
	l2c(l, data_out) ;
}













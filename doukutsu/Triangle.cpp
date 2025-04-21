#include <stdio.h>
#include <math.h>
#include "Define.h"

int gSin[256];
short gTan[33];

void InitTriangleTable( void )
{
	int i;
	for( i = 0; i < 256; i ++ ){
		gSin[i] = (int)( sin( (double)i * 6.2832 / 256 ) * (double)VS );
//		gSin[i] = (int)( sin( (double)i * 6.2832 / 256 ) * (double)VS );
	}
	float a, b;
	for( i = 0; i < 33; i++ ){
		a = (float)( 2 * 3.14159265 ) * i / 256;
		b = (float)( sin( a ) / cos( a ) );
		gTan[i] = (short)( b * 0x2000 );//0010_0000_0000_0000
	}
}

int GetSin( unsigned char deg )
{
	return( gSin[deg] );
}

int GetCos( unsigned char deg )
{
	deg += 64;//Cosテーブルはサインの+1/4から
	return( gSin[deg] );
}

unsigned char GetArktan( long x,long y )
{
	//苦肉の策
	x *= -1;
	y *= -1;

	short k;
	unsigned char a = 0;
	if( x > 0 ){
		if( y > 0 ){
			if(x > y){
				k = ( short )((y*0x2000)/x);
				while(k > gTan[a])a++;
			}else{
				k = (short)((x*0x2000)/y);
				while(k > gTan[a])a++;
				a = 64-a;
			}
		}else{
			if(x > -y){
				k = (short)((-y*0x2000)/x);
				while(k > gTan[a])a++;
				a = 256-a;
			}else{
				k = (short)((x*0x2000)/-y);
				while(k > gTan[a])a++;
				a = 192+a;
			}
		}
	}else{
		if(y > 0){
			if(-x > y){
				k = (short)((y*0x2000)/-x);
				while(k > gTan[a])a++;
				a = 128-a;
			}else{
				k = (short)((-x*0x2000)/y);
				while(k > gTan[a])a++;
				a = 64+a;
			}
		}else{
			if(-x > -y){
				k = (short)((-y*0x2000)/-x);
				while(k > gTan[a])a++;
				a = 128+a;
			}else{
				k = (short)((-x*0x2000)/-y);
				while(k > gTan[a])a++;
				a = 192-a;
			}
		}
	}
	return a;
}



#include <windows.h>
#include "Define.h"
#include "Draw.h"


typedef struct{
	long mode;
	long act_no;
	long flag;
	long cnt;
	long width;
	long x;
	long y;
	RECT rect1;
	RECT rect2;
}FLASH;
FLASH flash;

static unsigned long gFlashColor;
void InitFlash( void )
{
	gFlashColor = GetCortBoxColor( RGB(0xff,0xff,0xfe) );
}

// フラッシュの配置
void SetFlash( long x,long y, long mode )
{
	flash.act_no = 0;
	flash.flag   = 1;
	flash.x      = x;
	flash.y      = y;
	flash.mode   = mode;
	flash.cnt    = 0;
	flash.width  = 0;
}

// 爆発フラッシュ
void ActFlash_Explosion( long flx, long fly )
{
	long left,top,right,bottom;
	switch(flash.act_no){
	case 0:
		flash.cnt   += VS;
		flash.width += flash.cnt;

		left   = ( flash.x - flx - flash.width )/VS;
		top    = ( flash.y - fly - flash.width )/VS;
		right  = ( flash.x - flx + flash.width )/VS;
		bottom = ( flash.y - fly + flash.width )/VS;
		if( left  < 0 ) left = 0;
		if( top   < 0 ) top  = 0;
		if(right  > SURFACE_WIDTH  ) right  = SURFACE_WIDTH;
		if(bottom > SURFACE_HEIGHT ) bottom = SURFACE_HEIGHT;

		flash.rect1.left   = left;
		flash.rect1.right  = right;
		flash.rect1.top    = 0;
		flash.rect1.bottom = SURFACE_HEIGHT;

		flash.rect2.left   = 0;
		flash.rect2.right  = SURFACE_WIDTH;
		flash.rect2.top    = top;
		flash.rect2.bottom = bottom;
		

		if( flash.width > SURFACE_WIDTH*4*VS){
			flash.act_no = 1;
			flash.cnt    = 0;
			flash.width  = SURFACE_HEIGHT*VS;
		}
		break;
	case 1:
		flash.width -= flash.width/8;
		if(!(flash.width/(VS/2)))flash.flag = 0;
		top = (flash.y-fly - flash.width)/VS;
		if(top < 0)top = 0;
		bottom = (flash.y-fly + flash.width)/VS;
		if(bottom > SURFACE_HEIGHT)bottom = SURFACE_HEIGHT;

		flash.rect1.left = 0;
		flash.rect1.right = 0;
		flash.rect1.top = 0;
		flash.rect1.bottom = 0;

		flash.rect2.top = top;
		flash.rect2.bottom = bottom;
		flash.rect2.left = 0;
		flash.rect2.right = SURFACE_WIDTH;
		
		break;
	}

}

// 瞬きフラッシュ
void ActFlash_Flash( void )
{
	flash.cnt++;
	flash.rect1.left = 0;
	flash.rect1.right = 0;
	flash.rect1.top = 0;
	flash.rect1.bottom = 0;

	if( flash.cnt/2%2 ){
		flash.rect2.top = 0;
		flash.rect2.bottom = 240;
		flash.rect2.left = 0;
		flash.rect2.right = 320;
	}else{
		flash.rect2.left = 0;
		flash.rect2.right = 0;
		flash.rect2.top = 0;
		flash.rect2.bottom = 0;
	}
	if( flash.cnt > 20 )flash.flag = 0;
}

void ActFlash( long flx, long fly  )
{
	if(flash.flag == 0)return;
	switch( flash.mode ){
	case FLASHMODE_EXPLOSION: ActFlash_Explosion( flx, fly );break;
	case FLASHMODE_FLASH:     ActFlash_Flash();    break;
//	case FLASHMODE_FLASH:     ActFlash_WhiteOut();    break;
	}

}

void PutFlash( void )
{
	if( !flash.flag )return;

	CortBox( &flash.rect1, gFlashColor );
	CortBox( &flash.rect2, gFlashColor );

}

void ResetFlash( void )
{
	flash.flag = 0;
}

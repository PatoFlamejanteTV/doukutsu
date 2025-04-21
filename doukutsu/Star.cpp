#include <windows.h>
#include <stdio.h>
#include "Draw.h"
#include "Define.h"

#include "Tags.h"

extern MYCHAR gMC;

#define MAX_STAR 3

static CARET _star[MAX_STAR];
extern RECT grcGame;
extern long g_GameFlags;
void SetBullet( long no, long x, long y, long dir ); 

void InitStar( void )
{
	memset( _star, 0, sizeof(CARET)*MAX_STAR );
	_star[0].x = gMC.x;
	_star[0].y = gMC.y;
	_star[1].x = gMC.x;
	_star[1].y = gMC.y;
	_star[2].x = gMC.x;
	_star[2].y = gMC.y;
	_star[0].xm =  VS*2;
	_star[0].ym = -VS;
	_star[1].xm = -VS;
	_star[1].ym =  VS*2;
	_star[2].xm =  VS;
	_star[2].ym =  VS;
}


void ActStar( void )
{
	long i;
	static long _a = 0;

	_a++;
	_a = _a%3;

	for( i = 0; i < MAX_STAR; i++ ){
		if( i ){
			if( _star[ i-1 ].x < _star[ i ].x ) _star[ i ].xm -= VS/4;
			else                                _star[ i ].xm += VS/4;
			if( _star[ i-1 ].y < _star[ i ].y ) _star[ i ].ym -= VS/3;
			else                                _star[ i ].ym += VS/3;
		}else{
			if( gMC.x < _star[ i ].x ) _star[ i ].xm -= VS/4;
			else                       _star[ i ].xm += VS/4;
			if( gMC.y < _star[ i ].y ) _star[ i ].ym -= VS/3;
			else                       _star[ i ].ym += VS/3;
		}
		if( _star[ i ].xm >  5*VS ) _star[ i ].xm =  5*VS;
		if( _star[ i ].xm < -5*VS ) _star[ i ].xm = -5*VS;
		if( _star[ i ].ym >  5*VS ) _star[ i ].ym =  5*VS;
		if( _star[ i ].ym < -5*VS ) _star[ i ].ym = -5*VS;
		if( _star[ i ].xm >  5*VS ) _star[ i ].xm =  5*VS;
		if( _star[ i ].xm < -5*VS ) _star[ i ].xm = -5*VS;
		if( _star[ i ].ym >  5*VS ) _star[ i ].ym =  5*VS;
		if( _star[ i ].ym < -5*VS ) _star[ i ].ym = -5*VS;
		_star[ i ].x += _star[ i ].xm;
		_star[ i ].y += _star[ i ].ym;

		if( i < gMC.star ){
			if( gMC.equip & EQUIP_STAR && g_GameFlags & GAMEFLAG_KEYCONTROL && _a == i )
				SetBullet( BULLET_STAR, _star[_a].x, _star[_a].y, DIR_LEFT );
		}

	}
}

void PutStar( long fx, long fy )
{
	RECT rc[] = {
		{192, 0,200, 8},
		{192, 8,200,16},
		{192,16,200,24},
	};
	long i;
	if( gMC.cond & COND_HIDE      ) return;
	if( !(gMC.equip & EQUIP_STAR) ) return;
	for( i = 0; i < MAX_STAR; i++){
		if( i < gMC.star ){
			PutBitmap3( &grcGame,
				_star[i].x/VS - fx/VS - 4,
				_star[i].y/VS - fy/VS - 4,
				&rc[i], SURF_MYCHAR );
		}
	}
}

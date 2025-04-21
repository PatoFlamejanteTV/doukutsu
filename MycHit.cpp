#include <windows.h>
#include "Sound.h"
#include "Define.h"
#include "Tags.h"
#include "Map.h"

extern MYCHAR gMC;
extern long g_GameFlags;
extern long gKey;
extern long gKeyTrg;
// 操作キー
extern long gKeyJump;
extern long gKeyShot;
extern long gKeyArms;
extern long gKeyArmsRev;
extern long gKeyItem;
extern long gKeyMap;
extern long gKeyLeft;
extern long gKeyUp;
extern long gKeyRight;
extern long gKeyDown;

extern long gWaterY;

#define MAX_BLOCKEDDASH		(128*3)

void SetCaret( long x, long y, long code, long dir );

void ResetMyCharFlag( void )
{
	gMC.flag = 0;//最初のあたり判定
}

static void _PutlittleStar( void )
{
	if( !(gMC.cond & COND_HIDE) && gMC.ym < -VS ){
		PlaySoundObject( WAVE_HEAD, 1 );
		SetCaret( gMC.x, gMC.y - gMC.hit.top, CARET_SMALLSTAR, DIR_LEFT );
		SetCaret( gMC.x, gMC.y - gMC.hit.top, CARET_SMALLSTAR, DIR_LEFT );
	}
}

//ノーマルブロックとのあたり判定処理
long JudgeHitMyCharBlock( long x, long y )
{
	long hit;
	hit = 0;
	//left hit
	if( gMC.y - gMC.hit.top < ( y * PARTSSIZE + PARTSSIZE/2 - 4 ) * VS &&
		gMC.y + gMC.hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 + 4 ) * VS ){
		if( gMC.x - gMC.hit.back < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
			gMC.x - gMC.hit.back > x * PARTSSIZE * VS ){
			gMC.x = ( x * PARTSSIZE + PARTSSIZE/2 ) * VS + gMC.hit.back;
			if( gMC.xm < -MAX_BLOCKEDDASH )gMC.xm = -MAX_BLOCKEDDASH;//１ブロックの隙間に入るため。
			if( !(gKey & gKeyLeft) && gMC.xm < 0 )gMC.xm = 0;
			hit |= FLAG_HIT_LEFT;
		}
	}
	//right hit
	if( gMC.y - gMC.hit.top < ( y * PARTSSIZE + PARTSSIZE/2 - 4 ) * VS &&
		gMC.y + gMC.hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 + 4 ) * VS ){
		if( gMC.x + gMC.hit.back > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS &&
			gMC.x + gMC.hit.back < x * PARTSSIZE * VS ){
			gMC.x = ( x * PARTSSIZE - PARTSSIZE/2 ) * VS - gMC.hit.back;
			if( gMC.xm > MAX_BLOCKEDDASH )gMC.xm = MAX_BLOCKEDDASH;//１ブロックの隙間に入るため。
			if( !(gKey & gKeyRight) && gMC.xm > 0  )gMC.xm = 0;
			hit |= FLAG_HIT_RIGHT;
		}
	}
	//top hit
	if( gMC.x - gMC.hit.back < ( x * PARTSSIZE + PARTSSIZE/2 - 3 ) * VS &&
		gMC.x + gMC.hit.back > ( x * PARTSSIZE - PARTSSIZE/2 + 3 ) * VS ){
		if( gMC.y - gMC.hit.top < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS &&
			gMC.y - gMC.hit.top > y * PARTSSIZE * VS ){
			gMC.y = ( y * PARTSSIZE + PARTSSIZE/2 ) * VS + gMC.hit.top;
			if( !(gMC.cond & COND_HIDE) && gMC.ym < -VS ) _PutlittleStar();
			if( gMC.ym < 0 )gMC.ym = 0;
			hit |= FLAG_HIT_TOP;
		}
	}
	//bottom hit
	if( gMC.x - gMC.hit.back < ( x * PARTSSIZE + PARTSSIZE/2 - 3 ) * VS &&
		gMC.x + gMC.hit.back > ( x * PARTSSIZE - PARTSSIZE/2 + 3 ) * VS ){
		if( gMC.y + gMC.hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS &&
			gMC.y + gMC.hit.bottom < y * PARTSSIZE * VS ){
			gMC.y = ( y * PARTSSIZE - PARTSSIZE/2 ) * VS - gMC.hit.bottom;
			if( gMC.ym > VS*2 ) PlaySoundObject( WAVE_STEP, 1 );
			if( gMC.ym > 0 )gMC.ym = 0;
			hit |= FLAG_HIT_BOTTOM;
		}
	}
	return hit;
}

//三角形とのあたり判定処理
//左上/右上/左下/右下
long JudgeHitMyCharTriangleA( long x, long y )
{
	long hit = 0;
	if( gMC.x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		gMC.x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( gMC.y - gMC.hit.top < y * PARTSSIZE * VS - ( gMC.x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE/4*VS &&
			gMC.y + gMC.hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){
			gMC.y = y * PARTSSIZE * VS - ( gMC.x - (x * PARTSSIZE *VS  ) )/2 + PARTSSIZE/4*VS + gMC.hit.top;
			if( !(gMC.cond & COND_HIDE) && gMC.ym < -VS ) _PutlittleStar();
			if( gMC.ym < 0 )gMC.ym = 0;
			hit |= FLAG_HIT_TOP;
		}
	}
	return hit;
}
long JudgeHitMyCharTriangleB( long x, long y )
{
	long hit = 0;
	if( gMC.x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		gMC.x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( gMC.y - gMC.hit.top < y * PARTSSIZE * VS - ( gMC.x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE/4*VS  &&
			gMC.y + gMC.hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){
			gMC.y = y * PARTSSIZE * VS - ( gMC.x - (x * PARTSSIZE *VS  ) )/2 - PARTSSIZE/4*VS + gMC.hit.top;
			if( !(gMC.cond & COND_HIDE) && gMC.ym < -VS ) _PutlittleStar();
			if( gMC.ym < 0 )gMC.ym = 0;
			hit |= FLAG_HIT_TOP;
		}
	}
	return hit;
}
long JudgeHitMyCharTriangleC( long x, long y )
{
	long hit = 0;
	if( gMC.x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		gMC.x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( gMC.y - gMC.hit.top < y * PARTSSIZE * VS + ( gMC.x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE/4*VS &&
			gMC.y + gMC.hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){
			gMC.y = y * PARTSSIZE * VS + ( gMC.x - (x * PARTSSIZE *VS  ) )/2 - PARTSSIZE/4*VS + gMC.hit.top;
			if( !(gMC.cond & COND_HIDE) && gMC.ym < -VS ) _PutlittleStar();
			if( gMC.ym < 0 )gMC.ym = 0;
			hit |= FLAG_HIT_TOP;
		}
	}
	return hit;
}
long JudgeHitMyCharTriangleD( long x, long y )
{
	long hit = 0;
	if( gMC.x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		gMC.x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( gMC.y - gMC.hit.top < y * PARTSSIZE * VS + ( gMC.x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE/4*VS &&
			gMC.y + gMC.hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){
			gMC.y = y * PARTSSIZE * VS + ( gMC.x - (x * PARTSSIZE *VS  ) )/2 + PARTSSIZE/4*VS + gMC.hit.top;
			if( !(gMC.cond & COND_HIDE) && gMC.ym < -VS ) _PutlittleStar();
			if( gMC.ym < 0 )gMC.ym = 0;
			hit |= FLAG_HIT_TOP;
		}
	}
	return hit;
}
long JudgeHitMyCharTriangleE( long x, long y )
{
	long hit = 0;
	hit |= FLAG_SET_TRI_E;
	if( gMC.x    <  ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		gMC.x
//		-VS
		> ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( gMC.y + gMC.hit.bottom > y * PARTSSIZE * VS + ( gMC.x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE*VS/4 &&
			gMC.y - gMC.hit.top < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS ){
			gMC.y = y * PARTSSIZE * VS + ( gMC.x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE*VS/4 - gMC.hit.bottom;
			if( gMC.ym > VS*2 ) PlaySoundObject( WAVE_STEP, 1 );
			if( gMC.ym > 0 )gMC.ym = 0;
			hit |= FLAG_HIT_BOTTOM|FLAG_HIT_RDOWN;
		}
	}
	return hit;
}
long JudgeHitMyCharTriangleF( long x, long y )
{
	long hit = 0;
	hit |= FLAG_SET_TRI_F;
	if( gMC.x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		gMC.x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( gMC.y + gMC.hit.bottom > y * PARTSSIZE * VS + ( gMC.x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE*VS/4 &&
			gMC.y - gMC.hit.top < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS ){
			gMC.y = y * PARTSSIZE * VS + ( gMC.x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE*VS/4 - gMC.hit.bottom;
			if( gMC.ym > VS*2 ) PlaySoundObject( WAVE_STEP, 1 );
			if( gMC.ym > 0 )gMC.ym = 0;
			hit |= FLAG_HIT_BOTTOM|FLAG_HIT_RDOWN;
		}
	}
	return hit;
}

long JudgeHitMyCharTriangleG( long x, long y )
{
	long hit = 0;
	hit |= FLAG_SET_TRI_G;
	if( gMC.x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		gMC.x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( gMC.y + gMC.hit.bottom > y * PARTSSIZE * VS - ( gMC.x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE*VS/4 &&
			gMC.y - gMC.hit.top < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS ){
			gMC.y = y * PARTSSIZE * VS - ( gMC.x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE*VS/4 - gMC.hit.bottom;
			if( gMC.ym > VS*2 ) PlaySoundObject( WAVE_STEP, 1 );
			if( gMC.ym > 0 )gMC.ym = 0;
			hit |= FLAG_HIT_BOTTOM|FLAG_HIT_LDOWN;
		}
	}
	return hit;
}
long JudgeHitMyCharTriangleH( long x, long y )
{
	long hit = 0;
	hit |= FLAG_SET_TRI_H;
	if( gMC.x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		gMC.x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( gMC.y + gMC.hit.bottom > y * PARTSSIZE * VS - ( gMC.x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE*VS/4 &&
			gMC.y - gMC.hit.top < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS ){
			gMC.y = y * PARTSSIZE * VS - ( gMC.x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE*VS/4 - gMC.hit.bottom;
			if( gMC.ym > VS*2 ) PlaySoundObject( WAVE_STEP, 1 );
			if( gMC.ym > 0 )gMC.ym = 0;
			hit |= FLAG_HIT_BOTTOM|FLAG_HIT_LDOWN;
		}
	}

	return hit;
}

long JudgeHitMyCharWater( long x, long y )
{
	long hit = 0;
	if( gMC.x - gMC.hit.back   < ( x * PARTSSIZE + PARTSSIZE/2 -3 ) * VS &&
		gMC.x + gMC.hit.back   > ( x * PARTSSIZE - PARTSSIZE/2 +3 ) * VS &&
		gMC.y - gMC.hit.top    < ( y * PARTSSIZE + PARTSSIZE/2 -3 ) * VS &&
		gMC.y + gMC.hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 +8 ) * VS ){

		hit |= FLAG_HIT_WATER;
	}
	return hit;
}
long JudgeHitMyCharDamage( long x, long y )
{
	long hit = 0;
	if( gMC.x - VS*4  < ( x * PARTSSIZE + 4 ) * VS &&
		gMC.x + VS*4  > ( x * PARTSSIZE - 4 ) * VS &&
		gMC.y - VS*4  < ( y * PARTSSIZE + 3 ) * VS &&
		gMC.y + VS*4  > ( y * PARTSSIZE - 3 ) * VS ){

		hit |= FLAG_HIT_DAMAGE;
	}
	return hit;
}

long JudgeHitMyCharDamageW( long x, long y )
{
	long hit = 0;
	if( gMC.x - VS*4  < ( x * PARTSSIZE + 4 ) * VS &&
		gMC.x + VS*4  > ( x * PARTSSIZE - 4 ) * VS &&
		gMC.y - VS*4  < ( y * PARTSSIZE + 3 ) * VS &&
		gMC.y + VS*4  > ( y * PARTSSIZE - 3 ) * VS ){

		hit |= FLAG_RED_WATER|FLAG_HIT_WATER|FLAG_HIT_DAMAGE;
	}
	return hit;
}

//流れる←
long JudgeHitMyCharVectLeft( long x, long y )
{
	long hit = 0;
	if( gMC.x - gMC.hit.back   < ( x * PARTSSIZE + PARTSSIZE/2 -2 ) * VS &&
		gMC.x + gMC.hit.back   > ( x * PARTSSIZE - PARTSSIZE/2 +2 ) * VS &&
		gMC.y - gMC.hit.top    < ( y * PARTSSIZE + PARTSSIZE/2 -2 ) * VS &&
		gMC.y + gMC.hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 +2 ) * VS ){

		hit |= FLAG_HIT_VECT_LEFT;
	}
	return hit;
}
//流れる↑
long JudgeHitMyCharVectUp( long x, long y )
{
	long hit = 0;
	if( gMC.x - gMC.hit.back   < ( x * PARTSSIZE + PARTSSIZE/2 -2 ) * VS &&
		gMC.x + gMC.hit.back   > ( x * PARTSSIZE - PARTSSIZE/2 +2 ) * VS &&
		gMC.y - gMC.hit.top    < ( y * PARTSSIZE + PARTSSIZE/2 -2 ) * VS &&
		gMC.y + gMC.hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 +2 ) * VS ){

		hit |= FLAG_HIT_VECT_UP;
	}
	return hit;
}
//流れる→
long JudgeHitMyCharVectRight( long x, long y )
{
	long hit = 0;
	if( gMC.x - gMC.hit.back   < ( x * PARTSSIZE + PARTSSIZE/2 -2 ) * VS &&
		gMC.x + gMC.hit.back   > ( x * PARTSSIZE - PARTSSIZE/2 +2 ) * VS &&
		gMC.y - gMC.hit.top    < ( y * PARTSSIZE + PARTSSIZE/2 -2 ) * VS &&
		gMC.y + gMC.hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 +2 ) * VS ){

		hit |= FLAG_HIT_VECT_RIGHT;
	}
	return hit;
}
//流れる↓
long JudgeHitMyCharVectDown( long x, long y )
{
	long hit = 0;
	if( gMC.x - gMC.hit.back   < ( x * PARTSSIZE + PARTSSIZE/2 -2 ) * VS &&
		gMC.x + gMC.hit.back   > ( x * PARTSSIZE - PARTSSIZE/2 +2 ) * VS &&
		gMC.y - gMC.hit.top    < ( y * PARTSSIZE + PARTSSIZE/2 -2 ) * VS &&
		gMC.y + gMC.hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 +2 ) * VS ){

		hit |= FLAG_HIT_VECT_DOWN;
	}
	return hit;
}




//マイキャラの背景とのあたり判定
void HitMyCharMap( void )
{
	long x, y;
	long i;
	unsigned char atrb[4];

	x =  gMC.x / PARTSSIZE / VS;
	y =  gMC.y / PARTSSIZE / VS;

	//周囲４ブロックとのあたり判定
	long offx[4] = {0,1,0,1};
	long offy[4] = {0,0,1,1};

	for( i = 0; i < 4; i++ ){
		atrb[i] = GetAttribute( x + offx[i], y + offy[i] );
		switch( atrb[i] ){
		//ノーマルブロック
		case ATRB_BLOCK:
		case ATRB_BLOCKME:
		case ATRB_BACK_BULTH:
		case ATRB_SNACK: gMC.flag |= JudgeHitMyCharBlock( x + offx[i], y + offy[i] ); break;
		case ATRB_TRI_A: gMC.flag |= JudgeHitMyCharTriangleA( x + offx[i], y + offy[i] ); break;
		case ATRB_TRI_B: gMC.flag |= JudgeHitMyCharTriangleB( x + offx[i], y + offy[i] ); break;
		case ATRB_TRI_C: gMC.flag |= JudgeHitMyCharTriangleC( x + offx[i], y + offy[i] ); break;
		case ATRB_TRI_D: gMC.flag |= JudgeHitMyCharTriangleD( x + offx[i], y + offy[i] ); break;
		case ATRB_TRI_E: gMC.flag |= JudgeHitMyCharTriangleE( x + offx[i], y + offy[i] ); break;
		case ATRB_TRI_F: gMC.flag |= JudgeHitMyCharTriangleF( x + offx[i], y + offy[i] ); break;
		case ATRB_TRI_G: gMC.flag |= JudgeHitMyCharTriangleG( x + offx[i], y + offy[i] ); break;
		case ATRB_TRI_H: gMC.flag |= JudgeHitMyCharTriangleH( x + offx[i], y + offy[i] ); break;
//		case ATRB_WATER: gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] ); break;
		case ATRB_DAMAGE: gMC.flag |= JudgeHitMyCharDamage( x + offx[i], y + offy[i] ); break;

		case ATRB_DAMAGE_W:
	//		gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharDamageW( x + offx[i], y + offy[i] );
			break;

		case ATRB_LEFT:  gMC.flag |= JudgeHitMyCharVectLeft( x + offx[i], y + offy[i] ); break;
		case ATRB_UP:    gMC.flag |= JudgeHitMyCharVectUp( x + offx[i], y + offy[i] ); break;
		case ATRB_RIGHT: gMC.flag |= JudgeHitMyCharVectRight( x + offx[i], y + offy[i] ); break;
		case ATRB_DOWN:  gMC.flag |= JudgeHitMyCharVectDown( x + offx[i], y + offy[i] ); break;

		case ATRB_BACK_W: gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] ); break;
		case ATRB_FRONT_W:gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] ); break;

		case ATRB_BLOCK_W:
			gMC.flag |= JudgeHitMyCharBlock( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
		case ATRB_TRI_A_W:
			gMC.flag |= JudgeHitMyCharTriangleA( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
		case ATRB_TRI_B_W:
			gMC.flag |= JudgeHitMyCharTriangleB( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
		case ATRB_TRI_C_W:
			gMC.flag |= JudgeHitMyCharTriangleC( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
		case ATRB_TRI_D_W:
			gMC.flag |= JudgeHitMyCharTriangleD( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
		case ATRB_TRI_E_W:
			gMC.flag |= JudgeHitMyCharTriangleE( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
		case ATRB_TRI_F_W:
			gMC.flag |= JudgeHitMyCharTriangleF( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
		case ATRB_TRI_G_W:
			gMC.flag |= JudgeHitMyCharTriangleG( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
		case ATRB_TRI_H_W:
			gMC.flag |= JudgeHitMyCharTriangleH( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
//		case ATRB_SNACK_W:
		case ATRB_LEFT_W:
			gMC.flag |= JudgeHitMyCharVectLeft( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
		case ATRB_UP_W:
			gMC.flag |= JudgeHitMyCharVectUp( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
		case ATRB_RIGHT_W:
			gMC.flag |= JudgeHitMyCharVectRight( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
		case ATRB_DOWN_W:
			gMC.flag |= JudgeHitMyCharVectDown( x + offx[i], y + offy[i] );
			gMC.flag |= JudgeHitMyCharWater( x + offx[i], y + offy[i] );
			break;
		
		}
	}
	if( gMC.y > gWaterY+4*VS ) gMC.flag |= FLAG_HIT_WATER;
}

extern NPCHAR gNPC[];

//NPCとのあたり判定処理
long JudgeHitMyCharNPC( NPCHAR *npc )
{
	long hit;
	hit = 0;

	//left hit
	if( gMC.y - gMC.hit.top < npc->y + npc->hit.bottom - 3*VS &&
		gMC.y + gMC.hit.bottom > npc->y - npc->hit.top + 3*VS ){
		if( gMC.x - gMC.hit.back < npc->x + npc->hit.back &&
			gMC.x - gMC.hit.back > npc->x ){
			if( gMC.xm < VS ) gMC.xm += VS;
			hit |= FLAG_HIT_LEFT;
		}
	}
	//right hit
	if( gMC.y - gMC.hit.top < npc->y + npc->hit.bottom - 3*VS &&
		gMC.y + gMC.hit.bottom > npc->y - npc->hit.top + 3*VS ){
		if( gMC.x + gMC.hit.back-1*VS > npc->x - npc->hit.back &&
			gMC.x + gMC.hit.back-1*VS < npc->x ){//-1*VSは、斜め壁で辺り判定が通り抜ける
			if( gMC.xm > -VS ) gMC.xm -= VS;//////////MY□
			hit |= FLAG_HIT_RIGHT;////////////////////□ENE
		}
	}
	//top hit
	if( gMC.x - gMC.hit.back < npc->x + npc->hit.back - 3*VS &&
		gMC.x + gMC.hit.back > npc->x - npc->hit.back + 3*VS ){
		if( gMC.y - gMC.hit.top < npc->y + npc->hit.bottom &&
			gMC.y - gMC.hit.top > npc->y ){
			if( gMC.ym < 0 ) gMC.ym = 0;
			hit |= FLAG_HIT_TOP;
		}
	}
	//bottom hit(上に乗る)
	if( gMC.x - gMC.hit.back < npc->x + npc->hit.back - 3*VS &&
		gMC.x + gMC.hit.back > npc->x - npc->hit.back + 3*VS ){
		if( gMC.y + gMC.hit.bottom > npc->y - npc->hit.top &&
			gMC.y + gMC.hit.bottom < npc->y+3*VS ){

			if( npc->bits & BITS_BOUND_MYCHAR ){
				gMC.ym = npc->ym - VS;
				hit |= FLAG_HIT_BOTTOM;
					
			//浮気していなければ
			}else if( !(gMC.flag & FLAG_HIT_BOTTOM) && gMC.ym > npc->ym ){
				gMC.y = npc->y - npc->hit.top - gMC.hit.bottom +VS;
				gMC.ym = npc->ym;
				gMC.x += npc->xm;
				hit |= FLAG_HIT_BOTTOM;
			}
		}
	}
	
	return hit;
}

/*
//NPCとのあたり判定処理(壁タイプ)
long JudgeHitMyCharNPC2( NPCHAR *npc )
{
	long hit;
	hit = 0;

	//left hit
	if( gMC.y - gMC.hit.top    < npc->y + npc->hit.bottom - 3*VS &&
		gMC.y + gMC.hit.bottom > npc->y - npc->hit.top    + 3*VS ){
		if( gMC.x - gMC.hit.back < npc->x + npc->hit.back &&
			gMC.x - gMC.hit.back > npc->x ){
			if( gMC.xm < npc->xm ) gMC.xm = npc->xm;
			gMC.x = npc->x + npc->hit.back + gMC.hit.back;
			hit |= FLAG_HIT_LEFT;
		}
	}
	//right hit
	if( gMC.y - gMC.hit.top    < npc->y + npc->hit.bottom - 3*VS &&
		gMC.y + gMC.hit.bottom > npc->y - npc->hit.top    + 3*VS ){
		if( gMC.x + gMC.hit.back > npc->x - npc->hit.back &&
			gMC.x + gMC.hit.back < npc->x ){
			if( gMC.xm > npc->xm ) gMC.xm = npc->xm;
			gMC.x = npc->x - npc->hit.back - gMC.hit.back;
			hit |= FLAG_HIT_RIGHT;
		}
	}
	//top hit
	if( gMC.x - gMC.hit.back < npc->x + npc->hit.back - 3*VS &&
		gMC.x + gMC.hit.back > npc->x - npc->hit.back + 3*VS ){
		if( gMC.y - gMC.hit.top < npc->y + npc->hit.bottom &&
			gMC.y - gMC.hit.top > npc->y ){
			if( gMC.ym < 0 ) gMC.ym = 0;
			hit |= FLAG_HIT_TOP;
		}
	}
	//bottom hit(上に乗る)
	if( gMC.x - gMC.hit.back < npc->x + npc->hit.back - 3*VS &&
		gMC.x + gMC.hit.back > npc->x - npc->hit.back + 3*VS ){
		if( gMC.y + gMC.hit.bottom > npc->y - npc->hit.top &&
			gMC.y + gMC.hit.bottom < npc->y+3*VS ){

			if( gMC.ym - npc->ym > VS*2 ) PlaySoundObject( WAVE_STEP, 1 );
			if( npc->bits & BITS_BOUND_MYCHAR ){
				gMC.ym = npc->ym - VS;
				hit |= FLAG_HIT_BOTTOM;
					
			//浮気していなければ
			}else if( !(gMC.flag & FLAG_HIT_BOTTOM) && gMC.ym > npc->ym ){
				gMC.y = npc->y - npc->hit.top - gMC.hit.bottom +VS;
				gMC.ym = npc->ym;
				gMC.x += npc->xm;
				hit |= FLAG_HIT_BOTTOM;
			}
		}
	}
	
	return hit;
}
*/

//NPC透過とのあたり判定処理
unsigned char JudgeHitMyCharNPC3( NPCHAR *npc )
{
	if( npc->direct == DIR_LEFT ){
		if( gMC.x + 2*VS > npc->x - npc->hit.front &&
			gMC.x - 2*VS < npc->x + npc->hit.back &&
			gMC.y + 2*VS > npc->y - npc->hit.top &&
			gMC.y - 2*VS < npc->y + npc->hit.bottom ){
			return 1;
		}
	}else{
		if( gMC.x + 2*VS > npc->x - npc->hit.back &&
			gMC.x - 2*VS < npc->x + npc->hit.front &&
			gMC.y + 2*VS > npc->y - npc->hit.top &&
			gMC.y - 2*VS < npc->y + npc->hit.bottom ){
			return 1;
		}
	}
/*	if( npc->direct == DIR_LEFT ){
		if( gMC.x + gMC.hit.back   > npc->x - npc->hit.front &&
			gMC.x - gMC.hit.back   < npc->x + npc->hit.back &&
			gMC.y + gMC.hit.bottom > npc->y - npc->hit.top &&
			gMC.y - gMC.hit.top    < npc->y + npc->hit.bottom ){
			return 0x01;
		}
	}else{
		if( gMC.x + gMC.hit.back   > npc->x - npc->hit.back &&
			gMC.x - gMC.hit.back   < npc->x + npc->hit.front &&
			gMC.y + gMC.hit.bottom > npc->y - npc->hit.top &&
			gMC.y - gMC.hit.top    < npc->y + npc->hit.bottom ){
			return 0x01;
		}
	}
*/
	return 0;
}

// 壁タイプ
long JudgeHitMyCharNPC4( NPCHAR *npc )
{
	long hit;
	hit = 0;

	float fx1;
	float fy1;

	float fx2;
	float fy2;

	if( npc->x > gMC.x ) fx1 = (float)( npc->x - gMC.x );
	else                 fx1 = (float)( gMC.x  - npc->x );
	if( npc->y > gMC.y ) fy1 = (float)( npc->y - gMC.y );
	else                 fy1 = (float)( gMC.y  - npc->y );

	fx2 = (float)( npc->hit.back );
	fy2 = (float)( npc->hit.top );

	if( !fx1 ) fx1 = 1;
	if( !fx2 ) fx2 = 1;
	if( fy1/fx1 > fy2/fx2 ){ // 比較的上、もしくは比較的下
		if( gMC.x - gMC.hit.back < npc->x + npc->hit.back  &&
			gMC.x + gMC.hit.back > npc->x - npc->hit.back  ){
			if( gMC.y - gMC.hit.top < npc->y + npc->hit.bottom &&
				gMC.y - gMC.hit.top > npc->y ){

				if( gMC.ym < npc->ym ){
					gMC.y = npc->y + npc->hit.bottom + gMC.hit.top + VS;
					gMC.ym = npc->ym;
				}else if( gMC.ym < 0 ){
					gMC.ym = 0;
				}
				hit |= FLAG_HIT_TOP;
			}
			if( gMC.y + gMC.hit.bottom > npc->y - npc->hit.top &&
				gMC.y + gMC.hit.bottom < npc->y+3*VS ){

				if( gMC.ym - npc->ym > VS*2 ) PlaySoundObject( WAVE_STEP, 1 );
				if( gMC.unit == MYUNIT_STREAM ){
					gMC.y = npc->y - npc->hit.top - gMC.hit.bottom +VS;
					hit |= FLAG_HIT_BOTTOM;
				}else if( npc->bits & BITS_BOUND_MYCHAR ){
					gMC.ym = npc->ym - VS;
					hit |= FLAG_HIT_BOTTOM;
						
				//浮気していなければ
				}else if( !(gMC.flag & FLAG_HIT_BOTTOM) && gMC.ym > npc->ym ){
					gMC.y = npc->y - npc->hit.top - gMC.hit.bottom +VS;
					gMC.ym = npc->ym;
					gMC.x += npc->xm;
					hit |= FLAG_HIT_BOTTOM;
				}
			}
		}
	}else{
		if( gMC.y - gMC.hit.top    < npc->y + npc->hit.bottom  &&
			gMC.y + gMC.hit.bottom > npc->y - npc->hit.top     ){
			if( gMC.x - gMC.hit.back < npc->x + npc->hit.back &&
				gMC.x - gMC.hit.back > npc->x ){
				if( gMC.xm < npc->xm ) gMC.xm = npc->xm;
				gMC.x = npc->x + npc->hit.back + gMC.hit.back;
				hit |= FLAG_HIT_LEFT;
			}
			if( gMC.x + gMC.hit.back > npc->x - npc->hit.back &&
				gMC.x + gMC.hit.back < npc->x ){
				if( gMC.xm > npc->xm ) gMC.xm = npc->xm;
				gMC.x = npc->x - npc->hit.back - gMC.hit.back;
				hit |= FLAG_HIT_RIGHT;
			}
		}
	}

	return hit;
}

BOOL StartTextScript( long no );
void DamageMyChar( long a );
void AddExpMyChar( long x );
void AddLifeMyChar( long x );
void AddBulletMyChar( long no, long val );

void HitMyCharNpChar( void )
{
	long i;
	long hit = 0;
	if( !(gMC.cond & COND_ALIVE) ||
		( gMC.cond & COND_HIDE ) )return;

	for( i = 0; i < MAX_NPC; i++){
		if( !( gNPC[i].cond & COND_ALIVE )     ) continue;
		hit = 0;
		//まずはあたり判定を･･･
		if( gNPC[i].bits & BITS_BLOCK_MYCHAR ){
			hit = JudgeHitMyCharNPC( &gNPC[i] );
			gMC.flag |= hit;
		}else if( gNPC[i].bits & BITS_BLOCK_MYCHAR2 ){
			hit = JudgeHitMyCharNPC4( &gNPC[i] );
			gMC.flag |= hit;
		}else{
			hit = JudgeHitMyCharNPC3( &gNPC[i] );
		}

		//経験値
		if( hit && gNPC[i].code_char == 1 ){
//		if( hit && gNPC[i].bits & BITS_EXP ){
			PlaySoundObject( WAVE_GETEXP, 1 );
			AddExpMyChar( gNPC[i].exp );
			gNPC[i].cond = 0;
		}
		//弾数アイテム
		if( hit && gNPC[i].code_char == 86 ){
			PlaySoundObject( WAVE_ADDBULLET, 1 );
			AddBulletMyChar( gNPC[i].code_event, gNPC[i].exp );
			gNPC[i].cond = 0;
		}
		//ライフアイテム
		if( hit && gNPC[i].code_char == 87 ){
			PlaySoundObject( WAVE_LIFE, 1 );
			AddLifeMyChar( gNPC[i].exp );
			gNPC[i].cond = 0;
		}

		//イベント開始？！(接触)
		if( !(g_GameFlags & GAMEFLAG_TEXTSCRIPT) && hit && gNPC[i].bits & BITS_EVENT_HIT ){
			StartTextScript( gNPC[i].code_event );
//			gMC.ques = 0;
		}
		if( ( g_GameFlags & GAMEFLAG_KEYCONTROL ) && !(gNPC[i].bits & BITS_EVENT_CHECK) ){

			if( gNPC[i].bits & BITS_DAMAGE_SIDE ){
				if( hit & FLAG_HIT_RIGHT  && gNPC[i].xm < 0 ) DamageMyChar( gNPC[i].damage );
				if( hit & FLAG_HIT_LEFT   && gNPC[i].xm > 0 ) DamageMyChar( gNPC[i].damage );
				if( hit & FLAG_HIT_BOTTOM && gNPC[i].ym < 0 ) DamageMyChar( gNPC[i].damage );
				if( hit & FLAG_HIT_TOP    && gNPC[i].ym > 0 ) DamageMyChar( gNPC[i].damage );
			}else if( hit && (gNPC[i].damage) ){
				if( !(g_GameFlags & GAMEFLAG_TEXTSCRIPT)    ) DamageMyChar( gNPC[i].damage );
			}
		}
		//イベント開始？！（プレイヤーチェック）
		if( !(g_GameFlags & GAMEFLAG_TEXTSCRIPT) && hit && gMC.cond & COND_CHECK && gNPC[i].bits & BITS_EVENT_CHECK ){
			StartTextScript( gNPC[i].code_event );
			gMC.xm   = 0;
			gMC.ques = 0;
		}
	}

	if( gMC.ques ) SetCaret( gMC.x, gMC.y, CARET_QUESTION, 0 );
}





extern NPCHAR gBoss[];

void HitMyCharBoss( void )
{
	long b;
	long hit = 0;
	if( !(gMC.cond & COND_ALIVE) ||
		( gMC.cond & COND_HIDE ) )return;

	for( b = 0; b < MAX_BOSS_PARTS; b++){
		if( !( gBoss[b].cond & COND_ALIVE ) )continue;
		hit = 0;

		//まずはあたり判定を･･･
		if( gBoss[b].bits & BITS_BLOCK_MYCHAR ){
			hit = JudgeHitMyCharNPC(  &gBoss[b] );
			gMC.flag |= hit;
		}else if( gBoss[b].bits & BITS_BLOCK_MYCHAR2 ){
			hit = JudgeHitMyCharNPC4( &gBoss[b] );
			gMC.flag |= hit;
		}else{
			hit = JudgeHitMyCharNPC3( &gBoss[b] );
		}


		//イベント開始？！(接触)
		if( !(g_GameFlags & GAMEFLAG_TEXTSCRIPT) && hit && gBoss[b].bits & BITS_EVENT_HIT ){
			StartTextScript( gBoss[b].code_event );
			gMC.ques = 0;
		}
		if( gBoss[b].bits & BITS_DAMAGE_SIDE ){
			if( hit & FLAG_HIT_RIGHT && gBoss[b].xm < 0 ) DamageMyChar( gBoss[b].damage );
			if( hit & FLAG_HIT_LEFT  && gBoss[b].xm > 0 ) DamageMyChar( gBoss[b].damage );
		}else if( hit && (gBoss[b].damage) ){
			if( !(g_GameFlags & GAMEFLAG_TEXTSCRIPT)    ) DamageMyChar( gBoss[b].damage );
		}
		//イベント開始？！（プレイヤーチェック）
		if( !(g_GameFlags & GAMEFLAG_TEXTSCRIPT) && hit && gMC.cond & COND_CHECK && gBoss[b].bits & BITS_EVENT_CHECK ){
			StartTextScript( gBoss[b].code_event );
			gMC.xm   = 0;
			gMC.ques = 0;
		}
	}

	if( gMC.ques )SetCaret( gMC.x, gMC.y, CARET_QUESTION, 0 );
}
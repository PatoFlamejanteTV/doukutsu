#include <windows.h>
//#include "Draw.h"
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "Map.h"

extern MYCHAR gMC;
extern BULLET gBul[];
#define RANGE_BULLET_BLOCK	2*VS

long Random( int min, int max );
void SetCaret( long x, long y, long code, long dir );
void SetNpChar( long code_char, long x, long y, long xm, long ym,
				long dir, NPCHAR *npc, long start_index );

static void _Vanish( BULLET *bul )
{
	if( bul->code_bullet == BULLET_SPUR1 ||
		bul->code_bullet == BULLET_SPUR2 ||
		bul->code_bullet == BULLET_SPUR3 ){
		SetCaret( bul->x, bul->y, CARET_SMOKE, DIR_UP );
	}else{
		PlaySoundObject( WAVE_SMOKE, 1 );
	}
	bul->cond = 0;
	SetCaret( bul->x, bul->y, CARET_SMOKE, DIR_RIGHT );
}

long JudgeHitBulletBlock( long x, long y, BULLET *bul )
{
	long i;
	long hit = 0;

	if( bul->x - bul->blockXL < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		bul->x + bul->blockXL > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS &&
		bul->y - bul->blockYL < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		bul->y + bul->blockYL > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		hit |= FLAG_HIT_BLOCK;
	}
	
	// スナック破壊
	if( hit && (BBITS_BLOCKBUSTER|BBITS_BLOCKERASE) & bul->bbits && ATRB_SNACK == GetAttribute( x, y ) ){

		if( !(BBITS_BLOCKERASE & bul->bbits) ) bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_SMOKE, DIR_LEFT );
		PlaySoundObject( WAVE_BREAK1, 1 );
		for( i = 0; i < 4; i++ ){
			SetNpChar( 4, x*VS*PARTSSIZE, y*VS*PARTSSIZE, Random(-VS,VS), Random(-VS,VS), DIR_LEFT, NULL, MAX_NPC/2 );
		}
		ShiftMapParts( x, y );
	}

	return hit;
}


long JudgeHitBulletBlock2( long x, long y, unsigned char *atrb, BULLET *bul )
{
	long block[4];
	long workX, workY;//
	long hit, i;
	hit = 0;
	if( bul->bbits & BBITS_BLOCKERASE ){
		for( i = 0; i < 4; i++ ){
			if( *atrb == ATRB_BLOCK ||                        *atrb == ATRB_BLOCK_W ) block[i] = 1;
			else                                                                      block[i] = 0;
			atrb++;
		}
	}else{
		for( i = 0; i < 4; i++ ){
			if( *atrb == ATRB_BLOCK || *atrb == ATRB_SNACK || *atrb == ATRB_BLOCK_W ) block[i] = 1;
			else                                                                      block[i] = 0;
			atrb++;
		}
	}
	
//	if( *atrb == ATRB_BLOCK || *atrb == ATRB_SNACK || *atrb == ATRB_BLOCK_W )block[0] = 1;
//	else block[0] = 0;
//	atrb++;
//	if( *atrb == ATRB_BLOCK || *atrb == ATRB_SNACK || *atrb == ATRB_BLOCK_W )block[1] = 1;
//	else block[1] = 0;
//	atrb++;
//	if( *atrb == ATRB_BLOCK || *atrb == ATRB_SNACK || *atrb == ATRB_BLOCK_W )block[2] = 1;
//	else block[2] = 0;
//	atrb++;
//	if( *atrb == ATRB_BLOCK || *atrb == ATRB_SNACK || *atrb == ATRB_BLOCK_W )block[3] = 1;
//	else block[3] = 0;


	workX = (x*PARTSSIZE+PARTSSIZE/2)*VS;
	workY = (y*PARTSSIZE+PARTSSIZE/2)*VS;

	//■←
	if( block[0] && block[2] ){
		if( bul->x - bul->blockXL < workX )
			hit |= FLAG_HIT_LEFT;
	}else if( block[0] && !block[2] ){
		if( bul->x - bul->blockXL < workX &&
			bul->y - bul->blockYL < workY - 3*VS )
			hit |= FLAG_HIT_LEFT;
	}else if( !block[0] && block[2] ){
		if( bul->x - bul->blockXL < workX &&
			bul->y + bul->blockYL > workY + 3*VS )
			hit |= FLAG_HIT_LEFT;
	}
	//→■
	if( block[1] && block[3] ){
		if( bul->x + bul->blockXL > workX )
			hit |= FLAG_HIT_RIGHT;
	}else if( block[1] && !block[3] ){
		if( bul->x + bul->blockXL > workX &&
			bul->y - bul->blockYL < workY - 3*VS )
			hit |= FLAG_HIT_RIGHT;
	}else if( !block[1] && block[3] ){
		if( bul->x + bul->blockXL > workX &&
			bul->y + bul->blockYL > workY + 3*VS )
			hit |= FLAG_HIT_RIGHT;
	}
	//■
	//↑
	if( block[0] && block[1] ){
		if( bul->y - bul->blockYL < workY )
			hit |= FLAG_HIT_TOP;
	}else if( block[0] && !block[1] ){
		if( bul->y - bul->blockYL < workY &&
			bul->x - bul->blockXL < workX - 3*VS )
			hit |= FLAG_HIT_TOP;
	}else if( !block[0] && block[1] ){
		if( bul->y - bul->blockYL < workY &&
			bul->x + bul->blockXL > workX + 3*VS )
			hit |= FLAG_HIT_TOP;
	}
	//↓
	//■
	if( block[2] && block[3] ){
		if( bul->y + bul->blockYL > workY )
			hit |= FLAG_HIT_BOTTOM;
	}else if( block[2] && !block[3] ){
		if( bul->y + bul->blockYL > workY &&
			bul->x - bul->blockXL < workX - 3*VS )
			hit |= FLAG_HIT_BOTTOM;
	}else if( !block[2] && block[3] ){
		if( bul->y + bul->blockYL > workY &&
			bul->x + bul->blockXL > workX + 3*VS )
			hit |= FLAG_HIT_BOTTOM;
	}

	//バレットの位置補正
	if( bul->bbits & BBITS_REPLACE ){
		if( hit & FLAG_HIT_LEFT ){
			bul->x = workX + bul->blockXL;
		}else
		if( hit & FLAG_HIT_RIGHT ){
			bul->x = workX - bul->blockXL;
		}else
		if( hit & FLAG_HIT_TOP ){
			bul->y = workY + bul->blockYL;
		}else
		if( hit & FLAG_HIT_BOTTOM ){
			bul->y = workY - bul->blockYL;
		}
	}else{

		if( hit & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT|FLAG_HIT_TOP|FLAG_HIT_BOTTOM) ){
			_Vanish( bul );
		}

	}
	
	return hit;
}

//三角形とのあたり判定処理
//左上/右上/左下/右下
long JudgeHitBulletTriangleA( long x, long y, BULLET *bul )
{
	long hit = 0;
	if( bul->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		bul->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( bul->y - RANGE_BULLET_BLOCK < y * PARTSSIZE * VS - ( bul->x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE/4*VS &&
			bul->y + RANGE_BULLET_BLOCK > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){

			if( bul->bbits & BBITS_REPLACE )
				bul->y = y * PARTSSIZE * VS - ( bul->x - (x * PARTSSIZE *VS  ) )/2 + PARTSSIZE/4*VS + RANGE_BULLET_BLOCK;
			else{
				_Vanish( bul );
			}
			hit |= (FLAG_HIT_TOP|FLAG_HIT_RUP);
		}
	}
	return hit;
}

long JudgeHitBulletTriangleB( long x, long y, BULLET *bul )
{
	long hit = 0;
	if( bul->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		bul->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( bul->y - RANGE_BULLET_BLOCK < y * PARTSSIZE * VS - ( bul->x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE/4*VS  &&
			bul->y + RANGE_BULLET_BLOCK > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){

			if( bul->bbits & BBITS_REPLACE )
				bul->y = y * PARTSSIZE * VS - ( bul->x - (x * PARTSSIZE *VS  ) )/2 - PARTSSIZE/4*VS + RANGE_BULLET_BLOCK;
			else{
				_Vanish( bul );
			}
			hit |= (FLAG_HIT_TOP|FLAG_HIT_RUP);
		}
	}
	return hit;
}

long JudgeHitBulletTriangleC( long x, long y, BULLET *bul )
{
	long hit = 0;
	if( bul->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		bul->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( bul->y - RANGE_BULLET_BLOCK < y * PARTSSIZE * VS + ( bul->x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE/4*VS &&
			bul->y + RANGE_BULLET_BLOCK > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){

			if( bul->bbits & BBITS_REPLACE )
				bul->y = y * PARTSSIZE * VS + ( bul->x - (x * PARTSSIZE *VS  ) )/2 - PARTSSIZE/4*VS + RANGE_BULLET_BLOCK;
			else{
				_Vanish( bul );
			}
			hit |= (FLAG_HIT_TOP|FLAG_HIT_LUP);
		}
	}
	return hit;
}

long JudgeHitBulletTriangleD( long x, long y, BULLET *bul )
{
	long hit = 0;
	if( bul->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		bul->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( bul->y - RANGE_BULLET_BLOCK < y * PARTSSIZE * VS + ( bul->x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE/4*VS &&
			bul->y + RANGE_BULLET_BLOCK > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){

			if( bul->bbits & BBITS_REPLACE )
				bul->y = y * PARTSSIZE * VS + ( bul->x - (x * PARTSSIZE *VS  ) )/2 + PARTSSIZE/4*VS + RANGE_BULLET_BLOCK;
			else{
				_Vanish( bul );
			}
			hit |= (FLAG_HIT_TOP|FLAG_HIT_LUP);
		}
	}
	return hit;
}

long JudgeHitBulletTriangleE( long x, long y, BULLET *bul )
{
	long hit = 0;
//	hit |= FLAG_SET_RDOWN;
	if( bul->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		bul->x-VS > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( bul->y + RANGE_BULLET_BLOCK > y * PARTSSIZE * VS + ( bul->x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE*VS/4 &&
			bul->y - RANGE_BULLET_BLOCK < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS ){

			if( bul->bbits & BBITS_REPLACE )
				bul->y = y * PARTSSIZE * VS + ( bul->x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE*VS/4 - RANGE_BULLET_BLOCK;
			else{
				_Vanish( bul );
			}
			hit |= (FLAG_HIT_BOTTOM|FLAG_HIT_RDOWN);
		}
	}
	return hit;
}

long JudgeHitBulletTriangleF( long x, long y, BULLET *bul )
{
	long hit = 0;
	if( bul->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		bul->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( bul->y + RANGE_BULLET_BLOCK > y * PARTSSIZE * VS + ( bul->x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE*VS/4 &&
			bul->y - RANGE_BULLET_BLOCK < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS ){

			if( bul->bbits & BBITS_REPLACE )
				bul->y = y * PARTSSIZE * VS + ( bul->x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE*VS/4 - RANGE_BULLET_BLOCK;
			else{
				_Vanish( bul );
			}
			hit |= (FLAG_HIT_BOTTOM|FLAG_HIT_RDOWN);
		}
	}
	return hit;
}

long JudgeHitBulletTriangleG( long x, long y, BULLET *bul )
{
	long hit = 0;
	if( bul->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		bul->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( bul->y + RANGE_BULLET_BLOCK > y * PARTSSIZE * VS - ( bul->x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE*VS/4 &&
			bul->y - RANGE_BULLET_BLOCK < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS ){

			if( bul->bbits & BBITS_REPLACE )
				bul->y = y * PARTSSIZE * VS - ( bul->x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE*VS/4 - RANGE_BULLET_BLOCK;
			else{
				_Vanish( bul );
			}
			hit |= (FLAG_HIT_BOTTOM|FLAG_HIT_LDOWN);
		}
	}
	return hit;
}

long JudgeHitBulletTriangleH( long x, long y, BULLET *bul )
{
	long hit = 0;
//	hit |= FLAG_SET_LDOWN;
	if( bul->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		bul->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( bul->y + RANGE_BULLET_BLOCK > y * PARTSSIZE * VS - ( bul->x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE*VS/4 &&
			bul->y - RANGE_BULLET_BLOCK < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS ){

			if( bul->bbits & BBITS_REPLACE )
				bul->y = y * PARTSSIZE * VS - ( bul->x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE*VS/4 - RANGE_BULLET_BLOCK;
			else{
				_Vanish( bul );
			}
			hit |= (FLAG_HIT_BOTTOM|FLAG_HIT_LDOWN);
		}
	}

	return hit;
}





//弾の背景とのあたり判定
void HitBulletMap( void )
{
	long x, y;
	unsigned char atrb[4];

	int i, j;
	for( i = 0; i < MAX_BULLET; i++ ){
		if( (gBul[i].cond & COND_ALIVE) == 0 )continue;
		x =  gBul[i].x / PARTSSIZE / VS;
		y =  gBul[i].y / PARTSSIZE / VS;

		//周囲４ブロックとのあたり判定
		long offx[4] = {0,1,0,1};
		long offy[4] = {0,0,1,1};

		atrb[0] = GetAttribute( x + 0, y + 0 );
		atrb[1] = GetAttribute( x + 1, y + 0 );
		atrb[2] = GetAttribute( x + 0, y + 1 );
		atrb[3] = GetAttribute( x + 1, y + 1 );

		
		gBul[i].flag= 0; //&= ~FLAG_HIT_X;
		if( gBul[i].bbits & BBITS_THROWMAP ){
		}else{
			for( j = 0; j < 4; j++ ){
				if( (gBul[i].cond & COND_ALIVE) == 0 )continue;
				switch( atrb[j] ){
				//ノーマルブロック
				case ATRB_BLOCK:
				case ATRB_SNACK:
				case ATRB_EBLOCK:
				case ATRB_BLOCK_W:
				case ATRB_EBLOCK_W:
					gBul[i].flag |= JudgeHitBulletBlock( x + offx[j], y + offy[j], &gBul[i] );
					break;
				case ATRB_TRI_A:
				case ATRB_TRI_A_W:
					gBul[i].flag |= JudgeHitBulletTriangleA( x + offx[j], y + offy[j], &gBul[i] );
					break;
				case ATRB_TRI_B:
				case ATRB_TRI_B_W:
					gBul[i].flag |= JudgeHitBulletTriangleB( x + offx[j], y + offy[j], &gBul[i] );
					break;
				case ATRB_TRI_C:
				case ATRB_TRI_C_W:
					gBul[i].flag |= JudgeHitBulletTriangleC( x + offx[j], y + offy[j], &gBul[i] );
					break;
				case ATRB_TRI_D:
				case ATRB_TRI_D_W:
					gBul[i].flag |= JudgeHitBulletTriangleD( x + offx[j], y + offy[j], &gBul[i] );
					break;
				case ATRB_TRI_E:
				case ATRB_TRI_E_W:
					gBul[i].flag |= JudgeHitBulletTriangleE( x + offx[j], y + offy[j], &gBul[i] );
					break;
				case ATRB_TRI_F:
				case ATRB_TRI_F_W:
					gBul[i].flag |= JudgeHitBulletTriangleF( x + offx[j], y + offy[j], &gBul[i] );
					break;
				case ATRB_TRI_G:
				case ATRB_TRI_G_W:
					gBul[i].flag |= JudgeHitBulletTriangleG( x + offx[j], y + offy[j], &gBul[i] );
					break;
				case ATRB_TRI_H:
				case ATRB_TRI_H_W:
					gBul[i].flag |= JudgeHitBulletTriangleH( x + offx[j], y + offy[j], &gBul[i] );
					break;
	//			case ATRB_WATER:
	//				gBul[i].flag |= JudgeHitBulletWater( x + offx[j], y + offy[j], i );
	//				break;

				}
			}
			gBul[i].flag |= JudgeHitBulletBlock2( x, y, atrb, &gBul[i] );
		}
	}

}
#include <windows.h>
#include <stdio.h>
#include "Define.h"

#include "Tags.h"
#include "Map.h"

extern long gWaterY;

void SetExpObjects( long x, long y, long exp );

extern NPCHAR gNPC[];
void JadgeHitNpCharBlock( NPCHAR *npc, long x, long y )
{
	long hit;
	hit = 0;
	//left hit
	if( npc->y - npc->hit.top < ( y * PARTSSIZE + PARTSSIZE/2 - 3 ) * VS &&
		npc->y + npc->hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 + 3 ) * VS ){
		if( npc->x - npc->hit.back < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
			npc->x - npc->hit.back > x * PARTSSIZE * VS ){
			npc->x = ( x * PARTSSIZE + PARTSSIZE/2 ) * VS + npc->hit.back;
			hit |= FLAG_HIT_LEFT;
		}
	}
	//right hit
	if( npc->y - npc->hit.top < ( y * PARTSSIZE + PARTSSIZE/2 - 3 ) * VS &&
		npc->y + npc->hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 + 3 ) * VS ){
		if( npc->x + npc->hit.back > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS &&
			npc->x + npc->hit.back < x * PARTSSIZE * VS ){
			npc->x = ( x * PARTSSIZE - PARTSSIZE/2 ) * VS - npc->hit.back;
			hit |= FLAG_HIT_RIGHT;
		}
	}
	//top hit
	if( npc->x - npc->hit.back < ( x * PARTSSIZE + PARTSSIZE/2 - 3 ) * VS &&
		npc->x + npc->hit.back > ( x * PARTSSIZE - PARTSSIZE/2 + 3 ) * VS ){
		if( npc->y - npc->hit.top < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS &&
			npc->y - npc->hit.top > y * PARTSSIZE * VS ){
			npc->y = ( y * PARTSSIZE + PARTSSIZE/2 ) * VS + npc->hit.top;
			npc->ym = 0;
			hit |= FLAG_HIT_TOP;
		}
	}
	//bottom hit
	if( npc->x - npc->hit.back < ( x * PARTSSIZE + PARTSSIZE/2 - 3 ) * VS &&
		npc->x + npc->hit.back > ( x * PARTSSIZE - PARTSSIZE/2 + 3 ) * VS ){
		if( npc->y + npc->hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS &&
			npc->y + npc->hit.bottom < y * PARTSSIZE * VS ){
			npc->y = ( y * PARTSSIZE - PARTSSIZE/2 ) * VS - npc->hit.bottom;
			npc->ym = 0;
			hit |= FLAG_HIT_BOTTOM;
		}
	}
	npc->flag |= hit;
}

//三角形とのあたり判定処理
//左上/右上/左下/右下
void JudgeHitNpCharTriangleA( NPCHAR *npc, long x, long y )
{
	long hit = 0;
	if( npc->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		npc->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( npc->y - npc->hit.top < y * PARTSSIZE * VS - ( npc->x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE/4*VS &&
			npc->y + npc->hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){
			npc->y = y * PARTSSIZE * VS - ( npc->x - (x * PARTSSIZE *VS  ) )/2 + PARTSSIZE/4*VS + npc->hit.top;
			if( npc->ym < 0 )npc->ym = 0;
			hit |= FLAG_HIT_TOP;
		}
	}
	npc->flag |= hit;
}

void JudgeHitNpCharTriangleB( NPCHAR *npc, long x, long y )
{
	long hit = 0;
	if( npc->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		npc->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( npc->y - npc->hit.top < y * PARTSSIZE * VS - ( npc->x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE/4*VS  &&
			npc->y + npc->hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){
			npc->y = y * PARTSSIZE * VS - ( npc->x - (x * PARTSSIZE *VS  ) )/2 - PARTSSIZE/4*VS + npc->hit.top;
			if( npc->ym < 0 )npc->ym = 0;
			hit |= FLAG_HIT_TOP;
		}
	}
	npc->flag |= hit;
}

void JudgeHitNpCharTriangleC( NPCHAR *npc, long x, long y )
{
	long hit = 0;
	if( npc->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		npc->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( npc->y - npc->hit.top < y * PARTSSIZE * VS + ( npc->x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE/4*VS &&
			npc->y + npc->hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){
			npc->y = y * PARTSSIZE * VS + ( npc->x - (x * PARTSSIZE *VS  ) )/2 - PARTSSIZE/4*VS + npc->hit.top;
			if( npc->ym < 0 )npc->ym = 0;
			hit |= FLAG_HIT_TOP;
		}
	}
	npc->flag |= hit;
}

void JudgeHitNpCharTriangleD( NPCHAR *npc, long x, long y )
{
	long hit = 0;
	if( npc->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		npc->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( npc->y - npc->hit.top < y * PARTSSIZE * VS + ( npc->x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE/4*VS &&
			npc->y + npc->hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2 ) * VS ){
			npc->y = y * PARTSSIZE * VS + ( npc->x - (x * PARTSSIZE *VS  ) )/2 + PARTSSIZE/4*VS + npc->hit.top;
			if( npc->ym < 0 )npc->ym = 0;
			hit |= FLAG_HIT_TOP;
		}
	}
	npc->flag |= hit;
}

void JudgeHitNpCharTriangleE( NPCHAR *npc, long x, long y )
{
	long hit = 0;
	hit |= FLAG_SET_TRI_E;
	if( npc->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		npc->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( npc->y + npc->hit.bottom > y * PARTSSIZE * VS + ( npc->x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE*VS/4 &&
			npc->y - npc->hit.top < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS ){
			npc->y = y * PARTSSIZE * VS + ( npc->x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE*VS/4 - npc->hit.bottom;
			if( npc->ym > 0 )npc->ym = 0;
			hit |= FLAG_HIT_BOTTOM|FLAG_HIT_RDOWN;
		}
	}
	npc->flag |= hit;
}

void JudgeHitNpCharTriangleF( NPCHAR *npc, long x, long y )
{
	long hit = 0;
	hit |= FLAG_SET_TRI_F;
	if( npc->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		npc->x >= ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( npc->y + npc->hit.bottom > y * PARTSSIZE * VS + ( npc->x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE*VS/4 &&
			npc->y - npc->hit.top < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS ){
			npc->y = y * PARTSSIZE * VS + ( npc->x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE*VS/4 - npc->hit.bottom;
			if( npc->ym > 0 )npc->ym = 0;
			hit |= FLAG_HIT_BOTTOM|FLAG_HIT_RDOWN;
		}
	}
	npc->flag |= hit;
}

void JudgeHitNpCharTriangleG( NPCHAR *npc, long x, long y )
{
	long hit = 0;
	hit |= FLAG_SET_TRI_G;
	if( npc->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		npc->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( npc->y + npc->hit.bottom > y * PARTSSIZE * VS - ( npc->x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE*VS/4 &&
			npc->y - npc->hit.top < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS ){
			npc->y = y * PARTSSIZE * VS - ( npc->x - (x * PARTSSIZE *VS ) )/2 + PARTSSIZE*VS/4 - npc->hit.bottom;
			if( npc->ym > 0 )npc->ym = 0;
			hit |= FLAG_HIT_BOTTOM|FLAG_HIT_LDOWN;
		}
	}
	npc->flag |= hit;
}

void JudgeHitNpCharTriangleH( NPCHAR *npc, long x, long y )
{
	long hit = 0;
	hit |= FLAG_SET_TRI_H;
	if( npc->x < ( x * PARTSSIZE + PARTSSIZE/2 ) * VS &&
		npc->x > ( x * PARTSSIZE - PARTSSIZE/2 ) * VS ){
		if( npc->y + npc->hit.bottom > y * PARTSSIZE * VS - ( npc->x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE*VS/4 &&
			npc->y - npc->hit.top < ( y * PARTSSIZE + PARTSSIZE/2 ) * VS ){
			npc->y = y * PARTSSIZE * VS - ( npc->x - (x * PARTSSIZE *VS ) )/2 - PARTSSIZE*VS/4 - npc->hit.bottom;
			if( npc->ym > 0 )npc->ym = 0;
			hit |= FLAG_HIT_BOTTOM|FLAG_HIT_LDOWN;
		}
	}
	npc->flag |= hit;
}

void JudgeHitNpCharWater( NPCHAR *npc, long x, long y )
{
	long hit = 0;
	if( npc->x - npc->hit.back   < ( x * PARTSSIZE + PARTSSIZE/2-2 ) * VS &&
		npc->x + npc->hit.back   > ( x * PARTSSIZE - PARTSSIZE/2+2 ) * VS &&
		npc->y - npc->hit.top    < ( y * PARTSSIZE + PARTSSIZE/2-2 ) * VS &&
		npc->y + npc->hit.bottom > ( y * PARTSSIZE - PARTSSIZE/2+2 ) * VS ){
		hit |= FLAG_HIT_WATER;
	}
	npc->flag |= hit;
}

//NPキャラの背景とのあたり判定
void HitNpCharMap( void )
{
	long x, y;
	long judg;
//	unsigned char atrb;

	//周囲４(幅によっては９つ)ブロックとのあたり判定
	long offx[9] = {0,1,0,1,  2,2,2,0,1};
	long offy[9] = {0,0,1,1,  0,1,2,2,2};


	int i, j;
	for( i = 0; i < MAX_NPC; i++ ){
		if( !(gNPC[i].cond & COND_ALIVE)    ) continue;
		if( gNPC[i].bits & BITS_THROW_BLOCK ) continue;


		if( gNPC[i].size >= NPCSIZE_LARGE ){
			judg = 9;
			x =  (gNPC[i].x - 8*VS) / PARTSSIZE / VS;
			y =  (gNPC[i].y - 8*VS) / PARTSSIZE / VS;
		}else{
			judg = 4;
			x =  gNPC[i].x / PARTSSIZE / VS;
			y =  gNPC[i].y / PARTSSIZE / VS;
		}

		
		gNPC[i].flag= 0; //&= ~FLAG_HIT_X;
		for( j = 0; j < judg; j++ ){
			switch( GetAttribute( x + offx[j], y + offy[j] ) ){
			//ノーマルブロック
			case ATRB_EBLOCK: if( gNPC[i].bits & BITS_THROW_EBLOCK ) break;
			case ATRB_BLOCK:
			case ATRB_BACK_EB:
			case ATRB_BACK_BULTH:
			case ATRB_SNACK: JadgeHitNpCharBlock(     &gNPC[i], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_A: JudgeHitNpCharTriangleA( &gNPC[i], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_B: JudgeHitNpCharTriangleB( &gNPC[i], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_C: JudgeHitNpCharTriangleC( &gNPC[i], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_D: JudgeHitNpCharTriangleD( &gNPC[i], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_E: JudgeHitNpCharTriangleE( &gNPC[i], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_F: JudgeHitNpCharTriangleF( &gNPC[i], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_G: JudgeHitNpCharTriangleG( &gNPC[i], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_H: JudgeHitNpCharTriangleH( &gNPC[i], x + offx[j], y + offy[j] ); break;
			case ATRB_FRONT_W:
			case ATRB_BACK_W:
			case ATRB_DAMAGE_W:JudgeHitNpCharWater( &gNPC[i], x + offx[j], y + offy[j] ); break;
//			case ATRB_WATER: 

			case ATRB_BLOCK_W:
			case ATRB_EBLOCK_W:
			case ATRB_BACK_EB_W:
				JadgeHitNpCharBlock( &gNPC[i], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gNPC[i], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_A_W:
				JudgeHitNpCharTriangleA( &gNPC[i], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gNPC[i], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_B_W:
				JudgeHitNpCharTriangleB( &gNPC[i], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gNPC[i], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_C_W:
				JudgeHitNpCharTriangleC( &gNPC[i], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gNPC[i], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_D_W:
				JudgeHitNpCharTriangleD( &gNPC[i], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gNPC[i], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_E_W:
				JudgeHitNpCharTriangleE( &gNPC[i], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gNPC[i], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_F_W:
				JudgeHitNpCharTriangleF( &gNPC[i], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gNPC[i], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_G_W:
				JudgeHitNpCharTriangleG( &gNPC[i], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gNPC[i], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_H_W:
				JudgeHitNpCharTriangleH( &gNPC[i], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gNPC[i], x + offx[j], y + offy[j] );
				break;
			case ATRB_LEFT_W:
				gNPC[i].flag |= FLAG_HIT_WATER;
			case ATRB_LEFT:
				gNPC[i].flag |= FLAG_HIT_VECT_LEFT;  break;
			case ATRB_UP_W:
				gNPC[i].flag |= FLAG_HIT_WATER;
			case ATRB_UP:
				gNPC[i].flag |= FLAG_HIT_VECT_UP;    break;
			case ATRB_RIGHT_W:
				gNPC[i].flag |= FLAG_HIT_WATER;
			case ATRB_RIGHT:
				gNPC[i].flag |= FLAG_HIT_VECT_RIGHT; break;
			case ATRB_DOWN_W:
				gNPC[i].flag |= FLAG_HIT_WATER;
			case ATRB_DOWN:
				gNPC[i].flag |= FLAG_HIT_VECT_DOWN;  break;
			}
			if( gNPC[i].y > gWaterY+4*VS ) gNPC[i].flag |= FLAG_HIT_WATER;

		}
	}
}

extern BULLET gBul[];

void SetCaret( long x, long y, long code, long dir );
void SetValueView( long *px, long *py, long value );
void SetNPCFlag( long a );
BOOL GetNPCFlag( long a );
void SetNpChar( long code_char, long x, long y, long xm, long ym,
				long dir, NPCHAR *npc, long start_index );


#include "Sound.h"
BOOL StartTextScript( long no );

extern MYCHAR gMC;
void VanishNpChar( NPCHAR *npc );
void SetDestroyNpChar( long x, long y, long w, long num );
BOOL SetBulletObject( long x, long y, long val );
BOOL SetLifeObject(   long x, long y, long val );
long Random( int min, int max );


void LoseNpChar( NPCHAR *npc, BOOL bVanish )
{
	long val;

	PlaySoundObject( npc->destroy_voice, 1 );
	switch( npc->size ){
	case NPCSIZE_SMALL:  SetDestroyNpChar( npc->x, npc->y, npc->view.back,  3 );break;
	case NPCSIZE_MIDDLE: SetDestroyNpChar( npc->x, npc->y, npc->view.back,  7 );break;
	case NPCSIZE_LARGE:  SetDestroyNpChar( npc->x, npc->y, npc->view.back, 12 );break;
	}


	if( npc->exp ){
		switch( Random( 1,5 ) ){
		case 1:
			if( npc->exp > 6 ) val = 6;
			else               val = 2;
			SetLifeObject( npc->x, npc->y, val );
			break;
		case 2:
			if( npc->exp > 6 ) val = 3;
			else               val = 1;
			if( SetBulletObject( npc->x, npc->y, val ) ) break;
		default: SetExpObjects( npc->x, npc->y, npc->exp ); break;
		}
	}

	SetNPCFlag( npc->code_flag );

	if( npc->bits & BITS_VIEWDAMAGE ){
		// 経験値を持つ場合はダメージ表示
		if( npc->bits & BITS_VIEWDAMAGE && npc->damage_view ) SetValueView( &npc->x, &npc->y, npc->damage_view );
		if( bVanish ) VanishNpChar( npc );
	}else{
		npc->cond = 0;
	}

}

void HitNpCharBullet( void )
{

	long n, b;
	BOOL bHit;

	for( n = 0; n < MAX_NPC; n++ ){

		if( !(gNPC[n].cond & COND_ALIVE )     ) continue; 
		if(  gNPC[n].bits & BITS_BANISH_DAMAGE &&
			 gNPC[n].bits & BITS_EVENT_CHECK ) continue;

	
		for( b = 0; b < MAX_BULLET; b++ ){
			if( !(gBul[b].cond & COND_ALIVE) ) continue;
			if( gBul[b].damage   == -1       ) continue;

			bHit = FALSE;
			// 弾のヒット
			if(       gNPC[n].bits & BITS_BANISH_DAMAGE &&
				gNPC[n].x - gNPC[n].hit.back   < gBul[b].x + gBul[b].enemyXL &&
				gNPC[n].x + gNPC[n].hit.back   > gBul[b].x - gBul[b].enemyXL &&
				gNPC[n].y - gNPC[n].hit.top    < gBul[b].y + gBul[b].enemyYL &&
				gNPC[n].y + gNPC[n].hit.bottom > gBul[b].y - gBul[b].enemyYL ){

				bHit = TRUE;
			}else if( gNPC[n].bits & BITS_BLOCK_BULLET &&
				gNPC[n].x - gNPC[n].hit.back   < gBul[b].x + gBul[b].blockXL &&
				gNPC[n].x + gNPC[n].hit.back   > gBul[b].x - gBul[b].blockXL &&
				gNPC[n].y - gNPC[n].hit.top    < gBul[b].y + gBul[b].blockYL &&
				gNPC[n].y + gNPC[n].hit.bottom > gBul[b].y - gBul[b].blockYL ){

				bHit = TRUE;

			}
			if( bHit ){
				
				// ダメージ消滅場合
				if( gNPC[n].bits & BITS_BANISH_DAMAGE ){

					gNPC[n].life  -= gBul[b].damage;
					// 体力ゼロで消滅
					if( gNPC[n].life < 1){
						gNPC[n].life = 0;
						if( gNPC[n].bits & BITS_VIEWDAMAGE ) gNPC[n].damage_view -= gBul[b].damage;//*gBul[b].life;
						// 消滅イベント
						if( gMC.cond & COND_ALIVE && gNPC[n].bits & BITS_EVENT_BREAK ){
							StartTextScript( gNPC[n].code_event );
						}else{
							gNPC[n].cond |= COND_LOSE;
						}
					}else{
						if( gNPC[n].shock < WAIT_NPC_DAMAGE - 2 ){
							SetCaret( (gBul[b].x + gNPC[n].x)/2, (gBul[b].y + gNPC[n].y)/2, CARET_DAMAGE, DIR_LEFT );
							SetCaret( (gBul[b].x + gNPC[n].x)/2, (gBul[b].y + gNPC[n].y)/2, CARET_DAMAGE, DIR_LEFT );
							SetCaret( (gBul[b].x + gNPC[n].x)/2, (gBul[b].y + gNPC[n].y)/2, CARET_DAMAGE, DIR_LEFT );
							PlaySoundObject( gNPC[n].hit_voice, 1 );
							gNPC[n].shock = WAIT_NPC_DAMAGE;
						}
						if( gNPC[n].bits & BITS_VIEWDAMAGE ) gNPC[n].damage_view -= gBul[b].damage;
					}
					//弾丸の消滅
				}else if( gBul[b].code_bullet == BULLET_MISSILE1||
						  gBul[b].code_bullet == BULLET_MISSILE2||
						  gBul[b].code_bullet == BULLET_MISSILE3||
						  gBul[b].code_bullet == BULLET_SUPERMISSILE1||
						  gBul[b].code_bullet == BULLET_SUPERMISSILE2||
						  gBul[b].code_bullet == BULLET_SUPERMISSILE3 ){
				}else{
					if( !(gBul[b].bbits & BBITS_NOBREAK) ){
						
						SetCaret( (gBul[b].x + gNPC[n].x)/2, (gBul[b].y + gNPC[n].y)/2, CARET_SMOKE, DIR_RIGHT );
						PlaySoundObject( WAVE_NODAMAGE, 1 );
						gBul[b].life = 0;
						continue;
					}
				}
				gBul[b].life--;
			}
		}
		if( gNPC[n].cond & COND_LOSE ) LoseNpChar( &gNPC[n], TRUE );
	}
}
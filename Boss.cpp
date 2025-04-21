#include <windows.h>
#include "Sound.h"
#include "Draw.h"
#include "Define.h"

#include "Tags.h"


extern MYCHAR gMC;
NPCHAR gBoss[];
extern RECT grcGame;
void SetValueView( long *px, long *py, long value );
void SetCaret( long x, long y, long code, long dir );
unsigned char GetAttribute( long x, long y );
BOOL StartTextScript( long no );

void SetDestroyNpChar( long x, long y, long w, long num );

void InitBossChar( long code )
{
	memset( gBoss, 0, sizeof(NPCHAR)*MAX_BOSS_PARTS );
	gBoss[0].cond = COND_ALIVE;
	gBoss[0].code_char = code;
}

void PutBossChar( long fx, long fy )
{
	char a = 0;
	long b;

	long side;

	for( b = MAX_BOSS_PARTS-1; b >= 0; b-- ){
		if( gBoss[b].cond & COND_ALIVE ){
			
			//ダメージブレ
			if( gBoss[b].shock ){
				a = (-1) + gBoss[b].shock/2%2 * 2;
			}else{
				a = 0;
				if( gBoss[b].bits & BITS_VIEWDAMAGE && gBoss[b].damage_view ){
					SetValueView( &gBoss[b].x, &gBoss[b].y, gBoss[b].damage_view );//ダメージ表示
					gBoss[b].damage_view = 0;
				}
			}

			if( gBoss[b].direct == DIR_LEFT ) side = gBoss[b].view.front;
			else                              side = gBoss[b].view.back;
			PutBitmap3( &grcGame, 
				( gBoss[b].x - side             )/VS - fx/VS + a,
				( gBoss[b].y - gBoss[b].view.top)/VS - fy/VS,
				&gBoss[b].rect, SURF_NPC_BOSS );
		}
	}
}

void SetBossCharActNo( long a )
{
	gBoss[0].act_no = a;
}

extern BULLET gBul[];


void HitBossBullet( void )
{
	BOOL bHit;
	long bul;
	long bos;
	long bos_;
	for( bos = 0; bos < MAX_BOSS_PARTS; bos++ ){

		if( !(gBoss[bos].cond & COND_ALIVE) ) continue;

		for( bul = 0; bul < MAX_BULLET; bul++ ){

			if( !(gBul[bul].cond & COND_ALIVE) ) continue;
			if( gBul[bul].damage == -1         ) continue;
			bHit = FALSE;
			// 弾のヒット
			if( gBoss[bos].bits & BITS_BANISH_DAMAGE &&
				gBoss[bos].x - gBoss[bos].hit.back   < gBul[bul].x + gBul[bul].enemyXL &&
				gBoss[bos].x + gBoss[bos].hit.back   > gBul[bul].x - gBul[bul].enemyXL &&
				gBoss[bos].y - gBoss[bos].hit.top    < gBul[bul].y + gBul[bul].enemyYL &&
				gBoss[bos].y + gBoss[bos].hit.bottom > gBul[bul].y - gBul[bul].enemyYL ){
				bHit = TRUE;
				
			}else if( gBoss[bos].bits & BITS_BLOCK_BULLET &&
				gBoss[bos].x - gBoss[bos].hit.back   < gBul[bul].x + gBul[bul].blockXL &&
				gBoss[bos].x + gBoss[bos].hit.back   > gBul[bul].x - gBul[bul].blockXL &&
				gBoss[bos].y - gBoss[bos].hit.top    < gBul[bul].y + gBul[bul].blockYL &&
				gBoss[bos].y + gBoss[bos].hit.bottom > gBul[bul].y - gBul[bul].blockYL ){
				bHit = TRUE;
			}
			if( bHit ){
				if( gBoss[bos].bits & BITS_BANISH_DAMAGE){
					if( gBoss[bos].cond & COND_ZEROINDEXDAMAGE ) bos_ = 0;
					else                                         bos_ = bos;

					gBoss[bos_].life -= gBul[bul].damage;
					// 体力ゼロで消滅
					if( gBoss[bos_].life < 1 ){
						gBoss[bos_].life = bos_;
						if( gMC.cond & COND_ALIVE && gBoss[bos_].bits & BITS_EVENT_BREAK ){
							StartTextScript( gBoss[bos_].code_event );
						}else{
							PlaySoundObject( gBoss[bos_].destroy_voice, 1 );
							switch( gBoss[bos_].size ){
							case NPCSIZE_SMALL:  SetDestroyNpChar( gBoss[bos_].x, gBoss[bos_].y, gBoss[bos_].view.back,  4 );break;
							case NPCSIZE_MIDDLE: SetDestroyNpChar( gBoss[bos_].x, gBoss[bos_].y, gBoss[bos_].view.back,  8 );break;
							case NPCSIZE_LARGE:  SetDestroyNpChar( gBoss[bos_].x, gBoss[bos_].y, gBoss[bos_].view.back, 16 );break;
							}
							gBoss[bos_].cond = 0;
						}
					}else{
						if( gBoss[bos ].shock < WAIT_NPC_DAMAGE - 2 ){
							SetCaret( gBul[bul].x, gBul[bul].y, CARET_DAMAGE, DIR_LEFT );
							SetCaret( gBul[bul].x, gBul[bul].y, CARET_DAMAGE, DIR_LEFT );
							SetCaret( gBul[bul].x, gBul[bul].y, CARET_DAMAGE, DIR_LEFT );
							PlaySoundObject( gBoss[bos_].hit_voice, 1 );
						}
						gBoss[bos ].shock = 8;
						gBoss[bos_].shock = 8;
						gBoss[bos_].damage_view -= gBul[bul].damage;
					}
					//弾丸の消滅
					gBul[bul].life--;
					if( gBul[bul].life < 1 ){
						gBul[bul].cond = 0;
					}
				}else if( gBul[bul].code_bullet == BULLET_MISSILE1||
						  gBul[bul].code_bullet == BULLET_MISSILE2||
						  gBul[bul].code_bullet == BULLET_MISSILE3||
						  gBul[bul].code_bullet == BULLET_SUPERMISSILE1||
						  gBul[bul].code_bullet == BULLET_SUPERMISSILE2||
						  gBul[bul].code_bullet == BULLET_SUPERMISSILE3 ){
					gBul[bul].life--;
				}else{
					if( !(gBul[bul].bbits & BBITS_NOBREAK )){
						SetCaret( gBul[bul].x, gBul[bul].y, CARET_SMOKE, DIR_RIGHT );
						PlaySoundObject( WAVE_NODAMAGE, 1 );
						gBul[bul].cond = 0;
						continue;
					}
				}
			}
		}
	}

}



void ActBossChar_0(        void ){}
void ActBossChar_Omega(    void );
void ActBossChar_Frog(     void );
void ActBossChar_MonstX(   void );
void ActBossChar_Core(     void );
void ActBossChar_Ironhead( void );
void ActBossChar_Twin(     void );
void ActBossChar_Undead(   void );
void ActBossChar_Press(    void );
void ActBossChar_Ballos(   void );

BOSSFUNCTION gpBossFuncTbl[] = {
	ActBossChar_0,
	ActBossChar_Omega,
	ActBossChar_Frog,
	ActBossChar_MonstX,
	ActBossChar_Core,
	ActBossChar_Ironhead,//5
	ActBossChar_Twin,    //6
	ActBossChar_Undead,  //7,
	ActBossChar_Press,   //8,
	ActBossChar_Ballos,  //9,
};

void ActBossChar( void )
{
	long code_char;
	long bos;

	if( (gBoss[0].cond & COND_ALIVE) == 0 ) return;

	code_char = gBoss[0].code_char;
	gpBossFuncTbl[code_char]( );

	//ダメージ点滅
	for( bos = 0; bos < MAX_BOSS_PARTS; bos++ ){
		if( gBoss[bos].shock != 0 )gBoss[bos].shock--;
	}
}











void SetExpObjects( long x, long y, long exp );

extern NPCHAR gNPC[];
void JadgeHitNpCharBlock( NPCHAR *npc, long x, long y );
void JudgeHitNpCharTriangleA( NPCHAR *npc, long x, long y );
void JudgeHitNpCharTriangleB( NPCHAR *npc, long x, long y );
void JudgeHitNpCharTriangleC( NPCHAR *npc, long x, long y );
void JudgeHitNpCharTriangleD( NPCHAR *npc, long x, long y );
void JudgeHitNpCharTriangleE( NPCHAR *npc, long x, long y );
void JudgeHitNpCharTriangleF( NPCHAR *npc, long x, long y );
void JudgeHitNpCharTriangleG( NPCHAR *npc, long x, long y );
void JudgeHitNpCharTriangleH( NPCHAR *npc, long x, long y );
void JudgeHitNpCharWater( NPCHAR *npc, long x, long y );

// ボスとブロックの当たり判定
void HitBossMap( void )
{
	long x, y;
	long judg;
	unsigned char atrb[16];


	//周囲４(幅によっては９つ)ブロックとのあたり判定
	long offx[16] = {0,1,0,1,  2,2,2,0,1, -1,-1,-1,-1,  0, 1, 2};
	long offy[16] = {0,0,1,1,  0,1,2,2,2, -1, 0, 1, 2, -1,-1,-1};


	int b, j;
	for( b = 0; b < MAX_BOSS_PARTS; b++ ){
		if( !( gBoss[b].cond & COND_ALIVE )   ) continue;
		if( gBoss[b].bits & BITS_THROW_BLOCK  ) continue;


		if( gBoss[b].size >= NPCSIZE_LARGE ){
			judg = 16;
			x =  (gBoss[b].x) / PARTSSIZE / VS;
			y =  (gBoss[b].y) / PARTSSIZE / VS;
//			x =  (gBoss[b].x - 8*VS) / PARTSSIZE / VS;
//			y =  (gBoss[b].y - 8*VS) / PARTSSIZE / VS;
		}else{
			judg = 4;
			x =  gBoss[b].x / PARTSSIZE / VS;
			y =  gBoss[b].y / PARTSSIZE / VS;
		}

		
		gBoss[b].flag= 0; //&= ~FLAG_HIT_X;
		for( j = 0; j < judg; j++ ){
			atrb[j] = GetAttribute( x + offx[j], y + offy[j] );
			switch( atrb[j] ){
			//ノーマルブロック
			case ATRB_EBLOCK: if( gBoss[b].bits & BITS_THROW_EBLOCK ) break;
			case ATRB_BLOCK: 
			case ATRB_BACK_BULTH:
			case ATRB_SNACK: JadgeHitNpCharBlock(     &gBoss[b], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_A: JudgeHitNpCharTriangleA( &gBoss[b], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_B: JudgeHitNpCharTriangleB( &gBoss[b], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_C: JudgeHitNpCharTriangleC( &gBoss[b], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_D: JudgeHitNpCharTriangleD( &gBoss[b], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_E: JudgeHitNpCharTriangleE( &gBoss[b], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_F: JudgeHitNpCharTriangleF( &gBoss[b], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_G: JudgeHitNpCharTriangleG( &gBoss[b], x + offx[j], y + offy[j] ); break;
			case ATRB_TRI_H: JudgeHitNpCharTriangleH( &gBoss[b], x + offx[j], y + offy[j] ); break;
			case ATRB_FRONT_W:
			case ATRB_BACK_W:
//			case ATRB_DAMAGE_W:JudgeHitNpCharWater( &gBoss[b], x + offx[j], y + offy[j] ); break;
//			case ATRB_WATER: 

			case ATRB_BLOCK_W:
			case ATRB_EBLOCK_W:
				JadgeHitNpCharBlock( &gBoss[b], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gBoss[b], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_A_W:
				JudgeHitNpCharTriangleA( &gBoss[b], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gBoss[b], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_B_W:
				JudgeHitNpCharTriangleB( &gBoss[b], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gBoss[b], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_C_W:
				JudgeHitNpCharTriangleC( &gBoss[b], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gBoss[b], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_D_W:
				JudgeHitNpCharTriangleD( &gBoss[b], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gBoss[b], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_E_W:
				JudgeHitNpCharTriangleE( &gBoss[b], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gBoss[b], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_F_W:
				JudgeHitNpCharTriangleF( &gBoss[b], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gBoss[b], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_G_W:
				JudgeHitNpCharTriangleG( &gBoss[b], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gBoss[b], x + offx[j], y + offy[j] );
				break;
			case ATRB_TRI_H_W:
				JudgeHitNpCharTriangleH( &gBoss[b], x + offx[j], y + offy[j] );
				JudgeHitNpCharWater( &gBoss[b], x + offx[j], y + offy[j] );
				break;
			case ATRB_LEFT_W:
				FLAG_HIT_WATER;
			case ATRB_LEFT:
				FLAG_HIT_VECT_LEFT|FLAG_HIT_WATER; break;
			case ATRB_UP_W:
				FLAG_HIT_WATER;
			case ATRB_UP:
				FLAG_HIT_VECT_UP; break;
			case ATRB_RIGHT_W:
				FLAG_HIT_WATER;
			case ATRB_RIGHT:
				FLAG_HIT_VECT_RIGHT; break;
			case ATRB_DOWN_W:
				FLAG_HIT_WATER;
			case ATRB_DOWN:
				FLAG_HIT_VECT_DOWN; break;
			}
		}
	}
}

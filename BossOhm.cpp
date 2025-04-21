#include <windows.h>
#include "Sound.h"
#include "Draw.h"
#include "Define.h"

#include "Tags.h"

extern MYCHAR gMC;


NPCHAR gBoss[MAX_BOSS_PARTS];

long Random( int min, int max );
void SetNpChar( long code_char, long x, long y, long xm, long ym,
				long dir, NPCHAR *npc, long start_index );
void SetCaret( long x, long y, long code, long dir );
void SetQuake( long time );
void ChangeAllNpCharByCode( long code1, long code2 );

int GetSin( unsigned char deg );
int GetCos( unsigned char deg );
unsigned char GetArktan( long x,long y );

void SetDestroyNpChar( long x, long y, long w, long num );
void DeleteNpCharCode( long code, BOOL bSmoke );

void SetFlash( long x, long y, long mode );

long CountArmsBullet( long arms_code );

// ひざ
static void ActBoss01_12( void )
{
	long i;

	RECT rcLeft[] = {
		{ 80, 56,104, 72},
	};
	RECT rcRight[] = {
		{104, 56,128, 72},
	};



	


	for( i = 1; i < 3; i++ ){

		gBoss[i].y = (gBoss[0].y + gBoss[i+2].y-8*VS )/2;

		if( gBoss[i].direct == DIR_LEFT ){
			gBoss[i].x = gBoss[0].x - 16*VS;
			gBoss[i].rect = rcLeft[ gBoss[i].ani_no];
		}else{     
			gBoss[i].rect = rcRight[gBoss[i].ani_no];
			gBoss[i].x = gBoss[0].x + 16*VS;
		}
	}
}


// つま先
static void ActBoss01_34( void )
{
	long i;

	RECT rcLeft[] = {
		{  0, 56, 40, 88},
		{ 40, 56, 80, 88},
	};
	RECT rcRight[] = {
		{  0, 88, 40,120},
		{ 40, 88, 80,120},
	};

	for( i = 3; i < 5; i++ ){ 
		switch( gBoss[i].act_no ){
		case 0://初期パラメータ
			gBoss[i].act_no = 1;
		case 1://待機
			gBoss[i].y = gBoss[0].y;
			if( i == 3 ) gBoss[i].x = gBoss[0].x - 16*VS;
			if( i == 4 ) gBoss[i].x = gBoss[0].x + 16*VS;
			break;
		case 2://出現
			break;
		case 3://メイン

			gBoss[i].tgt_y = gBoss[0].y+24*VS;
			if( i == 3 ) gBoss[i].x = gBoss[0].x - 16*VS;
			if( i == 4 ) gBoss[i].x = gBoss[0].x + 16*VS;


			//目標座標を追いかける。
//			if( gBoss[i].x < gBoss[i].tgt_x ) gBoss[i].xm += VS;
//			if( gBoss[i].x > gBoss[i].tgt_x ) gBoss[i].xm -= VS;
//			if( gBoss[i].y < gBoss[i].tgt_y ) gBoss[i].ym += VS;
//			if( gBoss[i].y > gBoss[i].tgt_y ) gBoss[i].ym -= VS;

//			if( gBoss[i].xm >  MAX_MOVE )gBoss[i].xm =  MAX_MOVE;
//			if( gBoss[i].xm < -MAX_MOVE )gBoss[i].xm = -MAX_MOVE;
//			if( gBoss[i].ym >  MAX_MOVE )gBoss[i].ym =  MAX_MOVE;
//			if( gBoss[i].ym < -MAX_MOVE )gBoss[i].ym = -MAX_MOVE;
//			gBoss[i].y += gBoss[i].ym;

			gBoss[i].y += (gBoss[i].tgt_y - gBoss[i].y)/2;
			break;
		}



		
		// 地面についているとき 離れてる時
		if( gBoss[i].flag & FLAG_HIT_BOTTOM || gBoss[i].y <= gBoss[i].tgt_y )
			gBoss[i].ani_no = 0;
		else
			gBoss[i].ani_no = 1;
	
		if( gBoss[i].direct == DIR_LEFT ) gBoss[i].rect = rcLeft[ gBoss[i].ani_no];
		else                              gBoss[i].rect = rcRight[gBoss[i].ani_no];
	}
}

// マイキャラ辺り判定。
static void ActBoss01_5( void )
{
	switch( gBoss[5].act_no ){
	case 0:
		gBoss[5].bits |= BITS_BLOCK_MYCHAR|BITS_THROW_BLOCK;
		gBoss[5].hit.front  = 20*VS;
		gBoss[5].hit.top    = 36*VS;
		gBoss[5].hit.back   = 20*VS;
		gBoss[5].hit.bottom = 16*VS;
		gBoss[5].act_no = 1;
	case 1:
		gBoss[5].x = gBoss[0].x;
		gBoss[5].y = gBoss[0].y;
		break;
	}
}

void ActBossChar_Omega( void )
{


	switch( gBoss[0].act_no ){
	case 0:// 初期設定
		gBoss[0].x = 219*VS*PARTSSIZE;
		gBoss[0].y =  16*VS*PARTSSIZE;
		gBoss[0].view.front  = 40*VS;
		gBoss[0].view.top    = 40*VS;
		gBoss[0].view.back   = 40*VS;
		gBoss[0].view.bottom = 16*VS;
		gBoss[0].tgt_x = gBoss[0].x;
		gBoss[0].tgt_y = gBoss[0].y;
		gBoss[0].hit_voice = WAVE_NPC_LARGE;
		gBoss[0].hit.front  = 8*VS;
		gBoss[0].hit.top    = 24*VS;
		gBoss[0].hit.back   = 8*VS;
		gBoss[0].hit.bottom = 16*VS;
		gBoss[0].bits = BITS_THROW_BLOCK|BITS_EVENT_BREAK|BITS_VIEWDAMAGE;
		gBoss[0].size = NPCSIZE_LARGE;
		gBoss[0].exp  = 1;

		gBoss[0].code_event = 210;

		gBoss[0].life = 400;


		//ひざ
		gBoss[1].cond = COND_ALIVE;
		gBoss[1].view.front  = 12*VS;
		gBoss[1].view.top    =  8*VS;
		gBoss[1].view.back   = 12*VS;
		gBoss[1].view.bottom =  8*VS;
		gBoss[1].bits = BITS_THROW_BLOCK;

		gBoss[2] = gBoss[1];

		gBoss[1].direct = DIR_LEFT;
		gBoss[2].direct = DIR_RIGHT;


		// 足
		gBoss[3].cond = COND_ALIVE;
		gBoss[3].view.front  = 24*VS;
		gBoss[3].view.top    = 16*VS;
		gBoss[3].view.back   = 16*VS;
		gBoss[3].view.bottom = 16*VS;
		gBoss[3].hit_voice = WAVE_NPC_LARGE;
		gBoss[3].hit.front  = 8*VS;
		gBoss[3].hit.top    = 8*VS;
		gBoss[3].hit.back   = 8*VS;
		gBoss[3].hit.bottom = 8*VS;
		gBoss[3].bits = BITS_THROW_BLOCK;

		gBoss[3].x = gBoss[0].x - 16*VS;
		gBoss[3].y = gBoss[0].y;
		gBoss[3].direct = DIR_LEFT;


		gBoss[4] = gBoss[3];

		gBoss[4].direct = DIR_RIGHT;
		gBoss[3].x = gBoss[0].x + 16*VS;

		// ボスブロック
		gBoss[5].cond = COND_ALIVE;
	
	
	case 10:// 待機
		break;

	case 20:// 開始
		gBoss[0].act_no   = 30;
		gBoss[0].act_wait = 0;
		gBoss[0].ani_no   = 0;
	case 30:// 地震出現
		SetQuake( 2 );
		gBoss[0].y -= VS;
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait%4 == 0 ) PlaySoundObject( WAVE_QUAKE, 1 );
		if( gBoss[0].act_wait == 48 ){
			gBoss[0].act_wait = 0;
			gBoss[0].act_no   = 40;


			if( gBoss[0].life > 280 )break;
			//-jamp-
			gBoss[0].act_no   = 110;
			gBoss[0].bits |= BITS_BANISH_DAMAGE;
			gBoss[0].bits &= ~BITS_THROW_BLOCK;
			gBoss[3].bits &= ~BITS_THROW_BLOCK;
			gBoss[4].bits &= ~BITS_THROW_BLOCK;
			gBoss[3].act_no   = 3;
			gBoss[4].act_no   = 3;

			gBoss[5].hit.top  = 16*VS;

		}
		break;
	case 40:// 待機
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait == 48 ){
			gBoss[0].act_wait = 0;
			gBoss[0].act_no   = 50;
			gBoss[0].ani_wait = 0;

			gBoss[5].hit.top = 16*VS;
			PlaySoundObject( WAVE_BAIT, 1 );
		}
		break;
	case 50:// 開口
		gBoss[0].ani_wait++;
		if( gBoss[0].ani_wait > 2 ){
			gBoss[0].ani_wait = 0;
			gBoss[0].ani_no++;
		}
		if( gBoss[0].ani_no == 3 ){
			gBoss[0].act_no   = 60;
			gBoss[0].act_wait = 0;
			gBoss[0].bits |= BITS_BANISH_DAMAGE;
			gBoss[0].hit.front  = 16*VS;
			gBoss[0].hit.back   = 16*VS;
		}
		break;
	case 60:// 吐き出し
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait > 20 && 
			gBoss[0].act_wait < 80 &&
			(gBoss[0].act_wait%3) == 0){

			if( Random( 0, 9 ) < 8 )
				SetNpChar( 48, gBoss[0].x, gBoss[0].y - 16*VS, Random(-VS/2,VS/2), -16*VS/10, DIR_LEFT,  NULL, MAX_NPC/2 );
			else
				SetNpChar( 48, gBoss[0].x, gBoss[0].y - 16*VS, Random(-VS/2,VS/2), -16*VS/10, DIR_RIGHT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );

		}
		if( gBoss[0].act_wait == 200 || CountArmsBullet( 6 ) ){
			gBoss[0].ani_wait = 0;
			gBoss[0].act_no   = 70;
			PlaySoundObject( WAVE_BAIT, 1 );
		}

		break;
	case 70:// 閉口
		gBoss[0].ani_wait++;
		if( gBoss[0].ani_wait > 2 ){
			gBoss[0].ani_wait = 0;
			gBoss[0].ani_no--;
		}
		if( gBoss[0].ani_no == 1 )gBoss[0].damage  = 20;
		if( gBoss[0].ani_no == 0 ){
			PlaySoundObject( WAVE_BAIT, 0 );
			PlaySoundObject( WAVE_BREAK1, 1 );
			gBoss[0].act_no   = 80;
			gBoss[0].act_wait = 0;
			gBoss[0].bits &= ~BITS_BANISH_DAMAGE;
			gBoss[0].hit.front  = 24*VS;
			gBoss[0].hit.back   = 24*VS;

			gBoss[5].hit.top  = 36*VS;
			gBoss[0].damage  = 0;
		
		}
		break;
	case 80:// 待機
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait == 48 ){
			gBoss[0].act_wait = 0;
			gBoss[0].act_no   = 90;
		}
		break;
	case 90:// 地震沈没
		SetQuake( 2 );
		gBoss[0].y += VS;
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait%4 == 0 ) PlaySoundObject( WAVE_QUAKE, 1 );
		if( gBoss[0].act_wait == 48 ){
			gBoss[0].act_wait = 0;
			gBoss[0].act_no   = 100;
		}
		break;
	case 100:// 待機
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait == 120 ){
			gBoss[0].act_wait = 0;
			gBoss[0].act_no   = 30;
			gBoss[0].x = gBoss[0].tgt_x + Random(-64, 64 )*VS;
			gBoss[0].y = gBoss[0].tgt_y;
		}
		break;
//------------------------------------
	case 110:// 開口→待機
		gBoss[0].ani_wait++;
		if( gBoss[0].ani_wait > 2 ){
			gBoss[0].ani_wait = 0;
			gBoss[0].ani_no++;
		}
		if( gBoss[0].ani_no == 3 ){
			gBoss[0].act_no   = 120;
			gBoss[0].act_wait = 0;
			gBoss[0].hit.front  = 16*VS;
			gBoss[0].hit.back   = 16*VS;

		}
		break;
	case 120:// 開きっぱなし
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait == 50 || CountArmsBullet( 6 ) ){
			gBoss[0].act_no   = 130;
			PlaySoundObject( WAVE_BAIT, 1 );
			gBoss[0].act_wait = 0;
			gBoss[0].ani_wait = 0;

		}
		if( gBoss[0].act_wait < 30 && (gBoss[0].act_wait%5) == 0){
			SetNpChar( 48, gBoss[0].x, gBoss[0].y - 16*VS, Random(-VS*2/3,VS*2/3),
							-16*VS/10, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
		}
		break;
	case 130:// 閉口 →ジャンプ
		gBoss[0].ani_wait++;
		if( gBoss[0].ani_wait > 2 ){
			gBoss[0].ani_wait = 0;
			gBoss[0].ani_no--;
		}
		if( gBoss[0].ani_no == 1 )gBoss[0].damage  = 20;
		if( gBoss[0].ani_no == 0 ){
			gBoss[0].act_no   = 140;
			gBoss[0].bits |= BITS_BANISH_DAMAGE;
			gBoss[0].hit.front  = 16*VS;
			gBoss[0].hit.back   = 16*VS;
			//jump
			gBoss[0].ym = -MAX_MOVE;
			PlaySoundObject( WAVE_BAIT,   0 );
			PlaySoundObject( WAVE_BREAK1, 1 );
			PlaySoundObject( WAVE_BUNRET, 1 );
			if( gBoss[0].x < gMC.x ) gBoss[0].xm =  VS/2;
			if( gBoss[0].x > gMC.x ) gBoss[0].xm = -VS/2;
			gBoss[0].damage   = 0;
			gBoss[5].hit.top  = 36*VS;
		}
		break;
	case 140:// 滞空
		if( gMC.flag & FLAG_HIT_BOTTOM && gBoss[0].ym > 0 ) gBoss[5].damage = 20;
		else                                                gBoss[5].damage =  0;

		gBoss[0].ym += VS/14;
		if( gBoss[0].ym > MAX_MOVE ) gBoss[0].ym = MAX_MOVE;
		gBoss[0].y += gBoss[0].ym;
		gBoss[0].x += gBoss[0].xm;
		if( gBoss[0].flag & FLAG_HIT_BOTTOM ){
			gBoss[0].act_no   = 110;
			gBoss[0].act_wait = 0;
			gBoss[0].ani_wait = 0;
			gBoss[5].hit.top  = 16*VS;
			gBoss[5].damage = 0;
			PlaySoundObject( WAVE_QUAKE, 1 );
			PlaySoundObject( WAVE_BREAK1, 1 );
		
			SetQuake(30);
		}
		break;
	// やられ-------
	case 150:
		SetQuake( 2 );
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait%12 == 0 )
			PlaySoundObject( WAVE_NPC_LARGE, 1 );
			SetDestroyNpChar( gBoss[0].x + Random(-48,48)*VS,
							  gBoss[0].y + Random(-48,24)*VS, 1,  1 );
		if( gBoss[0].act_wait > 50*2 ){
			gBoss[0].act_wait =  0;
			gBoss[0].act_no   = 160;
			SetFlash( gBoss[0].x, gBoss[0].y, FLASHMODE_EXPLOSION );
			PlaySoundObject( WAVE_EXPLOSION, 1 );
		}
		break;
	case 160:
		SetQuake( 40 );
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait > 50 ){
			gBoss[0].cond = 0;
			gBoss[1].cond = 0;
			gBoss[2].cond = 0;
			gBoss[3].cond = 0;
			gBoss[4].cond = 0;
			gBoss[5].cond = 0;
		}
		break;
	}

	RECT rect[] = {
		{  0,  0, 80, 56},
		{ 80,  0,160, 56},
		{160,  0,240, 56},
		{ 80,  0,160, 56},
	};

	gBoss[0].rect = rect[gBoss[0].ani_no];

	gBoss[1].shock = gBoss[0].shock;
	gBoss[2].shock = gBoss[0].shock;
	gBoss[3].shock = gBoss[0].shock;
	gBoss[4].shock = gBoss[0].shock;

	//そのほか。
	ActBoss01_34();
	ActBoss01_12();
	ActBoss01_5();

	// やられ
	if( !gBoss[0].life && gBoss[0].act_no < 150 ){
		gBoss[0].act_no = 150;
		gBoss[0].act_wait = 0;
		gBoss[0].damage = 0;
		gBoss[5].damage = 0;
		
		DeleteNpCharCode( 48, TRUE );
	}

}



#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"


// ブルードラゴン
void ActNpc200( NPCHAR *npc )
{

	unsigned char deg;
	long xm, ym;
	RECT rcLeft[] = {
		{  0,  0, 40, 40},
		{ 40,  0, 80, 40},
		{ 80,  0,120, 40},
		{120,  0,160, 40},
		{160,  0,200, 40},
		{200,  0,240, 40},
	};
	RECT rcRight[] = {
		{  0, 40, 40, 80},
		{ 40, 40, 80, 80},
		{ 80, 40,120, 80},
		{120, 40,160, 80},
		{160, 40,200, 80},
		{200, 40,240, 80},
	};



	if( npc->act_no < 100 && npc->life < 1000-50 ){
		PlaySoundObject( WAVE_DESTROY_LARGE, 1 );
		SetDestroyNpChar( npc->x, npc->y, npc->view.back,  8 );
		SetExpObjects( npc->x, npc->y, npc->exp );
		npc->act_no   = 100;
		npc->bits     &= ~BITS_BANISH_DAMAGE;
		npc->damage   = 0;
	}



	switch( npc->act_no ){
	case 0:
		npc->act_no = 10;
		npc->count1 =  0;
	case 10:// 待機
		npc->ani_wait++;
		if( npc->ani_wait > 30 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;

		if( npc->count1 ) npc->count1--;
		if( !npc->count1 &&
			gMC.x > npc->x - VS*PARTSSIZE*7 &&
			gMC.x < npc->x + VS*PARTSSIZE*7 ){
			npc->act_no = 20;
		}
		break;

	case 20:
		npc->act_no   = 21;
		npc->act_wait =  0;
	case 21:
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 2;
		else                    npc->ani_no = 3;

		if( npc->act_wait > 30 ) npc->act_no = 30;

		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		break;
	case 30:
		npc->act_no   = 31;
		npc->act_wait =  0;
		npc->ani_no   =  4;
		npc->tgt_x = gMC.x;
		npc->tgt_y = gMC.y;
	case 31:
		npc->act_wait++;
		if( npc->act_wait < 40 && npc->act_wait%8==1 ){
			if( npc->direct == DIR_LEFT ) deg = GetArktan( npc->x - VS*14 - npc->tgt_x, npc->y - npc->tgt_y );
			else                          deg = GetArktan( npc->x + VS*14 - npc->tgt_x, npc->y - npc->tgt_y );
			deg += (unsigned char)Random( -6, 6 );

			ym = GetSin( deg ) * 3;
			xm = GetCos( deg ) * 3;
			if( npc->direct == DIR_LEFT ) SetNpChar( 202, npc->x-VS*14, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			else                          SetNpChar( 202, npc->x+VS*14, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			if( !(gMC.cond&COND_HIDE) ) PlaySoundObject( WAVE_FRONTIA, 1 );
		}

		if( npc->act_wait > 60 ){
			npc->act_no   = 10;
			npc->count1 = Random( 100, 200 );
//			npc->ani_no   = 0;
			npc->ani_wait = 0;
		}
		break;
	case 100:
		npc->ani_no = 5;
		break;
	
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// 倒れブルードラゴン
void ActNpc201( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{200,  0,240, 40},
	};
	RECT rcRight[] = {
		{200, 40,240, 80},
	};

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

//ドラゴンバブル
void ActNpc202( NPCHAR *npc )
{
	//地面に着地
	if( npc->flag & FLAG_HIT_X ){
		npc->cond = 0;
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
	}

	npc->y += npc->ym;
	npc->x += npc->xm;

	//アニメーション------------------
	RECT rect_left[] = {
		{184,216,200,240},
		{200,216,216,240},
		{216,216,232,240},
	
	};

	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2)
		npc->ani_no = 0;

	npc->rect = rect_left[npc->ani_no];
	//時間消滅
	npc->count1++;
	if( npc->count1 > 300 ){
		npc->cond = 0;
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
	}
}

//クリッター LV5
void ActNpc203( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 80, 16, 96},
		{ 16, 80, 32, 96},
		{ 32, 80, 48, 96},//ジャンプ
	};
	RECT rcRight[] = {
		{  0, 96, 16, 112},
		{ 16, 96, 32, 112},
		{ 32, 96, 48, 112},//ジャンプ
	};

	switch( npc->act_no ){
	case 0://初期配置
		npc->y += 3*VS;
		npc->act_no = 1;
	case 1://待機
		if( npc->x > gMC.x )npc->direct = DIR_LEFT;
		else                npc->direct = DIR_RIGHT;
		if( npc->act_wait >= 8 &&
			npc->x - PARTSSIZE*7*VS < gMC.x &&
			npc->x + PARTSSIZE*7*VS > gMC.x &&
			npc->y - PARTSSIZE*5*VS < gMC.y &&
			npc->y + PARTSSIZE*5*VS > gMC.y ){

			npc->ani_no = 1;
		}else{
			if( npc->act_wait < 8 )
				npc->act_wait++;
			npc->ani_no = 0;
		}

		if( npc->shock ){//ダメージかがむ
			npc->act_no   = 2;
			npc->ani_no   = 0;
			npc->act_wait = 0;
		}
		//近づきかがむ
		if( npc->act_wait >= 8 &&
			npc->x - PARTSSIZE*3*VS < gMC.x &&
			npc->x + PARTSSIZE*3*VS > gMC.x &&
			npc->y - PARTSSIZE*5*VS < gMC.y &&
			npc->y + PARTSSIZE*3*VS > gMC.y ){

			npc->act_no   = 2;
			npc->ani_no   = 0;
			npc->act_wait = 0;
		}		break;

	case 2://かがむ
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 3;
			npc->ani_no = 2;
			npc->ym     = -MAX_MOVE;
			if( !(gMC.cond&COND_HIDE) )PlaySoundObject( WAVE_NPCJUMP1, 1 );
			if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
			else                          npc->xm =  VS/2;
		}
		break;

	case 3://滞空
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->xm       = 0;
			npc->act_wait = 0;
			npc->ani_no   = 0;
			npc->act_no   = 1;
			if( !(gMC.cond&COND_HIDE) )PlaySoundObject( WAVE_STEP, 1 );
		}
		break;
	}

	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT )
		npc->rect = rcLeft[npc->ani_no];
	else 
		npc->rect = rcRight[npc->ani_no];

}

// 落石（小）
void ActNpc204( NPCHAR *npc )
{
	RECT rc[] = {
		{240, 80,256, 96},
		{240,144,256,160},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->tgt_x  = npc->x;
	case 1:
		if( gMC.x > npc->x - 12*VS &&
			gMC.x < npc->x + 12*VS &&
			gMC.y > npc->y ){
			npc->act_no = 2;
		}
		break;
	case 2://がたがた
		npc->act_wait++;
		if( (npc->act_wait/6)%2 ) npc->x = npc->tgt_x - VS;
		else                      npc->x = npc->tgt_x;
		if( npc->act_wait > 30 ){
			npc->act_no = 3;
			npc->ani_no = 1;
		}
		break;
	case 3:
		npc->ym += VS/16;
		if( npc->flag & FLAG_HIT_X ){
			if( !(gMC.cond&COND_HIDE) )PlaySoundObject( WAVE_BREAK1, 1 );
			SetDestroyNpChar( npc->x, npc->y, npc->view.back,  4 );
			npc->cond = 0;
			return;
		}
		break;
	}

	if( npc->ym > VS*6 ) npc->ym = VS*6;

	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];
}

// 落石（大）
void ActNpc205( NPCHAR *npc )
{
	RECT rc[] = {
		{112, 80,128,112},
		{128, 80,144,112},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->tgt_x  = npc->x;
		npc->y += 4*VS;
	case 1:
		if( gMC.x > npc->x - 12*VS &&
			gMC.x < npc->x + 12*VS &&
			gMC.y > npc->y ){
			npc->act_no = 2;
		}
		break;
	case 2://がたがた
		npc->act_wait++;
		if( (npc->act_wait/6)%2 ) npc->x = npc->tgt_x - VS;
		else                      npc->x = npc->tgt_x;
		if( npc->act_wait > 30 ){
			npc->act_no   = 3;
			npc->ani_no   = 1;
			npc->act_wait = 0;
		}
		break;
	case 3:
		npc->ym += VS/16;
		// damage
		if( gMC.y > npc->y ){
			npc->bits &= ~BITS_BLOCK_MYCHAR2;
			npc->damage = 127;
		}else{
			npc->bits |=  BITS_BLOCK_MYCHAR2;
			npc->damage = 0;
		}
		npc->act_wait++;
		if( npc->act_wait > 8 && npc->flag & FLAG_HIT_X ){
			npc->bits |= BITS_BLOCK_MYCHAR2;
			npc->act_no   = 4;
			npc->act_wait = 0;
			npc->ym       = 0;
			npc->damage   = 0;
			PlaySoundObject( WAVE_BREAK1, 1 );
			SetDestroyNpChar( npc->x, npc->y, npc->view.back, 4 );
			SetBullet( BULLET_DROP, npc->x, npc->y, DIR_LEFT );
			return;
		}
		break;
	case 4:
		npc->act_wait++;
		if( npc->act_wait > 4 ){
			npc->act_no = 5;
			npc->bits |= BITS_BANISH_DAMAGE;
		}
	case 5:
		break;
	}

	if( npc->ym > VS*6 ) npc->ym = VS*6;

	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];
}

// ボムカウンター
void ActNpc206( NPCHAR *npc )
{
//	long i;

	switch( npc->act_no ){
	case 0://初期化
		npc->act_no   = 1;
		npc->tgt_x    = npc->x;
		npc->tgt_y    = npc->y;
		npc->count1   = 120;
		npc->act_wait = Random( 0, 50 );

	case 1://ランダム待機
		npc->act_wait++;
		if( npc->act_wait < 50 )break;
		npc->act_wait = 0;
		npc->act_no   = 2;
		npc->ym       = VS*3/2;
		break;
	case 2://滞空
		if( gMC.x > npc->x - VS*PARTSSIZE*5 &&
			gMC.x < npc->x + VS*PARTSSIZE*5 ){
			npc->act_wait = 0;
			npc->act_no   = 3;
		}
		if( npc->shock ){
			npc->act_wait = 0;
			npc->act_no   = 3;
		}
		break;
	case 3:
		switch( npc->act_wait ){
		case 60*0:SetNpChar( 207, npc->x + VS*16, npc->y + 4*VS, 0, 0, 0, NULL, MAX_NPC/2 );break;
		case 60*1:SetNpChar( 207, npc->x + VS*16, npc->y + 4*VS, 0, 0, 1, NULL, MAX_NPC/2 );break;
		case 60*2:SetNpChar( 207, npc->x + VS*16, npc->y + 4*VS, 0, 0, 2, NULL, MAX_NPC/2 );break;
		case 60*3:SetNpChar( 207, npc->x + VS*16, npc->y + 4*VS, 0, 0, 3, NULL, MAX_NPC/2 );break;
		case 60*4:SetNpChar( 207, npc->x + VS*16, npc->y + 4*VS, 0, 0, 4, NULL, MAX_NPC/2 );break;
		case 60*5:
			npc->hit.back   = VS*128;
			npc->hit.front  = VS*128;
			npc->hit.top    = VS*100;
			npc->hit.bottom = VS*100;
			npc->damage = 30;
			PlaySoundObject( WAVE_EXPLOSION, 1 );
			SetDestroyNpChar( npc->x, npc->y, 128*VS, 100 );
			SetQuake( 20 );
			npc->cond |= COND_LOSE;
			break;
		}
		npc->act_wait++;
		break;
	}

	if( npc->act_no > 1 ){
		if( npc->tgt_y < npc->y ) npc->ym -= VS/32;
		if( npc->tgt_y > npc->y ) npc->ym += VS/32;

		if( npc->ym >  VS/2 )npc->ym =  VS/2;
		if( npc->ym < -VS/2 )npc->ym = -VS/2;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rect_left[] = {
		{ 80,80,120,120},
		{120,80,160,120},
		{160,80,200,120},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 4 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 ) npc->ani_no = 0;
	npc->rect = rect_left[npc->ani_no];
}

// カウンター
void ActNpc207( NPCHAR *npc )
{
	RECT rc[] = {
		{  0,144, 16,160},
		{ 16,144, 32,160},
		{ 32,144, 48,160},
		{ 48,144, 64,160},
		{ 64,144, 80,160},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->ani_no = npc->direct;
		PlaySoundObject( WAVE_ON, 1 );
	case 1:
		npc->x += VS*1;
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_wait = 0;
			npc->act_no   = 2;
		}
		break;
	case 2:
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->cond = 0;
			return;
		}
		break;
	}
	npc->rect = rc[npc->ani_no];
}

// パゴット=バスLV2
void ActNpc208( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{248, 80,272,104},
		{272, 80,296,104},
		{296, 80,320,104},
	};

	RECT rcRight[] = {
		{248,104,272,128},
		{272,104,296,128},
		{296,104,320,128},
	};


	switch( npc->act_no ){
	case 0:
		if(	gMC.x < npc->x + 1*PARTSSIZE*VS &&
			gMC.x > npc->x - 1*PARTSSIZE*VS ){

			npc->bits    |= BITS_BANISH_DAMAGE;
			npc->ym       = -VS;
			npc->tgt_x    = npc->x;//最初の座標を記憶
			npc->tgt_y    = npc->y;
			npc->act_no   = 1;
			npc->act_wait = 0;
			npc->count1   = npc->direct; // 方向を保存
			npc->count2   = 0;
			npc->damage   = 6;
			if( npc->direct == DIR_LEFT ){
				npc->x  = gMC.x + 16*PARTSSIZE*VS;
				npc->xm = -MAX_MOVE/2; 
			}else{
				npc->x  = gMC.x - 16*PARTSSIZE*VS;
				npc->xm = MAX_MOVE/2; 
			}
			return;
		}else{
			npc->rect.right = 0;
			npc->damage     = 0;
			npc->xm         = 0;
			npc->ym         = 0;
			npc->bits      &= ~BITS_BANISH_DAMAGE;
			return;
		}
		break;
	case 1:
		//向き
		if( npc->x > gMC.x ){
			npc->direct = DIR_LEFT;
			npc->xm -= 16;
		}else{
			npc->direct =   DIR_RIGHT;
			npc->xm += 16;
		}


		if( npc->flag & FLAG_HIT_LEFT  ) npc->xm =  VS;
		if( npc->flag & FLAG_HIT_RIGHT ) npc->xm = -VS;


		if( npc->y < npc->tgt_y ) npc->ym += 8;
		else                      npc->ym -= 8;

		// max move
		if( npc->xm >  MAX_MOVE/2 ) npc->xm =  MAX_MOVE/2;
		if( npc->xm < -MAX_MOVE/2 ) npc->xm = -MAX_MOVE/2;
		if( npc->ym >  VS       ) npc->ym =  VS;
		if( npc->ym < -VS       ) npc->ym = -VS;

		// damage move
		if( npc->shock ){
			npc->x += npc->xm/2;
			npc->y += npc->ym/2;
		}else{
			npc->x += npc->xm;
			npc->y += npc->ym;
		}

		if(	gMC.x > npc->x + 25*PARTSSIZE*VS ||
			gMC.x < npc->x - 25*PARTSSIZE*VS ){
			npc->act_no     = 0;
			npc->xm         = 0;
			npc->direct     = npc->count1;
			npc->x          = npc->tgt_x;//元の座標へ
			npc->rect.right = 0;//表示もしない
			npc->damage     = 0;//ダメージ無し
			return;
		}



		
		break;
	}

	if( npc->act_no ){

		unsigned char deg;
		long xm;
		long ym;



		if( npc->act_wait <  150 )npc->act_wait++;
		if(	npc->act_wait == 150 ){
			npc->count2++;
			if( npc->count2%8 == 0 &&
				npc->x < gMC.x + 10*PARTSSIZE*VS &&
				npc->x > gMC.x - 10*PARTSSIZE*VS ){

			
				deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
				deg += (unsigned char)Random( -6, 6 );

				ym = GetSin( deg ) * 3;
				xm = GetCos( deg ) * 3;
				SetNpChar( 209, npc->x, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
				PlaySoundObject( WAVE_POP, 1 );
			}
			if( npc->count2 > 16 ){
				npc->act_wait = 0;
				npc->count2   = 0;
			}
		}
	}

	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1 )npc->ani_no = 0;
	// 点滅
	if( npc->act_wait > 120 &&
		npc->act_wait/2%2 == 1 &&
		npc->ani_no == 1 ){
		npc->ani_no = 2;
	}

	if( npc->direct == DIR_LEFT)npc->rect = rcLeft[ npc->ani_no];
	else                        npc->rect = rcRight[npc->ani_no]; 


}

// 超音波
void ActNpc209( NPCHAR *npc )
{
	//地面に着地
	if( npc->flag & FLAG_HIT_X ){
		npc->cond = 0;
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
	}

	npc->y += npc->ym;
	npc->x += npc->xm;

	//アニメーション------------------
	RECT rect_left[] = {
		{232, 96,248,112},
		{200,112,216,128},
		{216,112,232,128},
		{232,112,248,128},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 2 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 3)npc->ani_no = 0;
	npc->rect = rect_left[npc->ani_no];

	//時間消滅
	npc->count1++;
	if( npc->count1 > 300 ){
		npc->cond = 0;
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
	}
}

// パゴットLV2
void ActNpc210( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,112, 16,128},
		{ 16,112, 32,128},
	};

	RECT rcRight[] = {
		{ 32,112, 48,128},
		{ 48,112, 64,128},
	};


	switch( npc->act_no ){
	case 0:
		if(	gMC.x < npc->x + 1*PARTSSIZE*VS &&
			gMC.x > npc->x - 1*PARTSSIZE*VS ){

			npc->bits   |= BITS_BANISH_DAMAGE;
			npc->ym     = -VS;
			npc->tgt_y  = npc->y;
			npc->act_no = 1;
			npc->damage = 2;
			if( npc->direct == DIR_LEFT ){
				npc->x  = gMC.x + 16*PARTSSIZE*VS;
				npc->xm = -MAX_MOVE/2; 
			}else{
				npc->x  = gMC.x - 16*PARTSSIZE*VS;
				npc->xm = MAX_MOVE/2; 
			}
		}else{
			npc->bits       &= ~BITS_BANISH_DAMAGE;
			npc->rect.right = 0;
			npc->damage     = 0;
			npc->xm         = 0;
			npc->ym         = 0;
			return;
		}
		break;
	case 1://左向き
		if( npc->x > gMC.x ){
			npc->direct = DIR_LEFT;
			npc->xm -= 16;
		}else{
			npc->direct =   DIR_RIGHT;
			npc->xm += 16;
		}
		if( npc->xm >  MAX_MOVE/2 ) npc->xm =  MAX_MOVE/2;
		if( npc->xm < -MAX_MOVE/2 ) npc->xm = -MAX_MOVE/2;

		if( npc->y < npc->tgt_y ) npc->ym += 8;
		else                      npc->ym -= 8;
		if( npc->ym >  VS ) npc->ym =  VS;
		if( npc->ym < -VS ) npc->ym = -VS;

		if( npc->shock ){
			npc->x += npc->xm/2;
			npc->y += npc->ym/2;
		}else{
			npc->x += npc->xm;
			npc->y += npc->ym;
		}
		break;
	}



	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1 )npc->ani_no = 0;

	if( npc->direct == DIR_LEFT)npc->rect = rcLeft[npc->ani_no];
	else                        npc->rect = rcRight[npc->ani_no]; 


}

// デストラップミニ
void ActNpc211( NPCHAR *npc )
{
	RECT rect[] = {
		{256,200,272,216},
		{272,200,288,216},
		{288,200,304,216},
		{304,200,320,216},
	};


	npc->rect = rect[npc->code_event];

}


// 飛竜
void ActNpc212( NPCHAR *npc )
{
	RECT rcRight[] = {
		{160,152,200,192},
		{200,152,240,192},

		{240,112,280,152},
		{280,112,320,152},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->y -= 4*VS;
	case 1:
		npc->ani_wait++;
		if( npc->ani_wait > 30 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		break;

	case 10:// カズマ、マイキャラ乗せ 浮上
		npc->act_no   = 11;
		npc->ani_no   = 2;
		npc->ani_wait = 0;
		npc->tgt_y    = npc->y - VS*16;
		npc->tgt_x    = npc->x - VS* 6;
		npc->ym       = 0;
		npc->bits     |= BITS_THROW_BLOCK;
	case 11://
		if( npc->x < npc->tgt_x ) npc->xm += VS/64;
		else                      npc->xm -= VS/64;
		if( npc->y < npc->tgt_y ) npc->ym += VS/64;
		else                      npc->ym -= VS/64;
		npc->x += npc->xm;
		npc->y += npc->ym;
		npc->ani_wait++;
		if( npc->ani_wait > 5 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 2;
		break;
	case 20:// カズマ、マイキャラ乗せ 飛行
		npc->act_no   = 21;
		npc->bits     |= BITS_THROW_BLOCK;
	case 21://
		if( npc->y < npc->tgt_y ) npc->ym += VS/32;
		else                      npc->ym -= VS/32;
		npc->xm += VS/16;
		if( npc->xm > VS*3 ) npc->xm = VS*3;
		if( npc->xm < -VS*3 ) npc->xm = -VS*3;
		npc->x += npc->xm;
		npc->y += npc->ym;
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 2;
		break;

	case 30:
		npc->act_no = 31;
		SetNpChar( 297, 0, 0, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
		break;

	}

	npc->rect = rcRight[npc->ani_no]; 
	if( (gMC.equip & EQUIP_MIMIGAH) && npc->ani_no > 1 ){
		npc->rect.top    += 40;
		npc->rect.bottom += 40;
	}
}


// ムーンゴースト
void ActNpc213( NPCHAR *npc )
{
	RECT rect[] = {
		{  0,  0,  0,  0},//

		{  0,  0, 48, 48},//1
		{ 48,  0, 96, 48},
		{ 96,  0,144, 48},

		{144,  0,192, 48},//4 溜め
		{192,  0,240, 48},
		{240,  0,288, 48},

		{  0, 48, 48, 96},//7 発射
		{ 48, 48, 96, 96},
		{ 96, 48,144, 96},
	};

	switch( npc->act_no ){
	case 0:
		npc->ani_no = 0;
		npc->tgt_x  = npc->x;
		npc->tgt_y  = npc->y;
	case 1://待機
		if( gMC.y > npc->y - 8*VS &&
			gMC.y < npc->y + 8*VS ){
			if( npc->direct == DIR_LEFT ) npc->y -= VS*PARTSSIZE*15;
			else                          npc->y += VS*PARTSSIZE*15;
			npc->act_no   = 10;
			npc->act_wait =  0;
			npc->ani_no   =  1;
			npc->ym       =  0;
			npc->bits    |= BITS_BANISH_DAMAGE;
		}
		break;

	case 10: // 自キャラに向かう


		//ani
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 1;

		npc->act_wait++;
		if( npc->act_wait > 200 ){
			npc->act_no   = 20;
			npc->act_wait =  0;
			npc->ani_no   =  4;
		}
		break;

	case 20: // 溜め
		//ani
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 6 )
			npc->ani_no = 4;

		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no   = 30;
			npc->act_wait =  0;
			npc->ani_no   =  7;
		}
		break;

	case 30: // 発射
		//ani
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 9 )
			npc->ani_no = 7;

		npc->act_wait++;

		if( (npc->act_wait % 5) == 1 ){
			SetNpChar( 214, npc->x, npc->y, Random( 4/2, 4*3 ) * VS/4,
						Random( -VS, VS ), DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_SPLASH, 1 );
		}

		if( npc->act_wait > 50 ){
			npc->act_no   = 10;
			npc->act_wait =  0;
			npc->ani_no   =  1;
		}
		break;

	case 40: // 帰る
		if( npc->y < npc->tgt_y ) npc->ym +=  VS/8;
		else                 npc->ym -=  VS/8;
		if( npc->ym < -VS*2 ) npc->ym = -VS*2;
		if( npc->ym >  VS*2 ) npc->ym =  VS*2;
		if( npc->shock ) npc->y += npc->ym/2;
		else             npc->y += npc->ym;

		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 6 )
			npc->ani_no = 4;
	
		
		if( gMC.y < npc->tgt_y + 15*VS*PARTSSIZE &&
			gMC.y > npc->tgt_y - 15*VS*PARTSSIZE ){
			npc->act_no   = 20;
			npc->act_wait =  0;
			npc->ani_no   =  4;
		}
		break;
	}


	if( npc->act_no >= 10 && npc->act_no <= 30 ){

		if( npc->y < gMC.y ) npc->ym +=  VS/20;
		else                 npc->ym -=  VS/20;
		if( npc->ym < -VS*2 ) npc->ym = -VS*2;
		if( npc->ym >  VS*2 ) npc->ym =  VS*2;

		if( npc->flag & FLAG_HIT_TOP    ) npc->ym =  VS;
		if( npc->flag & FLAG_HIT_BOTTOM ) npc->ym = -VS;

		if( npc->shock ) npc->y += npc->ym/2;
		else             npc->y += npc->ym;

		if( gMC.y > npc->tgt_y + 15*VS*PARTSSIZE ||
			gMC.y < npc->tgt_y - 15*VS*PARTSSIZE ){
			npc->act_no = 40;
		}

	}

	npc->rect = rect[npc->ani_no];
}

// ムーンボール
void ActNpc214( NPCHAR *npc )
{
	RECT rect[] = {
		{144, 48,176, 64},
		{176, 48,208, 64},
		{208, 48,240, 64},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->bits |= BITS_THROW_BLOCK;
	case 1:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 0;

		npc->xm -= VS/20;
		npc->x  += npc->xm;
		npc->y  += npc->ym;

		if( npc->xm < 0 ) npc->bits &= ~BITS_THROW_BLOCK;

		if( npc->flag & FLAG_HIT_X ){
			SetDestroyNpChar( npc->x, npc->y, npc->view.back,  4 );
			PlaySoundObject( WAVE_SMOKE, 1 );
			npc->cond = 0;

		}

		break;
	}

	npc->rect = rect[npc->ani_no];
}



// わにLV2
void ActNpc215( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0://マイキャラ待ち
		npc->ani_no = 0;
		npc->act_no = 1;
		npc->act_wait = 0;
		npc->tgt_y = npc->y;
		npc->bits &= ~BITS_BANISH_DAMAGE;//弾は効かない
		npc->bits &= ~BITS_BLOCK_BULLET;//弾は効かない
		npc->bits &= ~BITS_BLOCK_MYCHAR;//マイキャラすり抜け
		npc->bits &= ~BITS_THROW_BLOCK;
	case 1://待機
		if( gMC.x > npc->x - 12*VS  && gMC.x < npc->x + 12 *VS &&
			gMC.y > npc->y - 0*VS && gMC.y < npc->y+8*VS ){
			npc->act_no   = 15;
			npc->act_wait = 0;
		}
		break;

	case 15:
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			PlaySoundObject( WAVE_BAIT, 1 );
			npc->act_no = 20;
		}
		break;
	case 20://食らいつき
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_no++;
			npc->ani_wait = 0;
		}
		if(npc->ani_no == 3 ) npc->damage = 15;
		if(npc->ani_no == 4 ){
			npc->bits |= BITS_BANISH_DAMAGE;//弾が効くようになる
//			npc->bits |= BITS_BLOCK_BULLET;//弾が効くようになる
			npc->act_no = 30;
			npc->act_wait = 0;
		}
		break;
	case 30://・・・
		npc->bits |= BITS_BLOCK_MYCHAR;//マイキャラブロック
		npc->damage = 0;
		npc->act_wait++;
		if( npc->shock ){//|| npc->act_wait > 100){
			npc->act_no = 40;
			npc->act_wait = 0;
		}
		break;
	case 40://退散
		npc->bits |= BITS_THROW_BLOCK;
		npc->y += 1*VS;
		npc->act_wait++;
		if( npc->act_wait == 32 ){
			npc->bits &= ~BITS_BLOCK_MYCHAR;//マイキャラブロック
			npc->bits &= ~BITS_BANISH_DAMAGE;
			npc->act_no = 50;
			npc->act_wait = 0;
		}
		break;
	case 50://地中待機

		if( npc->act_wait < 100 ){
			npc->act_wait++;
		}else{
			npc->y = npc->tgt_y;
			npc->ani_no = 0;
			npc->act_no = 0;
		}
		break;
	}

	//RECT
	RECT rect[] = {
		{  0,  0,  0,  0},
		{  0, 96, 48,128},
		{ 48, 96, 96,128},
		{ 96, 96,144,128},
		{144, 96,192,128},
	};
	npc->rect = rect[npc->ani_no];

}

// Pixel
void ActNpc216( NPCHAR *npc )
{
	RECT rect[] = {
		{256,192,272,216},
	};

	npc->rect = rect[0];
}

// 伊藤
void ActNpc217( NPCHAR *npc )
{
	RECT rect[] = {
		{144,64,160,80},
		{160,64,176,80},
		{176,64,192,80},// ぎゃ
		{192,64,208,80},// ぶるぶる

		{144,80,160,96},// 4 右向き
		{160,80,176,96},
		{144,80,160,96},
		{176,80,192,96},
	};

	
	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->xm       = 0;
	case 1://待機
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		break;

	case 2://瞬く
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;

	case 10:// びっくり！
		npc->ani_no = 2;
		npc->xm = 0;
		break;

	case 20:// 吹っ飛び
		npc->act_no = 21;
		npc->ani_no =  2;
		npc->xm += VS;
		npc->ym -= VS*2;
		break;
	case 21:// 吹っ飛び

		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ani_no   = 3;
			npc->act_no   = 30;
			npc->act_wait = 0;
			npc->xm       = 0;
			npc->tgt_x    = npc->x;
		}

		break;

	case 30:// ぶるぶる
		npc->ani_no = 3;
		npc->act_wait++;
		if( (npc->act_wait/2)%2 ) npc->x = npc->tgt_x + VS;
		else                      npc->x = npc->tgt_x;

		break;

	case 40:// ヘリから飛び出し
		npc->act_no =  41;
		npc->ym     = -VS;
		npc->ani_no =   2;
	case 41:
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 42;
			npc->ani_no =  4;
		}
		break;
	case 42:// 右向き停止
		npc->xm = 0;
		npc->ani_no =  4;
		break;

	case 50:// 右へ歩く(32dotでとまる)
		npc->act_no   = 51;
		npc->act_wait =  0;
	case 51:
		npc->act_wait++;
		if( npc->act_wait > 32 ){
			npc->act_no = 42;
		}
		npc->xm = VS;
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 7 )
			npc->ani_no = 4;
		break;
	}
	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE )
		npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;


	npc->rect = rect[npc->ani_no];
}


// アーモンド波
void ActNpc218( NPCHAR *npc )
{
	RECT rc[] = {
		{256,120,288,152},
		{288,120,320,152},
	};

	npc->x += npc->xm;
	npc->y += npc->ym;
	npc->act_wait++;

	if( npc->act_wait > 200 ){
		npc->cond = 0;
	}

	npc->ani_wait++;
	if( npc->ani_wait > 2 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1 )
		npc->ani_no = 0;

	npc->rect = rc[npc->ani_no];
}

// 煙装置・流れ表現装置（アイロンヘッドステージ）
void ActNpc219( NPCHAR *npc )
{
	RECT rc = {0,0,0,0};

	if( npc->direct == DIR_LEFT ){
		if( Random( 0, 40 ) == 1 ){
			SetNpChar( 4, Random( -20, 20 )*VS+npc->x, npc->y, 0, -VS, DIR_LEFT, NULL, MAX_NPC/2 );
		}
	}else{
		SetNpChar( 199, npc->x + Random( -160, 160 )*VS, npc->y + Random( -128, 128 )*VS, 0, 0, DIR_RIGHT, NULL, MAX_NPC/2 ); 
	}

	npc->rect = rc;
}

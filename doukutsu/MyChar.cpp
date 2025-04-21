#include <windows.h>
#include "Draw.h"
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "Map.h"
#include "KeyControl.h"

#define WAIT_MYCHAR_RUN		4

MYCHAR gMC;
extern RECT grcGame;
extern long g_GameFlags;
// 操作キー
/*extern long gKeyJump;
extern long gKeyShot;
extern long gKeyArms;
extern long gKeyArmsRev;
extern long gKeyItem;
extern long gKeyMap;
extern long gKeyLeft;
extern long gKeyUp;
extern long gKeyRight;
extern long gKeyDown;
*/
void SetNpChar( long code_char, long x, long y, long xm, long ym, long dir, NPCHAR *npc, long start_index );
void SetCaret( long x, long y, long code, long dir );
void SetValueView( long *px, long *py, long value );
long GetItemArms( void );
BOOL StartTextScript( long no );
long Random( int min, int max );
extern long gKey;
extern long gKeyTrg;
extern ARMSDATA gArmsData[];
extern long gSelectedArms;
BOOL GetNPCFlag( long a );
void DamageMyChar( long a );

extern NPCHAR gNPC[];


//extern long gMyShopList[];
//マイキャラの初期化
void InitMyChar( void )
{
	memset( &gMC, 0, sizeof(MYCHAR) );

	gMC.cond   = COND_ALIVE;
	gMC.direct = DIR_RIGHT;
	//view
	gMC.view.back   = 8*VS;
	gMC.view.top    = 8*VS;
	gMC.view.front  = 8*VS;
	gMC.view.bottom = 8*VS;
	//hit
	gMC.hit.back   = 5*VS;
	gMC.hit.top    = 8*VS;
	gMC.hit.front  = 5*VS;
	gMC.hit.bottom = 8*VS;
	
	gMC.life     = 3;
	gMC.max_life = 3;
	gMC.unit = MYUNIT_NORMAL;

}



//表示情報の生成
void AnimationMyChar( BOOL bKey )
{
	RECT rcLeft[] = {
		{  0,  0, 16, 16},//停止
		{ 16,  0, 32, 16},//歩く
		{  0,  0, 16, 16},
		{ 32,  0, 48, 16},
		{  0,  0, 16, 16},
		{ 48,  0, 64, 16},//5上向き停止
		{ 64,  0, 80, 16},//6上向き歩く
		{ 48,  0, 64, 16},
		{ 80,  0, 96, 16},
		{ 48,  0, 64, 16},
		{ 96,  0,112, 16},//下向き
		{112,  0,128, 16},//調べる
		
	};
	RECT rcRight[] = {
		{  0, 16, 16, 32},//停止

		{ 16, 16, 32, 32},//歩く
		{  0, 16, 16, 32},
		{ 32, 16, 48, 32},
		{  0, 16, 16, 32},

		{ 48, 16, 64, 32},//上向き停止

		{ 64, 16, 80, 32},//上向き歩く
		{ 48, 16, 64, 32},
		{ 80, 16, 96, 32},
		{ 48, 16, 64, 32},

		{ 96, 16,112, 32},//下向き
		{112, 16,128, 32},//調べる
		
	};

	if( gMC.cond & COND_HIDE ) return;


	//地面
	if( gMC.flag & FLAG_HIT_BOTTOM ){
		
		// チェック中
		if( gMC.cond & COND_CHECK ){ gMC.ani_no = 11;
		// 上向き歩き	
		}else if( gKey&gKeyUp && gKey&(gKeyLeft|gKeyRight) && bKey ){

			gMC.cond |= COND_RUN;
			gMC.ani_wait++;
			if( gMC.ani_wait > WAIT_MYCHAR_RUN ){
				gMC.ani_wait = 0;
				gMC.ani_no++;
				if( gMC.ani_no == 7 ||
					gMC.ani_no == 9 ) PlaySoundObject( WAVE_RUN, 1 );
			}
			if( gMC.ani_no > 9 ||
				gMC.ani_no < 6 ) gMC.ani_no = 6;
		// 歩き
		}else if( gKey&(gKeyLeft|gKeyRight) && bKey ){

			gMC.cond |= COND_RUN;
			gMC.ani_wait++;
			if( gMC.ani_wait > WAIT_MYCHAR_RUN ){
				gMC.ani_wait = 0;
				gMC.ani_no++;
				if( gMC.ani_no == 2 ||
					gMC.ani_no == 4 ) PlaySoundObject( WAVE_RUN, 1 );
			}
			if( gMC.ani_no > 4 ||
				gMC.ani_no < 1 ) gMC.ani_no = 1;

		}else if( gKey & gKeyUp && bKey ){

			if( gMC.cond & COND_RUN ) PlaySoundObject( WAVE_RUN, 1 );
			gMC.cond  &= ~COND_RUN;
			gMC.ani_no = 5;

		}else{//停止

			if( gMC.cond & COND_RUN ) PlaySoundObject( WAVE_RUN, 1 );
			gMC.cond  &= ~COND_RUN;
			gMC.ani_no = 0;

		}
	}else{
		//空中
		if(      gMC.up   ) gMC.ani_no =  6;
		else if( gMC.down ) gMC.ani_no = 10;
		else{
			if( gMC.ym > 0 )gMC.ani_no =  1;
			else            gMC.ani_no =  3;
		}
	}

	if( gMC.direct == DIR_LEFT ) gMC.rect = rcLeft[ gMC.ani_no];
	else                         gMC.rect = rcRight[gMC.ani_no];
}

void ShowMyChar( BOOL bShow )
{
	if( bShow ) gMC.cond &= ~COND_HIDE;
	else        gMC.cond |=  COND_HIDE;//hide!!
}

void PutMyChar( long fx, long fy )
{
	long arms_offset_y;
	if( !(gMC.cond & COND_ALIVE) || gMC.cond & COND_HIDE )return;

	//武器のRECTを作る---------------------------
	gMC.rect_arms.left   = ( gArmsData[gSelectedArms].code%13 )* 24;
	gMC.rect_arms.right  = gMC.rect_arms.left + 24;
	gMC.rect_arms.top    = ( gArmsData[gSelectedArms].code/13 )* 96;
	gMC.rect_arms.bottom = gMC.rect_arms.top + 16;

	//向き
	if( gMC.direct == DIR_RIGHT ){
		gMC.rect_arms.top    += 16;
		gMC.rect_arms.bottom += 16;
	}
	if( gMC.up ){
		arms_offset_y = -4;
		gMC.rect_arms.top    += 32;
		gMC.rect_arms.bottom += 32;
	}else if( gMC.down ){
		arms_offset_y = +4;
		gMC.rect_arms.top    += 64;
		gMC.rect_arms.bottom += 64;
	}else{
		arms_offset_y = 0;
	}

	//縦ゆれ
	if( gMC.ani_no == 1 ||
		gMC.ani_no == 3 ||
		gMC.ani_no == 6 ||
		gMC.ani_no == 8 ) gMC.rect_arms.top += 1;

	//武器
	if( gMC.direct == DIR_LEFT ){
		PutBitmap3( &grcGame, ( gMC.x - gMC.view.front )/VS - fx/VS - 8,
							  ( gMC.y - gMC.view.top   )/VS - fy/VS + arms_offset_y,
								&gMC.rect_arms, SURF_ARMS );
	}else{
		PutBitmap3( &grcGame, ( gMC.x - gMC.view.front )/VS - fx/VS,
							  ( gMC.y - gMC.view.top   )/VS - fy/VS + arms_offset_y,
								&gMC.rect_arms, SURF_ARMS );
	}

	//ダメージ点滅-非表示(HIDE)------------------------------
	RECT rect;
	if( gMC.shock/2%2 ) return;

	rect = gMC.rect;
	if( gMC.equip & EQUIP_MIMIGAH ){
		rect.top    += 32;
		rect.bottom += 32;
	}
	PutBitmap3( &grcGame, ( gMC.x - gMC.view.front)/VS - fx/VS,
							( gMC.y - gMC.view.top)/VS - fy/VS,
							&rect, SURF_MYCHAR );

	//酸素装置
	{
		static unsigned char bubble;
		RECT rcBubble[2] = {
			{ 56, 96, 80,120},
			{ 80, 96,104,120},
		};
		gMC.bubble++;
		if( gMC.equip & EQUIP_AIR && gMC.flag & FLAG_HIT_WATER ){
			PutBitmap3( &grcGame, ( gMC.x )/VS -12 - fx/VS, ( gMC.y )/VS -12 - fy/VS, &rcBubble[gMC.bubble/2%2], SURF_CARET );
		}else if( gMC.unit == MYUNIT_STREAM ){
			PutBitmap3( &grcGame, ( gMC.x )/VS -12 - fx/VS, ( gMC.y )/VS -12 - fy/VS, &rcBubble[gMC.bubble/2%2], SURF_CARET );
		}
	}
}




//マイキャラの移動
#define MAX_DASH_MYCHAR		(512+300)
#define GRAVITY_MYCHAR1		80//普段の重力
#define GRAVITY_MYCHAR2		32//ジャンプボタンが押されているときの重力
#define JUMP_MYCHAR			(VS+VS+VS/2)
#define DASH_MYCHAR1		VS/ 6//VS/8
#define DASH_MYCHAR2		VS/16//VS/24//空中
#define RESIST_MYCHAR		VS/10//VS/12
#define BOOST_MYCHAR		VS/16
#define VECT_LEFT	(VS/4 + VS/64) //流され
#define VECT_UP		VS/4
#define VECT_RIGHT	(VS/4 + VS/64)
#define VECT_DOWN	VS/6



//ノーマル
void ActMyChar_Normal( BOOL bKey )
{
	long max_dash;
	long max_move;
	long gravity1;
	long gravity2;
	long jump;
	long dash1;
	long dash2;
	long resist;

	long a, x;

	if( gMC.cond & COND_HIDE ) return;

	//水中では
	if( gMC.flag & FLAG_HIT_WATER ){
		max_dash = MAX_DASH_MYCHAR / 2;
		max_move = MAX_MOVE        / 2;
		gravity1 = GRAVITY_MYCHAR1 / 2;
		gravity2 = GRAVITY_MYCHAR2 / 2;
		jump     = JUMP_MYCHAR     / 2;
		dash1    = DASH_MYCHAR1    / 2;
		dash2    = DASH_MYCHAR2    / 2;
		resist   = RESIST_MYCHAR   / 2;
	}else{
		max_dash = MAX_DASH_MYCHAR;
		max_move = MAX_MOVE;
		gravity1 = GRAVITY_MYCHAR1;
		gravity2 = GRAVITY_MYCHAR2;
		jump     = JUMP_MYCHAR;
		dash1    = DASH_MYCHAR1;
		dash2    = DASH_MYCHAR2;
		resist   = RESIST_MYCHAR;
	}

	gMC.ques = 0;
	if( !bKey ) gMC.boost_sw = 0;
	
	//足が地に着いている時
	if( gMC.flag & FLAG_HIT_BOTTOM || gMC.flag & FLAG_HIT_LDOWN || gMC.flag & FLAG_HIT_RDOWN ){
		gMC.boost_sw =  0;
		if(      gMC.equip & EQUIP_BOOST  ) gMC.boost_cnt = 50;
		else if( gMC.equip & EQUIP_BOOST2 ) gMC.boost_cnt = 50;
		else                                gMC.boost_cnt =  0;
		if( bKey ){
			//調べる(左右の操作は無効)
			if( gKeyTrg == gKeyDown &&
				gKey    == gKeyDown &&
				!( gMC.cond & COND_CHECK ) &&
				!(g_GameFlags & GAMEFLAG_TEXTSCRIPT) ){

				gMC.cond |= COND_CHECK;
				gMC.ques  = 1;

			}else if( gKey == gKeyDown ){
			}else{
				//左右移動
				if( gKey & gKeyLeft  && gMC.xm > -max_dash) gMC.xm -= dash1;
				if( gKey & gKeyRight && gMC.xm <  max_dash) gMC.xm += dash1;
				if( gKey & gKeyLeft  ) gMC.direct = DIR_LEFT;
				if( gKey & gKeyRight ) gMC.direct = DIR_RIGHT;
			}
		}
		//地面抵抗
		if( !( gMC.cond & COND_FLOW ) ){
			if( gMC.xm < 0 ){
				if( gMC.xm > -resist ) gMC.xm = 0;
				else                   gMC.xm += resist;
			}
			if( gMC.xm > 0 ){
				if( gMC.xm <  resist ) gMC.xm = 0;
				else                   gMC.xm -= resist;
			}
		}

	}else{//空中
		if( bKey ){

			if( gMC.equip & (EQUIP_BOOST|EQUIP_BOOST2) && gKeyTrg & gKeyJump && gMC.boost_cnt ){

				// ブースター１
				if( gMC.equip & EQUIP_BOOST ){
					gMC.boost_sw = 1;
					if( gMC.ym > VS/2 ) gMC.ym = gMC.ym/2;
				}
				// ブースター２
				if( gMC.equip & EQUIP_BOOST2  ){
					if(       gKey & gKeyUp    ){ // 上へ
						gMC.boost_sw =  2;
						gMC.xm       =  0;
                        gMC.ym       = -MAX_MOVE;
					}else if( gKey & gKeyLeft  ){ // ←へ
						gMC.boost_sw =  1;
						gMC.ym       =  0;
						gMC.xm       = -MAX_MOVE;
					}else if( gKey & gKeyRight ){ // →へ
						gMC.boost_sw =  1;
						gMC.ym       =  0;
						gMC.xm       =  MAX_MOVE;
                    }else if( gKey & gKeyDown  ){ // 下へ
						gMC.boost_sw = 3;
						gMC.xm =       0;
                        gMC.ym =       MAX_MOVE;
					}else{                       // 上へ
						gMC.boost_sw =  2;
						gMC.xm       =  0;
                        gMC.ym       = -MAX_MOVE;
					}
				}
			}

			//左右移動（自力で出力できる量は、最大移動量以下）
			if( gKey & gKeyLeft  && gMC.xm > -max_dash ) gMC.xm -= dash2;
			if( gKey & gKeyRight && gMC.xm <  max_dash ) gMC.xm += dash2;

			if( gKey & gKeyLeft  ) gMC.direct = DIR_LEFT;
			if( gKey & gKeyRight ) gMC.direct = DIR_RIGHT;
		}
		// ブースト２はブレーキ
		if( gMC.equip & EQUIP_BOOST2 && gMC.boost_sw ){
			if( !(gKey & gKeyJump) || !gMC.boost_cnt ){
				 if(      gMC.boost_sw == 1 ) gMC.xm = gMC.xm/2;
				 else if( gMC.boost_sw == 2 ) gMC.ym = gMC.ym/2;
			 }
		}

		if( !gMC.boost_cnt || !(gKey & gKeyJump) ){
			gMC.boost_sw = 0;
		}
	}


	if( bKey ){
		// ↑キー
		if( gKey & gKeyUp ) gMC.up = 1;
		else                gMC.up = 0;

		// ↓向き( 空中のみ)
		if( gKey & gKeyDown && !(gMC.flag & FLAG_HIT_BOTTOM) ) gMC.down = 1;
		else                                                   gMC.down = 0;
			

		//Jump
		if( gKeyTrg & gKeyJump ){
			if( gMC.flag & FLAG_HIT_BOTTOM ||
				gMC.flag & FLAG_HIT_LDOWN  ||
				gMC.flag & FLAG_HIT_RDOWN  ){

				if( gMC.flag & FLAG_HIT_VECT_UP ){
				}else{
					gMC.ym = -jump;
					PlaySoundObject( WAVE_JUMP, 1 );
				}
			}
		}

	}
	
	//チェック解除
	if( bKey && gKey & (gKeyLeft|gKeyRight|gKeyUp|gKeyJump|gKeyShot) ) gMC.cond &= ~COND_CHECK;

	if( gMC.boost_sw && gMC.boost_cnt  ) gMC.boost_cnt--;

	if( gMC.flag & FLAG_HIT_VECT_LEFT  ) gMC.xm -= VECT_LEFT;
	if( gMC.flag & FLAG_HIT_VECT_UP    ) gMC.ym -= VECT_UP;
	if( gMC.flag & FLAG_HIT_VECT_RIGHT ) gMC.xm += VECT_RIGHT;
	if( gMC.flag & FLAG_HIT_VECT_DOWN  ) gMC.ym += VECT_DOWN;

	//Gravity ===================================================
	// ブースター２
	if(      gMC.equip & EQUIP_BOOST2 && gMC.boost_sw ){
		if( gMC.boost_sw == 1 ){
			if( gMC.flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT) ) gMC.ym = -VS/2;
			if( gMC.direct == DIR_LEFT  ) gMC.xm -= VS/16;
			if( gMC.direct == DIR_RIGHT ) gMC.xm += VS/16;
			if( gKeyTrg & gKeyJump || gMC.boost_cnt%3 == 1 ){

				if( gMC.direct == DIR_LEFT  ) SetCaret( gMC.x + 2*VS, gMC.y + 2*VS, CARET_MISSILE, DIR_RIGHT );
				if( gMC.direct == DIR_RIGHT ) SetCaret( gMC.x - 2*VS, gMC.y + 2*VS, CARET_MISSILE, DIR_LEFT  );
				PlaySoundObject( WAVE_BOOST, 1 );
			}
		}else if( gMC.boost_sw == 2 ){
			gMC.ym -= VS/16;
			if( gKeyTrg & gKeyJump || gMC.boost_cnt%3 == 1 ){
				SetCaret( gMC.x, gMC.y + 6*VS, CARET_MISSILE, DIR_DOWN  );
				PlaySoundObject( WAVE_BOOST, 1 );
			}
		}else if( gMC.boost_sw == 3 ){
			if( gKeyTrg & gKeyJump || gMC.boost_cnt%3 == 1 ){
				SetCaret( gMC.x, gMC.y - 6*VS, CARET_MISSILE, DIR_UP    );
				PlaySoundObject( WAVE_BOOST, 1 );
			}
		}
	}
	// ノーマル
	else if( gMC.flag & ( FLAG_HIT_VECT_UP ) ) gMC.ym += gravity1;
	// ブースター１
	else if( gMC.equip & EQUIP_BOOST && gMC.boost_sw && gMC.ym > -2*VS ){

		gMC.ym -= BOOST_MYCHAR;
		if( gMC.boost_cnt%3 == 0 ){
			SetCaret( gMC.x, gMC.y + gMC.hit.bottom/2, CARET_MISSILE, DIR_DOWN );
			PlaySoundObject( WAVE_BOOST, 1 );
		}
		if( gMC.flag & FLAG_HIT_TOP )gMC.ym = VS;

	}
	else if( gMC.ym < 0 && bKey && gKey & gKeyJump ) gMC.ym += gravity2;
	else                                             gMC.ym += gravity1;


	//下り坂での浮くのを防ぐ
	if( !bKey || !(gKeyTrg & gKeyJump) ){
		if( gMC.flag & FLAG_HIT_LDOWN  &&                                gMC.xm < 0) gMC.ym = -gMC.xm; 
		if( gMC.flag & FLAG_HIT_RDOWN  &&                                gMC.xm > 0) gMC.ym =  gMC.xm;
		if( gMC.flag & FLAG_HIT_BOTTOM && gMC.flag & (FLAG_SET_TRI_H) && gMC.xm < 0) gMC.ym =  VS*2; 
		if( gMC.flag & FLAG_HIT_BOTTOM && gMC.flag & (FLAG_SET_TRI_E) && gMC.xm > 0) gMC.ym =  VS*2; 

		// 小ピラミッド
		if( gMC.flag & FLAG_HIT_BOTTOM && gMC.flag & FLAG_SET_TRI_F && gMC.flag & FLAG_SET_TRI_G )
			gMC.ym = VS*2;
	}

	//max
	if(   gMC.flag & FLAG_HIT_WATER ){
	}
	if(   gMC.flag & FLAG_HIT_WATER &&
		!(gMC.flag & (FLAG_HIT_VECT_LEFT|FLAG_HIT_VECT_RIGHT|FLAG_HIT_VECT_UP|FLAG_HIT_VECT_DOWN) ) ){
		if( gMC.xm < -MAX_MOVE/2 ) gMC.xm = -MAX_MOVE/2;
		if( gMC.xm >  MAX_MOVE/2 ) gMC.xm =  MAX_MOVE/2;
		if( gMC.ym < -MAX_MOVE/2 ) gMC.ym = -MAX_MOVE/2;
		if( gMC.ym >  MAX_MOVE/2 )
			gMC.ym =  MAX_MOVE/2;
	}else{
		if( gMC.xm < -MAX_MOVE   ) gMC.xm = -MAX_MOVE;
		if( gMC.xm >  MAX_MOVE   ) gMC.xm =  MAX_MOVE;
		if( gMC.ym < -MAX_MOVE   ) gMC.ym = -MAX_MOVE;
		if( gMC.ym >  MAX_MOVE   ) gMC.ym =  MAX_MOVE;
	}

	//Splash
	if( !gMC.sprash && gMC.flag & FLAG_HIT_WATER ){
		long dir;
		if( gMC.flag & FLAG_RED_WATER ) dir = DIR_RIGHT;
		else                            dir = DIR_LEFT;

		if( !(gMC.flag&FLAG_HIT_BOTTOM) && gMC.ym > VS ){
			for( a = 0; a < 8; a++ ){
				x = Random( -8, 8 )*VS + gMC.x;
				SetNpChar( 73, x, gMC.y,
					Random( -VS, VS ) + gMC.xm,
					Random( -VS, VS/4)*1 - gMC.ym/2, dir, NULL, 0 );
			}
			PlaySoundObject( WAVE_SPLASH2, 1 );
		}else if( gMC.xm > VS || gMC.xm < -VS ){ 
			for( a = 0; a < 8; a++ ){
				x = Random( -8, 8 )*VS + gMC.x;
				SetNpChar( 73, x, gMC.y,
					Random( -VS, VS ) + gMC.xm,
					Random( -VS, VS/4), dir, NULL, 0 );
			}
			PlaySoundObject( WAVE_SPLASH2, 1 );
		}
		gMC.sprash = 1;
	}
	if( !( gMC.flag & ( FLAG_HIT_WATER ) ) ){
		gMC.sprash = 0;
	}

	//DamageBlock
	if( gMC.flag & FLAG_HIT_DAMAGE ) DamageMyChar( 10 );


	//フレームターゲット X
	if( gMC.direct == DIR_LEFT ){
		gMC.index_x -= VS;
		if( gMC.index_x < -64*VS ) gMC.index_x = -64*VS;
	}else{
		gMC.index_x += VS;
		if( gMC.index_x >  64*VS ) gMC.index_x =  64*VS;
	}

	//フレームターゲット Y
	if(       gKey & gKeyUp   && bKey ){
		gMC.index_y -= VS;
		if( gMC.index_y < -64*VS ) gMC.index_y = -64*VS;
	}else if( gKey & gKeyDown && bKey ){
		gMC.index_y += VS;
		if( gMC.index_y >  64*VS ) gMC.index_y =  64*VS;
	}else{
		if( gMC.index_y >  VS ) gMC.index_y -= VS;
		if( gMC.index_y < -VS ) gMC.index_y += VS;
	}

	gMC.tgt_x = gMC.x + gMC.index_x;
	gMC.tgt_y = gMC.y + gMC.index_y;

	//移動量を反映------------------
	//あまり防止
	if( gMC.xm <= resist && gMC.xm >= -resist );
	else gMC.x += gMC.xm;
	gMC.y += gMC.ym;

}


//ストリーム
void ActMyChar_Stream( BOOL bKey )
{

	gMC.up   = 0;
	gMC.down = 0;

	if( bKey ){
		// 向き変更
//		if( gKeyTrg & gKeyJump ){
//			if( gMC.direct == DIR_LEFT ) gMC.direct = DIR_RIGHT;
//			else                         gMC.direct = DIR_LEFT;
//		}
		// 左右移動
		if( gKey & (gKeyLeft|gKeyRight) ){
			if( gKey & gKeyLeft  ) gMC.xm -= VS/2;
			if( gKey & gKeyRight ) gMC.xm += VS/2;
		}else{
			if( gMC.xm < VS/4 && gMC.xm > -VS/4 ) gMC.xm =     0;
			else if( gMC.xm > 0                 ) gMC.xm -= VS/4;
			else if( gMC.xm < 0                 ) gMC.xm += VS/4;
		}
		// 上下移動
		if( gKey & (gKeyUp|gKeyDown) ){
			if( gKey & gKeyUp    ) gMC.ym -= VS/2;
			if( gKey & gKeyDown  ) gMC.ym += VS/2;
		}else{
			if( gMC.ym < VS/4 && gMC.ym > -VS/4 ) gMC.ym =     0;
			else if( gMC.ym > 0                 ) gMC.ym -= VS/4;
			else if( gMC.ym < 0                 ) gMC.ym += VS/4;
		}
	}else{
			if( gMC.xm < VS/4 && gMC.xm > -VS/8 ) gMC.xm =     0;
			else if( gMC.xm > 0                 ) gMC.xm -= VS/4;
			else if( gMC.xm < 0                 ) gMC.xm += VS/4;
			if( gMC.ym < VS/4 && gMC.ym > -VS/8 ) gMC.ym =     0;
			else if( gMC.ym > 0                 ) gMC.ym -= VS/4;
			else if( gMC.ym < 0                 ) gMC.ym += VS/4;
	}

	if( gMC.ym < -VS && gMC.flag & FLAG_HIT_TOP    ){
		SetCaret( gMC.x, gMC.y - gMC.hit.top,    CARET_SMALLSTAR, 5 );
	}
	if( gMC.ym > VS && gMC.flag & FLAG_HIT_BOTTOM ){
		SetCaret( gMC.x, gMC.y + gMC.hit.bottom, CARET_SMALLSTAR, 5 );
	}

	if( gMC.xm >  VS*2 ) gMC.xm =  VS*2;
	if( gMC.xm < -VS*2 ) gMC.xm = -VS*2;
	if( gMC.ym >  VS*2 ) gMC.ym =  VS*2;
	if( gMC.ym < -VS*2 ) gMC.ym = -VS*2;

	if( (gKey & (gKeyLeft|gKeyUp))    == (gKeyLeft|gKeyUp)    ){
		if( gMC.xm < -780 ) gMC.xm = -780;
		if( gMC.ym < -780 ) gMC.ym = -780;
	}
	if( (gKey & (gKeyRight|gKeyUp))   == (gKeyRight|gKeyUp)   ){
		if( gMC.xm >  780 ) gMC.xm =  780;
		if( gMC.ym < -780 ) gMC.ym = -780;
	}
	if( (gKey & (gKeyLeft|gKeyDown))  == (gKeyLeft|gKeyDown)  ){
		if( gMC.xm < -780 ) gMC.xm = -780;
		if( gMC.ym >  780 ) gMC.ym =  780;
	}
	if( (gKey & (gKeyRight|gKeyDown)) == (gKeyRight|gKeyDown) ){
		if( gMC.xm >  780 ) gMC.xm =  780;
		if( gMC.ym >  780 ) gMC.ym =  780;
	}

	gMC.x += gMC.xm;
	gMC.y += gMC.ym;

}

static void AirProcess( void )
{
	if( gMC.equip & EQUIP_AIR ){
		gMC.air     = MAX_AIR;
		gMC.air_get = 0;
		return;
	}
	if( !( gMC.flag & ( FLAG_HIT_WATER ) ) ){
		gMC.air = MAX_AIR;
	}else{
		gMC.air--;
		if( gMC.air <= 0 ){
			if( GetNPCFlag( 4000 ) ){
				StartTextScript( 1100 );
			}else{
				StartTextScript(   41 );
				if( gMC.direct == DIR_LEFT ) SetCaret( gMC.x, gMC.y, CARET_WATERDEAD, DIR_LEFT  );
				else                         SetCaret( gMC.x, gMC.y, CARET_WATERDEAD, DIR_RIGHT );
				gMC.cond &= ~COND_ALIVE;
			}
		}
	}
	if( gMC.flag & (FLAG_HIT_WATER) ){
		gMC.air_get = 60;
	}else{
		if( gMC.air_get ) gMC.air_get--;
	}
}


//アクション
void ActMyChar( BOOL bKey )
{
	if( !(gMC.cond & COND_ALIVE) )return;
	if( gMC.exp_wait ) gMC.exp_wait--;
	if( gMC.shock    ) gMC.shock--;
	else if( gMC.exp_count ){
		SetValueView( &gMC.x, &gMC.y, gMC.exp_count );
		gMC.exp_count = 0;
	}

	switch( gMC.unit ){
	case MYUNIT_NORMAL:
		if( !(g_GameFlags & GAMEFLAG_TEXTSCRIPT) && bKey ) AirProcess( );
		ActMyChar_Normal( bKey );
		break;
	case MYUNIT_STREAM:
		ActMyChar_Stream( bKey );
		break;
	}

	gMC.cond &= ~COND_FLOW;
}

//マイキャラ情報
void GetMyCharPosition( long *x, long *y )
{
	*x = gMC.x;
	*y = gMC.y;
}

void InitStar();
void SetMyCharPosition( long x, long y )
{
	gMC.x = x;
	gMC.y = y;
	gMC.tgt_x = gMC.x;
	gMC.tgt_y = gMC.y;
	gMC.index_x = 0;
	gMC.index_y = 0;
	gMC.xm = 0;
	gMC.ym = 0;
	gMC.cond &= ~COND_CHECK;//こっち向け
	InitStar();
}

void MoveMyChar( long x, long y )
{
	gMC.x = x;
	gMC.y = y;
}

void ZeroMyCharXMove( void )
{
	gMC.xm = 0;
}


long GetUnitMyChar( void )
{
	return gMC.unit;
}


void SetMyCharDirect( unsigned char dir )
{	
	long i;

	if( dir == DIR_DOWN )gMC.cond |= COND_CHECK;
	else{
		gMC.cond &= ~COND_CHECK;

		if( dir < 10 ){
			gMC.direct = dir;
		}else{

			for( i = 0; i < MAX_NPC; i++ ){
				if( gNPC[i].code_event == dir ) break;
			}
			if( i == MAX_NPC ) return;
			if( gMC.x > gNPC[i].x ) gMC.direct = DIR_LEFT;
			else                    gMC.direct = DIR_RIGHT;
		}
	}
	gMC.xm = 0;
	AnimationMyChar( FALSE );
}

void ChangeMyUnit( unsigned char a )
{
	gMC.unit = a;
}

void PitMyChar( void )
{
	gMC.y += PARTSSIZE*VS*2;
}


void EquipItem( long flag, BOOL b )
{
	if( b ) gMC.equip |=  flag;
	else    gMC.equip &= ~flag;
}

//イベント終了時のチェック動作リセット
void ResetCheck( void )
{
	gMC.cond &= ~COND_CHECK;
}

// シャワー音ON
static long _noise_no    = 0;
static long _noise_freq  = 0;

void SetNoise( long no, long freq )
{
	_noise_freq  = freq;
	_noise_no    = no;
	switch( _noise_no ){
	case NOISE_RIVER:
		ChangeSoundFrequency( WAVE_RIVER1, _noise_freq );
		ChangeSoundFrequency( WAVE_RIVER2, _noise_freq + 100 );
		PlaySoundObject( WAVE_RIVER1, -1 );
		PlaySoundObject( WAVE_RIVER2, -1 );
		break;
	case NOISE_PROPERA:
		PlaySoundObject( WAVE_PROPERA, -1 );
		break;
	}

}

// シャワー音OFF
void CutNoise( void )
{
	_noise_no = 0;
	PlaySoundObject( WAVE_RIVER1,  0 );
	PlaySoundObject( WAVE_RIVER2,  0 );
	PlaySoundObject( WAVE_PROPERA, 0 );
}

void ResetNoise( void )
{
	switch( _noise_no ){
	case NOISE_RIVER:
		ChangeSoundFrequency( WAVE_RIVER1, _noise_freq );
		ChangeSoundFrequency( WAVE_RIVER2, _noise_freq + 100 );
		PlaySoundObject( WAVE_RIVER1, -1 );
		PlaySoundObject( WAVE_RIVER2, -1 );
		break;
	case NOISE_PROPERA:
		PlaySoundObject( WAVE_PROPERA, -1 );
		break;
	}
}

void SleepNoise( void )
{
	PlaySoundObject( WAVE_RIVER1,  0 );
	PlaySoundObject( WAVE_RIVER2,  0 );
	PlaySoundObject( WAVE_PROPERA, 0 );
}

#include <windows.h>
#include "Draw.h"
#include "Sound.h"

#include "Define.h"
#include "Position.h"
#include "Tags.h"

#define WAIT_MYCHAR_DAMAGE	128

extern MYCHAR gMC;

void SetCaret( long x, long y, long code, long dir );
void PutNumber4( long x, long y, long value, BOOL bZero );
void SetDestroyNpChar( long x, long y, long w, long num );
void SetValueView( long *px, long *py, long value );
BOOL StartTextScript( long no );
BOOL SubItemData( long code );

extern ARMSDATA gArmsData[];
extern long gSelectedArms;

extern RECT grcGame;
extern long g_GameFlags;

typedef struct{
	long exp[ 3];
}ARMSLEVELTABLE;

ARMSLEVELTABLE gArmsLevelTable[] ={
	{   0,   0,100},
	{  30,  40, 16},//   スネーク
	{  10,  20, 10},//   ポーラスター
	{  10,  20, 20},//   ファイアボール
	{  30,  40, 10},//   マシンガン
	{  10,  20, 10},//   ミサイル
	{  10,  20, 30},//   ボム
	{  10,  20,  5},// 7 バブリン
	{  10,  20,100},// 8
	{  30,  60,  0},// 9 ブレード
	{  30,  60, 10},//10 スーパーミサイル
	{  10,  20,100},//11
	{   1,   1,  1},//12 ネメシス
	{  40,  60,200},//13 シュプール
};


//経験値追加
void AddExpMyChar( long x )
{

	long lv;
	long arms_code;


	lv        = gArmsData[gSelectedArms].level-1;
	arms_code = gArmsData[gSelectedArms].code;

	gArmsData[gSelectedArms].exp += x;

	if( lv == 2 ){
		if( gArmsData[gSelectedArms].exp >= gArmsLevelTable[arms_code].exp[lv] ){
			gArmsData[gSelectedArms].exp  = gArmsLevelTable[arms_code].exp[lv];
			if( gMC.equip & EQUIP_STAR && gMC.star < 3 ) gMC.star++;
		}

	}else{

		for(  ; lv < 2; lv++ ){
			// レベルアップ
			if( gArmsData[gSelectedArms].exp >= gArmsLevelTable[arms_code].exp[lv] ){

				gArmsData[gSelectedArms].level++;
				gArmsData[gSelectedArms].exp = 0;

				if( gArmsData[gSelectedArms].code != 13 ){
					PlaySoundObject( WAVE_LEVELUP, 1 );
					SetCaret( gMC.x, gMC.y, CARET_LEVELUP, DIR_LEFT );
				}
			}
		}
	}

	if( gArmsData[gSelectedArms].code != 13 ){
		gMC.exp_count += x;
		gMC.exp_wait = 30;
	}else{
		gMC.exp_wait = 10;
	}
}

void ZeroExpMyChar( void )
{
	gArmsData[gSelectedArms].level = 1;
	gArmsData[gSelectedArms].exp   = 0;
}

BOOL IsMaxExpMyChar( void )
{
	long arms_code;
	if( gArmsData[gSelectedArms].level == 3 ){
		arms_code = gArmsData[gSelectedArms].code;
		if( gArmsData[gSelectedArms].exp >= gArmsLevelTable[arms_code].exp[2] ){
			return TRUE;
		}
	}
	return FALSE;
}


void DamageMyChar( long damage )
{
#ifdef _DEBUG
//	return;
#endif

	if( !(g_GameFlags|GAMEFLAG_KEYCONTROL) ) return;
	if( gMC.shock ) return;

	PlaySoundObject( WAVE_MY_DAMAGE, 1 );
	gMC.cond &= ~COND_CHECK;
	gMC.shock = WAIT_MYCHAR_DAMAGE;
	if( gMC.unit == MYUNIT_STREAM ){

	}else{
		gMC.ym    = -VS*2;
	}
	gMC.life  -= (short)damage;
	if( gMC.equip & EQUIP_STAR && gMC.star > 0 ){
		gMC.star  -= 1;
//		if( !gMC.star ) SubItemData( 38 );
	}

	// 経験値 -----------------------------
	if( gMC.equip & EQUIP_ARMSBARRIER ) gArmsData[gSelectedArms].exp -= damage*1;
	else                                gArmsData[gSelectedArms].exp -= damage*2;

	// 経験値がマイナス
	while( gArmsData[gSelectedArms].exp < 0 ){
		if( gArmsData[gSelectedArms].level > 1 ){
			long lv;
			long arms_code;
			gArmsData[gSelectedArms].level--;
			lv        = gArmsData[gSelectedArms].level-1;
			arms_code = gArmsData[gSelectedArms].code;
			gArmsData[gSelectedArms].exp = gArmsLevelTable[arms_code].exp[lv] +
											gArmsData[gSelectedArms].exp;
			if( gMC.life > 0 && gArmsData[gSelectedArms].code != 13 ){
				SetCaret( gMC.x, gMC.y, CARET_LEVELUP, DIR_RIGHT );
			}
		}else{
			gArmsData[gSelectedArms].exp = 0;
		}
	}

	SetValueView( &gMC.x, &gMC.y, -damage );
	if( gMC.life <= 0 ){
		PlaySoundObject( WAVE_MY_DESTROY, 1 );
		gMC.cond = 0;
		SetDestroyNpChar( gMC.x, gMC.y, VS*10, 64 );
		StartTextScript( 40 );
	}
}

// 全ての武器のレベルがゼロになる
void ZeroArmsEnergy_All( void )
{
	long a;
//	BOOL 
	for( a = 0; a < MAX_ARMSDATA; a++ ){
		gArmsData[a].level = 1;
		gArmsData[a].exp   = 0;
	}

//		SetCaret( gMC.x, gMC.y, CARET_LEVELUP, DIR_RIGHT );
//		PlaySoundObject( WAVE_MY_DAMAGE, 1 );

}

void AddBulletMyChar( long no, long val )
{

	long a;

	a = 0;
	while( a < MAX_ARMSDATA && gArmsData[a].code != 5 ) a++;
	
	if( a == MAX_ARMSDATA ){
		a = 0;
		while( a < MAX_ARMSDATA && gArmsData[a].code != 10 ) a++;

		if( a == MAX_ARMSDATA ) return;
	}

	gArmsData[a].num += val;
	if( gArmsData[a].num > gArmsData[a].max_num )
		gArmsData[a].num = gArmsData[a].max_num;

}

//体力回復
void AddLifeMyChar( long x )
{
	gMC.life += (short)x;
	if( gMC.life > gMC.max_life )
		gMC.life = gMC.max_life;
	gMC.lifeBr = gMC.life;
}

void AddMaxLifeMyChar( long val )
{
	gMC.max_life += (short)val;
	if( gMC.max_life > 232 )
		gMC.max_life = 232;
	gMC.life   += (short)val;
	gMC.lifeBr = gMC.life;
}

//アクション画面に残弾表示
extern long gArmsEnergyX;
void PutArmsEnergy( BOOL flash )
{
	static unsigned char add_flash = 0;
	RECT rcPer    = { 72, 48, 80, 56};
	RECT rcLv     = { 80, 80, 96, 88};
	RECT rcView   = { 0, 0, SURFACE_WIDTH, SURFACE_HEIGHT };
	RECT rcNone   = { 80, 48, 96, 56};


	if( gArmsEnergyX > ARMSENERGY_X ) gArmsEnergyX -= 2;
	if( gArmsEnergyX < ARMSENERGY_X ) gArmsEnergyX += 2;

	// 弾数を表示
	if( gArmsData[gSelectedArms].max_num ){
		PutNumber4( gArmsEnergyX + 32, ARMSENERGY_Y + 0, gArmsData[gSelectedArms].num, FALSE );
		PutNumber4( gArmsEnergyX + 32, ARMSENERGY_Y + 8, gArmsData[gSelectedArms].max_num, FALSE );
	}else{
		PutBitmap3( &rcView, gArmsEnergyX + 48, ARMSENERGY_Y + 0, &rcNone, SURF_TEXTBOX );
		PutBitmap3( &rcView, gArmsEnergyX + 48, ARMSENERGY_Y + 8, &rcNone, SURF_TEXTBOX );
	}

	
	if( flash == TRUE && gMC.shock/2%2 )return;

	
	// パーセンテージを表示
	PutBitmap3( &rcView, gArmsEnergyX + 32, ARMSENERGY_Y + 8, &rcPer,  SURF_TEXTBOX );

	// レベルを表示
	PutBitmap3( &rcView, gArmsEnergyX,      ARMSENERGY_Y + 16, &rcLv, SURF_TEXTBOX );
	PutNumber4(          gArmsEnergyX-8,    ARMSENERGY_Y + 16, gArmsData[gSelectedArms].level, FALSE );


	// 経験値を表示
	RECT rcExpBox   = {  0, 72, 40, 80};
	RECT rcExpVal   = {  0, 80,  0, 88};
	RECT rcExpMax   = { 40, 72, 80, 80};
	RECT rcExpFlash = { 40, 80, 80, 88};

	long lv;
	long arms_code;
	long exp_now;
	long exp_next;

	lv        = gArmsData[gSelectedArms].level-1;
	arms_code = gArmsData[gSelectedArms].code;
	exp_now   = gArmsData[gSelectedArms].exp;
	exp_next  = gArmsLevelTable[arms_code].exp[lv];

	//箱
	PutBitmap3( &rcView, gArmsEnergyX + 24, ARMSENERGY_Y + 16, &rcExpBox, SURF_TEXTBOX );

	if( lv == 2 && gArmsData[gSelectedArms].exp == gArmsLevelTable[arms_code].exp[lv] ){
		//最大
		PutBitmap3( &rcView, gArmsEnergyX + 24, ARMSENERGY_Y + 16, &rcExpMax, SURF_TEXTBOX );
	}else{
		//値
		if( exp_next )rcExpVal.right += (exp_now*40/exp_next);
		else          rcExpVal.right = 0;
		PutBitmap3( &rcView, gArmsEnergyX + 24, ARMSENERGY_Y + 16, &rcExpVal, SURF_TEXTBOX );
	}

	//フラッシュ
	if( gMC.exp_wait && (add_flash++)/2%2 ){
//		rcExpFlash.right += ( exp_now*40 / exp_next );
		PutBitmap3( &rcView, gArmsEnergyX + 24, ARMSENERGY_Y + 16, &rcExpFlash, SURF_TEXTBOX );
	}
}


// 常に全武器を表示したい

#define ACTIVEARMSLIST_X	16
#define ACTIVEARMSLIST_Y	16


void PutActiveArmsList( void )
{
	long x;
	long a;
	RECT rect = {0,0,0,16};
	
	long arms_num;
	//武器の数を取得
	arms_num = 0;
	while( gArmsData[arms_num].code ) arms_num++;
	if( !arms_num ) return;

	for( a = 0; a < arms_num; a++ ){

		x = gArmsEnergyX + ( a - gSelectedArms ) * 16;
		if(      x <  ARMSENERGY_X-8 ) x += 48 + arms_num * 16;
		else if( x >= ARMSENERGY_X+8 ) x += 48;
		if(      x >= ARMSENERGY_X +        48 + (arms_num-1) * 16 + 8 )
			x -= arms_num * 16 +            48;
		if(      x <  ARMSENERGY_X + 56 && x >= ARMSENERGY_X+8 )
			x -= 48;


		rect.left  = gArmsData[ a ].code * 16;
		rect.right = rect.left + 16;
		
		PutBitmap3( &grcGame, x, ACTIVEARMSLIST_Y, &rect, SURF_ARMSIMAGE );
	}
}


void PutMyLife( BOOL flash )
{
	RECT rcCase = {  0, 40,232, 48};
	RECT rcLife = {  0, 24,232, 32};
	RECT rcBr   = {  0, 32,232, 40};

	//ダメージ点滅
	if( flash == TRUE && gMC.shock/2%2 )return;

	if( gMC.lifeBr < gMC.life ) gMC.lifeBr = gMC.life;
	if( gMC.lifeBr > gMC.life ){
		gMC.lifeBr_count++;
		if( gMC.lifeBr_count > 30 ) gMC.lifeBr--;
	}else
		gMC.lifeBr_count = 0;

	// 表示
	rcCase.right = 24+40;//gMC.max_life*2 + 24;
	rcLife.right = (gMC.life  *40/gMC.max_life)-1; //gMC.life    *2;
	rcBr.right   = (gMC.lifeBr*40/gMC.max_life)-1;//gMC.lifeBr  *2;
	PutBitmap3( &grcGame, MYLIFE_X,    MYLIFE_Y, &rcCase, SURF_TEXTBOX );
	PutBitmap3( &grcGame, MYLIFE_X+24, MYLIFE_Y, &rcBr  , SURF_TEXTBOX );
	PutBitmap3( &grcGame, MYLIFE_X+24, MYLIFE_Y, &rcLife, SURF_TEXTBOX );
	PutNumber4(           MYLIFE_X-8,  MYLIFE_Y, gMC.lifeBr, FALSE );
//	PutBitmap3( &grcGame, gMC.max_life*2+MYLIFE_X+24, MYLIFE_Y, &rcCap, SURF_TEXTBOX );

}

//空気の表示
void PutMyAir( long x, long y )
{
	RECT rcAir[2] = {
		{112, 72,144, 80},
		{112, 80,144, 88},
	};
	if( gMC.equip & EQUIP_AIR ) return;
	if( gMC.air_get ){
		if( gMC.air_get%6 < 4 )
			PutNumber4( x+32, y, gMC.air/10, FALSE );
		if( gMC.air%30 > 10 ) PutBitmap3( &grcGame, x, y, &rcAir[0], SURF_TEXTBOX );
		else                  PutBitmap3( &grcGame, x, y, &rcAir[1], SURF_TEXTBOX );
	}
}



// タイムカウンター =======================================
#include "stdio.h"
static long _time_count = 0;
extern char gModulePath[];
long Random( int min, int max );

void PutTimeCounter( long x, long y )
{
	RECT rcTime[] = {
		{112,104,120,112},
		{120,104,128,112},
		{128,104,160,112},
	};
	if( gMC.equip & EQUIP_COUNTER ){
		if( g_GameFlags & GAMEFLAG_KEYCONTROL ){
			if( _time_count < 100*60*50 )_time_count++;
			if( _time_count%30 > 10 ) PutBitmap3( &grcGame, x, y, &rcTime[0], SURF_TEXTBOX );
			else                      PutBitmap3( &grcGame, x, y, &rcTime[1], SURF_TEXTBOX );
		}else{
			PutBitmap3( &grcGame, x, y, &rcTime[0], SURF_TEXTBOX );
		}
		PutNumber4( x+ 0, y,  _time_count/(50*60), FALSE  );
		PutNumber4( x+20, y, (_time_count/50)%60, TRUE   );
		PutNumber4( x+32, y, (_time_count/5)%10, FALSE );
		PutBitmap3( &grcGame, x+30, y, &rcTime[2], SURF_TEXTBOX );
	}else{
		_time_count = 0;
	}
}

typedef struct{
	long          counter[4];
	unsigned char random[ 4];
}COUNTERRECORDE;

BOOL SaveTimeCounter( void )
{
	long           i;
	unsigned char  *p;
	COUNTERRECORDE rec;
	FILE *fp;
	char path[MAX_PATH];

	if( !(gMC.equip & EQUIP_COUNTER) ) return TRUE;

	sprintf( path, "%s\\290.rec", gModulePath );

	// 比較
	fp = fopen( path, "rb" );
	if( fp ){
		fread( &rec, sizeof(COUNTERRECORDE), 1, fp );
		fclose( fp );

		p = (unsigned char *)&rec.counter[ 0 ];
		p[0] -= rec.random[ 0 ];
		p[1] -= rec.random[ 0 ];
		p[2] -= rec.random[ 0 ];
		p[3] -= rec.random[ 0 ]/2;
		if( rec.counter[0] < _time_count ) return TRUE;
	}

	for( i = 0; i < 4; i++ ){
		rec.counter[ i ] = _time_count;
		rec.random[  i ] = (unsigned char)(Random( 0, 250 ) + i);
		p = (unsigned char *)&rec.counter[ i ];
		p[0] += rec.random[ i ];
		p[1] += rec.random[ i ];
		p[2] += rec.random[ i ];
		p[3] += rec.random[ i ]/2;
	}

	fp = fopen( path, "wb" );
	if( !fp ) return FALSE; 
	fwrite( &rec, sizeof(COUNTERRECORDE), 1, fp );
	fclose( fp );

	return TRUE;
}

long LoadTimeCounter( void )
{
	long          i;
	unsigned char *p;
	COUNTERRECORDE rec;
	FILE *fp;
	char path[MAX_PATH];

	sprintf( path, "%s\\290.rec", gModulePath );
	fp = fopen( path, "rb" );
	if( !fp ) return 0; 
	fread( &rec, sizeof(COUNTERRECORDE), 1, fp );
	fclose( fp );

	for( i = 0; i < 4; i++ ){
		p = (unsigned char *)&rec.counter[ i ];
		p[0] -= rec.random[ i ];
		p[1] -= rec.random[ i ];
		p[2] -= rec.random[ i ];
		p[3] -= rec.random[ i ] / 2;
	}
	if( rec.counter[0] != rec.counter[1] ||
		rec.counter[0] != rec.counter[2] )
	{
		_time_count = 0;
		return 0;
	}

	_time_count = rec.counter[0];
	return _time_count;
}

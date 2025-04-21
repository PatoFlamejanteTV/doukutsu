#include <windows.h>
#include "Draw.h"
#include "Sound.h"
#include "Define.h"
#include "Tags.h"
#include "TextScr.h"

#include "Position.h"

#include "KeyControl.h"
#include "Escape.h"

extern long g_GameFlags;

extern HWND ghWnd;

void PutNumber4( long x, long y, long value, BOOL bZero );
void PutFramePerSecound( void );


ARMSDATA gArmsData[MAX_ARMSDATA];
ITEMDATA gItemData[MAX_ITEMDATA];
long gSelectedArms = 0;
long gSelectedItem = 0;
long gCampTitleY;
long gCampActive;// 0:Arms 1:Item
long gArmsEnergyX = ARMSENERGY_X;//+16;



#define ITEM_HNUM  6

BOOL AddArmsData( long code, long max_num );
void ResetSpurCharge( void );
void ClearArmsData( void )
{
	gArmsEnergyX = ARMSENERGY_X+16;
	memset( gArmsData, 0, sizeof(ARMSDATA)*MAX_ARMSDATA );

}

void ClearItemData( void )
{
	memset( gItemData, 0, sizeof(ITEMDATA)*MAX_ITEMDATA );
}

//武器を追加
BOOL AddArmsData( long code, long max_num )
{
	long i;

	i = 0; 
	while( i < MAX_ARMSDATA ){
		if( gArmsData[i].code == code )break;
		if( gArmsData[i].code == 0    )break;
		i++;
	}
	if( i == MAX_ARMSDATA )return FALSE;

	if( !gArmsData[i].code ){
		memset( &gArmsData[i], 0, sizeof(ARMSDATA) );
		gArmsData[i].level = 1;
	}

	gArmsData[i].code      = code;
	gArmsData[i].max_num  += max_num;
	gArmsData[i].num      += max_num;

	if( gArmsData[i].num > gArmsData[i].max_num )
		gArmsData[i].num = gArmsData[i].max_num;

	return TRUE;

}

//武器を削除
BOOL SubArmsData( long code )
{
	long i;

	for( i = 0; i < MAX_ARMSDATA; i++ )
		if( gArmsData[i].code == code ) break;
	if( i == MAX_ITEMDATA ) return FALSE;

	
	//シフト
	i++;
	for(      ; i < MAX_ARMSDATA; i++ ) gArmsData[i-1] = gArmsData[i];
	gArmsData[i-1].code = 0;
	gSelectedArms       = 0;

	return TRUE;
}

// 武器を交換
BOOL TradeArms( long code1, long code2, long max_num )
{
	long i;

	i = 0; 
	while( i < MAX_ARMSDATA ){
		if( gArmsData[i].code == code1 )break;
		i++;
	}
	if( i == MAX_ARMSDATA )return FALSE;

	gArmsData[i].level     = 1;
	gArmsData[i].code      = code2;
	gArmsData[i].max_num   += max_num;
	gArmsData[i].num       += max_num;
	gArmsData[i].exp       = 0;

	return TRUE;

}


// アイテムを追加
BOOL AddItemData( long code )
{
	long i;

	i = 0; 
	while( i < MAX_ITEMDATA ){
		if( gItemData[i].code == code ) break;
		if( gItemData[i].code == 0    ) break;
		i++;
	}
	if( i == MAX_ITEMDATA )return FALSE;

	gItemData[i].code =  code;

	return TRUE;

}

//アイテムを削除
BOOL SubItemData( long code )
{
	long i;

	for( i = 0; i < MAX_ITEMDATA; i++ )
		if( gItemData[i].code == code ) break;
	if( i == MAX_ITEMDATA )return FALSE;

	
	//シフト
	i++;
	for(      ; i < MAX_ITEMDATA; i++ ) gItemData[i-1] = gItemData[i];
	gItemData[i-1].code = 0;
	gSelectedItem       = 0;

	return TRUE;
}



void MoveCampCursor( void )
{
	BOOL bChange;
	long arms_num;
	long item_num;

	//武器の数を取得
	arms_num = 0;
	item_num = 0;
	while( gArmsData[arms_num].code ) arms_num++;
	while( gItemData[item_num].code ) item_num++;


	if( !arms_num && !item_num ) return;


	bChange = FALSE;

	// 武器ライン
	if( !gCampActive ){

		if( gKeyTrg&gKeyLeft            ){
			gSelectedArms--;
			bChange = TRUE;
		}
		if( gKeyTrg&gKeyRight           ){
			gSelectedArms++;
			bChange = TRUE;
		}
		if( gKeyTrg & (gKeyUp|gKeyDown) ){
			if( item_num ) gCampActive = 1;
			bChange = TRUE;
		}

		if( gSelectedArms < 0           ) gSelectedArms = arms_num -1;
		if( gSelectedArms > arms_num -1 ) gSelectedArms = 0;

	// アイテムライン
	}else{

		if( gKeyTrg&gKeyLeft ){// ←
			if( gSelectedItem%ITEM_HNUM == 0 ) gSelectedItem += ITEM_HNUM-1;
			else                               gSelectedItem--;
			bChange = TRUE;
		}
		if( gKeyTrg&gKeyRight ){// →
			if(      gSelectedItem == item_num - 1          ) gSelectedItem = (gSelectedItem/ITEM_HNUM)*ITEM_HNUM;
			else if( gSelectedItem%ITEM_HNUM == ITEM_HNUM-1 ) gSelectedItem -= ITEM_HNUM-1;
			else                                              gSelectedItem++;
			bChange = TRUE;
		}
		if( gKeyTrg&gKeyUp    ){// ↑
			if( gSelectedItem/ITEM_HNUM == 0 ) gCampActive    = 0;
			else                               gSelectedItem -= ITEM_HNUM;
			bChange = TRUE;
		}
		if( gKeyTrg&gKeyDown  ){// ↓
			if( gSelectedItem/ITEM_HNUM == (item_num-1)/ITEM_HNUM ) gCampActive    = 0;
			else                                                    gSelectedItem += ITEM_HNUM;
			bChange = TRUE;
		}

		if( gSelectedItem >= item_num ) gSelectedItem = item_num - 1;

		if( gCampActive && gKeyTrg & gKeyOk ) StartTextScript( 6000+gItemData[gSelectedItem].code );
	}

	// 武器ラインにいる時
	if( bChange ){
		if( gCampActive == 0 ){
			PlaySoundObject( WAVE_CHANGEARMS, 1 );
			if( arms_num ) StartTextScript( 1000+gArmsData[gSelectedArms].code );
			else           StartTextScript( 1000 );
		}else{
			PlaySoundObject( WAVE_CURSOR, 1 );
			if( item_num ) StartTextScript( 5000+gItemData[gSelectedItem].code );
			else           StartTextScript( 5000 );
		}
	}


}
void PutCampObject( void )
{
	static unsigned long flash = 0;

	long i;
	RECT rcArms;
	RECT rcItem;
	RECT rcPer    = { 72, 48, 80, 56};
	RECT rcNone   = { 80, 48, 96, 56};
	RECT rcLv     = { 80, 80, 96, 88};
	RECT rcView   = { 0, 0, SURFACE_WIDTH, SURFACE_HEIGHT };
	RECT rcCur1[] = {
		{  0, 88, 40,128},
		{ 40, 88, 80,128},
	};
	RECT rcCur2[] = {
		{ 80, 88,112,104},
		{ 80,104,112,120},
	};

	RECT rcTitle1 = { 80, 48,144, 56};
	RECT rcTitle2 = { 80, 56,144, 64};

	RECT rcBoxTop    = {  0,  0,244,  8};
	RECT rcBoxBody   = {  0,  8,244, 16};
	RECT rcBoxBottom = {  0, 16,244, 24};



		PutBitmap3( &rcView, 38, 8+8*0, &rcBoxTop,    SURF_TEXTBOX );
	for( i = 1; i < 18; i++ )
		PutBitmap3( &rcView, 38, 8+8*i, &rcBoxBody,   SURF_TEXTBOX );
		PutBitmap3( &rcView, 38, 8+8*i, &rcBoxBottom, SURF_TEXTBOX );



	// タイトル
	if( gCampTitleY > 16 )gCampTitleY -= 1;
	PutBitmap3( &rcView,  48, gCampTitleY,    &rcTitle1, SURF_TEXTBOX );
	PutBitmap3( &rcView,  48, gCampTitleY+52, &rcTitle2, SURF_TEXTBOX );


	flash++;

	//=Arms==============================================
	//カーソル
	if( gCampActive == 0 ) PutBitmap3( &rcView, 48 + gSelectedArms*40, ARMSSELECT_Y, &rcCur1[flash/2%2], SURF_TEXTBOX );
	else                   PutBitmap3( &rcView, 48 + gSelectedArms*40, ARMSSELECT_Y, &rcCur1[1],         SURF_TEXTBOX );
	//アイテム
	for( i = 0; i < MAX_ARMSDATA; i++ ){
		if( !gArmsData[i].code )break;
		// rect
		rcArms.left   = (gArmsData[i].code%16) * 16;
		rcArms.right  = rcArms.left + 16;
		rcArms.top    = (gArmsData[i].code/16) * 16;
		rcArms.bottom = rcArms.top  + 16;

		PutBitmap3( &rcView, 48 + i*40, ARMSSELECT_Y,      &rcArms, SURF_ARMSIMAGE );
		PutBitmap3( &rcView, 48 + i*40, ARMSSELECT_Y + 32, &rcPer,  SURF_TEXTBOX   );
		PutBitmap3( &rcView, 48 + i*40, ARMSSELECT_Y + 16, &rcLv,   SURF_TEXTBOX   );
		PutNumber4(          48 + i*40, ARMSSELECT_Y + 16, gArmsData[i].level, FALSE );
		if( gArmsData[i].max_num ){
			PutNumber4( 48 + i*40 ,     ARMSSELECT_Y + 24, gArmsData[i].num, FALSE     );
			PutNumber4( 48 + i*40 ,     ARMSSELECT_Y + 32, gArmsData[i].max_num, FALSE );
		}else{
			PutBitmap3( &rcView, 48 + i*40+16, ARMSSELECT_Y + 24, &rcNone, SURF_TEXTBOX );
			PutBitmap3( &rcView, 48 + i*40+16, ARMSSELECT_Y + 32, &rcNone, SURF_TEXTBOX );
		}
	}
	//=Item==============================================
	//カーソル
	if( gCampActive == 1 )
		PutBitmap3( &rcView,
				48          +(gSelectedItem%ITEM_HNUM)*32,
				ITEMSELECT_Y+(gSelectedItem/ITEM_HNUM)*16, &rcCur2[flash/2%2], SURF_TEXTBOX );
	else
		PutBitmap3( &rcView,
				48          +(gSelectedItem%ITEM_HNUM)*32,
				ITEMSELECT_Y+(gSelectedItem/ITEM_HNUM)*16, &rcCur2[1],         SURF_TEXTBOX );
	//アイテム
	for( i = 0; i < MAX_ITEMDATA; i++ ){
		if( !gItemData[i].code )break;
		rcItem.left   = (gItemData[i].code%8) * 32;
		rcItem.right  = rcItem.left + 32;
		rcItem.top    = (gItemData[i].code/8) * 16;
		rcItem.bottom = rcItem.top  + 16;

		PutBitmap3( &rcView, 48 + (i%ITEM_HNUM)*32, ITEMSELECT_Y+(i/ITEM_HNUM)*16, &rcItem, SURF_ITEMIMAGE );
	}

}


enum_ESCRETURN CampLoop( void )//long *p_code )
{
	char old_script_path[MAX_PATH];
	RECT rcView = { 0, 0, SURFACE_WIDTH, SURFACE_HEIGHT };

	GetTextScriptPath( old_script_path );
	LoadTextScript2( "ArmsItem.tsc" );
	gCampTitleY = 24;
	gCampActive = 0;
	gSelectedItem = 0;

	long arms_num = 0;
	while( gArmsData[arms_num].code )arms_num++;
	if( arms_num ) StartTextScript( 1000+gArmsData[gSelectedArms].code );
	else           StartTextScript( 5000+gItemData[gSelectedItem].code );

	for( ;; ){
		GetTrg();
		if( gKeyTrg & KEY_ESC ){
			switch( Call_Escape( ghWnd ) ){
			case enum_ESCRETURN_exit:     return enum_ESCRETURN_exit;
			case enum_ESCRETURN_restart:  return enum_ESCRETURN_restart;
			case enum_ESCRETURN_continue: break;
			}
		}
		//カーソル移動
		if( g_GameFlags & GAMEFLAG_KEYCONTROL ) MoveCampCursor();
		switch( TextScriptProc(  ) ){
		case enum_ESCRETURN_exit:     return enum_ESCRETURN_exit;
		case enum_ESCRETURN_restart:  return enum_ESCRETURN_restart;
		case enum_ESCRETURN_continue: break;
		}

		//表示
		PutBitmap4( &rcView, 0,0, &rcView, SURF_DISPLAY );
		PutCampObject();
		PutTextScript();
		PutFramePerSecound();

		if( gCampActive ){//アイテム
			if( g_GameFlags & GAMEFLAG_KEYCONTROL && gKeyTrg & (gKeyCancel|gKeyItem) ){
				StopTextScript();
				break;
			}
		}else{//武器
			if( gKeyTrg & (gKeyOk|gKeyCancel|gKeyItem) ){
				StopTextScript();
				break;
			}
		}

		if( !Flip_SystemTask( ghWnd ) ) return enum_ESCRETURN_exit;//強制終了
	}
	LoadTextScript_Stage( old_script_path );
	gArmsEnergyX = ARMSENERGY_X+16;
//	*p_code =  gArmsData[gSelectedArms].code;
	return enum_ESCRETURN_continue;
}


BOOL CheckItem( long a )
{
	long i;

	for( i = 0; i < MAX_ITEMDATA; i++ ){
		if( gItemData[i].code == a )return TRUE;
	}
	return FALSE;
}
BOOL CheckArms( long a )
{
	long i;

	for( i = 0; i < MAX_ARMSDATA; i++ ){
		if( gArmsData[i].code == a )return TRUE;
	}
	return FALSE;
}







BOOL UseArmsEnergy( long num )
{
	if( !gArmsData[gSelectedArms].max_num ) return TRUE;
	if( !gArmsData[gSelectedArms].num     ) return FALSE;
	gArmsData[gSelectedArms].num -= num;
	if( gArmsData[gSelectedArms].num < 0 ) gArmsData[gSelectedArms].num = 0;
	return TRUE;
}

BOOL ChargeArmsEnergy( long num )
{
	gArmsData[gSelectedArms].num += num;
	if( gArmsData[gSelectedArms].num > gArmsData[gSelectedArms].max_num )
		gArmsData[gSelectedArms].num = gArmsData[gSelectedArms].max_num;
	return TRUE;
}


void FullArmsEnergy( void )
{
	long a;
	for( a = 0; a < MAX_ARMSDATA; a++ ){
		if( !gArmsData[a].code )continue;
		gArmsData[a].num = gArmsData[a].max_num;
	}
}

long RotationArms( void )
{
	long arms_num;
	//武器の数を取得
	arms_num = 0;
	while( gArmsData[arms_num].code )arms_num++;
	if( !arms_num )return 0;

	ResetSpurCharge();

	gSelectedArms++;
	while( gSelectedArms < arms_num ){
		if( gArmsData[gSelectedArms].code )break;
		gSelectedArms++;
	}

	if( gSelectedArms == arms_num  ) gSelectedArms = 0;
	gArmsEnergyX = ARMSENERGY_X+16;
	PlaySoundObject( WAVE_CHANGEARMS, 1 );

	return gArmsData[gSelectedArms].code;
}

long RotationArmsRev( void )
{
	long arms_num;
	//武器の数を取得
	arms_num = 0;
	while( gArmsData[arms_num].code ) arms_num++;
	if( !arms_num )return 0;

	ResetSpurCharge();

	gSelectedArms--;
	if( gSelectedArms < 0 ) gSelectedArms = arms_num - 1;
	while( gSelectedArms < arms_num ){
		if( gArmsData[gSelectedArms].code )break;
		gSelectedArms--;
	}

	gArmsEnergyX = ARMSENERGY_X-16;
	PlaySoundObject( WAVE_CHANGEARMS, 1 );

	return gArmsData[gSelectedArms].code;
}

void ChangeToFirstArms( void )
{
	gSelectedArms = 0;
	gArmsEnergyX = ARMSENERGY_X+16;
	PlaySoundObject( WAVE_CHANGEARMS, 1 );
}







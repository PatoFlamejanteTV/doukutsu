#include <windows.h>
#include "Draw.h"
#include "Sound.h"
#include "Define.h"
#include "Tags.h"
#include "TextScr.h"

#include "KeyControl.h"
#include "Escape.h"

#define STAGESELECT_Y	(64)

//システム
extern HWND ghWnd;

extern RECT grcFull;
PERMITSTAGE gPermitStage[MAX_PERMITSTAGE];

static long gSelectedStage = 0;
static long gStageSelectTitleY;
void PutFramePerSecound( void );

//BOOL AddPermitStage( long index, long event );
void ClearPermitStage( void )
{
	memset( gPermitStage, 0, sizeof(PERMITSTAGE)*MAX_PERMITSTAGE );

}

//許可ステージを追加
BOOL AddPermitStage( long index, long event )
{
	long i;

	i = 0; 
	while( i < MAX_PERMITSTAGE ){
		if( gPermitStage[i].index == index )break;
		if( gPermitStage[i].index == 0     )break;
		i++;
	}
	if( i == MAX_PERMITSTAGE )return FALSE;

	gPermitStage[i].index = index;
	gPermitStage[i].event = event;

	return TRUE;

}


//アイテムを削除
BOOL SubPermitStage( long index )
{
	long i;

	for( i = 0; i < MAX_PERMITSTAGE; i++ )
		if( gPermitStage[i].index == index )break;
	if( i == MAX_ITEMDATA )return FALSE;

	//シフト
	i++;
	for(      ; i < MAX_PERMITSTAGE; i++ )
		gPermitStage[i-1] = gPermitStage[i];

	gPermitStage[i-1].index =  0;
	gPermitStage[i-1].event =  0;
	

	return TRUE;

}

void MoveStageSelectCursor( void )
{
	long stage_num;
	long stage_x;

	stage_num = 0;
	while( gPermitStage[stage_num].index )stage_num++;
	stage_x = ( SURFACE_WIDTH - (stage_num * 40)) / 2;


	if( !stage_num ) return;

	if( gKeyTrg&gKeyLeft  )gSelectedStage--;
	if( gKeyTrg&gKeyRight )gSelectedStage++;
	if( gSelectedStage < 0            ) gSelectedStage = stage_num -1;
	if( gSelectedStage > stage_num -1 ) gSelectedStage = 0;
	if( gKeyTrg&(gKeyLeft|gKeyRight) ){
		StartTextScript( 1000+gPermitStage[gSelectedStage].index );
	}

	if( gKeyTrg & (gKeyLeft|gKeyRight) )PlaySoundObject( WAVE_CURSOR, 1 );
}

void PutStageSelectObject( void )
{
	static unsigned long flash = 0;

	long i;
	RECT rcStage;
	RECT rcView    = { 0, 0, SURFACE_WIDTH, SURFACE_HEIGHT };
	RECT rcCur[] = {
		{ 80, 88,112,104},
		{ 80,104,112,120},
	};

	RECT rcTitle1 = { 80, 64,144, 72};

	long stage_num;
	long stage_x;


	if( gStageSelectTitleY > 46 )gStageSelectTitleY -= 1;
	PutBitmap3( &rcView,  128, gStageSelectTitleY,    &rcTitle1, SURF_TEXTBOX );

	//テーブル
	//武器の数を取得
	stage_num = 0;
	while( gPermitStage[stage_num].index )
		stage_num++;

	flash++;

	if( stage_num ){
		stage_x = (SURFACE_WIDTH - (stage_num * 40)) / 2;
		//カーソル
		PutBitmap3( &rcView,  stage_x + gSelectedStage*40, STAGESELECT_Y, &rcCur[flash/2%2],  SURF_TEXTBOX );
		//アイテム
		for( i = 0; i < MAX_PERMITSTAGE; i++ ){
			if( !gPermitStage[i].index )break;
			rcStage.left   = (gPermitStage[i].index%8) * 32;
			rcStage.right  = rcStage.left + 32;
			rcStage.top    = (gPermitStage[i].index/8) * 16;
			rcStage.bottom = rcStage.top  + 16;
			PutBitmap3( &rcView, stage_x + i*40, STAGESELECT_Y, &rcStage, SURF_STAGEIMAGE );
		}
	}

}


enum_ESCRETURN StageSelectLoop( long *p_event )
{
	char old_script_path[MAX_PATH];
	RECT rcView = { 0, 0, SURFACE_WIDTH, SURFACE_HEIGHT };

	gSelectedStage = 0;
	BackupSurface( SURF_DISPLAY, &grcFull );

	GetTextScriptPath( old_script_path );
	LoadTextScript2( "StageSelect.tsc" );
	gStageSelectTitleY = STAGESELECT_Y-10;
	StartTextScript( 1000+gPermitStage[gSelectedStage].index );
	for( ;; ){
		GetTrg();

		if( gKey & KEY_ESC ){
			switch( Call_Escape( ghWnd ) ){
			case enum_ESCRETURN_exit:     return enum_ESCRETURN_exit;
			case enum_ESCRETURN_restart:  return enum_ESCRETURN_restart;
			case enum_ESCRETURN_continue: break;
			}
		}
		//カーソル移動
		MoveStageSelectCursor();
		switch( TextScriptProc( ) ){
		case enum_ESCRETURN_exit:     return enum_ESCRETURN_exit;
		case enum_ESCRETURN_restart:  return enum_ESCRETURN_restart;
		case enum_ESCRETURN_continue: break;
		}
		//表示
		PutBitmap3( &rcView, 0,0, &rcView, SURF_DISPLAY );
		PutStageSelectObject();
		PutTextScript();

		if(       gKeyTrg & gKeyOk     ){
			StopTextScript();
			break;
		}else if( gKeyTrg & gKeyCancel ){
			StopTextScript();
			LoadTextScript_Stage( old_script_path );
			*p_event = 0;
			return enum_ESCRETURN_continue;
		}

		PutFramePerSecound();
		if( !Flip_SystemTask( ghWnd ) ) return enum_ESCRETURN_exit;//強制終了
	}
	LoadTextScript_Stage( old_script_path );
	*p_event = gPermitStage[gSelectedStage].event;
	return enum_ESCRETURN_continue;
}



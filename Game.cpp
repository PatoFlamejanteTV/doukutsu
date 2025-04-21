#include <windows.h>
#include <stdio.h>
#include <dsound.h>

#include "Organya.h"

#include "define.h"
#include "draw.h"
#include "Tags.h"
#include "Map.h"
#include "TextScr.h"

#include "sound.h"
#include "KeyControl.h"
#include "Escape.h"


extern HWND ghWnd;
extern MYCHAR gMC; 

RECT grcGame = {0,0,SURFACE_WIDTH,SURFACE_HEIGHT};
RECT grcFull = {0,0,SURFACE_WIDTH,SURFACE_HEIGHT};
long g_GameFlags;

BOOL GetCompileVersion( long *v1, long *v2, long *v3, long *v4 );
void PutFramePerSecound( void );
//乱数の発生
long Random(int min, int max)
{
    int range;
    range = max-min+1;
    return((rand()%range)+min);
}




void PutNumber4( long x, long y, long value, BOOL bZero )
{
	RECT rcClient = { 0, 0, SURFACE_WIDTH, SURFACE_HEIGHT};
	RECT rect[] = {
		{  0, 56,  8, 64},
		{  8, 56, 16, 64},
		{ 16, 56, 24, 64},
		{ 24, 56, 32, 64},
		{ 32, 56, 40, 64},
		{ 40, 56, 48, 64},
		{ 48, 56, 56, 64},
		{ 56, 56, 64, 64},
		{ 64, 56, 72, 64},
		{ 72, 56, 80, 64},

	};
	long tbl[] = {1000,100,10,1};

	long a;
	long sw;
	long offset;

	if( value > 9999 )value = 9999;
	
	offset = 0;
	sw     = 0;
	
	while( offset < 4 ){
		a = 0;
		while( value >= tbl[offset] ){
			value -= tbl[offset];
			a++;
			sw++;
		}
		if( (bZero && offset == 2) || sw || offset == 3 )
			PutBitmap3( &rcClient, x + offset*8, y, &rect[a], SURF_TEXTBOX );
		offset++;
	}
}


#define MODE_EXIT		0
#define MODE_OPENING	1
#define MODE_TITLE		2
#define MODE_ACTION		3


//MyChar Module
void AnimationMyChar( BOOL bKey );
void PutMyChar( long fx, long fy );
void PutMyLife( BOOL flash );
void PutMyAir( long x, long y );
void PutTimeCounter( long x, long y );
void InitMyChar( void );
void ActMyChar( BOOL bKey );
void ResetMyCharFlag( void );
void HitMyCharMap( void );
void HitMyCharNpChar( void );
void DamageMyChar( long damage );

//NpChar Module
void PutNpChar( long fx, long fy );
void InitNpChar( void );
void ActNpChar( void );
void HitNpCharMap( void );
void ReleaseNpChar( void );
void HitNpCharBullet( void );
long CountAliveNpChar( void );
// Boss
void InitBossLife( void );
void PutBossLife( void );
void ActBossChar( void );
void PutBossChar( long fx, long fy );
void HitBossBullet( void );
void HitBossMap( void );
void HitMyCharBoss( void );

//Bullet module
void InitBullet( void );
void PutBullet( long fx, long fy );
void ActBullet( void );
void HitBulletMap( void );
void ShootBullet( void );
void SetBullet( long no, long x, long y, long dir );

void InitFlags(     void );
void InitSkipFlags( void );
void PutArmsEnergy( BOOL flash );
void PutActiveArmsList( void );

//Caret Module
void InitCaret( void );
void SetCaret( long x, long y, long code, long dir );
void PutCaret( long fx, long fy );
void ActCaret( void );

//Frame
void MoveFrame3( void );
void SetFrameTargetMyChar( long wait );
void SetFrameMyChar( void );
void GetFramePosition( long *fx, long *fy );
void SetFramePosition( long  fx, long  fy );

//Fade
void InitFade(    void );
void ProcFade(    void );
void PutFade(     void );
void SetFadeMask( void );

// star
void InitStar( void );
void ActStar( void );
void PutStar( long fx, long fy );



BOOL TransferStage( long no, long w, long x, long y );
void ChangeMusic( long no );

void PutMapName( BOOL bMini );
void ClearArmsData( void );
enum_ESCRETURN CampLoop( );//long *p_code );
void ClearItemData( void );
long RotationArms( void );
long RotationArmsRev( void );
void ClearPermitStage( void );

BOOL IsProfile( void );

static BOOL _bContinue;

void CutNoise( void );
long LoadTimeCounter( void );

//モード群■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void ActValueView( void );
void PutValueView( long flx, long fly );

void InitFlash( void );
void ActFlash( long flx, long fly );
void PutFlash( void );

void ActBack( void );
void PutBack(  long fx, long fy );
void PutFront( long fx, long fy );
enum_ESCRETURN MiniMapLoop( void );
void StartMapping( void );

long gCounter;

// オープニング
static long _ModeOpening( HWND hWnd )
{
	//表示フレーム
	long frame_x;//XSIZE*VS;
	long frame_y;//SURFACE_HEIGHT*VS;
	DWORD wait;


	InitNpChar();
	InitCaret();
	InitStar();
	InitFade();
	InitFlash();
	InitBossLife();

	ChangeMusic( 0 );

	TransferStage( 72, 100, 3, 3 );
	SetFrameTargetMyChar( 16 );
	SetFadeMask();

	grcGame.left  = 0;
	g_GameFlags   = (GAMEFLAG_ACTION|GAMEFLAG_KEYCONTROL);
	CutNoise();

	wait = 0;
	//フィールドループ
	while( wait < 10*50 ){
		wait++;
		GetTrg();

		if( gKey & KEY_ESC ){
			switch( Call_Escape( ghWnd ) ){
			case enum_ESCRETURN_exit:     return MODE_EXIT;
			case enum_ESCRETURN_restart:  return MODE_OPENING;
			case enum_ESCRETURN_continue: break;
			}
		}
		if( gKey & gKeyOk ) break;

		// アクション============================

		ActNpChar();
		ActBossChar();
		ActBack();
		ResetMyCharFlag();//あたり判定をリセット
		HitMyCharMap();
		HitMyCharNpChar();
		HitMyCharBoss();
		HitNpCharMap();
		HitBossMap();
		HitBossBullet();

		ActCaret();

		MoveFrame3();
//		ActFlash( frame_x, frame_y);

		ProcFade();

		// 表示============================
		CortBox( &grcFull, 0 );
		GetFramePosition( &frame_x, &frame_y );

		PutBack(          frame_x, frame_y );
		PutStage_Back(    frame_x, frame_y );
		PutBossChar(      frame_x, frame_y );
		PutNpChar(        frame_x, frame_y );
		PutMapDataVector( frame_x, frame_y );
		PutStage_Front(   frame_x, frame_y );
		PutFront(         frame_x, frame_y );
//		PutFlash();
		PutCaret(         frame_x, frame_y );

		PutFade();

		switch( TextScriptProc(  ) ){
		case enum_ESCRETURN_exit:     return MODE_EXIT;
		case enum_ESCRETURN_restart:  return MODE_OPENING;
		case enum_ESCRETURN_continue: break;
		}

		PutMapName( FALSE );
		PutTextScript();
		PutFramePerSecound();

		if( !Flip_SystemTask( ghWnd ) ) return MODE_EXIT;

		gCounter++;
	}

	wait = GetTickCount();
	while( 	GetTickCount() < wait + 500 ){
		CortBox( &grcGame, 0 );
		PutFramePerSecound();
		if( !Flip_SystemTask( ghWnd ) ) return MODE_EXIT;//強制終了
	}

	return MODE_TITLE;
}

// タイトル
static long _ModeTitle( HWND hWnd )
{
	RECT rcTitle    = {  0,  0,144, 32};
	RECT rcPixel    = {  0,  0,160, 16};
	RECT rcNew      = {144,  0,192, 16};
	RECT rcContinue = {144, 16,192, 32};

	RECT rcVersion  = {152, 80,208, 88};
	RECT rcPeriod   = {152, 88,208, 96};

	RECT rcMyChar[] = {
		{  0, 16, 16, 32},
		{ 16, 16, 32, 32},
		{  0, 16, 16, 32},
		{ 32, 16, 48, 32},
	};
	RECT rcCurly[] = {
		{  0,112, 16, 128},
		{ 16,112, 32, 128},
		{  0,112, 16, 128},
		{ 32,112, 48, 128},
	};
	RECT rcToroko[] = {
		{ 64, 80, 80,  96},
		{ 80, 80, 96,  96},
		{ 64, 80, 80,  96},
		{ 96, 80,112,  96},
	};
	RECT rcKing[] = {
		{ 224, 48,240,  64},
		{ 288, 48,304,  64},
		{ 224, 48,240,  64},
		{ 304, 48,320,  64},
	};
	RECT rcSu[] = {
		{ 0, 16,16,  32},
		{32, 16,48,  32},
		{ 0, 16,16,  32},
		{48, 16,64,  32},
	};
	DWORD wait;


	InitCaret();
	InitStar();
	CutNoise();

	long anime = 0;
	long v1, v2, v3, v4;

	RECT char_rc;
	long char_type    = 0;
	long time_counter = 0;
	long char_y;
	long char_surf;
	long back_color;

	back_color = GetCortBoxColor( RGB(0x20, 0x20,0x20) );

	if( IsProfile() ) _bContinue = TRUE;
	else              _bContinue = FALSE;

	time_counter = LoadTimeCounter();
	if( time_counter && time_counter < 50*60*6 ) char_type = 1;
	if( time_counter && time_counter < 50*60*5 ) char_type = 2;
	if( time_counter && time_counter < 50*60*4 ) char_type = 3;
	if( time_counter && time_counter < 50*60*3 ) char_type = 4;

//	char_type = 4;

	if(      char_type == 1 ) ChangeMusic( 36 );
	else if( char_type == 2 ) ChangeMusic( 40 );
	else if( char_type == 3 ) ChangeMusic( 41 );
	else if( char_type == 4 ) ChangeMusic(  2 );
	else                      ChangeMusic( 24 );

	grcGame.left = 0;
	g_GameFlags  = 0;
	gMC.equip |= EQUIP_COUNTER;

	wait = 0;
	while( 1 ){
		if( wait <  10 ) wait++;
		GetTrg();

		if( wait >= 10 ){
			if( gKeyTrg & gKeyOk ){
				PlaySoundObject( WAVE_OK, 1 );
				break;
			}
		}
		if( gKey & KEY_ESC ){
			switch( Call_Escape( ghWnd ) ){
			case enum_ESCRETURN_exit:     return MODE_EXIT;
			case enum_ESCRETURN_restart:  return MODE_OPENING;
			case enum_ESCRETURN_continue: break;
			}
		}

		if( gKeyTrg & (gKeyUp|gKeyDown) ){
			PlaySoundObject( WAVE_CURSOR, 1 );
			if( _bContinue ) _bContinue = FALSE;
			else             _bContinue = TRUE;
		}

		ActCaret();

		anime++;
		if( anime >= 4*10 )anime = 0;

		CortBox( &grcGame, back_color );
		// version
		PutBitmap3( &grcGame, 320/2 - 60,    240-24,    &rcVersion,  SURF_TEXTBOX );
		PutBitmap3( &grcGame, 320/2 -  4,    240-24,    &rcPeriod,   SURF_TEXTBOX );
		GetCompileVersion( &v1, &v2, &v3, &v4 );
		PutNumber4(           320/2 - 8*2 -4,   240-24, v1, FALSE );
		PutNumber4(           320/2 - 8*0 -4,   240-24, v2, FALSE );
		PutNumber4(           320/2 + 8*2 -4,   240-24, v3, FALSE );
		PutNumber4(           320/2 + 8*4 -4,   240-24, v4, FALSE );

		PutBitmap3( &grcGame, (320-144)/2,  40,    &rcTitle,    SURF_TITLE );
		PutBitmap3( &grcGame, (320- 48)/2, 128,    &rcNew,      SURF_TITLE );
		PutBitmap3( &grcGame, (320- 48)/2, 128+20, &rcContinue, SURF_TITLE );
		PutBitmap3( &grcGame, (320-160)/2, 240-48, &rcPixel,    SURF_PIXEL );

		switch( char_type ){
		case 0: char_rc = rcMyChar[ (anime/10)%4 ]; char_surf = SURF_MYCHAR;      break;
		case 1: char_rc = rcCurly[  (anime/10)%4 ]; char_surf = SURF_NPC_REGULAR; break;
		case 2: char_rc = rcToroko[ (anime/10)%4 ]; char_surf = SURF_NPC_REGULAR; break;
		case 3: char_rc = rcKing[   (anime/10)%4 ]; char_surf = SURF_NPC_REGULAR; break;
		case 4: char_rc = rcSu[     (anime/10)%4 ]; char_surf = SURF_NPC_REGULAR; break;
		}
		if( !_bContinue ) char_y = 127;
		else              char_y = 127 + 20;
		if( !_bContinue ) PutBitmap3( &grcGame, (320- 48)/2 - 20, char_y, &char_rc, char_surf );
		else              PutBitmap3( &grcGame, (320- 48)/2 - 20, char_y, &char_rc, char_surf );

		PutCaret( 0, 0 );
		if( time_counter ) PutTimeCounter( 8*2, 8*1 ); 
		PutFramePerSecound();

		if( !Flip_SystemTask( ghWnd ) ) return MODE_EXIT;//強制終了
	}
	ChangeMusic( 0 );


	wait = GetTickCount();
	while( 	GetTickCount() < wait + 1000 ){
		CortBox( &grcGame, 0 );
		PutFramePerSecound();
		if( !Flip_SystemTask( ghWnd ) ) return MODE_EXIT;//強制終了
	}

	return MODE_ACTION;
}






//MODE_ACTION-------------------------------------
// Ending
void ActionStripper( void );
void PutStripper(    void );
void ActionCredit(   void );
void ActionIllust(   void );
void PutIllust(      void );
void InitCreditScript( void );
void ReleaseCreditScript( void );

BOOL LoadProfile( char *name );
BOOL SaveProfile( char *name );
BOOL InitializeGame( HWND hWnd );
static long _ModeAction( HWND hWnd )
{
	//表示フレーム
	long frame_x;//XSIZE*VS;
	long frame_y;//SURFACE_HEIGHT*VS;

	unsigned long swPlay;
	unsigned long _color;

	_color = GetCortBoxColor( RGB( 0, 0, 32 ) );

	swPlay        = 1;
	gCounter      = 0;
	grcGame.left  = 0;
	g_GameFlags   = (GAMEFLAG_ACTION|GAMEFLAG_KEYCONTROL);



	InitMyChar();
	InitNpChar();
	InitBullet();
	InitCaret();
	InitStar();
	InitFade();
	InitFlash();

	ClearArmsData();
	ClearItemData();
	ClearPermitStage();
	StartMapping();
	InitFlags();
	InitBossLife();

	if( _bContinue ){
		// アクションとキー操作を許可
		if( !LoadProfile( NULL ) ){
			if( !InitializeGame( hWnd ) ) return MODE_EXIT;
		}
	}else{
			if( !InitializeGame( hWnd ) ) return MODE_EXIT;
	}


	//フィールドループ
	while( 1 ){
		GetTrg();

#ifdef _DEBUG
		if( gKeyTrg & KEY_SHIFT ) swPlay++;//■ポーズ
		if( gKeyTrg & KEY_F2    ) SaveProfile( NULL );//■セーブ
#endif
		if( gKeyTrg & KEY_ESC   ){
			switch( Call_Escape( ghWnd ) ){
			case enum_ESCRETURN_exit:     return MODE_EXIT;
			case enum_ESCRETURN_restart:  return MODE_OPENING;
			case enum_ESCRETURN_continue: break;
			}
		}

		// アクション============================
		if( swPlay%2 && g_GameFlags & GAMEFLAG_ACTION ){

			if( g_GameFlags & GAMEFLAG_KEYCONTROL ) ActMyChar( TRUE  );
			else                                    ActMyChar( FALSE );
			ActStar();
			ActNpChar();
			ActBossChar();
			ActValueView();
			ActBack();
			ResetMyCharFlag();//あたり判定をリセット
			HitMyCharMap();
			HitMyCharNpChar();
			HitMyCharBoss();

			HitNpCharMap();
			HitBossMap();
			HitBulletMap();
			HitNpCharBullet();
			HitBossBullet();

			if( g_GameFlags & GAMEFLAG_KEYCONTROL )ShootBullet();
			ActBullet();
			ActCaret();

			MoveFrame3();
			ActFlash( frame_x, frame_y);

			if( g_GameFlags & GAMEFLAG_KEYCONTROL ) AnimationMyChar( TRUE  );
			else                                    AnimationMyChar( FALSE );
		}
		// エンディングアクション ==================
		if( g_GameFlags & GAMEFLAG_ENDING ){
			ActionCredit();
			ActionIllust();
			ActionStripper();
		}


		ProcFade();

		// 表示============================
		CortBox( &grcFull, _color );
		GetFramePosition( &frame_x, &frame_y );

		PutBack(          frame_x, frame_y );
		PutStage_Back(    frame_x, frame_y );
		PutBossChar(      frame_x, frame_y );
		PutNpChar(        frame_x, frame_y );
		PutBullet(        frame_x, frame_y );
		PutMyChar(        frame_x, frame_y );
		PutStar(          frame_x, frame_y );

		PutMapDataVector( frame_x, frame_y );
		PutStage_Front(   frame_x, frame_y );
		PutFront(         frame_x, frame_y );
		PutFlash( );
		PutCaret(         frame_x, frame_y );
		PutValueView(     frame_x, frame_y );
		PutBossLife();

		PutFade();

		if( !(g_GameFlags & GAMEFLAG_TEXTSCRIPT) ){
			//アイテム画面
			if( gKeyTrg & gKeyItem ){
				BackupSurface( SURF_DISPLAY, &grcGame );
				switch( CampLoop( ) ){
				case enum_ESCRETURN_exit:     return MODE_EXIT;
				case enum_ESCRETURN_restart:  return MODE_OPENING;
				case enum_ESCRETURN_continue: break;
				}
				gMC.cond &= ~COND_CHECK;
			}
			// 縮小マップ
			else if( gMC.equip & EQUIP_MAPSYSTEM && gKeyTrg & gKeyMap ){
				BackupSurface( SURF_DISPLAY, &grcGame );
				switch( MiniMapLoop( ) ){
				case enum_ESCRETURN_exit:     return MODE_EXIT;
				case enum_ESCRETURN_restart:  return MODE_OPENING;
				case enum_ESCRETURN_continue: break;
				}
			}
		}
			// 武器切り替え
		if( g_GameFlags & GAMEFLAG_KEYCONTROL ){
			if(      gKeyTrg & gKeyArms    ) RotationArms();	
			else if( gKeyTrg & gKeyArmsRev ) RotationArmsRev();	
		}
		if( swPlay%2 ){
			switch( TextScriptProc(  ) ){
			case enum_ESCRETURN_exit:     return MODE_EXIT;
			case enum_ESCRETURN_restart:  return MODE_OPENING;
			case enum_ESCRETURN_continue: break;
			}
		}

		PutMapName( FALSE );

		// パラメータ表示
		PutTimeCounter( 8*2, 8*1 );
		if( ( g_GameFlags & GAMEFLAG_KEYCONTROL ) ){
			PutMyLife(     TRUE );
			PutArmsEnergy( TRUE );
			PutMyAir(       SURFACE_WIDTH/2-40, SURFACE_HEIGHT/2-16 );
			PutActiveArmsList();
		}

		// エンディング表示 ==================
		if( g_GameFlags & GAMEFLAG_ENDING ){
			PutIllust();
			PutStripper();
		}

		PutTextScript();

		PutFramePerSecound();
#ifdef _DEBUG
//		a = CountAliveNpChar();
//		PutNumber4( (320-8*4-8), 16, a );
#endif

		if( !Flip_SystemTask( ghWnd ) ) return MODE_EXIT;

		gCounter++;
	}

	return MODE_EXIT;
}



BOOL LoadGenericData( void );//汎用画像のロード
BOOL LoadNpcTable( char *path );
BOOL OutputNpcTable( char *path );
void ReleaseNpcTable( void );
extern char gDataPath[];
BOOL SaveWindowRect( HWND hWnd, char *name );

extern BOOL gbFullScreen;

BOOL Game( HWND hWnd )
{
	long mode;

	//汎用データの初期化
	if( !LoadGenericData() ){
		MessageBox( hWnd, "汎用ファイルが読めない", "エラー", MB_OK );
		return FALSE;
	}

	PlaySoundObject( WAVE_DUMMY, -1 );

	//NPCテーブルを生成
	char path[MAX_PATH];
	sprintf( path, "%s\\npc.tbl", gDataPath );
	if( !LoadNpcTable( path ) ){
		MessageBox( hWnd, "NPCテーブルが読めない", "エラー", MB_OK );
		return FALSE;
	}

	InitTextScript2();
	InitSkipFlags();
	InitMapData2();
	InitCreditScript();

	mode = MODE_OPENING;
	while( mode != MODE_EXIT ){
		if( mode == MODE_OPENING ) mode = _ModeOpening( hWnd );
		if( mode == MODE_TITLE   ) mode = _ModeTitle(   hWnd );
		if( mode == MODE_ACTION  ) mode = _ModeAction(  hWnd );
	}

	PlaySoundObject( WAVE_DUMMY, 0 );

	EndMapData();
	EndTextScript();
	ReleaseNpcTable();
	ReleaseCreditScript();

	if( !gbFullScreen ) SaveWindowRect( hWnd, "window.rect" );
	return TRUE;

}
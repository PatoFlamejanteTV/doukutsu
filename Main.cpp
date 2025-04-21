#include <windows.h>
#include <stdio.h>

#include <winuser.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi")
#include             <imm.h>
#pragma comment(lib, "imm32")

#include "resource.h"
#include "define.h"
#include "draw.h"
#include "Sound.h"
#include "Input.h"

#include "Config.h"
#include "KeyControl.h"

char *g_AppName = "洞窟物語";


HWND ghWnd = NULL;



HINSTANCE ghInst;

// コンフィグ
BOOL gbUseJoystick = FALSE;
BOOL gbFullScreen  = FALSE;
static BOOL _bFPS  = FALSE;

static long gJoystickButtonTable[MAX_JOYSTICKBUTTON_USE];


static long gClientWidth; 
static long gClientHeight;
static BOOL _bActive = FALSE; //ウインドウアクティブ時のみゲーム処理

char   gModulePath[MAX_PATH];
char   gDataPath[MAX_PATH];

HRESULT CALLBACK MainProc(         HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL    CALLBACK DialogProc_Save(  HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL    CALLBACK DialogProc_YesNo( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL    CALLBACK DialogProc_Mute(  HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL Game( HWND hWnd );
void InitTriangleTable( void );
void PutNumber4( long x, long y, long value, BOOL bZero );

// 汎用
//void GetCompileDate( long *year, long *month, long *day );
//BOOL GetCompileVersion( long *v1, long *v2, long *v3, long *v4 );
BOOL OpenSoundVolume( HWND hWnd );
long GetDateLimit( SYSTEMTIME *st_limit1, SYSTEMTIME *st_limit2 );
BOOL AppendLogPlot( char *str, long value );
BOOL IsKeyFile( char *name );

BOOL LoadProfile( char *name );

BOOL LoadWindowRect( HWND hWnd, char *name, BOOL bSize );
BOOL CALLBACK DialogProc_About( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

void _SetMainWindowText( HWND hWnd )
{
	char str[256];
#ifdef NDEBUG
	sprintf( str, "%s",       g_AppName );
#else
	sprintf( str, "%s Debug", g_AppName );
#endif
	SetWindowText( hWnd, str );
	
}

static long _CountFramePerSecond( void )
{
	static first = 1;
	static unsigned long wait  = 0;
	static unsigned long count = 0;
	static long max_count      = 0;

	unsigned long a;
	if( first ){
		wait = GetTickCount();
		first = 0;
	}

	a = GetTickCount();

	count++;
	if( wait + 1000 <= a ){
		wait += 1000;
		max_count = count;
		count = 0;
	}
	return max_count;
}

void PutFramePerSecound( void )
{
	if( _bFPS ){
		long a = _CountFramePerSecond();
		PutNumber4( (320-8*4-8),  8, a, FALSE );
	}
}

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow )
{

	static char *mutex_name = "Doukutsu";
	static HANDLE hPrevMutex;
	static HANDLE hMutex;

	//多重起動の防止---------------------------------------
	hPrevMutex = OpenMutex( MUTEX_ALL_ACCESS, FALSE, mutex_name );
	if( hPrevMutex ){
		CloseHandle( hPrevMutex );
		return 0;
	}
	hMutex = CreateMutex( FALSE, 0, mutex_name );

	ghInst = hInst;

/*	{// 使用期限確認
		SYSTEMTIME st1, st2;
		memset( &st1, 0, sizeof(SYSTEMTIME) );
		memset( &st2, 0, sizeof(SYSTEMTIME) );
		st1.wYear  = 2004;
		st1.wMonth =   11;
		st1.wDay   =    8;
		st2.wYear  = 2005;
		st2.wMonth =    1;
		st2.wDay   =    1;

		switch( GetDateLimit( &st1, &st2 ) ){
		case -1:
			MessageBox( NULL, "date -1", g_AppName, MB_OK );
			ReleaseMutex( hMutex );
			return 0;
		case  0:break;
		case  1:
			MessageBox( NULL, "使用期限を過ぎています", g_AppName, MB_OK );
			ReleaseMutex( hMutex );
			return 0;
		}
	}
*/

	//汎用のパスを取得
	GetModuleFileName( NULL, gModulePath, MAX_PATH );
	PathRemoveFileSpec( gModulePath );
	strcpy( gDataPath, gModulePath );
	strcat( gDataPath, "\\data" );



	// コンフィグをロード
	CONFIGDATA conf;
	if( !LoadConfigData( &conf ) ){
		DefaultConfigData( &conf );
	}

	// キーボード操作設定 =====================
	// ジャンプ・ショット
	switch( conf.attack_button_mode ){
	case ATTACKBUTTONMODE_ZJUMP:
		gKeyJump = KEY_Z;
		gKeyShot = KEY_X;
		break;
	case ATTACKBUTTONMODE_XJUMP:
		gKeyJump = KEY_X;
		gKeyShot = KEY_Z;
		break;
	}
	// OK / キャンセル
	switch( conf.ok_button_mode ){
	case OKBUTTONMODE_JUMPOK:
		gKeyOk     = gKeyJump;
		gKeyCancel = gKeyShot;
		break;
	case OKBUTTONMODE_SHOTOK:
		gKeyOk     = gKeyShot;
		gKeyCancel = gKeyJump;
		break;
	}
	if( IsKeyFile( "s_reverse" ) ){
		gKeyArms    = KEY_A;
		gKeyArmsRev = KEY_S;
	}

	switch( conf.move_button_mode ){
	case MOVEBUTTONMODE_1:
		gKeyLeft  = KEY_LEFT;
		gKeyUp    = KEY_UP;
		gKeyRight = KEY_RIGHT;
		gKeyDown  = KEY_DOWN;
		break;
	case MOVEBUTTONMODE_2:
		gKeyLeft  = KEY_NE;
		gKeyUp    = (KEY_RI|KEY_RE);
		gKeyRight = KEY_ME;
		gKeyDown  = KEY_RU;
		break;
	}

		long i;

	for( i = 0; i < MAX_JOYSTICKBUTTON_USE; i++ ){
		switch( conf.joystick_button[i] ){
		case BUTTONACTION_JUMP:    gJoystickButtonTable[i] = gKeyJump;    break;
		case BUTTONACTION_SHOT:    gJoystickButtonTable[i] = gKeyShot;    break;
		case BUTTONACTION_ARMS:    gJoystickButtonTable[i] = gKeyArms;    break;
		case BUTTONACTION_ARMSREV: gJoystickButtonTable[i] = gKeyArmsRev; break;
		case BUTTONACTION_ITEM:    gJoystickButtonTable[i] = gKeyItem;    break;
		case BUTTONACTION_MAP:     gJoystickButtonTable[i] = gKeyMap;     break;
		}
	}


	WNDCLASSEX wc;
	HWND hWnd;
	RECT rect = {0, 0, SURFACE_WIDTH, SURFACE_HEIGHT};//for clear surface

	
	// ウインドウクラスの生成
	memset( &wc, 0, sizeof(WNDCLASSEX) );
	wc.cbSize        = sizeof(WNDCLASSEX);//WNDCLASSEXのサイズを設定
	wc.lpfnWndProc   = MainProc;
	wc.hInstance     = hInst;
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wc.lpszClassName = g_AppName;
	wc.hCursor       = LoadCursor( hInst, "CURSOR_NORMAL" );
	wc.hIcon         = LoadIcon( hInst, "0" );//アイコン（大）
	wc.hIconSm       = LoadIcon( hInst, "ICON_MINI" );//アイコン（小）

	
	//広さを計算
	long window_width;
	long window_height;
	long window_x;
	long window_y;

	// ウインドウモード 
	switch( conf.display_mode ){

	case WINDOWMODE_320x240:
	case WINDOWMODE_640x480:
		wc.lpszMenuName  = "MENU_MAIN";
		//ウインドウクラスを登録
		if( !RegisterClassEx( &wc ) ){
			ReleaseMutex( hMutex );
			return 0;
		}

		if( conf.display_mode == WINDOWMODE_320x240 ){
			gClientWidth  = 320;
			gClientHeight = 240;
		}else{
			gClientWidth  = 640;
			gClientHeight = 480;
		}

		window_width = 
			GetSystemMetrics(SM_CXFIXEDFRAME)*2+//フレームの幅
			gClientWidth+2;
		window_height = 
			GetSystemMetrics(SM_CYFIXEDFRAME)*2 +//フレームの高さ
			GetSystemMetrics(SM_CYCAPTION)+//キャプションの高さ
			GetSystemMetrics(SM_CYMENU)+//メニューバーの高さ
			gClientHeight+2;

		window_x = ( GetSystemMetrics(SM_CXSCREEN) - window_width  )/ 2;
		window_y = ( GetSystemMetrics(SM_CYSCREEN) - window_height )/ 2;

		SetClientOffset( GetSystemMetrics( SM_CXFIXEDFRAME )  +1,
						 GetSystemMetrics( SM_CYFIXEDFRAME )  +//フレームの高さ
						 GetSystemMetrics( SM_CYCAPTION )+//キャプションの高さ
						 GetSystemMetrics( SM_CYMENU )   +1 );
		// ウインドウの生成
		ghWnd = hWnd = CreateWindow( g_AppName, g_AppName,
							WS_CAPTION|WS_VISIBLE|WS_SYSMENU|WS_MINIMIZEBOX,
							window_x, window_y, 
							window_width, window_height,
							NULL, NULL, hInst, NULL );


		if( !hWnd ){
			ReleaseMutex( hMutex );
			return 0;
		}
		HMENU hMenu;
		hMenu = GetMenu( hWnd );
		if( conf.display_mode == WINDOWMODE_320x240 ){
			StartDirectDraw( hWnd, 0, 0 );
		}else{
			StartDirectDraw( hWnd, 1, 0 );
		}


		break;

	case WINDOWMODE_FULL16BIT:
	case WINDOWMODE_FULL24BIT:
	case WINDOWMODE_FULL32BIT:
		//ウインドウクラスを登録
		if( !RegisterClassEx( &wc ) ){
			ReleaseMutex( hMutex );
			return 0;
		}

		gClientWidth  = 640;
		gClientHeight = 480;

		SetClientOffset( 0,0 );
		// ウインドウの生成
		ghWnd = hWnd = CreateWindow( g_AppName, g_AppName,
							WS_POPUP|WS_VISIBLE|WS_SYSMENU,
							0, 0, 
							GetSystemMetrics(SM_CXSCREEN),
							GetSystemMetrics(SM_CYSCREEN),
							NULL, NULL, hInst, NULL );
		if( !hWnd ){
			ReleaseMutex( hMutex );
			return 0;
		}
		long bit;
		switch( conf.display_mode ){
		case WINDOWMODE_FULL16BIT: bit = 16; break;
		case WINDOWMODE_FULL24BIT: bit = 24; break;
		case WINDOWMODE_FULL32BIT: bit = 32; break;
		}
		StartDirectDraw( hWnd, 2, bit );
		gbFullScreen = TRUE;

		// カーソルを消す
		ShowCursor( FALSE );

		break;
	}




	// バッファをクリア
	{// LOADING..表示
		RECT rcLoading = {0,0,64,8};
		RECT rcFull    = {0,0, 0, 0};
		long b;
		rcFull.right = SURFACE_WIDTH;
		rcFull.bottom = SURFACE_HEIGHT;
		b = MakeSurface_File( "Loading", SURF_LOADING);
		CortBox( &rcFull, 0 );
		PutBitmap3( &rcFull, SURFACE_WIDTH/2-32, SURFACE_HEIGHT/2-4, &rcLoading, SURF_LOADING ); 
		if( !Flip_SystemTask( ghWnd ) ){
			ReleaseMutex( hMutex );
			return 1;//強制終了
		}
	}
	
	InitDirectSound( hWnd );
	if( conf.bJoystick && InitDirectInput( hInst, hWnd ) ){
		ResetJoystickStatus();
		gbUseJoystick = TRUE;
	}

	//テキストの表示
	InitTextObject( conf.font_name );

	//三角関数
	InitTriangleTable();

	//◆◆◆◆◆
	Game( hWnd );
	//◆◆◆◆◆

	//終了時に削除
	EndTextObject();
	EndDirectSound();
    EndDirectDraw( hWnd );


	ReleaseMutex( hMutex );
	return 1;
}

void PlayOrganyaMusic( void );
void StopOrganyaMusic( void );
void ResetNoise( void );
void SleepNoise( void );

void InactiveWindow( void )
{
	if( _bActive ){
		_bActive = FALSE;
		StopOrganyaMusic();
		SleepNoise();
	}
	PlaySoundObject( WAVE_DUMMY, 0 );
}

void ActiveWindow( void )
{
	if( !_bActive ){
		_bActive = TRUE;

		StopOrganyaMusic();
		PlayOrganyaMusic();
		ResetNoise();

		// サーフェースの復帰
	}
	PlaySoundObject( WAVE_DUMMY, -1 );
}


static void JoystickProc( void )
{
	long i;
	DIRECTINPUTSTATUS status;
	if( !GetJoystickStatus( &status ) )
		return;

	gKey &= KEY_ESC|KEY_F1|KEY_F2;
	if( status.bLeft  ) gKey |=  gKeyLeft;
	else                gKey &= ~gKeyLeft;
	if( status.bRight ) gKey |=  gKeyRight;
	else                gKey &= ~gKeyRight;
	if( status.bUp    ) gKey |=  gKeyUp;
	else                gKey &= ~gKeyUp;
	if( status.bDown  ) gKey |=  gKeyDown;
	else                gKey &= ~gKeyDown;

	for( i = 0; i < MAX_JOYSTICKBUTTON_USE; i++ ){
		gKey &= ~(gJoystickButtonTable[i]);
	}

	for( i = 0; i < MAX_JOYSTICKBUTTON_USE; i++ ){
		if( status.bButton[i] ) gKey |=   gJoystickButtonTable[i];
	}

}





// ファイルドロップ(アイコン)
BOOL Function_WM_DROPFILES( HWND hWnd, WPARAM wParam )
{
	char path[MAX_PATH];
	HDROP hDrop;
	hDrop = (HDROP)wParam;


	if( DragQueryFile( hDrop, -1, NULL, 0 ) != 0 ){
		DragQueryFile( hDrop,  0, path, MAX_PATH );

		if( !LoadProfile( path ) ){

		}
	}
	DragFinish( hDrop );

	return TRUE;
}



HRESULT CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

   	switch(msg){
	case WM_CREATE:
#ifdef NDEBUG
		{
			HMENU hMenu;
			hMenu = GetMenu( hWnd );
			DeleteMenu( hMenu, IDM_SAVE, MF_BYCOMMAND );		
			DrawMenuBar( hWnd );
		}
#endif
		{
			HMENU hMenu;
			hMenu = GetMenu( hWnd );
			if( !IsKeyFile( "mute" ) ) DeleteMenu( hMenu, IDM_MUTE, MF_BYCOMMAND );
			DrawMenuBar( hWnd );
			if(  IsKeyFile( "fps"  ) ) _bFPS = TRUE;
		}
		if( !gbFullScreen ) LoadWindowRect( hWnd, "window.rect", FALSE );
		_SetMainWindowText( hWnd );
		break;

	// 省電力モードの無効 / Alt のメニュー呼び出しを解除
	case WM_SYSCOMMAND:
		switch( wParam ){
		case SC_MONITORPOWER: break;
		case SC_KEYMENU:      break;// alt
		case SC_SCREENSAVE:   break;
		default: DefWindowProc( hWnd, msg, wParam, lParam ); break;
		}
		break;

	// IMEの解除
	case WM_IME_NOTIFY:
		{
			if( wParam == IMN_SETOPENSTATUS ){
				HIMC hImc = ImmGetContext( hWnd );
				ImmSetOpenStatus( hImc, FALSE );
				ImmReleaseContext( hWnd, hImc );
			}
		}
		break;

	case WM_KEYDOWN:
		switch( wParam ){
		case VK_ESCAPE	: gKey |= KEY_ESC; break;
//			PostMessage(hWnd, WM_DESTROY, 0, 0L); break;// 強制終了用

		case 'W'        : gKey |= KEY_W;     break;
		case VK_LEFT	: gKey |= KEY_LEFT;  break;
		case VK_RIGHT	: gKey |= KEY_RIGHT; break;
		case VK_UP		: gKey |= KEY_UP;    break;
		case VK_DOWN	: gKey |= KEY_DOWN;  break;
		case 0x58		: gKey |= KEY_X;     break;
		case 0x5a		: gKey |= KEY_Z;     break;
		case 0x53		: gKey |= KEY_S;     break;
		case 0x41		: gKey |= KEY_A;     break;
		case 0x10		: gKey |= KEY_SHIFT; break;
		case 0x70		: gKey |= KEY_F1;    break;
		case 0x71		: gKey |= KEY_F2;    break;
		case 0x51		: gKey |= KEY_Q;     break;

		case 0xbc		: gKey |= KEY_NE;    break;
		case 0xbe		: gKey |= KEY_RU;    break;
		case 0xbf		: gKey |= KEY_ME;    break;
		case 0x4c		: gKey |= KEY_RI;    break;
		case 0xbb		: gKey |= KEY_RE;    break;

		case VK_F5:     gbUseJoystick = FALSE; break;// キーボード操作へ
		}
		break;
	case WM_KEYUP:

		switch(wParam){
		case VK_ESCAPE	: gKey &= ~KEY_ESC;   break;
		case 'W'        : gKey &= ~KEY_W;     break;
		case VK_LEFT    : gKey &= ~KEY_LEFT;  break;
		case VK_RIGHT   : gKey &= ~KEY_RIGHT; break;
		case VK_UP      : gKey &= ~KEY_UP;    break;
		case VK_DOWN    : gKey &= ~KEY_DOWN;  break;
		case 0x58		: gKey &= ~KEY_X;     break;
		case 0x5a		: gKey &= ~KEY_Z;     break;
		case 0x53		: gKey &= ~KEY_S;     break;
		case 0x41		: gKey &= ~KEY_A;     break;
		case 0x10		: gKey &= ~KEY_SHIFT; break;
		case 0x70		: gKey &= ~KEY_F1;    break;
		case 0x71		: gKey &= ~KEY_F2;    break;
		case 0x51		: gKey &= ~KEY_Q;     break;

		case 0xbc		: gKey &= ~KEY_NE;    break;
		case 0xbe		: gKey &= ~KEY_RU;    break;
		case 0xbf		: gKey &= ~KEY_ME;    break;
		case 0x4c		: gKey &= ~KEY_RI;    break;
		case 0xbb		: gKey &= ~KEY_RE;    break;
		}
		break;

		//caicle

	//メニューコマンド	
	case WM_COMMAND:
		switch( LOWORD( wParam ) ){
		case IDM_EXIT:
			if( IDOK == DialogBoxParam( ghInst, "DLG_YESNO", hWnd, DialogProc_YesNo, (LPARAM)"終了？" ) )
				PostMessage(hWnd, WM_CLOSE, 0, 0L);
			break;

//		case IDA_MINIMIZE:
//			ShowWindow( hWnd, SW_MINIMIZE );
//			break;

		case IDM_VERSION:
			DialogBox( ghInst, "DLG_ABOUT", hWnd, DialogProc_About );
					

/*			{
				char str[64];
				long y, m, d;
				long v1,v2,v3,v4;
				GetCompileDate( &y, &m, &d );
				GetCompileVersion( &v1, &v2, &v3, &v4 );
				sprintf( str, gStrVersion, v1, v2, v3, v4, y, m, d );
				MessageBox( hWnd, str, g_AppName, MB_OK );
			}
*/			break;
		case IDM_SOUND:
			if( !OpenSoundVolume( hWnd ) ){
				MessageBox( hWnd, "ボリューム設定を起動できませんでした", g_AppName, MB_OK );
			}
			break;
		case IDM_SAVE:
			DialogBox( ghInst, "DLG_SAVE", hWnd, DialogProc_Save );
			break;
		case IDM_MUTE:
			DialogBox( ghInst, "DLG_MUTE", hWnd, DialogProc_Mute );
			break;

		}
		break;

	case WM_DROPFILES:
		Function_WM_DROPFILES( hWnd, wParam );
		break;

	case WM_ACTIVATE:
#ifdef _DEBUG
//		ActiveWindow();
//		break;
#endif
		{
			BOOL bActive;
			switch( LOWORD(wParam) ){
			case 0:
				bActive = FALSE;
				break;
			case 1:
			case 2:
				// 最小化確認
				if( HIWORD(wParam) ) bActive = FALSE;
				else				 bActive = TRUE;
				break;
			}
			if( bActive ) ActiveWindow();
			else          InactiveWindow();
		}
		break;


/*	//サイズ変更
	case WM_SIZE:
		switch( wParam ){
//		case SIZE_MAXIMIZED://最大化
		case SIZE_MINIMIZED://最小化
			InactiveWindow();
			break;
		case SIZE_RESTORED://それ以外
			ActiveWindow();
			break;
		}
		break;
*/
	case WM_CLOSE://Alt + F4 v.0.0.0.6
		StopOrganyaMusic();
		PostQuitMessage(0);
		break;

	default://上記スイッチ記述のないものはWindowsに処理を委ねる
		return DefWindowProc( hWnd, msg, wParam, lParam );
    }


	return TRUE;
}

//システムタスク
BOOL SystemTask(void)
{
	MSG msg;


	//メッセージループを生成
	while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE) || !_bActive ){
		if( !GetMessage( &msg, NULL, 0, 0 ) ) return(FALSE); 
		TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}   
	if( gbUseJoystick ) JoystickProc();
	return(TRUE);
}


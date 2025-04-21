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

char *g_AppName = "���A����";


HWND ghWnd = NULL;



HINSTANCE ghInst;

// �R���t�B�O
BOOL gbUseJoystick = FALSE;
BOOL gbFullScreen  = FALSE;
static BOOL _bFPS  = FALSE;

static long gJoystickButtonTable[MAX_JOYSTICKBUTTON_USE];


static long gClientWidth; 
static long gClientHeight;
static BOOL _bActive = FALSE; //�E�C���h�E�A�N�e�B�u���̂݃Q�[������

char   gModulePath[MAX_PATH];
char   gDataPath[MAX_PATH];

HRESULT CALLBACK MainProc(         HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL    CALLBACK DialogProc_Save(  HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL    CALLBACK DialogProc_YesNo( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL    CALLBACK DialogProc_Mute(  HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL Game( HWND hWnd );
void InitTriangleTable( void );
void PutNumber4( long x, long y, long value, BOOL bZero );

// �ėp
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

	//���d�N���̖h�~---------------------------------------
	hPrevMutex = OpenMutex( MUTEX_ALL_ACCESS, FALSE, mutex_name );
	if( hPrevMutex ){
		CloseHandle( hPrevMutex );
		return 0;
	}
	hMutex = CreateMutex( FALSE, 0, mutex_name );

	ghInst = hInst;

/*	{// �g�p�����m�F
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
			MessageBox( NULL, "�g�p�������߂��Ă��܂�", g_AppName, MB_OK );
			ReleaseMutex( hMutex );
			return 0;
		}
	}
*/

	//�ėp�̃p�X���擾
	GetModuleFileName( NULL, gModulePath, MAX_PATH );
	PathRemoveFileSpec( gModulePath );
	strcpy( gDataPath, gModulePath );
	strcat( gDataPath, "\\data" );



	// �R���t�B�O�����[�h
	CONFIGDATA conf;
	if( !LoadConfigData( &conf ) ){
		DefaultConfigData( &conf );
	}

	// �L�[�{�[�h����ݒ� =====================
	// �W�����v�E�V���b�g
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
	// OK / �L�����Z��
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

	
	// �E�C���h�E�N���X�̐���
	memset( &wc, 0, sizeof(WNDCLASSEX) );
	wc.cbSize        = sizeof(WNDCLASSEX);//WNDCLASSEX�̃T�C�Y��ݒ�
	wc.lpfnWndProc   = MainProc;
	wc.hInstance     = hInst;
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wc.lpszClassName = g_AppName;
	wc.hCursor       = LoadCursor( hInst, "CURSOR_NORMAL" );
	wc.hIcon         = LoadIcon( hInst, "0" );//�A�C�R���i��j
	wc.hIconSm       = LoadIcon( hInst, "ICON_MINI" );//�A�C�R���i���j

	
	//�L�����v�Z
	long window_width;
	long window_height;
	long window_x;
	long window_y;

	// �E�C���h�E���[�h 
	switch( conf.display_mode ){

	case WINDOWMODE_320x240:
	case WINDOWMODE_640x480:
		wc.lpszMenuName  = "MENU_MAIN";
		//�E�C���h�E�N���X��o�^
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
			GetSystemMetrics(SM_CXFIXEDFRAME)*2+//�t���[���̕�
			gClientWidth+2;
		window_height = 
			GetSystemMetrics(SM_CYFIXEDFRAME)*2 +//�t���[���̍���
			GetSystemMetrics(SM_CYCAPTION)+//�L���v�V�����̍���
			GetSystemMetrics(SM_CYMENU)+//���j���[�o�[�̍���
			gClientHeight+2;

		window_x = ( GetSystemMetrics(SM_CXSCREEN) - window_width  )/ 2;
		window_y = ( GetSystemMetrics(SM_CYSCREEN) - window_height )/ 2;

		SetClientOffset( GetSystemMetrics( SM_CXFIXEDFRAME )  +1,
						 GetSystemMetrics( SM_CYFIXEDFRAME )  +//�t���[���̍���
						 GetSystemMetrics( SM_CYCAPTION )+//�L���v�V�����̍���
						 GetSystemMetrics( SM_CYMENU )   +1 );
		// �E�C���h�E�̐���
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
		//�E�C���h�E�N���X��o�^
		if( !RegisterClassEx( &wc ) ){
			ReleaseMutex( hMutex );
			return 0;
		}

		gClientWidth  = 640;
		gClientHeight = 480;

		SetClientOffset( 0,0 );
		// �E�C���h�E�̐���
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

		// �J�[�\��������
		ShowCursor( FALSE );

		break;
	}




	// �o�b�t�@���N���A
	{// LOADING..�\��
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
			return 1;//�����I��
		}
	}
	
	InitDirectSound( hWnd );
	if( conf.bJoystick && InitDirectInput( hInst, hWnd ) ){
		ResetJoystickStatus();
		gbUseJoystick = TRUE;
	}

	//�e�L�X�g�̕\��
	InitTextObject( conf.font_name );

	//�O�p�֐�
	InitTriangleTable();

	//����������
	Game( hWnd );
	//����������

	//�I�����ɍ폜
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

		// �T�[�t�F�[�X�̕��A
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





// �t�@�C���h���b�v(�A�C�R��)
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

	// �ȓd�̓��[�h�̖��� / Alt �̃��j���[�Ăяo��������
	case WM_SYSCOMMAND:
		switch( wParam ){
		case SC_MONITORPOWER: break;
		case SC_KEYMENU:      break;// alt
		case SC_SCREENSAVE:   break;
		default: DefWindowProc( hWnd, msg, wParam, lParam ); break;
		}
		break;

	// IME�̉���
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
//			PostMessage(hWnd, WM_DESTROY, 0, 0L); break;// �����I���p

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

		case VK_F5:     gbUseJoystick = FALSE; break;// �L�[�{�[�h�����
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

	//���j���[�R�}���h	
	case WM_COMMAND:
		switch( LOWORD( wParam ) ){
		case IDM_EXIT:
			if( IDOK == DialogBoxParam( ghInst, "DLG_YESNO", hWnd, DialogProc_YesNo, (LPARAM)"�I���H" ) )
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
				MessageBox( hWnd, "�{�����[���ݒ���N���ł��܂���ł���", g_AppName, MB_OK );
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
				// �ŏ����m�F
				if( HIWORD(wParam) ) bActive = FALSE;
				else				 bActive = TRUE;
				break;
			}
			if( bActive ) ActiveWindow();
			else          InactiveWindow();
		}
		break;


/*	//�T�C�Y�ύX
	case WM_SIZE:
		switch( wParam ){
//		case SIZE_MAXIMIZED://�ő剻
		case SIZE_MINIMIZED://�ŏ���
			InactiveWindow();
			break;
		case SIZE_RESTORED://����ȊO
			ActiveWindow();
			break;
		}
		break;
*/
	case WM_CLOSE://Alt + F4 v.0.0.0.6
		StopOrganyaMusic();
		PostQuitMessage(0);
		break;

	default://��L�X�C�b�`�L�q�̂Ȃ����̂�Windows�ɏ������ς˂�
		return DefWindowProc( hWnd, msg, wParam, lParam );
    }


	return TRUE;
}

//�V�X�e���^�X�N
BOOL SystemTask(void)
{
	MSG msg;


	//���b�Z�[�W���[�v�𐶐�
	while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE) || !_bActive ){
		if( !GetMessage( &msg, NULL, 0, 0 ) ) return(FALSE); 
		TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}   
	if( gbUseJoystick ) JoystickProc();
	return(TRUE);
}


#include <windows.h>
#include <stdio.h>

#include "DirectX5\DxINPUT.H"
#include "Input.h"

LPDIRECTINPUT			gDirectInput = NULL;
LPDIRECTINPUTDEVICE2    gJoystick    = NULL;


// ジョイスティック検索用
typedef struct {
	LPDIRECTINPUT			pDirectInput;					// DirectInputオブジェクトのポインタ
	LPDIRECTINPUTDEVICE2	pJoystick;
} DIRECTINPUTDEVICEINFO;

static _neutral_x = 0;
static _neutral_y = 0;


// ジョイスティックの列挙
static BOOL CALLBACK DIEnumCallback( LPCDIDEVICEINSTANCE lpDDI, LPVOID lpContext )
{
	static LPDIRECTINPUTDEVICE	pJoystick ;
	static LPDIRECTINPUTDEVICE2	pJoystick2 ;
	HRESULT					    dirval ;

	static DIRECTINPUTDEVICEINFO* pDevInfo = (DIRECTINPUTDEVICEINFO*)lpContext ;

	// 一時的にデバイスを生成
	if( pDevInfo->pDirectInput->CreateDevice( lpDDI->guidInstance, &pJoystick, NULL ) != DI_OK )
	{
		pDevInfo->pJoystick = NULL ;
		return DIENUM_CONTINUE ;
	}

	// DIRECTINPUTDEVICE ではジョイスティックを使えないので、
	// DIRECTINPUTDEVICE2 を使う
	dirval = pJoystick->QueryInterface( IID_IDirectInputDevice2, (void **) &pJoystick2 ) ;
	
	// COM では QueryInterfaceに対して直接比較を行わず、FAILEDマクロを使うようにする
	if( FAILED( dirval ) ){
		gJoystick = NULL ;
		return DIENUM_CONTINUE ;
	}

	if( pJoystick ){
		pJoystick->Release();
		pJoystick = 0;
	}

	pDevInfo->pJoystick = pJoystick2 ;

	char debug[256] ;
	sprintf( debug, "DeviceGUID = %x\n", lpDDI->guidInstance ) ;
	OutputDebugString( debug ) ;

	return DIENUM_STOP ;
}

// 開放
void ReleaseDirectInput( void )
{
	if( gJoystick ){
		gJoystick->Release();
		gJoystick = NULL;
	}
	if( gDirectInput ){
		gDirectInput->Release();
		gDirectInput = NULL;
	}
}

// アクティブ状態
BOOL ActivateDirectInput( BOOL bActive )
{
	if( bActive == TRUE ){
		if( gJoystick ) gJoystick->Acquire();
	}else{
		if( gJoystick ) gJoystick->Unacquire();
	}
	return TRUE ;
}



// ジョイスティックデバイスの初期化
static BOOL initDeviceJoystick( HWND hWnd )
{
	DIRECTINPUTDEVICEINFO devInfo;

	devInfo.pJoystick    = NULL ;
	devInfo.pDirectInput = gDirectInput;

	devInfo.pDirectInput->AddRef();

	gDirectInput->EnumDevices( DIDEVTYPE_JOYSTICK, DIEnumCallback, &devInfo, DIEDFL_ATTACHEDONLY ) ;// Old?
//	gDirectInput->EnumDevices( DI8DEVCLASS_GAMECTRL, DIEnumCallback, &devInfo, DIEDFL_ATTACHEDONLY ) ;
	if( devInfo.pDirectInput ){
		devInfo.pDirectInput->Release();
		devInfo.pDirectInput = NULL;
	}
	if( devInfo.pJoystick == NULL ) return FALSE ;

	// 選択されたデバイスを設定
	gJoystick = devInfo.pJoystick ;

	// データフォーマット設定
	if( DI_OK != gJoystick->SetDataFormat( &c_dfDIJoystick ) ) return FALSE;
	if( DI_OK != gJoystick->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND ) ) return FALSE;

	// アクセス権の取得
	gJoystick->Acquire() ;

	return TRUE ;
}



// DirectInputの初期化
BOOL InitDirectInput( HINSTANCE hInst, HWND hWnd )
{

	// DirectInputオブジェクトを生成
	if( DI_OK != DirectInputCreate( hInst, DIRECTINPUT_VERSION, &gDirectInput, NULL ) ) return FALSE;
//	if( DI_OK != DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&gDirectInput, NULL ) ) return FALSE;

	// 各デバイスの初期化
	if( initDeviceJoystick( hWnd ) == FALSE ) return FALSE;
	
	return TRUE ;
}

BOOL GetJoystickStatus( DIRECTINPUTSTATUS *pStatus )
{
	DIJOYSTATE		state ;
	HRESULT			res ;

	if( gJoystick == NULL ) return FALSE ;

	// このメソッドを呼び出すことでデバイスのデータが更新される
	if( gJoystick->Poll() != DI_OK ) return FALSE ;

	// 状態の取得
	res = gJoystick->GetDeviceState( sizeof(DIJOYSTATE), &state );
	if( res != DI_OK ){
		if( res == DIERR_INPUTLOST ) ActivateDirectInput( FALSE );
		else  return FALSE ;
	}

	// ボタン
	for( long b = 0; b < MAX_JOYSTICKBUTTON; b++ ){
		if( state.rgbButtons[b] & 0x80 ) pStatus->bButton[b] = TRUE;
		else                             pStatus->bButton[b] = FALSE;
	}

	pStatus->bLeft = pStatus->bUp = pStatus->bRight = pStatus->bDown = FALSE;

	if(      state.lX < _neutral_x - 10000 ) pStatus->bLeft  = TRUE;
	else if( state.lX > _neutral_x + 10000 ) pStatus->bRight = TRUE;
	if(      state.lY < _neutral_y - 10000 ) pStatus->bUp    = TRUE;
	else if( state.lY > _neutral_y + 10000 ) pStatus->bDown  = TRUE;

/*	switch( state.lX ){
	case     0: pStatus->bLeft  = TRUE; break;
	case 65535: pStatus->bRight = TRUE; break;
	}
	switch( state.lY ){
	case     0: pStatus->bUp    = TRUE; break;
	case 65535: pStatus->bDown  = TRUE; break;
	}
*/

	return TRUE ;
}

BOOL ResetJoystickStatus( void )
{
	DIJOYSTATE		state ;
	HRESULT			res ;

	if( gJoystick == NULL ) return FALSE ;

	// このメソッドを呼び出すことでデバイスのデータが更新される
	if( gJoystick->Poll() != DI_OK ) return FALSE ;

	// 状態の取得
	res = gJoystick->GetDeviceState( sizeof(DIJOYSTATE), &state );
	if( res != DI_OK ){
		if( res == DIERR_INPUTLOST ) ActivateDirectInput( FALSE );
		else  return FALSE ;
	}

	_neutral_x = state.lX;
	_neutral_y = state.lY;


	return TRUE ;
}

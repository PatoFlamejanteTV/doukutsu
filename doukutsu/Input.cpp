#include <windows.h>
#include <stdio.h>

#include "DirectX5\DxINPUT.H"
#include "Input.h"

LPDIRECTINPUT			gDirectInput = NULL;
LPDIRECTINPUTDEVICE2    gJoystick    = NULL;


// �W���C�X�e�B�b�N�����p
typedef struct {
	LPDIRECTINPUT			pDirectInput;					// DirectInput�I�u�W�F�N�g�̃|�C���^
	LPDIRECTINPUTDEVICE2	pJoystick;
} DIRECTINPUTDEVICEINFO;

static _neutral_x = 0;
static _neutral_y = 0;


// �W���C�X�e�B�b�N�̗�
static BOOL CALLBACK DIEnumCallback( LPCDIDEVICEINSTANCE lpDDI, LPVOID lpContext )
{
	static LPDIRECTINPUTDEVICE	pJoystick ;
	static LPDIRECTINPUTDEVICE2	pJoystick2 ;
	HRESULT					    dirval ;

	static DIRECTINPUTDEVICEINFO* pDevInfo = (DIRECTINPUTDEVICEINFO*)lpContext ;

	// �ꎞ�I�Ƀf�o�C�X�𐶐�
	if( pDevInfo->pDirectInput->CreateDevice( lpDDI->guidInstance, &pJoystick, NULL ) != DI_OK )
	{
		pDevInfo->pJoystick = NULL ;
		return DIENUM_CONTINUE ;
	}

	// DIRECTINPUTDEVICE �ł̓W���C�X�e�B�b�N���g���Ȃ��̂ŁA
	// DIRECTINPUTDEVICE2 ���g��
	dirval = pJoystick->QueryInterface( IID_IDirectInputDevice2, (void **) &pJoystick2 ) ;
	
	// COM �ł� QueryInterface�ɑ΂��Ē��ڔ�r���s�킸�AFAILED�}�N�����g���悤�ɂ���
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

// �J��
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

// �A�N�e�B�u���
BOOL ActivateDirectInput( BOOL bActive )
{
	if( bActive == TRUE ){
		if( gJoystick ) gJoystick->Acquire();
	}else{
		if( gJoystick ) gJoystick->Unacquire();
	}
	return TRUE ;
}



// �W���C�X�e�B�b�N�f�o�C�X�̏�����
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

	// �I�����ꂽ�f�o�C�X��ݒ�
	gJoystick = devInfo.pJoystick ;

	// �f�[�^�t�H�[�}�b�g�ݒ�
	if( DI_OK != gJoystick->SetDataFormat( &c_dfDIJoystick ) ) return FALSE;
	if( DI_OK != gJoystick->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND ) ) return FALSE;

	// �A�N�Z�X���̎擾
	gJoystick->Acquire() ;

	return TRUE ;
}



// DirectInput�̏�����
BOOL InitDirectInput( HINSTANCE hInst, HWND hWnd )
{

	// DirectInput�I�u�W�F�N�g�𐶐�
	if( DI_OK != DirectInputCreate( hInst, DIRECTINPUT_VERSION, &gDirectInput, NULL ) ) return FALSE;
//	if( DI_OK != DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&gDirectInput, NULL ) ) return FALSE;

	// �e�f�o�C�X�̏�����
	if( initDeviceJoystick( hWnd ) == FALSE ) return FALSE;
	
	return TRUE ;
}

BOOL GetJoystickStatus( DIRECTINPUTSTATUS *pStatus )
{
	DIJOYSTATE		state ;
	HRESULT			res ;

	if( gJoystick == NULL ) return FALSE ;

	// ���̃��\�b�h���Ăяo�����ƂŃf�o�C�X�̃f�[�^���X�V�����
	if( gJoystick->Poll() != DI_OK ) return FALSE ;

	// ��Ԃ̎擾
	res = gJoystick->GetDeviceState( sizeof(DIJOYSTATE), &state );
	if( res != DI_OK ){
		if( res == DIERR_INPUTLOST ) ActivateDirectInput( FALSE );
		else  return FALSE ;
	}

	// �{�^��
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

	// ���̃��\�b�h���Ăяo�����ƂŃf�o�C�X�̃f�[�^���X�V�����
	if( gJoystick->Poll() != DI_OK ) return FALSE ;

	// ��Ԃ̎擾
	res = gJoystick->GetDeviceState( sizeof(DIJOYSTATE), &state );
	if( res != DI_OK ){
		if( res == DIERR_INPUTLOST ) ActivateDirectInput( FALSE );
		else  return FALSE ;
	}

	_neutral_x = state.lX;
	_neutral_y = state.lY;


	return TRUE ;
}

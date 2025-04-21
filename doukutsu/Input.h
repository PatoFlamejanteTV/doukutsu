
BOOL InitDirectInput( HINSTANCE hInst, HWND hWnd );
void ReleaseDirectInput( void );
BOOL ActivateDirectInput( BOOL bActive );

#define MAX_JOYSTICKBUTTON	32

typedef struct{
	BOOL bLeft;
	BOOL bRight;
	BOOL bUp;
	BOOL bDown;
	BOOL bButton[MAX_JOYSTICKBUTTON];
}DIRECTINPUTSTATUS;

BOOL GetJoystickStatus( DIRECTINPUTSTATUS *pStatus );
BOOL ResetJoystickStatus( void );

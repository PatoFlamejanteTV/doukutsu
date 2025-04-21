#include <windows.h>
#include <stdio.h>

#include "Draw.h"
#include "Define.h"
#include "Escape.h"
#include "KeyControl.h"



extern RECT grcFull;
void PutFramePerSecound( void );


// ESC 終了
enum_ESCRETURN Call_Escape( HWND hWnd )
{
	RECT rc = {0,128,208,144};

	//フィールドループ
	while( 1 ){
		GetTrg();
		if( gKeyTrg & KEY_ESC ){ gKeyTrg = 0; return enum_ESCRETURN_exit;     }
		if( gKeyTrg & KEY_F1  ){ gKeyTrg = 0; return enum_ESCRETURN_continue; }
		if( gKeyTrg & KEY_F2  ){ gKeyTrg = 0; return enum_ESCRETURN_restart;  }
		CortBox( &grcFull, 0 );
		PutBitmap3( &grcFull,   320/2-104, 240/2-8, &rc, SURF_TEXTBOX );
		PutFramePerSecound();
		if( !Flip_SystemTask( hWnd ) ){ gKeyTrg = 0; return enum_ESCRETURN_exit; }
	}

	return enum_ESCRETURN_exit;
}

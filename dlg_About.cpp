#include <windows.h>
#include <stdio.h>

#include "resource.h"




void CenteringWindowByParent( HWND hWnd );
char *gStrVersion = "version.%d.%d.%d.%d\r\n2004/12/20 - %04d/%02d/%02d\r\n開発室Pixel";
BOOL GetCompileVersion( long *v1, long *v2, long *v3, long *v4 );
void GetCompileDate( long *year, long *month, long *day );

BOOL CALLBACK DialogProc_About( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

	switch( message ){

	//ダイアログ起動
	case WM_INITDIALOG:

		{
			char str[100];
			long y, m, d;
			long v1,v2,v3,v4;
			GetCompileDate( &y, &m, &d );
			GetCompileVersion( &v1, &v2, &v3, &v4 );
			sprintf( str, gStrVersion, v1, v2, v3, v4, y, m, d );
			SetDlgItemText( hWnd, IDC_VERSION, str );
		}

//		SetDlgItemText( hWnd, IDC_MESSAGE, "beta test" );

		CenteringWindowByParent( hWnd );
//		ChangeDlgItemText_Japanese( hWnd );


		return 1;

	//ボタンクリック
	case WM_COMMAND:

		switch(LOWORD(wParam)){
		//キャンセル
//		case IDCANCEL:
//			EndDialog( hWnd, FALSE );
//			break;
		//OK
		case IDOK:
			EndDialog( hWnd, TRUE );
			break;
		}

		default:return FALSE;
	
	}
	return TRUE;
}
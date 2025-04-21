#include <windows.h>
#include <stdio.h>

#include "resource.h"


void CenteringWindowByParent( HWND hDlg );

BOOL SaveProfile( char *name );

BOOL CALLBACK DialogProc_Save( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

	switch( message ){

	//ダイアログ起動
	case WM_INITDIALOG:

		SetDlgItemText( hWnd, IDC_FILENAME, "000.dat" );
		CenteringWindowByParent( hWnd );

		return 1;

	//ボタンクリック
	case WM_COMMAND:

		switch(LOWORD(wParam)){
		//キャンセル
		case IDCANCEL:
			EndDialog( hWnd, FALSE );
			break;
		//OK
		case IDOK:
			{
				char name[100];
				GetDlgItemText( hWnd, IDC_FILENAME, name, 100 );
				SaveProfile( name );
			}
			EndDialog( hWnd, TRUE );
			break;
		}

		default:return FALSE;
	
	}
	return TRUE;
}
#include <windows.h>
#include <stdio.h>

#include "resource.h"


void CenteringWindowByParent( HWND hDlg );

extern long g_mute[];

BOOL CALLBACK DialogProc_Mute( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

	switch( message ){

	//ダイアログ起動
	case WM_INITDIALOG:

		CenteringWindowByParent( hWnd );
		CheckDlgButton( hWnd, IDC_CHECK0, g_mute[0] ? 1 : 0 );
		CheckDlgButton( hWnd, IDC_CHECK1, g_mute[1] ? 1 : 0 );
		CheckDlgButton( hWnd, IDC_CHECK2, g_mute[2] ? 1 : 0 );
		CheckDlgButton( hWnd, IDC_CHECK3, g_mute[3] ? 1 : 0 );
		CheckDlgButton( hWnd, IDC_CHECK4, g_mute[4] ? 1 : 0 );
		CheckDlgButton( hWnd, IDC_CHECK5, g_mute[5] ? 1 : 0 );
		CheckDlgButton( hWnd, IDC_CHECK6, g_mute[6] ? 1 : 0 );
		CheckDlgButton( hWnd, IDC_CHECK7, g_mute[7] ? 1 : 0 );

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
			g_mute[0] = IsDlgButtonChecked( hWnd, IDC_CHECK0 );
			g_mute[1] = IsDlgButtonChecked( hWnd, IDC_CHECK1 );
			g_mute[2] = IsDlgButtonChecked( hWnd, IDC_CHECK2 );
			g_mute[3] = IsDlgButtonChecked( hWnd, IDC_CHECK3 );
			g_mute[4] = IsDlgButtonChecked( hWnd, IDC_CHECK4 );
			g_mute[5] = IsDlgButtonChecked( hWnd, IDC_CHECK5 );
			g_mute[6] = IsDlgButtonChecked( hWnd, IDC_CHECK6 );
			g_mute[7] = IsDlgButtonChecked( hWnd, IDC_CHECK7 );

			EndDialog( hWnd, TRUE );
			break;
		}

		default:return FALSE;
	
	}
	return TRUE;
}
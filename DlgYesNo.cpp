#include <windows.h>
#include <stdio.h>

#include "resource.h"


void CenteringWindowByParent( HWND hDlg );

BOOL CALLBACK DialogProc_YesNo( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

	switch( message ){
	case WM_INITDIALOG:
		SetDlgItemText( hWnd, IDC_MESSAGE, (char *)lParam );
		CenteringWindowByParent( hWnd );
		return 1;
	case WM_COMMAND:

		switch(LOWORD(wParam)){
		case IDCANCEL: EndDialog( hWnd, IDCANCEL ); break;
		case IDOK:     EndDialog( hWnd, IDOK     ); break;
		}
	default:return FALSE;
	
	}
	return TRUE;
}
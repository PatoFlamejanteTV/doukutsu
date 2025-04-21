// �ėp�֐��Q
#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>


#pragma comment(lib,"version")

extern char gModulePath[];

// �R���p�C�����̎擾
void GetCompileDate( long *year, long *month, long *day )
{
	long i;
	char strMonth[16];

	char *table[] = {
		"XXX","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" 
	};

	sscanf(__DATE__, "%s %d %d", strMonth, day, year );

	for(i = 0; i < 12; i++){
		if( !memcmp( strMonth, table[i], 3 ) )break;
	}

	*month = i;
	return;
}

// ���\�[�X�o�[�W�������擾
BOOL GetCompileVersion( long *v1, long *v2, long *v3, long *v4 )
{
	LPVOID lpData = 0;
	BOOL bRet = FALSE;

	DWORD dummy; 
	DWORD size;

	char file_name[MAX_PATH];

	GetModuleFileName( NULL, file_name, MAX_PATH );

	size = GetFileVersionInfoSize( file_name, &dummy); 
	if (size == 0) goto End;

	lpData = malloc(size); 
	if(lpData == NULL) goto End;
	if(GetFileVersionInfo(file_name, 0, size, lpData) == FALSE)
		goto End;


	VS_FIXEDFILEINFO *info;
	UINT vSize; 

	if(VerQueryValue(lpData, "\\", (LPVOID*)&info, &vSize) == FALSE) 
		goto End;


	*v1 = HIWORD(info->dwFileVersionMS);
	*v2 = LOWORD(info->dwFileVersionMS);
	*v3 = HIWORD(info->dwFileVersionLS);
	*v4 = LOWORD(info->dwFileVersionLS);

	bRet = TRUE;
End:
	if(lpData) free(lpData);

	return bRet;
}

// �T�E���h�{�����[�����J��
BOOL OpenSoundVolume( HWND hWnd )
{
	HINSTANCE hShell1;
	HINSTANCE hShell2;
	long a;
	char path[ MAX_PATH];
	char path1[MAX_PATH];
	char path2[MAX_PATH];

	GetSystemDirectory( path, MAX_PATH );
	sprintf( path1, "%s\\Sndvol32.exe", path );
	a = strlen( path );
	while( path[a] != '\\' ) a--;
	path[a] = '\0';
	sprintf( path2, "%s\\Sndvol32.exe", path );

	hShell1 = ShellExecute( hWnd,"open",path1,NULL,NULL,SW_SHOW);
	hShell2 = ShellExecute( hWnd,"open",path2,NULL,NULL,SW_SHOW);
	if( (int)hShell1 <= 32 && (int)hShell2 <= 32 ) return FALSE;

	return TRUE;
}

void DeleteLog( void ){
	char path[MAX_PATH];
	sprintf( path, "%s\\debug.txt", gModulePath );
	DeleteFile( path );
}


BOOL WriteLog( char *str, long a, long b, long c )
{
//	return TRUE;

	FILE *fp;
	char path[MAX_PATH];

	sprintf( path, "%s\\debug.txt", gModulePath );

	fp = fopen( path, "a+t" );
	if( !fp )return FALSE;

	fprintf( fp, "%s,%d,%d,%d\n", str, a, b, c );
	fclose( fp );
	return TRUE;
}





// ���t�e�X�g
long GetDateLimit( SYSTEMTIME *st_limit1, SYSTEMTIME *st_limit2 )
{
	SYSTEMTIME st_now;
	FILETIME ft_limit, ft_now;

	// ���̎��Ԃ��擾
	GetSystemTime( &st_now );
	SystemTimeToFileTime( &st_now, &ft_now );

	SystemTimeToFileTime( st_limit1, &ft_limit );
	if( CompareFileTime( &ft_limit, &ft_now ) >= 0) return -1;

	SystemTimeToFileTime( st_limit2, &ft_limit );
	if( CompareFileTime( &ft_limit, &ft_now ) <= 0) return  1;

	return 0;
}

// �X�e�[�^�X�t�H���_�ɃL�[�t�@�C�����m�F����
BOOL IsKeyFile( char *name )
{
	char path[MAX_PATH];
	FILE *fp;

	sprintf( path, "%s\\%s", gModulePath, name );
	fp = fopen( path, "rb" );
	if( !fp ) return FALSE;
	fclose( fp );

	return TRUE;
}



BOOL ErrorLog( char *str, long value )
{
	FILE *fp;
	char path[MAX_PATH];

	sprintf( path, "%s\\%s", gModulePath, "error.log" );
	fp = fopen( path, "a+t" );
	if( !fp )return FALSE;
	fprintf( fp, "%s,%d\n", str, value );
	fclose( fp );

	return TRUE;
};


long GetFileSizeLong( char *path )
{
	long   size;
	HANDLE hFile;

	size = 0;
	//�T�C�Y�̊m�F
	hFile = CreateFile( path, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )return -1;
	size = GetFileSize( hFile, NULL );
	CloseHandle( hFile );

	return size;
}

// �V�t�gJIS���m�F
BOOL IsShiftJIS( unsigned char c )
{
	if( c >= 0x81 && c <= 0x9F ) return TRUE;
	if( c >= 0xE0 && c <= 0xEF ) return TRUE;
	
	return FALSE;
}



// �e�E�C���h�E�̐^���ɔz�u
void CenteringWindowByParent( HWND hWnd )
{
	HWND hParent;
	RECT rcParent, rcMe, rcDesk;
	long x, y;

	// �f�X�N�g�b�v�N���C�A���g�̈���擾
	SystemParametersInfo( SPI_GETWORKAREA, 0, &rcDesk, 0 );
	GetWindowRect( hWnd,    &rcMe );

	// �e��RECT���擾
	hParent = GetParent( hWnd );
	if( hParent ) GetWindowRect( hParent, &rcParent );
	else          SystemParametersInfo( SPI_GETWORKAREA, 0, &rcParent, 0 );

	// �ʒu
	x = rcParent.left + ( (rcParent.right  - rcParent.left) - (rcMe.right  - rcMe.left) ) /2;
	y = rcParent.top  + ( (rcParent.bottom - rcParent.top ) - (rcMe.bottom - rcMe.top ) ) /2;

	// �X�N���[�����ɔ[�܂�悤�ړ�
	if( x                          < rcDesk.left   ) x = rcDesk.left;
	if( y                          < rcDesk.top    ) y = rcDesk.top;
	if( x + (rcMe.right-rcMe.left) > rcDesk.right  ) x = rcDesk.right  - (rcMe.right-rcMe.left);
	if( y + (rcMe.bottom-rcMe.top) > rcDesk.bottom ) y = rcDesk.bottom - (rcMe.bottom-rcMe.top);
	
	SetWindowPos( hWnd, NULL, x, y, 0, 0, SWP_NOSIZE );

}

// �E�C���h�ERECT�̃��[�h
// (WM_CREATE�ŌĂԂƃt���X�N���[���֐��Ŏ��s����)
BOOL LoadWindowRect( HWND hWnd, char *name, BOOL bSize )
{
	FILE *fp;
	RECT rect;
	char path[MAX_PATH];

	RECT rcDesk;
	long max_w, max_h, min_w, min_h;
	long showCmd = SW_NORMAL;

	sprintf( path, "%s\\%s", gModulePath, name );
	fp = fopen( path, "rb" );
	if( fp ){

		fread( &rect,    sizeof(RECT), 1, fp );
		fread( &showCmd, sizeof(long), 1, fp );
		fclose( fp );

		// �f�X�N�g�b�v�N���C�A���g�̈���擾
		SystemParametersInfo( SPI_GETWORKAREA, 0, &rcDesk, 0 );
		// �E�C���h�E�ő�/�ŏ��T�C�Y���擾
		max_w = GetSystemMetrics( SM_CXMAXIMIZED );
		max_h = GetSystemMetrics( SM_CYMAXIMIZED );
		min_w = GetSystemMetrics( SM_CXMIN       );
		min_h = GetSystemMetrics( SM_CYMIN       );

		// ��������
		if( rect.right  - rect.left < min_w ) rect.right  = rect.left + min_w;
		if( rect.bottom - rect.top  < min_h ) rect.bottom = rect.top  + min_h;
		// �傫����
		if( rect.right  - rect.left > max_w ) rect.right  = rect.left + max_w;
		if( rect.bottom - rect.top  > max_h ) rect.bottom = rect.top  + max_w;

		// �X�N���[�����ɔ[�܂�p�Ɉړ�
		if( rect.left < rcDesk.left ){
			rect.right  += ( rcDesk.left - rect.left );
			rect.left   += ( rcDesk.left - rect.left );
		}
		if( rect.top  < rcDesk.top ){
			rect.bottom += ( rcDesk.top  - rect.top  );
			rect.top    += ( rcDesk.top  - rect.top  );
		}
		if( rect.right  > rcDesk.right ){
			rect.left   -= rect.right  - rcDesk.right;
			rect.right  -= rect.right  - rcDesk.right;
		}
		if( rect.bottom > rcDesk.bottom ){
			rect.top    -= rect.bottom - rcDesk.bottom;
			rect.bottom -= rect.bottom - rcDesk.bottom;
		}
		// �ړ��̂݁^�T�C�Y���w��
		if( bSize ) MoveWindow(   hWnd, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, FALSE );
		else        SetWindowPos( hWnd, NULL, rect.left, rect.top, 0,0, SWP_NOSIZE );
	}

	if( showCmd == SW_SHOWMAXIMIZED ){
		if( !ShowWindow( hWnd, SW_SHOWMAXIMIZED ) ) return FALSE;
	}else
		ShowWindow( hWnd, SW_NORMAL        );

	return TRUE;
}
 
// �E�C���h�ERECT�̃Z�[�u
BOOL SaveWindowRect( HWND hWnd, char *name )
{
	FILE *fp;
	WINDOWPLACEMENT place;
	char path[MAX_PATH];
	RECT rc;

	// �E�C���h�E�̏�Ԃ��擾����
	if( !GetWindowPlacement( hWnd, &place ) ) return FALSE;
	// �m�[�}���\���̏ꍇ�̓X�N���[�����W���擾
	if( place.showCmd == SW_NORMAL ){
		if( !GetWindowRect( hWnd, &rc ) ) return FALSE;
		place.rcNormalPosition = rc;
	}

	sprintf( path, "%s\\%s", gModulePath, name );
	fp = fopen( path, "wb" );
	if( !fp ) return FALSE;

	fwrite( &place.rcNormalPosition, sizeof(RECT), 1, fp );
	fwrite( &place.showCmd,          sizeof(long), 1, fp );
	fclose( fp );

	return TRUE;
}

static char *_extra_text = "(C)Pixel";

BOOL IsEnableBitmap( char *path )
{

	FILE *fp;
	long len;
	char str[16];

	len = strlen( _extra_text );
	fp  = fopen( path, "rb" );
	if( !fp ) return FALSE;

	fseek( fp, -1 * len, SEEK_END );
	fread( str, sizeof(char), len, fp );
	fclose( fp );

	if( memcmp( str, _extra_text, len ) ) return FALSE;

	return TRUE;
}
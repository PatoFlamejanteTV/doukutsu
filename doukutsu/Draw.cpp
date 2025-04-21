#include <windows.h>
#include <stdio.h>

#include "DirectX5\DxDRAW.H"
#include "Draw.h"

#define MAX_SURFACE		40
#define MAX_IMAGENAME	20

#ifdef _DEBUG
#define FLIPWAIT        20
#else
#define FLIPWAIT        20
#endif

#define SURFACERESOURCE_INVALID  0
#define SURFACERESOURCE_NONE     1
#define SURFACERESOURCE_RESOURCE 2
#define SURFACERESOURCE_FILE     3

typedef struct{
	char name[MAX_IMAGENAME];
	long x_size;
	long y_size;
	long resource;
	BOOL bSystem;
}SURFACEINFORMATIONSTRUCT;

//DirectDraw構造体
static LPDIRECTDRAW        _DDraw   = NULL;// DirectDrawオブジェクト
static LPDIRECTDRAWSURFACE _surf_F  = NULL;// 表バッファ
static LPDIRECTDRAWSURFACE _surf_B  = NULL;// 裏バッファ
static LPDIRECTDRAWSURFACE _surf_S[MAX_SURFACE] = {NULL};// パターン
static LPDIRECTDRAWCLIPPER _clipper;

static SURFACEINFORMATIONSTRUCT _surf_info[MAX_SURFACE];

//フリップ用（ウインドウモード）
static long _client_x;
static long _client_y;
static long _mag = 1;
static RECT _client_rect;
static RECT _surface_rect;
static BOOL _bFullScreen;

extern char gDataPath[];


// 特別 /////////////////
BOOL SystemTask( void );

BOOL IsEnableBitmap( char *path );
BOOL ErrorLog( char *str, long value );


//フリップで使う オフセット／画像サイズ
void SetClientOffset( long offset_x, long offset_y )//, long width, long height )
{
	_client_x = offset_x;
	_client_y = offset_y;
}

static long _RestoreAllSurface( void );
// リストアーズ
void RestoreStripper( void );
void RestoreMapName( void );
void RestoreTextScript( void );


BOOL Flip_SystemTask( HWND hWnd )
{
	static DWORD wait;
	static RECT  rc;

	if(     !SystemTask() ) return FALSE;
	while( GetTickCount() < wait + FLIPWAIT ){
		if( !SystemTask() ) return FALSE;
	}
	if( wait + FLIPWAIT*5 < GetTickCount() ) wait  = GetTickCount();
	else                                     wait += FLIPWAIT;


	GetWindowRect( hWnd, &rc );
	rc.left   += _client_x; 
	rc.top    += _client_y;
	rc.right  = rc.left + _client_rect.right;
	rc.bottom = rc.top  + _client_rect.bottom;


	_surf_F->Blt( &rc, _surf_B, &_surface_rect, DDBLT_WAIT, 0 );

	if( _RestoreAllSurface() ){
//		MessageBox( hWnd, "lost-device", "test", MB_OK ); 
		RestoreStripper(  );
		RestoreMapName(  );
		RestoreTextScript(  );
	}
	return TRUE;
}

// DirectDrawの開始
BOOL StartDirectDraw( HWND hWnd, long mode, long bit )
{

	// DirectDrawの初期化
	if( DirectDrawCreate(NULL, &_DDraw, NULL) != DD_OK ) return FALSE;

	// 復帰用イメージ名の初期化
	memset( _surf_info, 0, sizeof(SURFACEINFORMATIONSTRUCT) * MAX_SURFACE );

	// ディスプレイモード
	switch(  mode  ){
	case 0:// ウインドウ 320 * 240
		_mag          = 1;
		_bFullScreen = FALSE;
		_DDraw->SetCooperativeLevel( hWnd, DDSCL_NORMAL );
		break;
	case 1:// ウインドウ 640 * 480
		_mag          = 2;
		_bFullScreen = FALSE;
		_DDraw->SetCooperativeLevel( hWnd, DDSCL_NORMAL );
		break;
	case 2:// フルスクリーン 640 * 480
		_mag          = 2;
		_bFullScreen = TRUE;
		_DDraw->SetCooperativeLevel( hWnd,DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN );	
		_DDraw->SetDisplayMode( 320*_mag, 240*_mag, bit ); //ﾓｰﾄﾞの Xｻｲｽﾞ,Yｻｲｽﾞ,ﾋﾟｸｾﾙのﾋﾞｯﾄ長
		break;
	}

	_surface_rect.left   = 0;
	_surface_rect.top    = 0;
	_surface_rect.right  = 320 * _mag;
	_surface_rect.bottom = 240 * _mag;

	_client_rect.right   = 320 * _mag;
	_client_rect.bottom  = 240 * _mag;

	// プライマリバッファの初期化
	DDSURFACEDESC   ddsDesc;
	memset( &ddsDesc, 0, sizeof(DDSURFACEDESC) );
	ddsDesc.dwSize            = sizeof(ddsDesc);
	ddsDesc.dwFlags           = DDSD_CAPS;                     //ddsCaps が有効
	ddsDesc.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE; //ﾌﾟﾗｲﾏﾘｻｰﾌｪｽ
	ddsDesc.dwBackBufferCount = 0;
	if( DD_OK != _DDraw->CreateSurface(&ddsDesc, &_surf_F, NULL) ) return FALSE;
	
	// セカンダリバッファの生成
    memset( &ddsDesc, 0, sizeof(DDSURFACEDESC) );
    ddsDesc.dwSize         = sizeof(ddsDesc);
    ddsDesc.dwFlags        = DDSD_CAPS      //ddsCapsが有効
							|DDSD_WIDTH     //dwWidthが有効
							|DDSD_HEIGHT;   //dwHeightが有効
	ddsDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsDesc.dwWidth        = 320*_mag;
	ddsDesc.dwHeight       = 240*_mag;
	if( DD_OK != _DDraw->CreateSurface( &ddsDesc, &_surf_B, NULL) )
		return FALSE;

	
	//クリッパーの作成
	_DDraw->CreateClipper( NULL , &_clipper , NULL );
	_clipper->SetHWnd( 0 , hWnd );
	_surf_F->SetClipper( _clipper );

	return TRUE;
}

// DirectDrawの終了
void EndDirectDraw( HWND hWnd )
{
	long i;

	for(i = 0; i < MAX_SURFACE; i++ ){
	    if( _surf_S[i] != NULL ){
			_surf_S[i]->Release();
			_surf_S[i] = NULL;
		}
    }
    if( _surf_F != NULL){
		_surf_F->Release();
		_surf_F = NULL;
		_surf_B = NULL;
	}
	if( _bFullScreen ) _DDraw->SetCooperativeLevel( hWnd, DDSCL_NORMAL );
	if( _DDraw != NULL ){
		_DDraw->Release();
		_DDraw = NULL;
	}
	memset( _surf_info, 0, sizeof(SURFACEINFORMATIONSTRUCT) * MAX_SURFACE );
}

void ReleaseSurface( long s )
{
	if( _surf_S[ s ] != NULL){
		_surf_S[ s ]->Release();
		_surf_S[ s ] = NULL;
	}
	memset( &_surf_info[ s ], 0, sizeof(SURFACEINFORMATIONSTRUCT) );
}


// サーフェスを作ってビットマップを読み込む(リソースから)
BOOL MakeSurface_Resource( char *name, long surf_no )
{
    HBITMAP       hBmp, hBmpold;//, hBmpOld
	BITMAP        bmp;
	DDSURFACEDESC ddsDesc;

	if( surf_no >= MAX_SURFACE ) return FALSE;
    if( _surf_S[surf_no]       ) return FALSE;


	// リソースからの読み込み
	hBmp = (HBITMAP)LoadImage( GetModuleHandle(NULL), name, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );
    if( !hBmp ) return FALSE;
	GetObject( hBmp, sizeof(BITMAP), &bmp );


	// パターン用バッファの生成
    ZeroMemory( &ddsDesc, sizeof(DDSURFACEDESC) );
    ddsDesc.dwSize         = sizeof(DDSURFACEDESC);
    ddsDesc.dwFlags        = DDSD_CAPS     //ddsCapsが有効
							|DDSD_WIDTH    //dwWidthが有効
							|DDSD_HEIGHT;  //dwHeightが有効
//	ddsDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
	ddsDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsDesc.dwWidth        = bmp.bmWidth  *_mag; //幅を設定
	ddsDesc.dwHeight       = bmp.bmHeight *_mag; //高さを設定
	if( DD_OK != _DDraw->CreateSurface( &ddsDesc, &_surf_S[surf_no], NULL) ) return FALSE;


    // パターンのバッファへの転送
	HDC hDCs;
	HDC hDCd;
 	// x2 Blt
	RECT rc1, rc2;
	rc1.left   = 0;
	rc1.top    = 0;
	rc1.right  = bmp.bmWidth;
	rc1.bottom = bmp.bmHeight;
	rc2.left   = 0;
	rc2.top    = 0;
	rc2.right  = bmp.bmWidth  * _mag;
	rc2.bottom = bmp.bmHeight * _mag;

	hDCs    = CreateCompatibleDC( NULL ); 
    hBmpold = (HBITMAP)SelectObject( hDCs, hBmp );
    _surf_S[surf_no]->GetDC( &hDCd );
	StretchBlt( hDCd, rc2.left, rc2.top, rc2.right, rc2.bottom,
				hDCs, rc1.left, rc1.top, rc1.right, rc1.bottom, SRCCOPY );
    _surf_S[surf_no]->ReleaseDC( hDCd );
    SelectObject( hDCs, hBmpold );
    DeleteDC( hDCs );

    // カラーキーの設定
	DDCOLORKEY    ddColorKey;

	ddColorKey.dwColorSpaceLowValue  = 0;
	ddColorKey.dwColorSpaceHighValue = 0;
	_surf_S[surf_no]->SetColorKey( DDCKEY_SRCBLT, &ddColorKey );
	_surf_S[surf_no]->SetClipper( _clipper );

	// 情報
	_surf_info[ surf_no ].resource = SURFACERESOURCE_RESOURCE;
	_surf_info[ surf_no ].x_size   = bmp.bmWidth;
	_surf_info[ surf_no ].y_size   = bmp.bmHeight;
	_surf_info[ surf_no ].bSystem  = FALSE;
	strcpy( _surf_info[ surf_no ].name, name );

	// 使用Pixelを計算
//	gTotalPixel += ddsDesc.dwWidth * ddsDesc.dwHeight;
    return TRUE;
}


//サーフェスを作ってビットマップを読み込む(ファイルから)
BOOL MakeSurface_File( char *name, long surf_no )
{
    HBITMAP       hBmp, hBmpold;
	BITMAP        bmp;

	char path[MAX_PATH];
	sprintf( path, "%s\\%s.pbm", gDataPath, name );

	if( !IsEnableBitmap( path )  ){
		ErrorLog( path, 0 );
		return FALSE;
	}
	if( surf_no > MAX_SURFACE    ){
		ErrorLog( "surface no", surf_no );
		return FALSE;
	}
	if( _surf_S[surf_no] != NULL ){
		ErrorLog( "existing", surf_no );
		return FALSE;
	}
	
	hBmp = (HBITMAP)LoadImage( GetModuleHandle(NULL), path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	if( !hBmp ){
		ErrorLog( path, 1 );
		return FALSE;
	}
	GetObject( hBmp, sizeof(BITMAP), &bmp );

	// パターン用バッファの生成
	DDSURFACEDESC ddsDesc;
    memset( &ddsDesc, 0, sizeof(DDSURFACEDESC) );//指定ﾒﾓﾘのｸﾘｱ
    ddsDesc.dwSize         = sizeof(DDSURFACEDESC);
    ddsDesc.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
//	ddsDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
	ddsDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsDesc.dwWidth        = bmp.bmWidth  *_mag; //幅を設定
	ddsDesc.dwHeight       = bmp.bmHeight *_mag; //高さを設定
	_DDraw->CreateSurface( &ddsDesc, &_surf_S[surf_no], NULL );


    // パターンのバッファへの転送
	HDC hDCs;
	HDC hDCd;
 	// x2 Blt
	RECT rc1, rc2;
	rc1.left   = 0;
	rc1.top    = 0;
	rc1.right  = bmp.bmWidth;
	rc1.bottom = bmp.bmHeight;
	rc2.left   = 0;
	rc2.top    = 0;
	rc2.right  = bmp.bmWidth  * _mag;
	rc2.bottom = bmp.bmHeight * _mag;

	hDCs    = CreateCompatibleDC( NULL ); 
    hBmpold = (HBITMAP)SelectObject( hDCs, hBmp );
    _surf_S[surf_no]->GetDC( &hDCd );
	StretchBlt( hDCd, rc2.left, rc2.top, rc2.right, rc2.bottom,
				hDCs, rc1.left, rc1.top, rc1.right, rc1.bottom, SRCCOPY );
    _surf_S[surf_no]->ReleaseDC( hDCd );
    SelectObject( hDCs, hBmpold );
    DeleteDC( hDCs );

    // カラーキーの設定
	DDCOLORKEY ddColorKey;
	ddColorKey.dwColorSpaceLowValue  = 0;
    ddColorKey.dwColorSpaceHighValue = 0;
    _surf_S[surf_no]->SetColorKey( DDCKEY_SRCBLT, &ddColorKey );
	_surf_S[surf_no]->SetClipper( _clipper );

    DeleteObject(hBmp);

	// 情報
	_surf_info[ surf_no ].resource = SURFACERESOURCE_FILE;
	_surf_info[ surf_no ].x_size   = bmp.bmWidth;
	_surf_info[ surf_no ].y_size   = bmp.bmHeight;
	_surf_info[ surf_no ].bSystem  = FALSE;
	strcpy( _surf_info[ surf_no ].name, name );

	return TRUE;
}


BOOL ReloadBitmap_Resource( char *name, long surf_no )
{
    HBITMAP       hBmp, hBmpold;
	BITMAP        bmp;

	if( surf_no >= MAX_SURFACE ) return FALSE;

	// リソースからの読み込み
	hBmp = (HBITMAP)LoadImage( GetModuleHandle(NULL), name, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );
    if( !hBmp ) return FALSE;
	GetObject( hBmp, sizeof(BITMAP), &bmp );


    // パターンのバッファへの転送
	HDC hDCs;
	HDC hDCd;
 	// x2 Blt
	RECT rc1, rc2;
	rc1.left   = 0;
	rc1.top    = 0;
	rc1.right  = bmp.bmWidth;
	rc1.bottom = bmp.bmHeight;
	rc2.left   = 0;
	rc2.top    = 0;
	rc2.right  = bmp.bmWidth  * _mag;
	rc2.bottom = bmp.bmHeight * _mag;

	hDCs    = CreateCompatibleDC( NULL ); 
    hBmpold = (HBITMAP)SelectObject( hDCs, hBmp );
    _surf_S[surf_no]->GetDC( &hDCd );
	StretchBlt( hDCd, rc2.left, rc2.top, rc2.right, rc2.bottom,
				hDCs, rc1.left, rc1.top, rc1.right, rc1.bottom, SRCCOPY );
    _surf_S[surf_no]->ReleaseDC( hDCd );
    SelectObject( hDCs, hBmpold );
    DeleteDC( hDCs );

    // カラーキーの設定
	DDCOLORKEY    ddColorKey;

	ddColorKey.dwColorSpaceLowValue  = 0;
	ddColorKey.dwColorSpaceHighValue = 0;
	_surf_S[surf_no]->SetColorKey( DDCKEY_SRCBLT, &ddColorKey );
	_surf_S[surf_no]->SetClipper( _clipper );

	// 情報
	_surf_info[ surf_no ].resource = SURFACERESOURCE_RESOURCE;
	strcpy( _surf_info[ surf_no ].name, name );

	// 使用Pixelを計算
//	gTotalPixel += ddsDesc.dwWidth * ddsDesc.dwHeight;
    return TRUE;
}







//既存のサーフェスにビットマップを読み込む(ファイルから)
BOOL ReloadBitmap_File( char *name, long surf_no )
{

    HBITMAP       hBmp, hBmpold;
	BITMAP        bmp;

	char path[MAX_PATH];

	sprintf( path, "%s\\%s.pbm", gDataPath, name );

	if( !IsEnableBitmap( path )  ){
		ErrorLog( path, 0 );
		return FALSE;
	}
	if( surf_no > MAX_SURFACE    ){
		ErrorLog( "surface no", surf_no );
		return FALSE;
	}
//	if( _surf_S[surf_no] != NULL ){
//		ErrorLog( "existing", surf_no );
//		return FALSE;
//	}

	hBmp = (HBITMAP)LoadImage( GetModuleHandle( NULL ), path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	if( !hBmp ){
		ErrorLog( path, 1 );
		return FALSE;
	}

	GetObject( hBmp, sizeof(BITMAP), &bmp );
	
    // パターンのバッファへの転送
	HDC hDCs;
	HDC hDCd;
 	// x2 Blt
	RECT rc1, rc2;
	rc1.left   = 0;
	rc1.top    = 0;
	rc1.right  = bmp.bmWidth;
	rc1.bottom = bmp.bmHeight;
	rc2.left   = 0;
	rc2.top    = 0;
	rc2.right  = bmp.bmWidth  * _mag;
	rc2.bottom = bmp.bmHeight * _mag;

	hDCs    = CreateCompatibleDC( NULL ); 
    hBmpold = (HBITMAP)SelectObject( hDCs, hBmp );
    _surf_S[surf_no]->GetDC( &hDCd );
	StretchBlt( hDCd, rc2.left, rc2.top, rc2.right, rc2.bottom,
				hDCs, rc1.left, rc1.top, rc1.right, rc1.bottom, SRCCOPY );
    _surf_S[surf_no]->ReleaseDC( hDCd );
    SelectObject( hDCs, hBmpold );
    DeleteDC( hDCs );

	DeleteObject( hBmp );

	// 情報
	_surf_info[ surf_no ].resource = SURFACERESOURCE_FILE;
	strcpy( _surf_info[ surf_no ].name, name );
	return TRUE;

}

BOOL MakeSurface_Generic(long bxsize, long bysize, long surf_no, BOOL bSystem )
{
	DDSURFACEDESC ddsDesc;
    DDCOLORKEY    ddck;

	if( surf_no > MAX_SURFACE ) return FALSE;
    if( _surf_S[surf_no]      ) return FALSE;
	
	// パターン用バッファの生成
    ZeroMemory(&ddsDesc, sizeof(ddsDesc));			//指定ﾒﾓﾘのｸﾘｱ

    ddsDesc.dwSize         = sizeof(ddsDesc);
    ddsDesc.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	if( bSystem ) ddsDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
	else          ddsDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsDesc.dwWidth        = bxsize*_mag;			//幅を設定
	ddsDesc.dwHeight       = bysize*_mag;			//高さを設定
	_DDraw->CreateSurface( &ddsDesc, &_surf_S[surf_no], NULL );
	
	// カラーキーの設定
    ddck.dwColorSpaceLowValue  = 0;
    ddck.dwColorSpaceHighValue = 0;
    _surf_S[surf_no]->SetColorKey( DDCKEY_SRCBLT, &ddck );
//	_surf_S[surf_no]->SetClipper( _clipper );

	// 情報
	_surf_info[ surf_no ].resource = SURFACERESOURCE_NONE;
	_surf_info[ surf_no ].x_size   = ddsDesc.dwWidth  / _mag;
	_surf_info[ surf_no ].y_size   = ddsDesc.dwHeight / _mag;
	if( bSystem ) _surf_info[ surf_no ].bSystem  = TRUE;
	else          _surf_info[ surf_no ].bSystem  = FALSE;
	strcpy( _surf_info[ surf_no ].name, "generic" );

	return TRUE;
}



void BackupSurface( long surf_no, RECT *rect )
{
	static DDBLTFX ddbltfx;
	static RECT    rcWork;

	ZeroMemory(&ddbltfx,sizeof(DDBLTFX));
	ddbltfx.dwSize = sizeof(DDBLTFX);


	rcWork.left   = rect->left   *_mag;
	rcWork.top    = rect->top    *_mag;
	rcWork.right  = rect->right  *_mag;
	rcWork.bottom = rect->bottom *_mag;

	_surf_S[surf_no]->Blt( &rcWork, _surf_B, &rcWork, DDBLT_WAIT, &ddbltfx );
}


void PutBitmap3( RECT *rcView,long x, long y, RECT* rect, long surf_no )
{
	static RECT rcWork;
	static RECT rcSet;

	rcWork = *rect;

	if( x + rect->right - rect->left > rcView->right){//右が出たら
		rcWork.right -=  ( x + rect->right - rect->left) - rcView->right ;
	}
	if( x < rcView->left){//左が出たら
		rcWork.left += rcView->left - x;
		x = rcView->left;
	}
	if( y + rect->bottom - rect->top > rcView->bottom){//右が出たら
		rcWork.bottom -= ( y + rect->bottom - rect->top) - rcView->bottom;
	}
	if( y < rcView->top){//上が出たら
		rcWork.top += rcView->top - y;
		y = rcView->top;
	}
 
	rcSet.left    = x;
	rcSet.top     = y;
	rcSet.right   = x + rcWork.right - rcWork.left;
	rcSet.bottom  = y + rcWork.bottom - rcWork.top;

	rcSet.left    *= _mag;
	rcSet.top     *= _mag;
	rcSet.right   *= _mag;
	rcSet.bottom  *= _mag;
	
	rcWork.left   *= _mag;
	rcWork.top    *= _mag;
	rcWork.right  *= _mag;
	rcWork.bottom *= _mag;

	_surf_B->Blt( &rcSet, _surf_S[surf_no], &rcWork, DDBLT_WAIT|DDBLT_KEYSRC, 0 );
}

// 抜き色無し
void PutBitmap4( RECT *rcView,long x, long y, RECT* rect, long surf_no )
{
	static RECT rcWork;
	static RECT rcSet;

	rcWork = *rect;

	if( x + rect->right - rect->left > rcView->right){//右が出たら
		rcWork.right -=  ( x + rect->right - rect->left) - rcView->right ;
	}
	if( x < rcView->left){//左が出たら
		rcWork.left += rcView->left - x;
		x = rcView->left;
	}
	if( y + rect->bottom - rect->top > rcView->bottom){//右が出たら
		rcWork.bottom -= ( y + rect->bottom - rect->top) - rcView->bottom;
	}
	if( y < rcView->top){//上が出たら
		rcWork.top += rcView->top - y;
		y = rcView->top;
	}

	rcSet.left    = x;
	rcSet.top     = y;
	rcSet.right   = x + rcWork.right - rcWork.left;
	rcSet.bottom  = y + rcWork.bottom - rcWork.top;

	rcSet.left    *= _mag;
	rcSet.top     *= _mag;
	rcSet.right   *= _mag;
	rcSet.bottom  *= _mag;
	
	rcWork.left   *= _mag;
	rcWork.top    *= _mag;
	rcWork.right  *= _mag;
	rcWork.bottom *= _mag;

	_surf_B->Blt( &rcSet, _surf_S[surf_no], &rcWork, DDBLT_WAIT, 0 );
}

void Surface2Surface( long x, long y, RECT* rect, long to, long from )
{
	static RECT rcSet;
	static RECT rcWork;

	rcWork.left   = rect->left   *_mag;
	rcWork.top    = rect->top    *_mag;
	rcWork.right  = rect->right  *_mag;
	rcWork.bottom = rect->bottom *_mag;

	rcSet.left   = x;
	rcSet.top    = y;
	rcSet.right  = x + rect->right - rect->left;
	rcSet.bottom = y + rect->bottom - rect->top;

	rcSet.left   *= _mag;
	rcSet.top    *= _mag;
	rcSet.right  *= _mag;
	rcSet.bottom *= _mag;

	_surf_S[to]->Blt( &rcSet, _surf_S[from], &rcWork, DDBLT_WAIT|DDBLT_KEYSRC, 0 );
}

// CortBox用の色を取得
unsigned long GetCortBoxColor( COLORREF rgb )
{

	HDC hDC;
	DDSURFACEDESC ddsd;

	unsigned long color;
	COLORREF temp_rgb;

	// バックバッファの最初のPixelを保存して
	// 指定の色をつける
	if( DD_OK != _surf_B->GetDC( &hDC ) )
		return CLR_INVALID;
	temp_rgb = GetPixel( hDC, 0, 0 );
	SetPixel( hDC, 0, 0, rgb );
	_surf_B->ReleaseDC( hDC );

	// バックバッファにアクセスして最初のPixelを取得
	memset( &ddsd, 0, sizeof(DDSURFACEDESC) );
	ddsd.dwSize =     sizeof(DDSURFACEDESC);

	if( DD_OK != _surf_B->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) )
		return CLR_INVALID;
	color = *(DWORD *)ddsd.lpSurface;
	if( ddsd.ddpfPixelFormat.dwRGBBitCount < sizeof(DWORD) * 8 ) 
		color &= ( 1 << ddsd.ddpfPixelFormat.dwRGBBitCount ) - 1;

	_surf_B->Unlock( NULL );

	// 最初に置かれていた色を配置
	if( DD_OK != _surf_B->GetDC( &hDC ) )
		return CLR_INVALID;
	SetPixel( hDC, 0, 0, temp_rgb );
	_surf_B->ReleaseDC( hDC );

	return color;
}

void CortBox( RECT *rect, long col )
{
	static DDBLTFX ddbltfx;
	static RECT    rcWork;

	ZeroMemory(&ddbltfx,sizeof(DDBLTFX));
	ddbltfx.dwSize      = sizeof(DDBLTFX);
	ddbltfx.dwFillColor = col;

	rcWork.left   = rect->left   *_mag;
	rcWork.top    = rect->top    *_mag;
	rcWork.right  = rect->right  *_mag;
	rcWork.bottom = rect->bottom *_mag;

	_surf_B->Blt( &rcWork, NULL, NULL, DDBLT_COLORFILL|DDBLT_WAIT, &ddbltfx );
}

void CortBox2( RECT *rect, long col, long surf )
{
	static DDBLTFX ddbltfx;
	static RECT    rcWork;

	ZeroMemory(&ddbltfx,sizeof(DDBLTFX));
	ddbltfx.dwSize      = sizeof(DDBLTFX);
	ddbltfx.dwFillColor = col;


	rcWork.left   = rect->left   *_mag;
	rcWork.top    = rect->top    *_mag;
	rcWork.right  = rect->right  *_mag;
	rcWork.bottom = rect->bottom *_mag;

	_surf_info[ surf ].resource = SURFACERESOURCE_NONE;

	_surf_S[surf]->Blt( &rcWork, NULL, NULL, DDBLT_COLORFILL|DDBLT_WAIT, &ddbltfx );
}




BOOL _out( unsigned char c )
{
	return TRUE;

	FILE *fp;
	char path[MAX_PATH];

	sprintf( path, "%s\\out.txt", gDataPath );

	fp = fopen( path, "a+t" );
	if( !fp )return FALSE;

	fprintf( fp, "%c", c );
	fclose( fp );
	return TRUE;
}

// 失ったサーフェスを復帰する
static long _RestoreAllSurface( void )
{
	long num = 0;

	if( !_surf_F ) return num;
	if( !_surf_B ) return num;

	if( _surf_F->IsLost() == DDERR_SURFACELOST ){
		 num++;
		_surf_F->Restore();
		_out( 'f' );
	}
	if( _surf_B->IsLost() == DDERR_SURFACELOST ){
		 num++;
		_surf_B->Restore();
		_out( 'b' );
	}

	for( long s = 0; s < MAX_SURFACE; s++ ){
		if( _surf_S[ s ] ){
			if( _surf_S[ s ]->IsLost() == DDERR_SURFACELOST ){
				 num++;
				_surf_S[ s ]->Restore();
				_out( (unsigned char)(s + '0') );

				if( !_surf_info[ s ].bSystem ){

					switch( _surf_info[ s ].resource ){
					case SURFACERESOURCE_INVALID:
						break;
					case SURFACERESOURCE_NONE:
						RECT rc;
						rc.left   = 0;
						rc.top    = 0;
						rc.right  = _surf_info[ s ].x_size;
						rc.bottom = _surf_info[ s ].y_size;
						CortBox2( &rc, 0, s );
						break;
					case SURFACERESOURCE_RESOURCE:
						ReloadBitmap_Resource( _surf_info[ s ].name, s );
						break;
					case SURFACERESOURCE_FILE:
						ReloadBitmap_File(     _surf_info[ s ].name, s );
						break;
					}
				}
			}
		}
	}
	return num;
}


HFONT    ghFont;

//初期化時に作成
void InitTextObject(char *name)
{
	long size1, size2;
	switch( _mag ){
	case 1: size1 = 12; size2 =  6; break;
	case 2: size1 = 20; size2 = 10; break;
	}

	{
		ghFont = CreateFont(
			size1,size2, //サイズ
			0,        //角度
			0,
			FW_NORMAL,
			FALSE,//Italic
			FALSE,//Underline
			FALSE,//StrikeOut
			DEFAULT_CHARSET,
			OUT_TT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			FIXED_PITCH,name);
	}

	if( !ghFont ){
		ghFont = CreateFont(
			size1,size2, //サイズ
			0,        //角度
			0,
			FW_NORMAL,
			FALSE,//Italic
			FALSE,//Underline
			FALSE,//StrikeOut
			DEFAULT_CHARSET,
			OUT_TT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			FIXED_PITCH,NULL);
	}
}

//表示関数
void PutText( long x, long y, char *text, DWORD color )
{
	HFONT   oldfont;
	HDC     hdcd;

	_surf_B->GetDC(&hdcd);
	oldfont = (HFONT)SelectObject(hdcd,ghFont);
	SetBkMode(hdcd,TRANSPARENT);           //背景色を透過
	SetTextColor(hdcd,color);//テキストカラー設定

	TextOut( hdcd, x * _mag, y * _mag, text, strlen(text) ); //テキスト表示

	SelectObject(hdcd,oldfont);
	_surf_B->ReleaseDC(hdcd);
}

void PutText2( long x, long y, LPSTR text, DWORD color, long surf )
{
	HFONT   oldfont;
	HDC     hdcd;

	_surf_S[surf]->GetDC(&hdcd);
	oldfont = (HFONT)SelectObject(hdcd,ghFont);
	SetBkMode(hdcd,TRANSPARENT);           //背景色を透過
	SetTextColor(hdcd,color);//テキストカラー設定

	TextOut( hdcd, x * _mag, y * _mag, text, strlen(text) ); //テキスト表示

	SelectObject(hdcd,oldfont);
	_surf_S[surf]->ReleaseDC(hdcd);
}


//終了時に削除
void EndTextObject(void)
{
	DeleteObject(ghFont);
}


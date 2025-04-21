#include <windows.h>
#include <stdio.h>
#include "Draw.h"
#include "Define.h"
#include "Tags.h"


MAPDATA gMap;

char *code_pxma = "PXM";
extern RECT grcGame;

#define MAX_WIDTH_MAP	640
#define MAX_LENGTH_MAP	480

extern char gDataPath[];

void SetNpChar( long code_char, long x, long y, long xm, long ym, long dir, NPCHAR *npc, long start_index );

//マップ情報の初期化
BOOL InitMapData2( void )
{
	//allocate
	gMap.data = (unsigned char *)malloc( MAX_WIDTH_MAP*	MAX_LENGTH_MAP );
	return TRUE;
}

//マップ情報の初期化（Load）(2:容量確保をしない)
BOOL LoadMapData2( char *path_map )
{
	FILE *fp;
	char check[3];
	char path[MAX_PATH];
	sprintf( path, "%s\\%s", gDataPath, path_map );

	fp = fopen( path, "rb" );
	if( !fp )return FALSE;
	fread( check, sizeof(unsigned char), 3, fp );//CODE(3)
	if( memcmp(  check, code_pxma, 3 ) ){
		//error : code
		fclose( fp );
		return FALSE;
	}

	unsigned char dum;
	fread( &dum, sizeof(unsigned char), 1, fp );//parts size
	fread( &gMap.width, sizeof(short), 1, fp );//width
	fread( &gMap.length, sizeof(short), 1, fp );//length

	if( !gMap.data ){
		//error : allocate memory
		fclose( fp );
		return FALSE;
	}
	//マップイメージのロード
	fread( gMap.data, sizeof(unsigned char), gMap.width * gMap.length, fp );//data
	fclose( fp );
	
		
	return TRUE;
}


//マップアトリビュート
BOOL LoadAttributeData( char *path_atrb )
{
	FILE *fp;
	char path[MAX_PATH];
	sprintf( path, "%s\\%s", gDataPath, path_atrb );
	fp = fopen( path, "rb" );
	if( !fp ) return FALSE;
	fread( gMap.atrb, sizeof(char), 256, fp );
	fclose( fp );
	return TRUE;
}

//マップデータの開放(ゲーム終了時にのみ開放)
void EndMapData( void )
{
	free( gMap.data );
}

void ReleasePartsImage( void )
{
	ReleaseSurface( SURF_PARTS );
}


//マップの情報
void GetMapData( unsigned char **data, short *mw, short *ml )
{
	if( data )*data = gMap.data;
	if( mw )*mw = gMap.width;
	if( ml )*ml = gMap.length;
}

//指定のパーツの属性を返す
unsigned char GetAttribute( long x, long y )
{
	long a;

	if( x < 0 || y < 0 || x >= gMap.width || y >= gMap.length )
		return ATRB_DISABLE;

	//パーツ
	a = *( gMap.data + x + ( y * gMap.width ) );
	return gMap.atrb[a];
}

//指定のパーツを消す
void DeleteMapParts( long x, long y )
{
//	long a;
	//パーツ
	*( gMap.data + x + ( y * gMap.width ) ) = 0;
}
//指定のパーツをシフト（ミサイルでの破壊）
void ShiftMapParts( long x, long y )
{
	//パーツ
	*( gMap.data + x + ( y * gMap.width ) ) -= 1;
}
//指定のパーツを変更
BOOL ChangeMapParts( long x, long y, unsigned char no )
{
	//パーツ
	if( *( gMap.data + x + ( y * gMap.width ) ) == no ) return FALSE;

	*( gMap.data + x + ( y * gMap.width ) ) = no;
	for( long i = 0; i < 3; i++ )
		SetNpChar( 4, x*VS*PARTSSIZE, y*VS*PARTSSIZE, 0, 0, DIR_LEFT,  NULL, 0 );
	return TRUE;
}

//マップ表示(奥)
void PutStage_Back( long fx, long fy )
{
	int i,j;
	RECT rect;
	long offset;
	long num_x;
	long num_y;
	long put_x;
	long put_y;
	//ループ回数
	num_x = SURFACE_WIDTH / PARTSSIZE + 1;
	num_y = SURFACE_HEIGHT / PARTSSIZE + 1;
	//配置スタート
	put_x = (fx/VS + PARTSSIZE/2) / PARTSSIZE;
	put_y = (fy/VS + PARTSSIZE/2) / PARTSSIZE;

	long atrb;

	for( j = put_y; j < put_y + num_y; j++ ){
		for( i = put_x; i < put_x + num_x; i++ ){

			offset = j * gMap.width + i;//オフセット

			atrb = GetAttribute( i, j );

			//奥に表示しないものを省く
			if( atrb >= 0x20 )continue;

			//RECT生成
			rect.left = PARTSSIZE * ( *(gMap.data+offset)%16 );
			rect.top = PARTSSIZE * ( *(gMap.data+offset)/16 );
			rect.right = rect.left + PARTSSIZE;
			rect.bottom = rect.top + PARTSSIZE;
			//表示
			PutBitmap3( &grcGame,
				(i * PARTSSIZE - PARTSSIZE/2) - fx/VS,
				(j * PARTSSIZE - PARTSSIZE/2) - fy/VS,
				&rect, SURF_PARTS);
		}
	}
}


//マップ表示(手前)
void PutStage_Front( long fx, long fy )
{
	RECT rcSnack = {256, 48, 272, 64};
	int i,j;
	RECT rect;
	long offset;
	long num_x;
	long num_y;
	long put_x;
	long put_y;
	//ループ回数
	num_x = SURFACE_WIDTH / PARTSSIZE + 1;
	num_y = SURFACE_HEIGHT / PARTSSIZE + 1;
	//配置スタート
	put_x = (fx/VS + PARTSSIZE/2) / PARTSSIZE;
	put_y = (fy/VS + PARTSSIZE/2) / PARTSSIZE;

	long atrb;

	for( j = put_y; j < put_y + num_y; j++ ){
		for( i = put_x; i < put_x + num_x; i++ ){

			offset = j * gMap.width + i;//オフセット

			atrb = GetAttribute( i, j );

			//手前に表示しないものは省く

			if( atrb < 0x40 || atrb >= 0x80)continue;


			//RECT生成
			rect.left = PARTSSIZE * ( *(gMap.data+offset)%16 );
			rect.top = PARTSSIZE * ( *(gMap.data+offset)/16 );
			rect.right = rect.left + PARTSSIZE;
			rect.bottom = rect.top + PARTSSIZE;
			//表示
			PutBitmap3( &grcGame,
				(i * PARTSSIZE - PARTSSIZE/2) - fx/VS,
				(j * PARTSSIZE - PARTSSIZE/2) - fy/VS,
				&rect, SURF_PARTS);
			if( atrb == ATRB_SNACK ){
				PutBitmap3( &grcGame,
					(i * PARTSSIZE - PARTSSIZE/2) - fx/VS,
					(j * PARTSSIZE - PARTSSIZE/2) - fy/VS,
					&rcSnack, SURF_NPC_SYMBOL);
			}
		}
	}
}

//マップ表示(移動)
void PutMapDataVector( long fx, long fy )
{
	int i,j;
	RECT rect;
	long offset;
	long num_x;
	long num_y;
	long put_x;
	long put_y;

	static unsigned char count = 0;//スクロールアニメーション
	count+=2;

	//ループ回数
	num_x = SURFACE_WIDTH  / PARTSSIZE + 1;
	num_y = SURFACE_HEIGHT / PARTSSIZE + 1;
	//配置スタート
	put_x = (fx/VS + PARTSSIZE/2) / PARTSSIZE;
	put_y = (fy/VS + PARTSSIZE/2) / PARTSSIZE;

	long atrb;

	for( j = put_y; j < put_y + num_y; j++ ){
		for( i = put_x; i < put_x + num_x; i++ ){

			//ATTRIBUTEの取得
			offset = j * gMap.width + i;//オフセット
			atrb = GetAttribute( i, j );

			//上下左右以外は省く
			if( atrb != ATRB_LEFT &&
				atrb != ATRB_UP &&
				atrb != ATRB_RIGHT &&
				atrb != ATRB_DOWN &&
				atrb != ATRB_LEFT_W &&
				atrb != ATRB_UP_W &&
				atrb != ATRB_RIGHT_W &&
				atrb != ATRB_DOWN_W )continue;

			//RECT生成
			switch( atrb ){
			case ATRB_LEFT://←
			case ATRB_LEFT_W://←
				rect.left   = 224 + count%16;
				rect.right  = rect.left + PARTSSIZE;
				rect.top    = 48;//これは直値
				rect.bottom = rect.top  + PARTSSIZE;
				break;
			case ATRB_UP://↑
			case ATRB_UP_W://↑
				rect.left   = 224;
				rect.right  = rect.left + PARTSSIZE;
				rect.top    = 48 + count%16;
				rect.bottom = rect.top  + PARTSSIZE;
				break;
			case ATRB_RIGHT://→
			case ATRB_RIGHT_W://→
				rect.left   = 224 + 16 - count%16;
				rect.right  = rect.left + PARTSSIZE;
				rect.top    = 48;
				rect.bottom = rect.top  + PARTSSIZE;
				break;
			case ATRB_DOWN://↓
			case ATRB_DOWN_W://↓
				rect.left   = 224;
				rect.right  = rect.left + PARTSSIZE;
				rect.top    = 48 + 16 - count%16;
				rect.bottom = rect.top  + PARTSSIZE;
				break;
			}
			//表示
			PutBitmap3( &grcGame,
				(i * PARTSSIZE - PARTSSIZE/2) - fx/VS,
				(j * PARTSSIZE - PARTSSIZE/2) - fy/VS,
				&rect, SURF_CARET);
		}
	}
}


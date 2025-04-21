#include <windows.h>
#include <stdio.h>

#include "Define.h"
#include "Tags.h"


static char *gDefaultName = "Profile.dat";
static char *gProfileCode = "Do041220";

BOOL TransferStage( long no, long w, long x, long y );
void InitMyChar( void );
void InitStar( void );
extern HWND ghWnd;

extern MYCHAR gMC;
extern long gStageNo;
extern long gMusicNo;
extern unsigned char gFlagNPC[];
extern ARMSDATA    gArmsData[];
extern ITEMDATA    gItemData[];
extern PERMITSTAGE gPermitStage[];
extern long gSelectedArms;
extern long gSelectedItem;
extern long gCampActive;
extern char gMapping[];

extern char gModulePath[];
extern long gCounter;

void GetStageInfo( long *no_stage, long *no_music );

typedef struct{

	char code[8];     // プロファイル識別コード
	long stage;       // ステージ番号
	long music;       // BGM番号

	long x;           // 座標
	long y;
	long direct;      // 向き
	short max_life;    // 最大体力
	short star;
	short life;        // 現体力
	short a;
	long select_arms; // 選択武器
	long select_item; // 選択アイテム
	long equip;       // 装備フラグ
	long unit;
	long counter;     // プレイカウンタ

	ARMSDATA    arms[           MAX_ARMSDATA    ];          // 所持武器
	ITEMDATA    items[          MAX_ITEMDATA    ];         // 所持道具
	PERMITSTAGE permitstage[    MAX_PERMITSTAGE ];// 転送許可
	char        permit_mapping[ MAX_MAPPING     ]; // マッピング許可

	char        FLAG[4];//フラグクシンボル 'FLAG'
	
	unsigned char flags[MAX_FLAG/8];        // フラグ群

}PROFILEDATA;

// プロファイルの存在
BOOL IsProfile()
{
	HANDLE hFile;
	char path[MAX_PATH];
	sprintf( path, "%s\\%s", gModulePath, gDefaultName );

	hFile = CreateFile( path, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )return FALSE;
	CloseHandle( hFile );
	return TRUE;

}
// プロファイルを保存
BOOL SaveProfile( char *name )
{
	FILE *fp;
	PROFILEDATA profile;
	char *FLAG = "FLAG";

	char path[MAX_PATH];
	if( name ) sprintf( path, "%s\\%s", gModulePath, name         );
	else       sprintf( path, "%s\\%s", gModulePath, gDefaultName );
	fp = fopen( path, "wb" );
	if( !fp )return FALSE;

	// プロファイル構造体に投入
	memset( &profile, 0, sizeof(PROFILEDATA) );
	memcpy( profile.code, gProfileCode, 8 );
	memcpy( profile.FLAG, FLAG, 4 );
	profile.stage       = gStageNo;
	profile.music       = gMusicNo;
	profile.x           = gMC.x;
	profile.y           = gMC.y;
	profile.direct      = gMC.direct;
	profile.max_life    = gMC.max_life;
	profile.life        = gMC.life;
	profile.star        = gMC.star;
	profile.select_arms = gSelectedArms;
	profile.select_item = gSelectedItem;
	profile.equip       = gMC.equip;
	profile.unit        = gMC.unit;
	profile.counter     = gCounter;
	memcpy(  &profile.arms[          0], &gArmsData[   0], sizeof(ARMSDATA)    * MAX_ARMSDATA    );
	memcpy(  &profile.items[         0], &gItemData[   0], sizeof(ITEMDATA)    * MAX_ITEMDATA    );
	memcpy(  &profile.permitstage[   0], &gPermitStage[0], sizeof(PERMITSTAGE) * MAX_PERMITSTAGE );
	memcpy(  &profile.permit_mapping[0], &gMapping[    0], sizeof(char)        * MAX_MAPPING     );
	memcpy(  &profile.flags[         0], &gFlagNPC[    0], sizeof(unsigned char) * MAX_FLAG/8    );

	fwrite( &profile, sizeof(PROFILEDATA), 1, fp );

	fclose( fp );

#ifdef _DEBUG
	// gCounter
	sprintf( path, "%s\\%s", gModulePath, "counter.dat" );
	fp = fopen( path, "wt" );
	fprintf( fp, "count:%d\r\n", gCounter );
#endif

	return TRUE;
}


BOOL ChangeOrganyaVolume( long vol );
void ChangeMusic( long no );
void ClearFade( void );
void SetFrameTargetMyChar( long wait );
void SetFrameMyChar( void );
void InitBossLife(   void );
void CutNoise( void );
void ClearValueView( void );
extern long gCurlyShoot_wait;

// プロファイルを読み込む／準備
BOOL LoadProfile( char *name )
{

	FILE        *fp;
	PROFILEDATA profile;
	char        path[MAX_PATH];

	if( name ) sprintf( path, "%s",     name                      );
	else       sprintf( path, "%s\\%s", gModulePath, gDefaultName );

	//読みこみ------------------------------------------
	fp = fopen( path, "rb" );
	if( !fp )return FALSE;
	// チェックコード
	fread( profile.code, 8, 1, fp );
	if( memcmp( profile.code, gProfileCode, 8 ) ) return FALSE;
	fseek( fp, 0, SEEK_SET );
	memset( &profile, 0, sizeof(PROFILEDATA) );
	fread( &profile, sizeof(PROFILEDATA), 1, fp );
	fclose( fp );//------------------------------------------

	gSelectedArms    = profile.select_arms;
	gSelectedItem    = profile.select_item;
	gCounter         = profile.counter;

	memcpy( gArmsData,    profile.arms,           sizeof(ARMSDATA)      * MAX_ARMSDATA    );
	memcpy( gItemData,    profile.items,          sizeof(ITEMDATA)      * MAX_ITEMDATA    );
	memcpy( gPermitStage, profile.permitstage,    sizeof(PERMITSTAGE)   * MAX_PERMITSTAGE );
	memcpy( gMapping,     profile.permit_mapping, sizeof(char)          * MAX_MAPPING     );
	memcpy( gFlagNPC,     profile.flags,          sizeof(unsigned char) * MAX_FLAG/8      );
	ChangeMusic( profile.music );

	InitMyChar();

	if( !TransferStage( profile.stage, 0, 0, 1 ) ){
		return FALSE;
	}

	gMC.equip    = profile.equip;
	gMC.unit     = profile.unit;
	gMC.direct   = profile.direct;
	gMC.max_life = profile.max_life;
	gMC.life     = profile.life;
	gMC.star     = profile.star;
	gMC.cond     = COND_ALIVE;
	gMC.air      = MAX_AIR;
	gMC.lifeBr   = gMC.life;
	gMC.x        = profile.x;
	gMC.y        = profile.y;

	//装備した武器を更新
	gMC.rect_arms.left   = gArmsData[gSelectedArms].code % 10* 24;
	gMC.rect_arms.right  = gMC.rect_arms.left + 24;
	gMC.rect_arms.top    = gArmsData[gSelectedArms].code / 10* 32;
	gMC.rect_arms.bottom = gMC.rect_arms.top  + 16;

	ClearFade();
	SetFrameMyChar();
	SetFrameTargetMyChar( 16 );
	InitBossLife();
	CutNoise();
	InitStar();
	ClearValueView();
	gCurlyShoot_wait = 0;

	
	return TRUE;
}

// ゲームの初期化、物語の開始
void SetFadeMask( void );
void ClearArmsData( void );
void ClearItemData( void );
void ClearPermitStage( void );
void StartMapping( void );
void InitFlags( void );

BOOL InitializeGame( HWND hWnd )
{
	// プロファイル対象
	InitMyChar();
	gSelectedArms = 0;
	gSelectedItem = 0;
	gCounter      = 0;
	ClearArmsData();
	ClearItemData();
	ClearPermitStage();
	StartMapping();
	InitFlags();

	if( !TransferStage( 13, 200, 10, 8 ) ){
		MessageBox( hWnd, "ステージの読み込みに失敗", "エラー", MB_OK );
		return FALSE;
	}

	ClearFade();
	SetFrameMyChar();
	SetFrameTargetMyChar( 16 );
	InitBossLife();
	CutNoise();
	ClearValueView();
	gCurlyShoot_wait = 0;

	SetFadeMask();
	SetFrameTargetMyChar( 16 );
	return TRUE;
}

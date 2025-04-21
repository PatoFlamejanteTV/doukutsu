#include <windows.h>
#include <stdio.h>
#include "Draw.h"
#include "Sound.h"

#include "Define.h"
#include "KeyControl.h"
#include "Escape.h"


extern RECT grcFull;
extern RECT grcGame;

typedef struct{
	long size;
	char *pData;

	long offset;
	long wait;
	long mode;

	long start_x;

}CREDITSCRIPT;

#define CREDITMODE_STOP   0
#define CREDITMODE_READ   1
#define CREDITMODE_WAIT   2

CREDITSCRIPT _Credit;


typedef struct{

	long flag;
	long x, y;
	long cast;
	char str[64];
}STRIPPER;

typedef struct{
	long act_no;
	long x;
}ILLUST;



#define MAX_STRIPPER 16
#define STRIPPERFLAG_ALIVE 0x80
#define STRIPPER_START_X   160 * VS + 16 * VS
#define STRIPPER_START_Y   240 * VS +  8 * VS

extern char gDataPath[];

STRIPPER _Strip[MAX_STRIPPER];
ILLUST   _Illust;

long GetFileSizeLong( char *path );
BOOL IsShiftJIS(  unsigned char c );
BOOL GetNPCFlag( long a );
void ChangeMusic( long no );
void SetOrganyaFadeout( void );

void EncryptionBinaryData2( unsigned char *pData, long size );


//////////////////////////////////
// 短冊
//////////////////////////////////

// 短冊アクション
void ActionStripper( void )
{
	long s;
	
	for( s = 0; s < MAX_STRIPPER; s++ ){
		if( _Strip[s].flag & STRIPPERFLAG_ALIVE ){
			if( _Credit.mode != CREDITMODE_STOP ) _Strip[s].y -= VS/2;
		}
		if( _Strip[s].y <= -16*VS ) _Strip[s].flag = 0;
	}
}

// 短冊表示
void PutStripper( void )
{
	long s;
	RECT rc;

	for( s = 0; s < MAX_STRIPPER; s++ ){
		if( _Strip[ s ].flag & STRIPPERFLAG_ALIVE ){
			rc.left   =   0;
			rc.right  = 320;
			rc.top    = s * 16;
			rc.bottom = rc.top + 16;
			PutBitmap3( &grcFull, _Strip[s].x/VS, _Strip[s].y/VS, &rc, SURF_STRIPPER );
			// cast
			rc.left   = 24 * (_Strip[ s ].cast%13);
			rc.right  = rc.left + 24;
			rc.top    = 24 * (_Strip[ s ].cast/13);
			rc.bottom = rc.top  + 24;
			PutBitmap3( &grcFull, _Strip[s].x/VS-24, _Strip[s].y/VS-8, &rc, SURF_CASTS );
		}
	}
}

// 短冊配置
void SetStripper( long x, long y, char *text, long cast )
{
	long s;
	RECT rc;
	
	// 未使用短冊を検索
	for( s = 0; s < MAX_STRIPPER; s++ ){
		if( !( _Strip[s].flag & STRIPPERFLAG_ALIVE ) ) break;
	}
	if( s == MAX_STRIPPER ) return;
	

	_Strip[s].flag = STRIPPERFLAG_ALIVE;
	_Strip[s].x    = x;
	_Strip[s].y    = y;
	_Strip[s].cast = cast;
	strcpy( _Strip[s].str, text );

	//cortbox();
	//settext();
	rc.left   = 0;
	rc.right  =   320;
	rc.top    =      s * 16;
	rc.bottom = rc.top + 16;
	CortBox2( &rc, COLOR_TEXTBACK, SURF_STRIPPER );
	PutText2( 0, rc.top, text, COLOR_TEXT, SURF_STRIPPER );
}

void RestoreStripper( void )
{
	long s;
	RECT rc;
	
	// 未使用短冊を検索
	for( s = 0; s < MAX_STRIPPER; s++ ){
		if(  _Strip[s].flag & STRIPPERFLAG_ALIVE ){
			rc.left   = 0;
			rc.right  =   320;
			rc.top    =      s * 16;
			rc.bottom = rc.top + 16;
			CortBox2( &rc, COLOR_TEXTBACK, SURF_STRIPPER );
			PutText2( 0, rc.top, _Strip[s].str, COLOR_TEXT, SURF_STRIPPER );
		}
	}
}

//////////////////////////////////
// イラスト
//////////////////////////////////
void ActionIllust( void )
{
	switch( _Illust.act_no ){
	case 0:
		_Illust.x = -160*VS;
		break;
	case 1:
		_Illust.x += VS*40;
		if( _Illust.x > 0 )
			_Illust.x = 0;
		break;
	case 2:
		_Illust.x -= VS*40;
		if( _Illust.x < -VS*160 )
			_Illust.x = -VS*160;
		break;
	}
}

void PutIllust( void )
{
	RECT rcIllust = {0,0,160,240};

	PutBitmap3( &grcFull, _Illust.x/VS, 0, &rcIllust, SURF_ILLUST );
}

void ReloadIllust( long a )
{
	char name[16];
	sprintf( name, "CREDIT%02d", a );
	ReloadBitmap_Resource( name, SURF_ILLUST );
}


//////////////////////////////////
// クレジットスクリプト
//////////////////////////////////

static char *_credit_script = "credit.tsc";

void InitCreditScript( void )
{
	memset( &_Credit, 0, sizeof(CREDITSCRIPT) );
	memset( _Strip, 0, sizeof(STRIPPER) * MAX_STRIPPER );
}

void ReleaseCreditScript( void )
{
	if( _Credit.pData ){
		free( _Credit.pData );
		_Credit.pData = NULL;
	}
}
// スクリプトの読み込み
BOOL StartCreditScript( void )
{
	FILE *fp;
	char path[MAX_PATH];

	if( _Credit.pData ){
		free( _Credit.pData );
		_Credit.pData = NULL;

	}

	// 容量を確保
	sprintf( path, "%s\\%s", gDataPath, _credit_script );
	_Credit.size = GetFileSizeLong( path );
	if( _Credit.size == -1 ) return FALSE;
	_Credit.pData = (char *)malloc( _Credit.size );
	if( !_Credit.pData     ) return FALSE;

	// 読み込む
	fp = fopen( path, "rb" );
	if( !fp ){
		free( _Credit.pData );
		return FALSE;
	}
	fread( _Credit.pData, sizeof(char), _Credit.size, fp );

	EncryptionBinaryData2( (unsigned char *)_Credit.pData, _Credit.size );

	_Credit.offset = 0;
	_Credit.wait   = 0;
	_Credit.mode   = CREDITMODE_READ;
	_Illust.x      = -160*VS;
	_Illust.act_no = 0;

	grcGame.left  = 160;

	if( !ReloadBitmap_File( "casts", SURF_CASTS ) ) return FALSE;

	
	memset( &_Strip[0], 0, sizeof(STRIPPER)*MAX_STRIPPER );

	return TRUE;
}

// 数値の取得
static long _GetScriptNumber( char *text )
{
	return  (text[0] - '0') * 1000 +
			(text[1] - '0') *  100 +
			(text[2] - '0') *   10 +
			(text[3] - '0') *    1;
}

static void _ActionCredit_Read( void )
{
	long a, b, len;
	char text[40];

	while( 1 ){
		if( _Credit.offset >= _Credit.size ) return;
		

		// コマンド識別 =======================================================
		switch( _Credit.pData[_Credit.offset] ){
		// 短冊発行 ---------------------
		case '[':
			_Credit.offset++;
			a = _Credit.offset;
			while( _Credit.pData[a] != ']' ){
				if( IsShiftJIS( (unsigned char)_Credit.pData[a] ) ) a += 2;
				else                                                a += 1;
			}
			len = a - _Credit.offset;
			memcpy( text, &_Credit.pData[_Credit.offset], len );
			text[len]       = '\0';
			_Credit.offset  =    a;
			_Credit.offset++;
			len             = _GetScriptNumber( &_Credit.pData[_Credit.offset] );
			SetStripper( _Credit.start_x, STRIPPER_START_Y, text, len );
			_Credit.offset += 4;
			return;
		// 待機  ---------------------
		case '-':
			_Credit.offset++;
			_Credit.wait = _GetScriptNumber( &_Credit.pData[_Credit.offset] );
			_Credit.offset += 4;
			_Credit.mode = CREDITMODE_WAIT;
			return;
		// 短冊位置 -----------------
		case '+':
			_Credit.offset++;
			_Credit.start_x = _GetScriptNumber( &_Credit.pData[_Credit.offset] ) * VS;
			_Credit.offset += 4;
			return;
		// 停止  ---------------------
		case '/':
			_Credit.mode = CREDITMODE_STOP;
			return;
		// BGM変更
		case '!':
			_Credit.offset++;
			a = _GetScriptNumber( &_Credit.pData[_Credit.offset] );
			_Credit.offset += 4;
			ChangeMusic( a );
			return;
		// フェードアウト
		case '~':
			_Credit.offset++;
			SetOrganyaFadeout();
			return;
		// ジャンプ
		case 'j':
			_Credit.offset++;
			b = _GetScriptNumber( &_Credit.pData[_Credit.offset] );
			_Credit.offset += 4;
			if( 1 ){
				while(  _Credit.offset < _Credit.size ){
					if( _Credit.pData[_Credit.offset] == 'l' ){
						_Credit.offset++;
						a = _GetScriptNumber( &_Credit.pData[_Credit.offset] );
						_Credit.offset += 4;
						if( b == a ) break;
					}else if( IsShiftJIS( (unsigned char)_Credit.pData[_Credit.offset] ) ){
						_Credit.offset += 2;
					}else{
						_Credit.offset += 1;
					}
				}
			}
			return;
		// flagジャンプ
		case 'f':
			_Credit.offset++;
			a = _GetScriptNumber( &_Credit.pData[_Credit.offset] );
			_Credit.offset += 4 + 1;
			b = _GetScriptNumber( &_Credit.pData[_Credit.offset] );
			_Credit.offset += 4;
			if( GetNPCFlag( a ) ){
				while(  _Credit.offset < _Credit.size ){
					if( _Credit.pData[_Credit.offset] == 'l' ){
						_Credit.offset++;
						a = _GetScriptNumber( &_Credit.pData[_Credit.offset] );
						_Credit.offset += 4;
						if( b == a ) break;
					}else if( IsShiftJIS( (unsigned char)_Credit.pData[_Credit.offset] ) ){
						_Credit.offset += 2;
					}else{
						_Credit.offset += 1;
					}
				}
			}
			return;
		// 次へ -----------------
		default:
			_Credit.offset++;
			break;
		}
	}
}

// スクリプト処理
void ActionCredit( void )
{
	
	if( _Credit.offset >= _Credit.size    ) return;

	switch( _Credit.mode ){
	case CREDITMODE_STOP: break;
	case CREDITMODE_READ: _ActionCredit_Read(); break;
	case CREDITMODE_WAIT:
		_Credit.wait--;
		if( _Credit.wait <= 0 ){
			_Credit.mode = CREDITMODE_READ;
		}
		break;
	}
}


// TEXTSCRIPTから呼ぶ関数
void SetCreditIllust( long a )
{
	ReloadIllust( a );
	_Illust.act_no = 1;
}

void CutCreditIllust( void )
{
	_Illust.act_no = 2;
}













typedef struct {
	long x, y;
}SPRITESTRUCT;
void PutFramePerSecound( void );
void PutTimeCounter( long x, long y );


// 島沈むシーン
enum_ESCRETURN Scene_DownIsland( HWND hWnd, long mode )
{
//	long fps;
	SPRITESTRUCT sprite;
	long wait;

	RECT rc_frame  = { 80, 80, 240, 160};
	RECT rc_sky    = {  0,  0, 160,  80};
	RECT rc_ground = {160, 48, 320,  80};
	RECT rc_sprite = {160,  0, 200,  24};

	sprite.x = 320/2*VS + 8*VS;
	sprite.y = (48+16)   *VS;

	//フィールドループ
	for( wait = 0; wait < 50*18; wait++ ){
		GetTrg();
		if( gKey & KEY_ESC ){
			switch( Call_Escape( hWnd ) ){
			case enum_ESCRETURN_exit:     return enum_ESCRETURN_exit;
			case enum_ESCRETURN_restart:  return enum_ESCRETURN_restart;
			case enum_ESCRETURN_continue: break;
			}
		}

		switch( mode ){
		case 0: sprite.y += VS/10; break;
		case 1:
			if(      wait <  50* 7 ) sprite.y += VS/10;
			else if( wait <  50*10 ) sprite.y += VS/20;
			else if( wait <  50*12 ) sprite.y += VS/40;
			else if( wait == 50*15 ) wait      = 50*18;
			break;
		}

		CortBox( &grcFull, 0 );
		PutBitmap3( &rc_frame,    80, 80,                          &rc_sky,    SURF_NPC_ENEMY );
		PutBitmap3( &rc_frame, sprite.x/VS - 20, sprite.y/VS - 12, &rc_sprite, SURF_NPC_ENEMY );
		PutBitmap3( &rc_frame,    80, 160 - 32,                    &rc_ground, SURF_NPC_ENEMY );
		PutTimeCounter( 8*2, 8*1 );
		PutFramePerSecound();
		if( !Flip_SystemTask( hWnd ) ) return enum_ESCRETURN_exit;
	}

	return enum_ESCRETURN_continue;
}



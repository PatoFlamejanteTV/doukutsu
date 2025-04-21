#include <windows.h>
#include <stdio.h>
#include "Draw.h"
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "Map.h"

//void SetDestroyCaret( long x, long y, long eff );
BOOL GetNPCFlag( long a );
long Random( int min, int max );
void SetCaret( long x, long y, long code, long dir );

NPCHAR gNPC[MAX_NPC];

extern char     gDataPath[];
extern NPCTABLE *gNpcTable;
extern MYCHAR   gMC;

long gCurlyShoot_wait;
long gCurlyShoot_x;
long gCurlyShoot_y;
long gSuperXpos;
long gSuperYpos;


//NPCの初期化
void InitNpChar( void )
{
	memset( gNPC, 0, sizeof(NPCHAR)*MAX_NPC );
}

//イベントファイルタグ
typedef struct{
	short x;// グリッド座標 X
	short y;// グリッド座標 Y
	short code_flag;	//対応フラグコード
	short code_event;	//イベントコード
	short code_char;	//キャラコードアクションコード
	unsigned short bits;//属性などのビット0~15
}EVENTDATA;//12byte
char *gPassPixEve = "PXE";

// 固有パラメータをセット
static void _SetUniqueParameter( NPCHAR *npc )
{
	long code;

	code = npc->code_char;

	npc->surf           = gNpcTable[code].surf;
	npc->hit_voice      = gNpcTable[code].hit_voice;
	npc->destroy_voice  = gNpcTable[code].destroy_voice;
	npc->damage         = gNpcTable[code].damage;
	npc->size           = gNpcTable[code].size;
	npc->life           = gNpcTable[code].life;

	npc->hit.front      = gNpcTable[code].hit.front   *VS;
	npc->hit.back       = gNpcTable[code].hit.back    *VS;
	npc->hit.top        = gNpcTable[code].hit.top     *VS;
	npc->hit.bottom     = gNpcTable[code].hit.bottom  *VS;
	npc->view.front     = gNpcTable[code].view.front  *VS;
	npc->view.back      = gNpcTable[code].view.back   *VS;
	npc->view.top       = gNpcTable[code].view.top    *VS;
	npc->view.bottom    = gNpcTable[code].view.bottom *VS;

	

}

BOOL LoadEvent( char *path_event )
{
	long i, n;
	FILE *fp;
	long count;
	char code[4];
	EVENTDATA eve;

	char path[MAX_PATH];
	sprintf( path, "%s\\%s", gDataPath, path_event );

	fp = fopen( path, "rb" );
	if( !fp )return FALSE;

	fread( code, sizeof( char ), 4, fp );
	if( memcmp( code, gPassPixEve, 3 ) )return FALSE;
	fread( &count, sizeof( long ), 1, fp );


	memset( gNPC, 0, sizeof(NPCHAR) * MAX_NPC );
	n = MAX_NPC/3;
	for( i = 0; i < count; i++ ){
		fread( &eve, 12, 1, fp );

		gNPC[n].direct     = (eve.bits&BITS_DIRECT_RIGHT) ? DIR_RIGHT : DIR_LEFT;
		gNPC[n].code_char  = eve.code_char;
		gNPC[n].code_event = eve.code_event;
		gNPC[n].code_flag  = eve.code_flag;
		gNPC[n].x          = eve.x * PARTSSIZE * VS;
		gNPC[n].y          = eve.y * PARTSSIZE * VS;
		gNPC[n].bits       = eve.bits;

		//テーブルよりロード
		gNPC[n].bits      |= gNpcTable[gNPC[n].code_char].bits;
		gNPC[n].exp        = gNpcTable[gNPC[n].code_char].exp;

		_SetUniqueParameter( &gNPC[n] );

		if( gNPC[n].bits & BITS_FLAG_ALIVE ){//フラグにより存在
			if( GetNPCFlag( gNPC[n].code_flag ) == TRUE ){//フラグが立っていれば
				gNPC[n].cond |= COND_ALIVE;
			}
		}else if( gNPC[n].bits & BITS_FLAG_DEAD ){//フラグにより削除
			if( GetNPCFlag( gNPC[n].code_flag ) == FALSE ){//フラグが倒れていれば
				gNPC[n].cond |= COND_ALIVE;
			}
		}else{//特に無ければ生かす
			gNPC[n].cond = COND_ALIVE;
		}

		n++;
	}

	fclose( fp );
	return TRUE;
}

void SetNpChar( long code_char, long x, long y, long xm, long ym,
				long dir, NPCHAR *npc, long start_index )
{
	long n;
	
	n = start_index;//後から出てきたやつを、手前に出したい
	while( n < MAX_NPC && gNPC[n].cond != 0 ) n++;
	if( n == MAX_NPC )return;

	memset( &gNPC[n], 0, sizeof(NPCHAR) );

	gNPC[n].cond           |= COND_ALIVE;
	gNPC[n].direct         = dir;
	gNPC[n].code_char      = code_char;
	gNPC[n].x              = x;
	gNPC[n].y              = y;
	gNPC[n].xm             = xm;
	gNPC[n].ym             = ym;
	gNPC[n].pNpc           = npc;

	gNPC[n].bits           = gNpcTable[gNPC[n].code_char].bits;
	gNPC[n].exp            = gNpcTable[gNPC[n].code_char].exp;

	_SetUniqueParameter( &gNPC[n] );

}

//爆風配置
void SetDestroyNpChar( long x, long y, long w, long num )
{
	long i;
	long offset_x;
	long offset_y;
	w = w/VS;

	for( i = 0; i < num; i++ ){
		offset_x = Random( -w, w )*VS;
		offset_y = Random( -w, w )*VS;
		SetNpChar( 4, x + offset_x, y + offset_y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
	}
	SetCaret(x, y, CARET_BIGSMOKE, DIR_LEFT );
}

//爆風配置
void SetDestroyNpCharUp( long x, long y, long w, long num )
{
	long i;
	long offset_x;
	long offset_y;
	w = w/VS;

	for( i = 0; i < num; i++ ){
		offset_x = Random( -w, w )*VS;
		offset_y = Random( -w, w )*VS;
		SetNpChar( 4, x + offset_x, y + offset_y, 0, 0, DIR_UP, NULL, MAX_NPC/2 );
	}
	SetCaret(x, y, CARET_BIGSMOKE, DIR_LEFT );
}

void SetExpObjects( long x, long y, long exp )
{
	long n;
	long sub_exp;
	
	n = MAX_NPC/2;
	while( exp ){
		while( n < MAX_NPC && gNPC[n].cond != 0 ) n++;
		if( n == MAX_NPC )return;

		memset( &gNPC[n], 0, sizeof(NPCHAR) );

		if(      exp >= 20 ){
			exp    -= 20;
			sub_exp = 20;
		}
		else if( exp >=  5 ){
			exp    -= 5;
			sub_exp = 5;
		}
		else if( exp >=  1 ){
			exp    -= 1;
			sub_exp = 1;
		}

		gNPC[n].cond      |= COND_ALIVE;
		gNPC[n].direct     = DIR_LEFT;
		gNPC[n].code_char  = 1;//経験値キャラ
		gNPC[n].x          = x;
		gNPC[n].y          = y;

		//属性ビット
		gNPC[n].bits       = gNpcTable[gNPC[n].code_char].bits;
		gNPC[n].exp        = sub_exp;

		_SetUniqueParameter( &gNPC[n] );
	}
}

// 弾数アイテムを配置（弾数アイテムを持っていなければ FALSE ）
extern ARMSDATA gArmsData[];
BOOL SetBulletObject( long x, long y, long val )
{
	long n;
	long bullet_no;

	{// 何番を出すか選択------------------------
		long tamakazu_ari[10];
		long t = 0;
		//候補を上げる
		memset( tamakazu_ari, 0, sizeof(long)*10 );
		for( n = 0; n < MAX_ARMSDATA; n++ ){
			switch( gArmsData[n].code ){
			case  5:tamakazu_ari[t] = 0; t++; break;
			case 10:tamakazu_ari[t] = 1; t++; break;
			default:
				tamakazu_ari[t] = 0;
				break;
			}
		}
		if( t == 0 )return FALSE;
		n         = Random( 1, t*10 );
		bullet_no = tamakazu_ari[ n % t ];
	}

	n = MAX_NPC/2;
	while( n < MAX_NPC && gNPC[n].cond != 0 ) n++;
	if( n == MAX_NPC )return FALSE;

	memset( &gNPC[n], 0, sizeof(NPCHAR) );

	gNPC[n].cond       |= COND_ALIVE;
	gNPC[n].direct     = DIR_LEFT;
	gNPC[n].code_event = bullet_no;
	gNPC[n].code_char  = 86;//弾数キャラ
	gNPC[n].x          = x;
	gNPC[n].y          = y;

	//属性ビット
	gNPC[n].bits       = gNpcTable[gNPC[n].code_char].bits;
	gNPC[n].exp        = val;
					   
		_SetUniqueParameter( &gNPC[n] );

	return TRUE;

}

BOOL SetLifeObject( long x, long y, long val )
{
	long n;


	n = MAX_NPC/2;
	while( n < MAX_NPC && gNPC[n].cond != 0 ) n++;
	if( n == MAX_NPC )return FALSE;

	memset( &gNPC[n], 0, sizeof(NPCHAR) );


	gNPC[n].cond       |= COND_ALIVE;
	gNPC[n].direct     = DIR_LEFT;
	gNPC[n].code_char  = 87;//ライフキャラ
	gNPC[n].x          = x;
	gNPC[n].y          = y;

	//属性ビット
	gNPC[n].bits       = gNpcTable[gNPC[n].code_char].bits;
	gNPC[n].exp        = val;
					   
		_SetUniqueParameter( &gNPC[n] );

	return TRUE;

}



void VanishNpChar( NPCHAR *npc )
{
	long x, y;
	x = npc->x;
	y = npc->y;

	memset( npc, 0, sizeof(NPCHAR) );
	
	npc->count1  = 0;
	npc->x = x;
	npc->y = y;
	npc->cond      |= COND_ALIVE;
	npc->direct     = DIR_LEFT;
	npc->code_char  = 3;//消滅待機オブジェクト

	//属性ビット
	npc->bits       = gNpcTable[npc->code_char].bits;
	npc->exp        = gNpcTable[npc->code_char].exp;

		_SetUniqueParameter( npc );



}


//NPCの表示
extern RECT grcGame;
void SetValueView( long *px, long *py, long value );
void PutNpChar( long fx, long fy )
{
	long n;
	char a = 0;

	long side;

	for( n = 0; n < MAX_NPC; n++ ){
		if( gNPC[n].cond & COND_ALIVE ){
			
			//ダメージブレ
			if( gNPC[n].shock ){
				a = (-1) + gNPC[n].shock/2%2 * 2;
			}else{
				a = 0;
				if( gNPC[n].bits & BITS_VIEWDAMAGE && gNPC[n].damage_view ){
					SetValueView( &gNPC[n].x, &gNPC[n].y, gNPC[n].damage_view );//ダメージ表示
					gNPC[n].damage_view = 0;
				}
			}

			if( gNPC[n].direct == DIR_LEFT ) side = gNPC[n].view.front;
			else                             side = gNPC[n].view.back;
			PutBitmap3( &grcGame, 
				( gNPC[n].x - side            )/VS - fx/VS + a,
				( gNPC[n].y - gNPC[n].view.top)/VS - fy/VS,
				&gNPC[n].rect, gNPC[n].surf );
		}
	}
}

extern NPCFUNCTION gpNpcFuncTbl[];
void ActNpChar( void )
{
	long i;
	long code_char;
	for( i = 0; i < MAX_NPC; i++ ){
		if( gNPC[i].cond & COND_ALIVE ){
			code_char = gNPC[i].code_char;
			gpNpcFuncTbl[code_char]( &gNPC[i] );

			//ダメージ点滅
			if( gNPC[i].shock != 0 )gNPC[i].shock--;
		}
	}
}

// イベントNOで検索し変更
void ChangeNpCharByEvent( long code_event, long code_char, long dir )
{
	long n;
	for( n = 0; n < MAX_NPC; n++ ){
		if( gNPC[n].cond & COND_ALIVE && gNPC[n].code_event == code_event ){

			//キャラクタ固有のビットを倒す（イベントビットはそのまま）
			gNPC[n].bits  &= ~BITS_ACTION;

			gNPC[n].code_char     = code_char;

			gNPC[n].bits         |= gNpcTable[gNPC[n].code_char].bits;
 			gNPC[n].exp           = gNpcTable[gNPC[n].code_char].exp;

			_SetUniqueParameter( &gNPC[n] );

			gNPC[n].cond    |= COND_ALIVE;
			gNPC[n].act_no   = 0;
			gNPC[n].act_wait = 0;
			gNPC[n].count1   = 0;
			gNPC[n].count2   = 0;
			gNPC[n].ani_no   = 0;
			gNPC[n].ani_wait = 0;
			gNPC[n].xm       = 0;
			gNPC[n].ym       = 0;

			if( dir == 5 ){
			}else if( dir == 4 ){
				if( gNPC[n].x < gMC.x ) gNPC[n].direct = DIR_RIGHT;
				else                    gNPC[n].direct = DIR_LEFT;
			}else gNPC[n].direct   = dir;

			//RECT生成のために一回だけ呼ぶ
			gpNpcFuncTbl[code_char]( &gNPC[n] );
		}
	}
}

void ChangeCheckableNpCharByEvent( long code_event, long code_char, long dir )
{
	long n;
	for( n = 0; n < MAX_NPC; n++ ){
		if( gNPC[n].cond & COND_ALIVE && gNPC[n].code_event == code_event ){

			//キャラクタ固有のビットを倒す（イベントビットはそのまま）
			gNPC[n].bits         &= ~BITS_ACTION;
			gNPC[n].bits         |=  BITS_EVENT_CHECK;

			gNPC[n].code_char     = code_char;

			gNPC[n].bits         |= gNpcTable[gNPC[n].code_char].bits;
 			gNPC[n].exp           = gNpcTable[gNPC[n].code_char].exp;

			_SetUniqueParameter( &gNPC[n] );

			gNPC[n].cond         |= COND_ALIVE;
			gNPC[n].act_no        = 0;
			gNPC[n].act_wait      = 0;
			gNPC[n].count1        = 0;
			gNPC[n].count2        = 0;
			gNPC[n].ani_no        = 0;
			gNPC[n].ani_wait      = 0;
			gNPC[n].xm            = 0;
			gNPC[n].ym            = 0;

			if(       dir == 5 ){
			}else if( dir == 4 ){
				if( gNPC[n].x < gMC.x ) gNPC[n].direct = DIR_RIGHT;
				else                    gNPC[n].direct = DIR_LEFT;
			}else gNPC[n].direct   = (char)dir;

			//RECT生成のために一回だけ呼ぶ
			gpNpcFuncTbl[code_char]( &gNPC[n] );
		}
	}
}



// act_no をセット
void SetNpCharActionNo( long code_event, long act_no, long dir )
{
	long n;
	n = 0;
	while( n < MAX_NPC ){
		if( gNPC[n].cond & COND_ALIVE && gNPC[n].code_event == code_event )
			break;
		n++;
	}
	if( n == MAX_NPC )return;

	gNPC[n].act_no = act_no;
	if(       dir == 5 ){ // 変形無し
	}else if( dir == 4 ){
		if( gNPC[n].x < gMC.x ) gNPC[n].direct = DIR_RIGHT;
		else                    gNPC[n].direct = DIR_LEFT;
	}else gNPC[n].direct   = dir;

}

// 座標を変更
void MoveNpChar( long code_event, long x, long y, long dir )
{
	long n;
	n = 0;
	while( n < MAX_NPC ){
		if( gNPC[n].cond & COND_ALIVE && gNPC[n].code_event == code_event )
			break;
		n++;
	}
	if( n == MAX_NPC )return;

	gNPC[n].x = x;
	gNPC[n].y = y;

	if(       dir == 5 ){ // 変形無し
	}else if( dir == 4 ){
		if( gNPC[n].x < gMC.x ) gNPC[n].direct = DIR_RIGHT;
		else                    gNPC[n].direct = DIR_LEFT;
	}else gNPC[n].direct   = (char)dir;

}

//マイキャラをバックステップ
void BackStepMyChar( long code_event )
{
	long n;
	n = 0;


	gMC.cond &= ~COND_CHECK;
	gMC.ym = -VS;
	if( code_event == 0 ){
		gMC.direct = DIR_LEFT;
		gMC.xm = +VS;
		return;
	}else if( code_event == 2 ){
		gMC.direct = DIR_RIGHT;
		gMC.xm = -VS;
		return;
	}

	while( n < MAX_NPC ){
		if( gNPC[n].cond & COND_ALIVE && gNPC[n].code_event == code_event )
			break;
		n++;
	}
	if( n == MAX_NPC )return;

	if( gNPC[n].x < gMC.x ){
		gMC.direct = DIR_LEFT;
		gMC.xm = +VS;
	}else{
		gMC.direct = DIR_RIGHT;
		gMC.xm = -VS;
	}
}


void SetNPCFlag( long a );
void DeleteNpCharEvent( long code )
{
	long i;
	for( i = 0; i < MAX_NPC; i++ ){
		if( gNPC[i].cond & COND_ALIVE ){
			if( gNPC[i].code_event == code ){
				gNPC[i].cond = 0;
				SetNPCFlag( gNPC[i].code_flag );
			}
		}
	}
}

// NPC 消滅
void DeleteNpCharCode( long code, BOOL bSmoke )
{
	long n;
	for( n = 0; n < MAX_NPC; n++ ){

		if( gNPC[n].cond & COND_ALIVE && gNPC[n].code_char == code ){
			gNPC[n].cond = 0;

			SetNPCFlag( gNPC[n].code_flag );
			
			if( bSmoke ){
				PlaySoundObject( gNPC[n].destroy_voice, 1 );
				switch( gNPC[n].size ){
				case NPCSIZE_SMALL:  SetDestroyNpChar( gNPC[n].x, gNPC[n].y, gNPC[n].view.back,  4 );break;
				case NPCSIZE_MIDDLE: SetDestroyNpChar( gNPC[n].x, gNPC[n].y, gNPC[n].view.back,  8 );break;
				case NPCSIZE_LARGE:  SetDestroyNpChar( gNPC[n].x, gNPC[n].y, gNPC[n].view.back, 16 );break;
				}
			}
			
		}
	}
}

void GetNpCharPosition( long *x, long *y, long i )
{
	*x = gNPC[i].x;
	*y = gNPC[i].y;
}

// 指定のNPCが存在する？
BOOL IsNpCharCode( long code )
{
	long i;
	for( i = 0; i < MAX_NPC; i++ ){
		if( gNPC[i].cond & COND_ALIVE ){
			if( gNPC[i].code_char == code ){
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL GetNpCharAlive( long code_event )
{
	long i;
	
	for( i = 0; i < MAX_NPC; i++ ){
		if( gNPC[i].cond & COND_ALIVE &&
			gNPC[i].code_event == code_event ) break;	
	}
	if( i < MAX_NPC )return TRUE;
	return FALSE;
}


long CountAliveNpChar( void )
{
	long n;
	long count;

	count = 0;

	for( n = 0; n < MAX_NPC; n++ ){
		if( gNPC[n].cond & COND_ALIVE ) count++;
	}

	return count;
}
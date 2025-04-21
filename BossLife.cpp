#include <windows.h>
#include "Draw.h"
#include "Define.h"
#include "Tags.h"

#define BL_X	32
#define BL_Y	(240-20)

typedef struct{
	long flag;
	long *pLife;
	long max;
	long br;//ブラー。スペルは？
	long count;
}BOSSLIFE;
BOSSLIFE gBL;

void InitBossLife( void )
{
	gBL.flag = 0;
}

extern NPCHAR gNPC[];
extern NPCHAR gBoss[];

BOOL StartBossLife( long code_event )
{
	long i;
	i = 0;
	while( i < MAX_NPC && gNPC[i].code_event != code_event)i++;
	if( i == MAX_NPC )return FALSE;

	gBL.flag  = 1;
	gBL.max   = gNPC[i].life;
	gBL.br    = gNPC[i].life;
	gBL.pLife = &(gNPC[i].life);

	return TRUE;
}
BOOL StartBossLife2( void )
{

	gBL.flag = 1;
	gBL.max   = gBoss[0].life;
	gBL.br    = gBoss[0].life;
	gBL.pLife = &gBoss[0].life;

	return TRUE;
}

extern RECT grcGame;
void PutBossLife( void )
{
	RECT rcText = {  0, 48,  32, 56};
	RECT rcBox1 = {  0,  0, 244,  8};
	RECT rcBox2 = {  0, 16, 244, 24};
	RECT rcLife = {  0, 24,   0, 32};
	RECT rcBr   = {  0, 32, 232, 40};
	if( !gBL.flag )return;

	if( *gBL.pLife < 1 ){
		gBL.flag = 0;
		return;
	}

	//ライフ
	rcLife.right = *gBL.pLife * 198 / gBL.max;
	//ブラー
	if( gBL.br > *gBL.pLife ){
		gBL.count++;
		if( gBL.count > 30 )
			gBL.br--;
	}else gBL.count = 0;
	rcBr.right = gBL.br * 198 / gBL.max;

	PutBitmap3( &grcGame, BL_X,    BL_Y,   &rcBox1, SURF_TEXTBOX );
	PutBitmap3( &grcGame, BL_X,    BL_Y+8, &rcBox2, SURF_TEXTBOX );
	PutBitmap3( &grcGame, BL_X+40, BL_Y+4, &rcBr,   SURF_TEXTBOX );
	PutBitmap3( &grcGame, BL_X+40, BL_Y+4, &rcLife, SURF_TEXTBOX );
	PutBitmap3( &grcGame, BL_X+8,  BL_Y+4, &rcText, SURF_TEXTBOX );
}
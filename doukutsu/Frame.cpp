#include <windows.h>

#include "Define.h"

void GetMyCharPosition( long *x, long *y );
void GetMapData( unsigned char  **data, short *mw, short *ml );
typedef struct{
	long x;
	long y;
	long *tgt_x;
	long *tgt_y;
	long wait;//大きければゆっくり
	long quake;
	long quake2;
}FRAME;
FRAME gFrame;

/*
//フレームがマイキャラを追いかける
void MoveFrame2MyChar( void )
{
	long mc_x, mc_y;
	short map_w, map_l;

	GetMyCharPosition( &mc_x, &mc_y );
	GetMapData( NULL, &map_w, &map_l );
	gFrameX += (mc_x - SURFACE_WIDTH*VS/2 - gFrameX)/16;
	gFrameY += (mc_y - SURFACE_HEIGHT*VS/2 - gFrameY)/16;
	if( gFrameX/VS <0 ) gFrameX = 0;
	if( gFrameY/VS <0 ) gFrameY = 0;
	if( gFrameX > ( (map_w-1) * PARTSSIZE - SURFACE_WIDTH)*VS )
		gFrameX = ( (map_w-1) * PARTSSIZE - SURFACE_WIDTH )*VS;
	if( gFrameY > ( (map_l-1) * PARTSSIZE - SURFACE_HEIGHT)*VS )
		gFrameY = ( (map_l-1) * PARTSSIZE - SURFACE_HEIGHT )*VS;

}
*/

//フレームが標的とする座標をあらかじめ入れておく
long Random( int min, int max );
void MoveFrame3( void )
{
//	long mc_x, mc_y;
	short map_w, map_l;

//	GetMyCharPosition( &mc_x, &mc_y );
	GetMapData( NULL, &map_w, &map_l );
	gFrame.x += (*gFrame.tgt_x - SURFACE_WIDTH*VS/2 - gFrame.x)/gFrame.wait;
	gFrame.y += (*gFrame.tgt_y - SURFACE_HEIGHT*VS/2 - gFrame.y)/gFrame.wait;
	if( gFrame.x/VS <0 ) gFrame.x = 0;
	if( gFrame.y/VS <0 ) gFrame.y = 0;
	if( gFrame.x > ( (map_w-1) * PARTSSIZE - SURFACE_WIDTH)*VS )
		gFrame.x = ( (map_w-1) * PARTSSIZE - SURFACE_WIDTH )*VS;
	if( gFrame.y > ( (map_l-1) * PARTSSIZE - SURFACE_HEIGHT)*VS )
		gFrame.y = ( (map_l-1) * PARTSSIZE - SURFACE_HEIGHT )*VS;

	if( gFrame.quake2 ){
		gFrame.x += Random( -5, 5 )*VS;
		gFrame.y += Random( -3, 3 )*VS;
		gFrame.quake2--;
	}else if( gFrame.quake ){
		gFrame.x += Random( -1, 1 )*VS;
		gFrame.y += Random( -1, 1 )*VS;
		gFrame.quake--;
	}
}

void GetFramePosition( long *fx, long *fy )
{
	*fx = gFrame.x;
	*fy = gFrame.y;
}

void SetFramePosition( long fx, long fy )
{
	short map_w, map_l;
	gFrame.quake  = 0;
	gFrame.quake2 = 0;

	GetMapData( NULL, &map_w, &map_l );
	gFrame.x = fx;
	gFrame.y = fy;
	if( gFrame.x/VS <0 ) gFrame.x = 0;
	if( gFrame.y/VS <0 ) gFrame.y = 0;
	if( gFrame.x > ( (map_w-1) * PARTSSIZE - SURFACE_WIDTH)*VS )
		gFrame.x = ( (map_w-1) * PARTSSIZE - SURFACE_WIDTH )*VS;
	if( gFrame.y > ( (map_l-1) * PARTSSIZE - SURFACE_HEIGHT)*VS )
		gFrame.y = ( (map_l-1) * PARTSSIZE - SURFACE_HEIGHT )*VS;
}

//マイキャラを中心にする
void SetFrameMyChar( void )
{
	long mc_x, mc_y;
	short map_w, map_l;

	GetMyCharPosition( &mc_x, &mc_y );
	GetMapData( NULL, &map_w, &map_l );

	gFrame.x = (mc_x - SURFACE_WIDTH *VS/2);
	gFrame.y = (mc_y - SURFACE_HEIGHT*VS/2);

	if( gFrame.x/VS <0 ) gFrame.x = 0;
	if( gFrame.y/VS <0 ) gFrame.y = 0;
	if( gFrame.x > ( (map_w-1) * PARTSSIZE - SURFACE_WIDTH)*VS )
		gFrame.x = ( (map_w-1) * PARTSSIZE - SURFACE_WIDTH )*VS;
	if( gFrame.y > ( (map_l-1) * PARTSSIZE - SURFACE_HEIGHT)*VS )
		gFrame.y = ( (map_l-1) * PARTSSIZE - SURFACE_HEIGHT )*VS;

}


#include "Tags.h"

extern MYCHAR gMC;
void SetFrameTargetMyChar( long wait )
{
	gFrame.tgt_x = &gMC.tgt_x;
	gFrame.tgt_y = &gMC.tgt_y;
	gFrame.wait = wait;
}

extern NPCHAR gNPC[];
void SetFrameTargetNpChar( long event, long wait )
{
	long i;
	for( i = 0; i < MAX_NPC; i++ ){
		if( gNPC[i].code_event == event )break;
	}
	if( i == MAX_NPC )return;
	gFrame.tgt_x = &gNPC[i].x;
	gFrame.tgt_y = &gNPC[i].y;
	gFrame.wait = wait;
}

extern NPCHAR gBoss[];
void SetFrameTargetBoss( long no, long wait )
{
	gFrame.tgt_x = &gBoss[no].x;
	gFrame.tgt_y = &gBoss[no].y;
	gFrame.wait = wait;
}


void SetQuake( long time )
{
	gFrame.quake = time;
}
void SetQuake2( long time )
{
	gFrame.quake2 = time;
}

void ResetQuake( void )
{
	gFrame.quake  = 0;
	gFrame.quake2 = 0;
}

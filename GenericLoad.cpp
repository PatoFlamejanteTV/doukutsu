#include <windows.h>
#include <stdio.h>
#include "Draw.h"
#include "Sound.h"
#include "Define.h"

#include "PixTone.h"
long MakePixToneObject( PIXTONEPARAMETER *ptp, long ptp_num, int no );

static PIXTONEPARAMETER gPtpTable[] = {
//00 PoleSt1
{1,5000,5,10.00,32,0,4,4.00,32,0,0,0.00,63,0,63, 6,63, 45, 8,119,46}, 
{1,1000,0, 4.00,32,0,3,1.00,63,0,0,0.00,63,0,63,64,63,128,63,255,63},
//02 Frontia
{1,10000,0,30.00,32,0,3,1.00,32,0,0,0.00,63,0,0,19,44,111,13,198,9},
{1,10000,5,2.00,31,0,3,1.00,57,219,0,2.00,32,0,0,19,44,111,13,198,9},
//04 FireBall
{1,4000,5,0.40,32,0,3,1.00,53,0,0,0.00,63,0,12,19,63,111,21,198,18},  
{1,1000,1,12.00,32,0,2,1.00,63,0,0,0.00,63,0,63,64,63,128,63,255,0},
//06 Jump
{1,1000,5,1.00, 32,0,3,1.00,63,0,0,0.00,63,0,0,28,63,53,31,210,31},
//07 Run 
{1,1000,1,5.00,32,0,3,1.00,63,0,0,0.00,0,0,63,64,63,128,31,255,0},
//08 Step	
{1,3000,1,17.00,34,0,3,2.00,40,0,4,1.00,31,0,63,64,63,225,63,255,0},
//09 NpcSmall
{1,6000,1,930.00,22,0,0,0.70,53,0,0,7.00,32,0,63,64,63,202,63,255,0},   
{1,6000,1,918.00,23,0,0,0.70,53,0,0,7.00,32,0,63,64,63,202,63,255,0},
//11 NpcMiddle
{1,10000,2,200.00,32,0,0,1.00,51,0,1,20.00,31,0,63,64,63,89,30,208,28}, 
{1,10000,5,23.00,16,0,0,1.00,58,0,1,17.00,32,0,63,64,63,96,51,202,31},
//13 NpcLarge
{1,20000,2,100.00,21,0,0,1.00,46,0,1,40.00,32,0,63,64,63,128,63,162,0}, 
{1,20000,5,5.00,21,0,0,1.00,51,0,1,40.00,32,0,63,64,63,128,63,162,28},
{1,10000,5,20.00,23,0,3,0.70,26,235,5,1.00,9,0,63,38,55,87,32,98,29},
{1,2000,1,20.00,32,0,3,2.00,63,0,4,1.00,29,0,63,64,63,128,63,255,63},
//17 Destroy Middle
{1,15000,5,10.00,32,0,3,0.70,26,235,0,0.00,63,0,63,38,55,187,15,255,0}, 
{1,4000,1,20.00,32,0,3,2.00,63,0,4,1.00,29,0,63,64,63,128,63,255,63},   //18
//19 Destroy Large
{1,22000,5,6.00,32,0,3,0.70,26,246,0,0.00,63,0,63,38,55,187,15,255,0},  
{1,8000,0,20.00,32,0,3,2.00,63,0,4,1.00,29,0,63,64,63,128,63,255,63},   //20
//21 Npc Insect
{1,10000,4,400.00,13,0,0,0.80,63,0,4,8.00,31,0,63,64,63,191,32,255,0},  
{1,10000,4,800.00,14,0,0,0.80,63,0,4,8.00,30,125,63,64,63,128,63,166,0},
 //23 My Damage
{1,5000,2,50.00,39,0,3,0.50,40,217,1,0.00,32,0,63,64,63,128,34,198,32},
{1,5000,5,10.00,39,0,3,0.50,24,217,1,4.00,32,0,0,4,63,128,34,198,32},
//25 My Destroy
{1,40000,5,10.00,32,0,3,1.00,32,241,0,0.00,32,0,63,64,63,128,63,255,0}, 
{1,40000,5,20.00,32,0,0,0.00,32,0,5,0.10,32,0,63,64,63,128,63,255,0},
{1,30000,2,400.00,32,0,3,0.30,60,250,0,20.00,32,0,63,64,63,128,63,255,0},
//28 Npc Insect
{1,10000,4,400.00,13,0,0,0.80,63,0,4,8.00,50,0,63,64,63,191,32,255,0},  
{1,10000,4,800.00,5,0,0,0.80,63,0,4,8.00,63,125,63,64,63,128,63,166,0}, //29
//30 YesNo
{1,6000,3,123.00,32,0,4,1.00,16,222,4,4.00,37,0,0,6,63,104,25,255,0},
//31 KeyBreak
{1,4000,5,4.50,32,0,3,1.00,46,102,3,2.50,47,0,63,64,63,128,63,255,0},
//32 Door
{1,5000,4,40.00,52,0,2,1.00,63,0,3,10.00,63,0,63,47,27,47,29,255,0},	
//33 Cursor
{1,3000,0,99.00,32,0,2,1.00,55,197,5,0.00,0,0,63,0,63,164,28,255,0},
//34 Ok
{1,10000,1,601.00,32,0,4,0.50,15,235,0,10.00,54,0,63,0,63,0,63,255,0},
//35 Change Arms
{1,8000,5,10.00,15,0,4,0.50,16,239,4,0.00,50,0,63,0,63,96,17,255,0},
//36 life
{1,20000,1,832.00,32,0,2,1.00,46,0,0,27.00,63,0,63,0,63,140,10,255,0},
{1,20000,1,918.00,32,0,2,1.00,46,0,0,21.00,63,0,63,0,63,140,10,255,0},
//38 Message
{1,4000,1,54.00,32,0,5,0.10,33,0,0,0.00,32,0,53,57,44,128,24,255,0},
{1,10000,1,246.00,23,0,4,0.60,22,239,4,6.00,63,0,0,11,63,13,63,255,0},  //39 GetExp
{1,10000,1,294.00,23,0,4,0.60,22,247,4,6.00,63,140,0,15,63,17,63,255,0},//40
{1,22050,0,117.00,63,0,5,2.00,18,0,5,0.00,0,0,63,0,63,64,19,255,0},     //41 Quake
{1,5000,0,28.00,32,0,3,3.00,27,0,5,0.00,0,0,63,0,63,0,63,255,0},
{1,10000,0,322.00,32,0,2,8.00,37,0,2,0.00,0,0,0,13,63,106,11,255,0},     //43 splash
{1,10000,5,7.30,32,0,5,0.20,29,0,0,0.00,32,0,63,91,63,149,25,255,0},     //44 Break1(block)
{1,1000,0,6.00,32,0,3,1.00,32,0,0,0.00,32,0,63,64,63,128,63,255,63},     //45
{1,20000,2,186.00,32,0,0,4.00,13,98,3,4.00,5,0,63,64,28,255,0,255,0},    //46 Bunret
{1,20000,2,285.00,19,0,3,4.00,21,0,3,4.00,33,130,63,64,63,255,0,255,0},
{1,10000,0,970.00,32,0,2,1.00,35,195,0,31.00,31,0,63,64,63,128,63,255,0},//48 LevelUp
{1,20000,5,6.00,32,0,3,1.00,54,239,0,0.00,32,0,63,64,63,128,63,255,35},  //49 Explosion
{1,40000,5,4.00,32,0,3,0.00,32,230,0,0.00,32,0,63,64,63,128,63,255,0},
{1,40000,1,238.00,32,0,3,1.00,14,0,4,30.00,32,0,63,64,63,128,63,255,0},
{1,3000,2,62.00,32,0,2,3.00,63,0,3,3.00,14,0,63,0,63,210,32,255,0},      //52 Pop
{1,5000,2,58.00,32,0,2,3.00,63,0,2,3.00,32,0,63,0,63,49,27,255,0},       //53
//54 Smoke 
{1,3000,0,13.00,24,0,3,2.00,40,0,4,1.00,31,0,63,64,63,225,63,255,0},     
{1,3000,5,6.00,32,0,5,1.00,32,0,3,0.00,0,0,0,0,63,45,23,255,0},        
//56 Mizari1
{1,20000,1,477.00,40,0,5,93.00,39,0,4,17.00,19,0,0,64,63,128,63,255,0},  
//57 GetItem
{1,6000,5,11.00,32,0,5,1.00,32,0,3,3.00,32,0,63,0,63,0,63,255,0},        
{1,6000,1,329.00,20,0,2,2.00,47,77,3,3.00,63,0,63,64,63,128,63,255,0},
//59 NoDamage
{1,8000,4,2000.00,32,0,2,1.00,0,0,0,0.00,32,0,43,21,7,255,0,255,0},    
//60 Addbul
{1,5000,1,231.00,32,0,4,1.00,32,65,3,2.00,32,0,63,64,63,128,63,255,63},  
//61 ON!
{1,3000,0,107.00,32,0,4,1.00,15,0,0,0.00,17,0,63,64,63,128,63,255,63},  
//62 Bom
{1,20000,5,4.00,32,0,5,1.00,32,170,5,0.00,32,0,63,38,22,255,0,255,0},
{1,5000,1,16.00,32,0,3,1.00,32,238,0,0.00,0,0,63,64,63,128,63,255,63},
{1,20000,5,4.00,32,0,0,0.70,61,43,3,1.00,22,224,63,64,63,204,46,255,0},
//65 expHop
{1,5000,0,880.00,19,0,0,0.00,32,0,0,8.00,32,0,0,11,63,34,25,255,0},
//66 sonic
{1,2000,5,3.00,16,0,3,2.00,12,0,3,1.00,37,0,0,51,63,132,24,255,0},
{1,22050,1,400.00,32,0,0,20.00,10,0,3,10.00,8,0,0,6,63,60,21,255,0},//**
//68 pata
{1,2000,5,2.00,32,0,2,3.00,54,0,0,0.00,32,0,0,17,63,98,22,255,0},
// 69 bubblin
{1,8000,1,814.00,32,0,2,11.00,32,0,3,16.00,32,0,63,23,63,74,12,255,0},
// 70 PoleSt2
{1,10000,5,21.00,21,0,0,5.00,32,178,0,3.00,33,181,63,38,63,104,20,255,0},
{1,6000,5,1.00,28,0,3,6.00,56,0,0,8.00,32,0,63,57,63,98,20,255,0},
// 72 Spine
{1,4000,5,6.00,32,0,3,2.00,32,0,3,2.00,32,0,63,26,30,66,29,255,0},
// 73 Thundr
{1,22050,5,711.00,32,0,5,7.00,32,0,0,0.00,32,0,0,13,0,17,63,255,0},
{1,2000,5,2.00,32,0,5,1.00,32,0,0,0.00,32,0,63,64,63,128,63,255,63},
{1,62050,5,40.00,32,0,5,1.00,32,0,5,0.00,0,0,0,17,63,36,63,255,0},
// 76 npc Gohst
{1,8000,0,77.00,32,0,3,3.00,56,189,0,0.00,17,0,0,38,63,140,28,255,0},
{1,8000,5,8.00,32,0,3,3.00,54,189,3,3.00,32,0,63,38,42,140,21,255,0},
// 78 Bait
{1,9050,5,9.00,26,0,0,1.00,32,209,0,0.00,32,0,63,64,63,132,63,255,0},
{1,9050,2,43.00,32,0,3,1.00,47,172,0,0.00,32,0,63,64,63,198,63,255,0},
// 80 Charge
{1,22050,1,754.00,32,0,2,0.50,14,126,4,18.00,17,0,0,64,63,128,63,255,0},
{1,22050,0,597.00,12,0,2,0.50,14,126,4,18.00,17,0,0,64,40,128,41,255,0},
// 82 Bowwow
{1,6000,2,217.00,32,0,0,0.70,16,0,3,0.00,32,0,0,32,63,49,15,255,0},
// 83 Sword
{1,5000,5,1.00,32,0,3,1.00,32,0,0,0.00,32,0,0,64,63,81,28,255,0},
{1,10000,5,11.00,32,0,2,4.00,32,0,0,3.00,32,0,0,64,63,128,30,255,0},
// 85 Stone
{1,10000,5,1.00,32,0,4,1.00,32,0,0,0.00,32,0,63,64,32,128,33,255,0},
// 86 NpcJump
{1,10000,2,168.00,32,0,0,0.50,29,173,0,0.00,32,0,0,13,63,68,35,255,0},
// 87 BigJump
{1,10000,2,50.00,32,0,0,0.50,29,173,2,100.00,32,0,0,13,63,68,35,255,0},
// 88 Heri
{1,4000,5,11.00,25,0,3,3.00,32,0,0,0.00,32,0,0,9,63,128,14,255,0},
// 89 Heri2
{1,4000,5,3.00,27,0,3,3.00,32,0,0,0.00,32,0,0,9,63,128,14,255,0},
// 90 Cata
{1,3000,4,9.00,20,0,3,3.00,32,0,0,0.00,32,0,63,64,63,128,63,255,0},
// 91 Cata2
{1,3000,4,18.00,20,0,3,3.00,32,0,0,0.00,32,0,63,64,63,128,63,255,0},
// 92 Boost
{1,3000,5,4.00,32,0,3,1.00,32,0,0,0.00,32,0,0,64,63,128,63,255,0},
// 93 AlmondDmg
{1,12000,5,2.00,32,0,3,1.70,53,0,0,0.00,32,0,63,64,63,162,25,255,0},
{1,12000,3,77.00,32,0,3,1.00,61,200,0,19.00,22,0,63,64,63,172,25,255,0},
// 95 Org Bass
{1,5000,0,16.00,63,0,3,1.00,32,0,0,0.00,32,0,63,64,63,166,35,255,0},
{1,1000,5,1.00,16,0,0,1.00,32,0,0,0.00,32,0,63,64,63,91,28,255,0},
// 97 Snare
{1,5000,0,20.00,30,0,3,1.00,44,0,0,0.00,32,0,63,64,63,111,19,255,0},
{1,10000,5,14.00,41,0,5,3.00,32,0,0,0.00,32,0,63,64,18,91,12,255,0},
// 99 HiClose
{1,1000,5,48.00,30,0,5,1.00,32,0,0,0.00,32,0,63,64,63,166,27,255,0},
// 100 HiOpen
{1,10000,5,48.00,30,0,5,1.00,32,0,0,0.00,32,0,63,64,43,166,41,255,7},
// 101 Tom
{1,4000,5,35.00,30,0,3,35.00,32,0,0,0.00,32,0,63,53,21,166,13,255,0},
{1,10000,1,63.00,32,0,3,1.00,32,0,0,0.00,32,0,63,64,39,91,20,255,0},
// 103 splash2
{1,22050,5,52.00,22,0,5,1.00,32,0,0,0.00,32,0,3,23,23,57,10,255,0},
{1,22050,5,80.00,22,0,5,1.00,32,0,0,0.00,32,0,63,23,23,57,10,255,0},
// 105 river
{1,44100,5,54.00,12,0,5,1.00,32,0,5,1.00,27,0,40,64,63,128,38,255,63},
{1,44100,5,119.00,9,0,5,2.00,32,0,0,0.00,32,0,61,64,39,128,60,255,28},
// 107 empty
{1,4000,5,11.00,32,0,5,1.00,17,197,0,0.00,32,0,51,100,63,100,0,255,0},
{1,4000,5,21.00,32,0,5,1.00,17,197,0,0.00,32,0,0,119,0,117,63,255,0},
// 109 girl dmg
{1,5000,2,143.00,25,0,3,0.50,40,217,1,0.00,32,0,63,64,63,128,34,198,32},
{1,5000,5,10.00,23,0,3,0.50,24,217,1,4.00,32,0,0,4,63,128,34,198,32},
// 111 Pad
{1,4000,5,6.00,32,0,3,2.00,32,0,3,2.00,32,0,63,26,30,66,29,255,0},
{1,4000,0,150.00,32,0,0,0.00,32,0,3,2.00,32,0,63,26,30,66,29,255,0},
// 113 CoreOpen
{1,40050,4,100.00,32,0,3,8.00,22,0,3,8.00,8,0,63,28,44,81,23,255,0},
{1,40050,4,150.00,32,0,3,8.00,22,0,3,8.00,8,0,63,28,44,81,23,255,0},
{1,40050,4,294.00,32,0,3,8.00,22,0,3,8.00,8,0,63,28,44,81,23,255,0},
// 116 Secret
{1,5000,3,621.00,32,0,2,2.00,22,0,3,6.00,32,0,0,4,63,77,30,255,0},
// 117 CorePow
{1,30000,5,28.00,44,0,3,1.00,45,0,0,0.00,32,0,63,64,63,208,63,255,0},
{1,30000,2,101.00,44,0,2,0.50,63,118,0,0.00,32,0,63,64,63,176,63,255,0},
{1,30000,2,86.00,44,0,2,0.50,63,118,0,0.00,32,0,63,64,63,176,63,255,0},
// 120 Propera
{1,2000,5,1.00,32,0,0,1.00,32,0,0,0.00,32,0,63,64,63,128,63,255,63},
{1,2000,3,99.00,12,0,3,1.00,32,0,0,0.00,32,0,63,64,63,128,63,255,63},
// 122 npc engel
{1,6000,3,388.00,22,0,0,0.70,53,0,0,7.00,32,0,25,64,63,202,63,255,0},
{1,6000,1,918.00,23,0,0,0.70,53,0,0,7.00,32,0,63,64,63,202,63,255,0},
// 124 nemesis
{1,10000,5,118.00,19,0,3,1.00,63,0,0,3.00,32,0,0,19,63,66,41,255,0},
{1,10000,5,6.00,24,0,3,1.00,32,208,2,8.00,12,0,63,64,63,128,38,255,0},
// 126 spur_charge
{1,400,1,20.00,12,0,0,0.00,32,0,0,0.00,32,0,0,43,63,193,63,255,0},//1
{1,400,1,30.00,12,0,0,0.00,32,0,0,0.00,32,0,0,43,63,193,63,255,0},//2
{1,400,1,40.00,12,0,0,0.00,32,0,0,0.00,32,0,0,43,63,193,63,255,0},//3
// 129 spur lv1
{1,8000,3,800.00,24,0,0,8.00,4,0,0,0.00,32,0,0,13,63,138,63,255,0},
{1,8000,5,53.00,12,0,5,1.00,32,0,0,0.00,32,0,0,0,63,68,63,255,0},
// 131 spur lv2
{1,8000,3,400.00,24,0,0,8.00,4,0,0,0.00,32,0,0,13,63,138,63,255,0},
{1,8000,5,53.00,12,0,5,1.00,32,0,0,0.00,32,0,0,0,63,68,63,255,0},
// 133 spur lv3
{1,8000,3,200.00,32,0,0,8.00,4,0,0,0.00,32,0,0,13,63,138,63,255,0},
{1,8000,5,25.00,17,0,3,1.00,32,0,0,0.00,32,0,0,0,63,68,63,255,0},

// 135 charge_max
{1,8000,4,800.00,32,0,4,2.00,21,0,4,8.00,32,0,0,106,63,130,27,255,0},
// 136 head
{1,3000,4,31.00,10,0,3,2.00,40,0,4,1.00,31,0,0,9,63,94,63,255,0},
// 137 npc jump2
{1,5000,2,168.00,32,0,0,0.50,29,173,0,0.00,32,0,0,13,63,68,35,255,0},
// 138 dummy
{1,1000,0,20.00,0,0,0,0.00,0,0,0,0.00,0,0,0,64,0,128,0,255,0},
//{1,22050,0,440.00,32,0,0,0.00,32,0,0,0.00,32,0,63,64,63,128,63,255,63},


};


BOOL LoadGenericData( void )
{
	long pt_size;

	MakeSurface_Resource(       "PIXEL",            SURF_PIXEL       );

	BOOL bError;
	bError = FALSE;

	if( !MakeSurface_File( "MyChar",       SURF_MYCHAR      ) ) bError = TRUE;	
	if( !MakeSurface_File( "Title",        SURF_TITLE       ) ) bError = TRUE;
	if( !MakeSurface_File( "ArmsImage",    SURF_ARMSIMAGE   ) ) bError = TRUE;
	if( !MakeSurface_File( "Arms",         SURF_ARMS        ) ) bError = TRUE;
	if( !MakeSurface_File( "ItemImage",    SURF_ITEMIMAGE   ) ) bError = TRUE;
	if( !MakeSurface_File( "StageImage",   SURF_STAGEIMAGE  ) ) bError = TRUE;
	if( !MakeSurface_File( "Npc\\NpcSym",  SURF_NPC_SYMBOL  ) ) bError = TRUE;	
	if( !MakeSurface_File( "Npc\\NpcRegu", SURF_NPC_REGULAR ) ) bError = TRUE;	
	if( !MakeSurface_File( "TextBox",      SURF_TEXTBOX     ) ) bError = TRUE;
	if( !MakeSurface_File( "Caret",        SURF_CARET       ) ) bError = TRUE;
	if( !MakeSurface_File( "Bullet",       SURF_BULLET      ) ) bError = TRUE;	
	if( !MakeSurface_File( "Face",         SURF_FACE        ) ) bError = TRUE;		
	if( !MakeSurface_File( "Fade",         SURF_FADE        ) ) bError = TRUE;

	MakeSurface_Resource( "CREDIT01", SURF_ILLUST );

	if( bError ) return FALSE;

	MakeSurface_Generic( SURFACE_WIDTH, SURFACE_HEIGHT, SURF_DISPLAY,   TRUE  );//アイテム画面用ＢＧ
	MakeSurface_Generic( SURFACE_WIDTH, SURFACE_HEIGHT, SURF_BACK,      FALSE );//背景             
	MakeSurface_Generic( SURFACE_WIDTH, SURFACE_HEIGHT, SURF_MINIMAP,   TRUE  );//ミニマップ  
	MakeSurface_Generic( SURFACE_WIDTH, SURFACE_HEIGHT, SURF_CASTS,     FALSE );//キャスト  

	MakeSurface_Generic(           256,            256, SURF_PARTS,     FALSE );// マップパーツ   
	MakeSurface_Generic(           160,             16, SURF_MAPNAME,   FALSE );// マップ名称表示 
	MakeSurface_Generic(            40,            240, SURF_VALUEVIEW, FALSE );
	MakeSurface_Generic(           320,            240, SURF_NPC_ENEMY, FALSE );// マップ毎NPC
	MakeSurface_Generic(           320,            240, SURF_NPC_BOSS,  FALSE );// マップ毎NPC
										  
	MakeSurface_Generic(           320,            240, SURF_STRIPPER,  FALSE );// エンディングクレジット

	//フェード



	//効果音---------------------------------------------------------
	pt_size = 0;
	pt_size += MakePixToneObject( &gPtpTable[  0], 2, WAVE_POLESTAR       );
	pt_size += MakePixToneObject( &gPtpTable[  2], 2, WAVE_FRONTIA        );
	pt_size += MakePixToneObject( &gPtpTable[  4], 2, WAVE_FIRE           );
	pt_size += MakePixToneObject( &gPtpTable[  6], 1, WAVE_JUMP           );
	pt_size += MakePixToneObject( &gPtpTable[  7], 1, WAVE_RUN            );
	pt_size += MakePixToneObject( &gPtpTable[  8], 1, WAVE_STEP           );
	pt_size += MakePixToneObject( &gPtpTable[  9], 2, WAVE_NPC_SMALL      );
	pt_size += MakePixToneObject( &gPtpTable[ 11], 2, WAVE_NPC_MIDDLE     );
	pt_size += MakePixToneObject( &gPtpTable[ 33], 1, WAVE_CURSOR         );
	pt_size += MakePixToneObject( &gPtpTable[ 38], 1, WAVE_MESSAGE        );
	pt_size += MakePixToneObject( &gPtpTable[ 56], 1, WAVE_WARP           );
	pt_size += MakePixToneObject( &gPtpTable[ 61], 1, WAVE_ON             );
	pt_size += MakePixToneObject( &gPtpTable[ 62], 3, WAVE_BOM            );
	pt_size += MakePixToneObject( &gPtpTable[ 65], 1, WAVE_EXPHOP         );
	pt_size += MakePixToneObject( &gPtpTable[ 66], 1, WAVE_SONIC          );
	pt_size += MakePixToneObject( &gPtpTable[ 68], 1, WAVE_PATA           );
//	LoadSoundObject( "Wave\\Message.wav", WAVE_MESSAGE );

	pt_size += MakePixToneObject( &gPtpTable[ 49], 3, WAVE_EXPLOSION      );
	pt_size += MakePixToneObject( &gPtpTable[ 52], 3, WAVE_POP            );
	pt_size += MakePixToneObject( &gPtpTable[ 13], 2, WAVE_NPC_LARGE      );
	pt_size += MakePixToneObject( &gPtpTable[ 28], 2, WAVE_NPC_INSECT     );
	pt_size += MakePixToneObject( &gPtpTable[ 15], 2, WAVE_DESTROY_SMALL  );
	pt_size += MakePixToneObject( &gPtpTable[ 17], 2, WAVE_DESTROY_MIDDLE );
	pt_size += MakePixToneObject( &gPtpTable[ 19], 2, WAVE_DESTROY_LARGE  );
	pt_size += MakePixToneObject( &gPtpTable[ 30], 1, WAVE_YESNO          );
	pt_size += MakePixToneObject( &gPtpTable[ 32], 1, WAVE_DOOR           );
	pt_size += MakePixToneObject( &gPtpTable[ 35], 1, WAVE_CHANGEARMS     );
	pt_size += MakePixToneObject( &gPtpTable[ 46], 2, WAVE_BUNRET         );
	pt_size += MakePixToneObject( &gPtpTable[ 48], 1, WAVE_LEVELUP        );
	pt_size += MakePixToneObject( &gPtpTable[ 54], 2, WAVE_SMOKE          );
	pt_size += MakePixToneObject( &gPtpTable[ 39], 1, WAVE_GETEXP         );
	pt_size += MakePixToneObject( &gPtpTable[ 23], 2, WAVE_MY_DAMAGE      );
	pt_size += MakePixToneObject( &gPtpTable[ 25], 3, WAVE_MY_DESTROY     );
	pt_size += MakePixToneObject( &gPtpTable[ 34], 1, WAVE_OK             );
	pt_size += MakePixToneObject( &gPtpTable[ 36], 2, WAVE_LIFE           );
	pt_size += MakePixToneObject( &gPtpTable[ 31], 1, WAVE_KEYBREAK       );
	pt_size += MakePixToneObject( &gPtpTable[ 41], 2, WAVE_QUAKE          );
	pt_size += MakePixToneObject( &gPtpTable[ 43], 1, WAVE_SPLASH         );
	pt_size += MakePixToneObject( &gPtpTable[ 44], 2, WAVE_BREAK1         );
	pt_size += MakePixToneObject( &gPtpTable[ 57], 2, WAVE_GETITEM        );
	pt_size += MakePixToneObject( &gPtpTable[ 59], 1, WAVE_NODAMAGE       );
	pt_size += MakePixToneObject( &gPtpTable[ 60], 1, WAVE_ADDBULLET      );
	pt_size += MakePixToneObject( &gPtpTable[ 69], 1, WAVE_BUBBLIN        );
	pt_size += MakePixToneObject( &gPtpTable[ 70], 2, WAVE_POLEST2        );
	pt_size += MakePixToneObject( &gPtpTable[ 72], 1, WAVE_SPINE          );
	pt_size += MakePixToneObject( &gPtpTable[ 73], 3, WAVE_THUNDER        );
	pt_size += MakePixToneObject( &gPtpTable[ 76], 2, WAVE_NPC_GOHST      );
	pt_size += MakePixToneObject( &gPtpTable[ 78], 2, WAVE_BAIT           );
	pt_size += MakePixToneObject( &gPtpTable[ 80], 2, WAVE_CHARGE         );
	pt_size += MakePixToneObject( &gPtpTable[ 81], 1, WAVE_SECRET         );
	pt_size += MakePixToneObject( &gPtpTable[ 82], 1, WAVE_BOWWOW         );
	pt_size += MakePixToneObject( &gPtpTable[ 83], 2, WAVE_SWORD          );
	pt_size += MakePixToneObject( &gPtpTable[ 85], 1, WAVE_STONE          );
	pt_size += MakePixToneObject( &gPtpTable[ 86], 1, WAVE_NPCJUMP1        );
	pt_size += MakePixToneObject( &gPtpTable[ 87], 1, WAVE_BIGJUMP        );
	pt_size += MakePixToneObject( &gPtpTable[ 88], 1, WAVE_HERI           );
	pt_size += MakePixToneObject( &gPtpTable[ 89], 1, WAVE_HERI2          );
	pt_size += MakePixToneObject( &gPtpTable[ 90], 1, WAVE_CATA           );
	pt_size += MakePixToneObject( &gPtpTable[ 91], 1, WAVE_CATA2          );
	pt_size += MakePixToneObject( &gPtpTable[ 92], 1, WAVE_BOOST          );
	pt_size += MakePixToneObject( &gPtpTable[ 93], 2, WAVE_ALMONDDMG      );
																	  
	pt_size += MakePixToneObject( &gPtpTable[ 95], 2, WAVE_ORG_DRAM       );
	pt_size += MakePixToneObject( &gPtpTable[ 97], 2, WAVE_ORG_DRAM+1     );
	pt_size += MakePixToneObject( &gPtpTable[ 99], 1, WAVE_ORG_DRAM+2     );
	pt_size += MakePixToneObject( &gPtpTable[100], 1, WAVE_ORG_DRAM+3     );
	pt_size += MakePixToneObject( &gPtpTable[101], 2, WAVE_ORG_DRAM+4     );
	pt_size += MakePixToneObject( &gPtpTable[111], 2, WAVE_ORG_DRAM+5     );
																	  
	pt_size += MakePixToneObject( &gPtpTable[103], 2, WAVE_SPLASH2        );
	pt_size += MakePixToneObject( &gPtpTable[105], 2, WAVE_RIVER1         );
	pt_size += MakePixToneObject( &gPtpTable[105], 2, WAVE_RIVER2         );
	pt_size += MakePixToneObject( &gPtpTable[107], 2, WAVE_ARMS_EMPTY     );
	pt_size += MakePixToneObject( &gPtpTable[109], 2, WAVE_GIRLDMG        );
	pt_size += MakePixToneObject( &gPtpTable[113], 3, WAVE_COREOPEN       );
	pt_size += MakePixToneObject( &gPtpTable[116], 1, WAVE_SECRET         );
	pt_size += MakePixToneObject( &gPtpTable[117], 3, WAVE_COREPOW        );
	pt_size += MakePixToneObject( &gPtpTable[120], 2, WAVE_PROPERA        );
	pt_size += MakePixToneObject( &gPtpTable[122], 2, WAVE_NPC_ENGEL      );
	pt_size += MakePixToneObject( &gPtpTable[124], 2, WAVE_NEMESIS        );

	pt_size += MakePixToneObject( &gPtpTable[126], 1, WAVE_CHARGE_LV1     );
	pt_size += MakePixToneObject( &gPtpTable[127], 1, WAVE_CHARGE_LV2     );
	pt_size += MakePixToneObject( &gPtpTable[128], 1, WAVE_CHARGE_LV3     );
	pt_size += MakePixToneObject( &gPtpTable[129], 2, WAVE_SPUR_LV1       );
	pt_size += MakePixToneObject( &gPtpTable[131], 2, WAVE_SPUR_LV2       );
	pt_size += MakePixToneObject( &gPtpTable[133], 2, WAVE_SPUR_LV3       );
	pt_size += MakePixToneObject( &gPtpTable[135], 1, WAVE_CHARGE_MAX     );
	pt_size += MakePixToneObject( &gPtpTable[136], 1, WAVE_HEAD           );
	pt_size += MakePixToneObject( &gPtpTable[137], 1, WAVE_NPCJUMP2       );
	pt_size += MakePixToneObject( &gPtpTable[138], 1, WAVE_DUMMY          );
	
	
	char str[64];
	sprintf( str, "PixTone = %d byte", pt_size );
//	MessageBox( NULL, str, "wave size", MB_OK );


	return TRUE;
}


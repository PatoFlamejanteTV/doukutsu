#include <windows.h>
#include <stdio.h>
#include "Define.h"
#include "Draw.h"

#include "Map.h"

void ReadyMapName( unsigned char *str );

BOOL LoadEvent( char *path );
BOOL LoadTextScript_Stage( char *path ); 
BOOL InitBack( char *fName, long type );

void SetMyCharPosition( long x, long y );
void SetFrameMyChar( void );
BOOL StartTextScript( long no );

void ClearBullet(    void );
void ClearValueView( void );
void ResetQuake(     void );
void InitCaret(      void );
void InitBossChar( long code );
void ResetFlash( void );


void InitStar( void );

long gStageNo = 0;
long gMusicNo = 0;
long gOldPos  = 0;
long gOldNo   = 0;

//マップデータ入れ替えテーブル
typedef struct{

	char parts[32];//パーツキー（パーツ画像/パーツ属性）
	char map[  32];//マップキー（マップデータ/NPC配置データ/スクリプト）
	long bkType;
	char back[ 32];
	char npc[  32];
	char boss[ 32];
	char boss_no;
	char name[ 32];

}TRANSMAPTABLE;

#define BKACT_FIXED 0
#define BKACT_SCRHF 1
#define BKACT_SCROL 2
#define BKACT_WATER 3
#define BKACT_BLACK 4
#define BKACT_FLOW  5

TRANSMAPTABLE gTMT[] = { 
	{"0",     "0",      BKACT_BLACK,"bk0",     "Guest", "0",     0, "無"                },//00
	{"Pens",  "Pens1",  BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "アーサーの家"      },//01
	{"Eggs",  "Eggs",   BKACT_SCRHF,"BkGreen", "Eggs1", "Ravil", 0, "タマゴ回廊"        },//02
	{"EggX",  "EggX",   BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "タマゴ No.00"      },//03
	{"EggIn", "Egg6",   BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "タマゴ No.06"      },//04
	{"Store", "EggR",   BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "タマゴ監視室"      },//05 
	{"Weed",  "Weed",   BKACT_SCRHF,"BkBlue",  "Weed",  "0",     0, "クサムラ"          },//06
	{"Barr",  "Santa",  BKACT_BLACK,"bk0",     "Weed",  "0",     0, "サンタの家"        },//07
	{"Barr",  "Chako",  BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "チャコの家"        },//08 Chako
	{"Maze",  "MazeI",  BKACT_BLACK,"bk0",     "Maze",  "0",     0, "迷宮Ｉ"            },//09 迷宮1
	{"Sand",  "Sand",   BKACT_SCRHF,"BkGreen", "Sand",  "Omg",   1, "砂区"              },//10 
	{"Mimi",  "Mimi",   BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "ミミガーの村"      },//11 
	{"Cave",  "Cave",   BKACT_BLACK,"bk0",     "Cemet", "0",     0, "最初の洞窟"        },//12 最初の洞窟
	{"Cave",  "Start",  BKACT_BLACK,"bk0",     "Cemet", "0",     0, "スタート地点"      },//13 スタート地点
	{"Mimi",  "Barr",   BKACT_BLACK,"bk0",     "Cemet", "Bllg",  0, "バラック小屋"      },//14 バラック小屋
	{"Mimi",  "Pool",   BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "貯水池"            },//15 貯水池
	{"Mimi",  "Cemet",  BKACT_BLACK,"bk0",     "Cemet", "0",     0, "はかば"            },//16 墓地
	{"Mimi",  "Plant",  BKACT_SCRHF,"BkGreen", "Plant", "0",     0, "山下農園"          },//17 山下農園
	{"Store", "Shelt",  BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "シェルター"        },//18 シェルター
	{"Pens",  "Comu",   BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "集会場"            },//19 comu
	{"Mimi",  "MiBox",  BKACT_BLACK,"bk0",     "0",     "0",     0, "セーブポイント"    },//20 MiBox
	{"Store", "EgEnd1", BKACT_BLACK,"bk0",     "0",     "0",     0, "タマゴ回廊の個室"  },//21 EggEnd 
	{"Store", "Cthu",   BKACT_BLACK,"bk0",     "0",     "0",     0, "クトゥルーの住処"  },//22 
	{"EggIn", "Egg1",   BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "タマゴ No.01"      },//23
	{"Pens",  "Pens2",  BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "アーサーの家"      },//24
	{"Barr",  "Malco",  BKACT_SCRHF,"BkBlue",  "Weed",  "Bllg",  0, "電源室"            },//25 
	{"Barr",  "WeedS",  BKACT_SCRHF,"BkBlue",  "0",     "0",     0, "セーブポイント"    },//26 
	{"Store", "WeedD",  BKACT_SCRHF,"BkBlue",  "0",     "0",     0, "処刑室"            },//27 
	{"Weed",  "Frog" ,  BKACT_SCROL,"BkGreen", "Weed",  "Frog",  2, "ガム"              },//28
	{"Sand",  "Curly",  BKACT_BLACK,"bk0",     "Sand",  "Curly", 0, "砂区駐在所"        },//29
	{"Pens",  "WeedB",  BKACT_SCRHF,"BkBlue",  "Ravil", "0",     0, "クサムラの小屋"    },//30
	{"River", "Stream", BKACT_FLOW, "BkBlue",  "Stream","IronH", 5, "大動脈"            },//31 Stream
	{"Pens",  "CurlyS", BKACT_BLACK,"bk0",     "Sand",  "Curly", 0, "小部屋"            },//32
	{"Barr",  "Jenka1", BKACT_BLACK,"bk0",     "Sand",  "Bllg",  0, "ジェンカの家"      },//33
	{"Sand",  "Dark",   BKACT_SCRHF,"bkBlack", "Sand",  "0",     0, "廃屋"              },//34
	{"Gard",  "Gard",   BKACT_SCRHF,"BkGard",  "Toro",  "Bllg",  0, "砂区倉庫"          },//35
	{"Barr",  "Jenka2", BKACT_BLACK,"bk0",     "Sand",  "Bllg",  0, "ジェンカの家"      },//36
	{"Sand",  "SandE",  BKACT_SCRHF,"BkGreen", "Sand",  "Bllg",  0, "砂区"              },//37 //砂区イベント用
	{"Maze",  "MazeH",  BKACT_BLACK,"bk0",     "Maze",  "0",     0, "迷宮Ｈ"            },//38 迷宮2
	{"Maze",  "MazeW",  BKACT_SCRHF,"BkMaze",  "Maze",  "X",     3, "迷宮Ｗ"            },//39 迷宮3
	{"Maze",  "MazeO",  BKACT_BLACK,"bk0",     "Guest", "0",     0, "キャンプ"          },//40 迷宮の中継点
	{"Maze",  "MazeD",  BKACT_BLACK,"bk0",     "Guest", "Dark",  0, "診療所跡"          },//41 
	{"Store", "MazeA",  BKACT_BLACK,"bk0",     "Maze",  "0",     0, "迷宮の店"          },//42 
	{"Maze",  "MazeB",  BKACT_SCRHF,"BkBlue",  "Maze",  "0",     0, "迷宮Ｂ"            },//43 ブースター入手
	{"Maze",  "MazeS",  BKACT_SCROL,"BkGray",  "Maze",  "Bllg",  0, "大石の塞ぐ所"      },//44 
	{"Maze",  "MazeM",  BKACT_SCRHF,"BkRed",   "Maze",  "0",     0, "迷宮Ｍ"            },//45 巣窟
	{"Cave",  "Drain",  BKACT_WATER,"BkWater", "Cemet", "0",     0, "暗い所"            },//46 アーモンドの手前
	{"Almond","Almond", BKACT_WATER,"BkWater", "Cemet", "Almo1", 4, "コア"              },//47 アーモンド
	{"River", "River",  BKACT_SCROL,"bkGreen", "Weed",  "0",     0, "水路"              },//48 川？
	{"Eggs",  "Eggs2",  BKACT_SCRHF,"BkGreen", "Eggs2", "0",     0, "タマゴ回廊？"      },//49
	{"Store", "Cthu2",  BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "クトゥルーの住処？"},//50
	{"Store", "EggR2",  BKACT_BLACK,"bk0",     "Eggs1", "TwinD", 6, "タマゴ監視室？"    },//51 
	{"EggX",  "EggX2",  BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "タマゴ No.00"      },//52
	{"Oside", "Oside",  BKACT_CROUD,"BkMoon",  "Moon",  "0",     0, "外壁"              },//53
	{"Store", "EgEnd2", BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "タマゴ回廊の個室"  },//54 EggEnd 
	{"Store", "Itoh" ,  BKACT_SCROL,"bkBlue",  "Guest", "0",     0, "倉庫"              },//55 Camp1 Help! 
	{"Cent",  "Cent",   BKACT_SCRHF,"bkGreen", "Guest", "Cent",  0, "大農園"            },//56 Cent 
	{"Jail",  "Jail1",  BKACT_BLACK,"bk0",     "Guest", "Cent",  0, "第１牢"            },//57
	{"Jail",  "Momo",   BKACT_BLACK,"bk0",     "Guest", "0",     0, "カクレガ"          },//58
	{"Jail",  "lounge", BKACT_BLACK,"bk0",     "Guest", "0",     0, "休憩所"            },//59
	{"Store", "CentW",  BKACT_BLACK,"bk0",     "Guest", "Cent",  0, "転送室"            },//60
	{"Store", "Jail2",  BKACT_BLACK,"bk0",     "Guest", "Cent",  0, "第２牢"            },//61
	{"White", "Blcny1", BKACT_CROU2,"BkFog",   "Ravil", "Heri",  0, "バルコニー"        },//62
	{"Jail",  "Priso1", BKACT_BLACK,"BkGray",  "Red",   "0",     0, "最後の洞窟"        },//63
	{"White", "Ring1",  BKACT_CROU2,"BkFog",   "Guest", "Miza",  0, "王の玉座"          },//64
	{"White", "Ring2",  BKACT_CROU2,"BkFog",   "Guest", "Dr",    0, "王の食卓"          },//65
	{"Pens",  "Prefa1", BKACT_BLACK,"Bk0",     "0",     "0",     0, "プレハブ"          },//66
	{"Jail",  "Priso2", BKACT_BLACK,"BkGray",  "Red",   "0",     0, "最後の洞窟・裏"    },//67
	{"White", "Ring3",  BKACT_BLACK,"Bk0",     "Miza",  "Almo2", 7, "黒い広間"          },//68
	{"Pens",  "Little", BKACT_SCROL,"BkBlue",  "Guest", "0",     0, "リトル家"          },//69
	{"White", "Blcny2", BKACT_CROU2,"BkFog",   "Ravil", "Heri",  0, "バルコニー"        },//70
	{"Fall",  "Fall",   BKACT_SCRHF,"BkFall",  "Guest", "Heri",  0, "落下"              },//71
	{"White", "Kings",  BKACT_BLACK,"Bk0",     "Kings", "0",     0, "u"                 },//72
	{"Pens",  "Pixel",  BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "水路の小部屋"      },//73

	{"Maze",  "e_Maze", BKACT_SCRHF,"BkMaze",  "Guest", "Maze",  3, ""                  },//74
	{"Barr",  "e_Jenk", BKACT_BLACK,"bk0",     "Sand",  "Bllg",  0, ""                  },//75
	{"Barr",  "e_Malc", BKACT_SCRHF,"BkBlue",  "Weed",  "Bllg",  0, ""                  },//76
	{"Mimi",  "e_Ceme", BKACT_BLACK,"bk0",     "Plant", "0",     0, ""                  },//77
	{"Fall",  "e_Sky",  BKACT_SCRHF,"BkFall",  "Guest", "Heri",  0, ""                  },//78

	{"Pens",  "Prefa2", BKACT_BLACK,"Bk0",     "0",     "0",     0, "プレハブ"          },//79
	{"Hell",  "Hell1",  BKACT_SCROL,"bkRed",   "Hell",  "0",     0, "聖域地下１階"      },//80
	{"Hell",  "Hell2",  BKACT_SCROL,"bkRed",   "Hell",  "0",     0, "聖域地下２階"      },//81
	{"Hell",  "Hell3",  BKACT_SCRHF,"bkRed",   "Hell",  "Press", 8, "聖域地下３階"      },//82
	{"Cave",  "Mapi",   BKACT_SCROL,"bk0",     "Cemet", "0",     0, "物置"              },//83
	{"Hell",  "Hell4",  BKACT_BLACK,"bk0",     "Hell",  "0",     0, "通路？"            },//84
	{"Hell",  "Hell42", BKACT_BLACK,"bk0",     "Hell",  "Press", 8, "通路？"            },//85
	{"Hell",  "Statue", BKACT_SCRHF,"bkBlue",  "0",     "Cent",  0, "石像の間"          },//86
	{"Hell",  "Ballo1", BKACT_SCROL,"bkBlue",  "Priest","Ballos",9, "封印の間"          },//87
	{"White", "Ostep",  BKACT_CROU2,"BkFog",   "0",     "0",     0, "わたり廊下"        },//88

	{"Labo",  "e_Labo", BKACT_BLACK,"bk0",     "Guest", "0",     0, ""                  },//89
	{"Cave",  "Pole",   BKACT_BLACK,"bk0",     "Guest", "0",     0, "はぐれ銃鍛冶"      },//90
	{"0",     "Island", BKACT_BLACK,"bk0",     "Island","0",     0, ""                  },//91
	{"Hell",  "Ballo2", BKACT_SCROL,"bkBlue",  "Priest","Bllg",  9, "封印の間"          },//92
	{"White", "e_Blcn", BKACT_CROU2,"BkFog",   "Miza",  "0",     9, ""                  },//93
	{"Oside", "Clock",  BKACT_CROUD,"BkMoon",  "Moon",  "0",     0, "時計屋"            },//94
};

//マップデータ群の書き換え（スクリプトデータはCheckUsingScriptで）
BOOL TransferStage( long no, long w, long x, long y )
{

	SetMyCharPosition( x * PARTSSIZE * VS, y * PARTSSIZE * VS );
	

	char path[MAX_PATH];
	char path_dir[20];
	BOOL bError;


	bError = FALSE;


	strcpy( path_dir, "stage" );

	//マップパーツ
	sprintf( path, "%s\\Prt%s", path_dir, gTMT[no].parts );
	if( !ReloadBitmap_File(  path, SURF_PARTS ) ) bError = TRUE;
	//マップパーツ属性
	sprintf( path, "%s\\%s.pxa",    path_dir, gTMT[no].parts );
	if( !LoadAttributeData(    path )            ) bError = TRUE;
	//マップデータ
	sprintf( path, "%s\\%s.pxm",    path_dir, gTMT[no].map   );
	if( !LoadMapData2(         path )            ) bError = TRUE;
	//イベント配置データ
	sprintf( path, "%s\\%s.pxe",    path_dir, gTMT[no].map   );
	if( !LoadEvent(            path )            ) bError = TRUE;
	//イベントスクリプデータ
	sprintf( path, "%s\\%s.tsc",    path_dir, gTMT[no].map   );
	if( !LoadTextScript_Stage( path )            ) bError = TRUE;
	//最奥
	sprintf( path, "%s", gTMT[no].back );
	if( !InitBack(            path, gTMT[no].bkType ) ) bError = TRUE;


	// NPC ===========================
	strcpy( path_dir, "Npc" );

	//NPC_ENEMY
	sprintf( path, "%s\\Npc%s", path_dir, gTMT[no].npc );
	if( !ReloadBitmap_File( path, SURF_NPC_ENEMY ) ) bError = TRUE;
	//NPC_BOSS
	sprintf( path, "%s\\Npc%s", path_dir, gTMT[no].boss );
	if( !ReloadBitmap_File( path, SURF_NPC_BOSS  ) ) bError = TRUE;

	if( bError ) return FALSE;

	ReadyMapName( (unsigned char *)gTMT[no].name );
	StartTextScript( w );
	SetFrameMyChar();

	//掃除
	ClearBullet();
	InitCaret();
	ClearValueView();
	ResetQuake();
	InitBossChar( gTMT[no].boss_no );
	ResetFlash();
	//ステージNoの保存
	gStageNo = no;

	return TRUE;
}

#include "dsound.h"
#include "Organya.h"

char *gMusicTable[] = {

	"XXXX",
	"WANPAKU", // 1
	"ANZEN",   // 2
	"GAMEOVER",// 3
	"GRAVITY", // 4
	"WEED",    // 5
	"MDOWN2",  // 6
	"FIREEYE", // 7
	"VIVI",    // 8
	"MURA",    // 9
	"FANFALE1",//10
	"GINSUKE", //11
	"CEMETERY",//12
	"PLANT",   //13
	"KODOU",   //14
	"FANFALE3",//15
	"FANFALE2",//16
	"DR",      //17
	"ESCAPE",  //18
	"JENKA",   //19
	"MAZE",    //20
	"ACCESS",  //21
	"IRONH",   //22
	"GRAND",   //23
	"Curly",   //24
	"OSIDE",   //25
	"REQUIEM", //26
	"WANPAK2", //27
	"QUIET",   //28
	"LASTCAVE",//29
	"BALCONY", //30
	"LASTBTL", //31
	"LASTBT3", //32
	"ENDING",  //33
	"ZONBIE",  //34
	"BDOWN",   //35
	"HELL",    //36
	"JENKA2",  //37
	"MARINE",  //38
	"BALLOS",  //39
	"TOROKO",  //40
	"WHITE",   //41
};

void ChangeMusic( long no )
{

	if( no && no == gMusicNo ) return;

	//一曲前のポジション
	gOldPos = GetOrganyaPosition();
	gOldNo  = gMusicNo;
	StopOrganyaMusic();
	
	LoadOrganya( gMusicTable[no] );
	ChangeOrganyaVolume( 100 );
	SetOrganyaPosition( 0 );
	PlayOrganyaMusic();
	gMusicNo = no;

}

void ReCallMusic( void )
{

	StopOrganyaMusic();
//	if( !gOldNo )return;
	LoadOrganya( gMusicTable[gOldNo] );
	SetOrganyaPosition( gOldPos );
	ChangeOrganyaVolume( 100 );
	PlayOrganyaMusic();
	gMusicNo = gOldNo;

}



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

//�}�b�v�f�[�^����ւ��e�[�u��
typedef struct{

	char parts[32];//�p�[�c�L�[�i�p�[�c�摜/�p�[�c�����j
	char map[  32];//�}�b�v�L�[�i�}�b�v�f�[�^/NPC�z�u�f�[�^/�X�N���v�g�j
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
	{"0",     "0",      BKACT_BLACK,"bk0",     "Guest", "0",     0, "��"                },//00
	{"Pens",  "Pens1",  BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "�A�[�T�[�̉�"      },//01
	{"Eggs",  "Eggs",   BKACT_SCRHF,"BkGreen", "Eggs1", "Ravil", 0, "�^�}�S��L"        },//02
	{"EggX",  "EggX",   BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "�^�}�S No.00"      },//03
	{"EggIn", "Egg6",   BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "�^�}�S No.06"      },//04
	{"Store", "EggR",   BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "�^�}�S�Ď���"      },//05 
	{"Weed",  "Weed",   BKACT_SCRHF,"BkBlue",  "Weed",  "0",     0, "�N�T����"          },//06
	{"Barr",  "Santa",  BKACT_BLACK,"bk0",     "Weed",  "0",     0, "�T���^�̉�"        },//07
	{"Barr",  "Chako",  BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "�`���R�̉�"        },//08 Chako
	{"Maze",  "MazeI",  BKACT_BLACK,"bk0",     "Maze",  "0",     0, "���{�h"            },//09 ���{1
	{"Sand",  "Sand",   BKACT_SCRHF,"BkGreen", "Sand",  "Omg",   1, "����"              },//10 
	{"Mimi",  "Mimi",   BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "�~�~�K�[�̑�"      },//11 
	{"Cave",  "Cave",   BKACT_BLACK,"bk0",     "Cemet", "0",     0, "�ŏ��̓��A"        },//12 �ŏ��̓��A
	{"Cave",  "Start",  BKACT_BLACK,"bk0",     "Cemet", "0",     0, "�X�^�[�g�n�_"      },//13 �X�^�[�g�n�_
	{"Mimi",  "Barr",   BKACT_BLACK,"bk0",     "Cemet", "Bllg",  0, "�o���b�N����"      },//14 �o���b�N����
	{"Mimi",  "Pool",   BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "�����r"            },//15 �����r
	{"Mimi",  "Cemet",  BKACT_BLACK,"bk0",     "Cemet", "0",     0, "�͂���"            },//16 ��n
	{"Mimi",  "Plant",  BKACT_SCRHF,"BkGreen", "Plant", "0",     0, "�R���_��"          },//17 �R���_��
	{"Store", "Shelt",  BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "�V�F���^�["        },//18 �V�F���^�[
	{"Pens",  "Comu",   BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "�W���"            },//19 comu
	{"Mimi",  "MiBox",  BKACT_BLACK,"bk0",     "0",     "0",     0, "�Z�[�u�|�C���g"    },//20 MiBox
	{"Store", "EgEnd1", BKACT_BLACK,"bk0",     "0",     "0",     0, "�^�}�S��L�̌�"  },//21 EggEnd 
	{"Store", "Cthu",   BKACT_BLACK,"bk0",     "0",     "0",     0, "�N�g�D���[�̏Z��"  },//22 
	{"EggIn", "Egg1",   BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "�^�}�S No.01"      },//23
	{"Pens",  "Pens2",  BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "�A�[�T�[�̉�"      },//24
	{"Barr",  "Malco",  BKACT_SCRHF,"BkBlue",  "Weed",  "Bllg",  0, "�d����"            },//25 
	{"Barr",  "WeedS",  BKACT_SCRHF,"BkBlue",  "0",     "0",     0, "�Z�[�u�|�C���g"    },//26 
	{"Store", "WeedD",  BKACT_SCRHF,"BkBlue",  "0",     "0",     0, "���Y��"            },//27 
	{"Weed",  "Frog" ,  BKACT_SCROL,"BkGreen", "Weed",  "Frog",  2, "�K��"              },//28
	{"Sand",  "Curly",  BKACT_BLACK,"bk0",     "Sand",  "Curly", 0, "���撓�ݏ�"        },//29
	{"Pens",  "WeedB",  BKACT_SCRHF,"BkBlue",  "Ravil", "0",     0, "�N�T�����̏���"    },//30
	{"River", "Stream", BKACT_FLOW, "BkBlue",  "Stream","IronH", 5, "�哮��"            },//31 Stream
	{"Pens",  "CurlyS", BKACT_BLACK,"bk0",     "Sand",  "Curly", 0, "������"            },//32
	{"Barr",  "Jenka1", BKACT_BLACK,"bk0",     "Sand",  "Bllg",  0, "�W�F���J�̉�"      },//33
	{"Sand",  "Dark",   BKACT_SCRHF,"bkBlack", "Sand",  "0",     0, "�p��"              },//34
	{"Gard",  "Gard",   BKACT_SCRHF,"BkGard",  "Toro",  "Bllg",  0, "����q��"          },//35
	{"Barr",  "Jenka2", BKACT_BLACK,"bk0",     "Sand",  "Bllg",  0, "�W�F���J�̉�"      },//36
	{"Sand",  "SandE",  BKACT_SCRHF,"BkGreen", "Sand",  "Bllg",  0, "����"              },//37 //����C�x���g�p
	{"Maze",  "MazeH",  BKACT_BLACK,"bk0",     "Maze",  "0",     0, "���{�g"            },//38 ���{2
	{"Maze",  "MazeW",  BKACT_SCRHF,"BkMaze",  "Maze",  "X",     3, "���{�v"            },//39 ���{3
	{"Maze",  "MazeO",  BKACT_BLACK,"bk0",     "Guest", "0",     0, "�L�����v"          },//40 ���{�̒��p�_
	{"Maze",  "MazeD",  BKACT_BLACK,"bk0",     "Guest", "Dark",  0, "�f�Ï���"          },//41 
	{"Store", "MazeA",  BKACT_BLACK,"bk0",     "Maze",  "0",     0, "���{�̓X"          },//42 
	{"Maze",  "MazeB",  BKACT_SCRHF,"BkBlue",  "Maze",  "0",     0, "���{�a"            },//43 �u�[�X�^�[����
	{"Maze",  "MazeS",  BKACT_SCROL,"BkGray",  "Maze",  "Bllg",  0, "��΂̍ǂ���"      },//44 
	{"Maze",  "MazeM",  BKACT_SCRHF,"BkRed",   "Maze",  "0",     0, "���{�l"            },//45 ���A
	{"Cave",  "Drain",  BKACT_WATER,"BkWater", "Cemet", "0",     0, "�Â���"            },//46 �A�[�����h�̎�O
	{"Almond","Almond", BKACT_WATER,"BkWater", "Cemet", "Almo1", 4, "�R�A"              },//47 �A�[�����h
	{"River", "River",  BKACT_SCROL,"bkGreen", "Weed",  "0",     0, "���H"              },//48 ��H
	{"Eggs",  "Eggs2",  BKACT_SCRHF,"BkGreen", "Eggs2", "0",     0, "�^�}�S��L�H"      },//49
	{"Store", "Cthu2",  BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "�N�g�D���[�̏Z���H"},//50
	{"Store", "EggR2",  BKACT_BLACK,"bk0",     "Eggs1", "TwinD", 6, "�^�}�S�Ď����H"    },//51 
	{"EggX",  "EggX2",  BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "�^�}�S No.00"      },//52
	{"Oside", "Oside",  BKACT_CROUD,"BkMoon",  "Moon",  "0",     0, "�O��"              },//53
	{"Store", "EgEnd2", BKACT_BLACK,"bk0",     "Eggs1", "0",     0, "�^�}�S��L�̌�"  },//54 EggEnd 
	{"Store", "Itoh" ,  BKACT_SCROL,"bkBlue",  "Guest", "0",     0, "�q��"              },//55 Camp1 Help! 
	{"Cent",  "Cent",   BKACT_SCRHF,"bkGreen", "Guest", "Cent",  0, "��_��"            },//56 Cent 
	{"Jail",  "Jail1",  BKACT_BLACK,"bk0",     "Guest", "Cent",  0, "��P�S"            },//57
	{"Jail",  "Momo",   BKACT_BLACK,"bk0",     "Guest", "0",     0, "�J�N���K"          },//58
	{"Jail",  "lounge", BKACT_BLACK,"bk0",     "Guest", "0",     0, "�x�e��"            },//59
	{"Store", "CentW",  BKACT_BLACK,"bk0",     "Guest", "Cent",  0, "�]����"            },//60
	{"Store", "Jail2",  BKACT_BLACK,"bk0",     "Guest", "Cent",  0, "��Q�S"            },//61
	{"White", "Blcny1", BKACT_CROU2,"BkFog",   "Ravil", "Heri",  0, "�o���R�j�["        },//62
	{"Jail",  "Priso1", BKACT_BLACK,"BkGray",  "Red",   "0",     0, "�Ō�̓��A"        },//63
	{"White", "Ring1",  BKACT_CROU2,"BkFog",   "Guest", "Miza",  0, "���̋ʍ�"          },//64
	{"White", "Ring2",  BKACT_CROU2,"BkFog",   "Guest", "Dr",    0, "���̐H��"          },//65
	{"Pens",  "Prefa1", BKACT_BLACK,"Bk0",     "0",     "0",     0, "�v���n�u"          },//66
	{"Jail",  "Priso2", BKACT_BLACK,"BkGray",  "Red",   "0",     0, "�Ō�̓��A�E��"    },//67
	{"White", "Ring3",  BKACT_BLACK,"Bk0",     "Miza",  "Almo2", 7, "�����L��"          },//68
	{"Pens",  "Little", BKACT_SCROL,"BkBlue",  "Guest", "0",     0, "���g����"          },//69
	{"White", "Blcny2", BKACT_CROU2,"BkFog",   "Ravil", "Heri",  0, "�o���R�j�["        },//70
	{"Fall",  "Fall",   BKACT_SCRHF,"BkFall",  "Guest", "Heri",  0, "����"              },//71
	{"White", "Kings",  BKACT_BLACK,"Bk0",     "Kings", "0",     0, "u"                 },//72
	{"Pens",  "Pixel",  BKACT_SCRHF,"BkBlue",  "Guest", "0",     0, "���H�̏�����"      },//73

	{"Maze",  "e_Maze", BKACT_SCRHF,"BkMaze",  "Guest", "Maze",  3, ""                  },//74
	{"Barr",  "e_Jenk", BKACT_BLACK,"bk0",     "Sand",  "Bllg",  0, ""                  },//75
	{"Barr",  "e_Malc", BKACT_SCRHF,"BkBlue",  "Weed",  "Bllg",  0, ""                  },//76
	{"Mimi",  "e_Ceme", BKACT_BLACK,"bk0",     "Plant", "0",     0, ""                  },//77
	{"Fall",  "e_Sky",  BKACT_SCRHF,"BkFall",  "Guest", "Heri",  0, ""                  },//78

	{"Pens",  "Prefa2", BKACT_BLACK,"Bk0",     "0",     "0",     0, "�v���n�u"          },//79
	{"Hell",  "Hell1",  BKACT_SCROL,"bkRed",   "Hell",  "0",     0, "����n���P�K"      },//80
	{"Hell",  "Hell2",  BKACT_SCROL,"bkRed",   "Hell",  "0",     0, "����n���Q�K"      },//81
	{"Hell",  "Hell3",  BKACT_SCRHF,"bkRed",   "Hell",  "Press", 8, "����n���R�K"      },//82
	{"Cave",  "Mapi",   BKACT_SCROL,"bk0",     "Cemet", "0",     0, "���u"              },//83
	{"Hell",  "Hell4",  BKACT_BLACK,"bk0",     "Hell",  "0",     0, "�ʘH�H"            },//84
	{"Hell",  "Hell42", BKACT_BLACK,"bk0",     "Hell",  "Press", 8, "�ʘH�H"            },//85
	{"Hell",  "Statue", BKACT_SCRHF,"bkBlue",  "0",     "Cent",  0, "�Α��̊�"          },//86
	{"Hell",  "Ballo1", BKACT_SCROL,"bkBlue",  "Priest","Ballos",9, "����̊�"          },//87
	{"White", "Ostep",  BKACT_CROU2,"BkFog",   "0",     "0",     0, "�킽��L��"        },//88

	{"Labo",  "e_Labo", BKACT_BLACK,"bk0",     "Guest", "0",     0, ""                  },//89
	{"Cave",  "Pole",   BKACT_BLACK,"bk0",     "Guest", "0",     0, "�͂���e�b��"      },//90
	{"0",     "Island", BKACT_BLACK,"bk0",     "Island","0",     0, ""                  },//91
	{"Hell",  "Ballo2", BKACT_SCROL,"bkBlue",  "Priest","Bllg",  9, "����̊�"          },//92
	{"White", "e_Blcn", BKACT_CROU2,"BkFog",   "Miza",  "0",     9, ""                  },//93
	{"Oside", "Clock",  BKACT_CROUD,"BkMoon",  "Moon",  "0",     0, "���v��"            },//94
};

//�}�b�v�f�[�^�Q�̏��������i�X�N���v�g�f�[�^��CheckUsingScript�Łj
BOOL TransferStage( long no, long w, long x, long y )
{

	SetMyCharPosition( x * PARTSSIZE * VS, y * PARTSSIZE * VS );
	

	char path[MAX_PATH];
	char path_dir[20];
	BOOL bError;


	bError = FALSE;


	strcpy( path_dir, "stage" );

	//�}�b�v�p�[�c
	sprintf( path, "%s\\Prt%s", path_dir, gTMT[no].parts );
	if( !ReloadBitmap_File(  path, SURF_PARTS ) ) bError = TRUE;
	//�}�b�v�p�[�c����
	sprintf( path, "%s\\%s.pxa",    path_dir, gTMT[no].parts );
	if( !LoadAttributeData(    path )            ) bError = TRUE;
	//�}�b�v�f�[�^
	sprintf( path, "%s\\%s.pxm",    path_dir, gTMT[no].map   );
	if( !LoadMapData2(         path )            ) bError = TRUE;
	//�C�x���g�z�u�f�[�^
	sprintf( path, "%s\\%s.pxe",    path_dir, gTMT[no].map   );
	if( !LoadEvent(            path )            ) bError = TRUE;
	//�C�x���g�X�N���v�f�[�^
	sprintf( path, "%s\\%s.tsc",    path_dir, gTMT[no].map   );
	if( !LoadTextScript_Stage( path )            ) bError = TRUE;
	//�ŉ�
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

	//�|��
	ClearBullet();
	InitCaret();
	ClearValueView();
	ResetQuake();
	InitBossChar( gTMT[no].boss_no );
	ResetFlash();
	//�X�e�[�WNo�̕ۑ�
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

	//��ȑO�̃|�W�V����
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



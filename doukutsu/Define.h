
//サーフェス(ゲームモジュールでのみ使用)
#define SURFACE_WIDTH		320
#define SURFACE_HEIGHT		240




#define VS					512










//サーフェス----------------------------
#define SURF_TITLE        0
#define SURF_PIXEL        1
#define SURF_PARTS        2
#define SURF_FADE         6
#define SURF_ITEMIMAGE    8
#define SURF_MINIMAP      9
#define SURF_DISPLAY     10
#define SURF_ARMS        11
#define SURF_ARMSIMAGE   12
#define SURF_MAPNAME     13
#define SURF_STAGEIMAGE  14
#define SURF_LOADING     15
#define SURF_MYCHAR      16
#define SURF_BULLET      17

#define SURF_CARET       19
#define SURF_NPC_SYMBOL  20
#define SURF_NPC_ENEMY   21
#define SURF_NPC_BOSS    22
#define SURF_NPC_REGULAR 23


#define SURF_TEXTBOX	 26
#define SURF_FACE		 27
#define SURF_BACK		 28
#define SURF_VALUEVIEW	 29
#define SURF_TEXTSCRIPT	 30//~34(5)

#define SURF_STRIPPER    35
#define SURF_ILLUST      36
#define SURF_CASTS       37


//音


#define WAVE_CURSOR		1
#define WAVE_MESSAGE	2
#define WAVE_HEAD		3
#define WAVE_CHANGEARMS	4
#define WAVE_YESNO		5
#define WAVE_NPCJUMP2	6
#define WAVE_DUMMY      7


#define WAVE_DOOR		11
#define WAVE_BREAK1		12
#define WAVE_HITBLOCK	13// 壁に当たる
#define WAVE_GETEXP		14
#define WAVE_JUMP		15
#define WAVE_MY_DAMAGE	16
#define WAVE_MY_DESTROY	17
#define WAVE_OK			18
//#define WAVE_CAMP		19
#define WAVE_LIFE		20
#define WAVE_SPLASH		21
#define WAVE_KEYBREAK	22
#define WAVE_STEP		23
#define WAVE_RUN		24
#define WAVE_BUNRET		25
#define WAVE_QUAKE		26
#define WAVE_LEVELUP	27
#define WAVE_SMOKE		28
#define WAVE_WARP   	29
#define WAVE_NPCJUMP1   30
#define WAVE_NODAMAGE 	31
#define WAVE_POLESTAR	32
#define WAVE_FRONTIA	33
#define WAVE_FIRE		34
#define WAVE_EXPLOSION	35
#define WAVE_ARMS_EMPTY	37
#define WAVE_GETITEM	38
#define WAVE_POP		39
#define WAVE_RIVER1		40
#define WAVE_RIVER2		41
#define WAVE_ADDBULLET	42
#define WAVE_ON			43
#define WAVE_BOM		44
#define WAVE_EXPHOP		45
#define WAVE_SONIC		46
#define WAVE_PATA		47
#define WAVE_BUBBLIN	48
#define WAVE_POLEST2	49
#define WAVE_NPC_SMALL	50
#define WAVE_NPC_MIDDLE	51
#define WAVE_NPC_LARGE	52
#define WAVE_NPC_INSECT	53
#define WAVE_NPC_GOHST  54
#define WAVE_NPC_ENGEL  55
#define WAVE_SPLASH2    56
#define WAVE_GIRLDMG    57
#define WAVE_PROPERA    58
#define WAVE_CHARGE_LV1 59
#define WAVE_CHARGE_LV2 60
#define WAVE_CHARGE_LV3 61
#define WAVE_SPUR_LV1   62
#define WAVE_SPUR_LV2   63
#define WAVE_SPUR_LV3   64
#define WAVE_CHARGE_MAX 65
#define WAVE_DESTROY_SMALL	70
#define WAVE_DESTROY_MIDDLE	71
#define WAVE_DESTROY_LARGE	72
#define WAVE_SPINE   100
#define WAVE_THUNDER 101
#define WAVE_BAIT    102
#define WAVE_CHARGE  103
#define WAVE_SECRET  104
#define WAVE_BOWWOW  105
#define WAVE_SWORD   106
#define WAVE_STONE   107
#define WAVE_BIGJUMP 108
#define WAVE_HERI    109
#define WAVE_HERI2   110
#define WAVE_CATA    111
#define WAVE_CATA2   112
#define WAVE_BOOST   113
#define WAVE_ALMONDDMG 114
#define WAVE_COREOPEN  115
#define WAVE_COREPOW   116
#define WAVE_NEMESIS   117


#define WAVE_ORG_DRAM 150

//パレット予約
#define OFFSET_PALETTE_MINIMAP		0x00
#define OFFSET_PALETTE_GAMESYSTEM	0x10	
#define OFFSET_PALETTE_MYCHAR		0x20
#define OFFSET_PALETTE_NPCHAR		0x30//~0x40/0x50/0x60
#define OFFSET_PALETTE_CARET		0x70
#define OFFSET_PALETTE_ITEM			0x80
#define OFFSET_PALETTE_SPECIAL		0x90
#define OFFSET_PALETTE_MAPPARTS		0xB0
#define OFFSET_PALETTE_BACK			0xC0

//BULLETコード
#define BULLET_FRONTIA1		1
#define BULLET_FRONTIA2		2
#define BULLET_FRONTIA3		3
#define BULLET_POLESTAR1	4
#define BULLET_POLESTAR2	5
#define BULLET_POLESTAR3	6
#define BULLET_FIREBALL1	7
#define BULLET_FIREBALL2	8
#define BULLET_FIREBALL3	9
#define BULLET_MACHINEGUN1	10
#define BULLET_MACHINEGUN2	11
#define BULLET_MACHINEGUN3	12
#define BULLET_MISSILE1		13
#define BULLET_MISSILE2		14
#define BULLET_MISSILE3		15
#define BULLET_BOM1			16
#define BULLET_BOM2			17
#define BULLET_BOM3			18
#define BULLET_BUBBLIN1     19
#define BULLET_BUBBLIN2     20
#define BULLET_BUBBLIN3     21
#define BULLET_SPINE        22
#define BULLET_EDGE         23
#define BULLET_DROP         24
#define BULLET_SWORD1       25
#define BULLET_SWORD2       26
#define BULLET_SWORD3       27
#define BULLET_SUPERMISSILE1 28
#define BULLET_SUPERMISSILE2 29
#define BULLET_SUPERMISSILE3 30
#define BULLET_SUPERBOM1	 31
#define BULLET_SUPERBOM2	 32
#define BULLET_SUPERBOM3	 33
#define BULLET_NEMESIS1 	 34
#define BULLET_NEMESIS2 	 35
#define BULLET_NEMESIS3 	 36
#define BULLET_SPUR1 		 37
#define BULLET_SPUR2 	     38
#define BULLET_SPUR3 		 39
#define BULLET_SPURTAIL1 	 40
#define BULLET_SPURTAIL2 	 41
#define BULLET_SPURTAIL3 	 42
#define BULLET_NEMESIS_C 	 43
#define BULLET_ENEMYCLEAR	 44
#define BULLET_STAR 		 45


//カレットコード
#define CARET_SPLASH	1
#define CARET_SMOKE		2
#define CARET_FLASH		3
#define CARET_XXXXX2	4
#define CARET_ZZZ		5
//#define CARET_BOTAN		6
#define CARET_MISSILE	7
#define CARET_WATERDEAD	8
#define CARET_QUESTION	9
#define CARET_LEVELUP	10
#define CARET_DAMAGE	11
#define CARET_BIGSMOKE	12
#define CARET_SMALLSTAR	13
#define CARET_BOM		14
#define CARET_BUBBLIN	15
#define CARET_EMPTY     16

#define NPCSIZE_SMALL	1
#define NPCSIZE_MIDDLE	2
#define NPCSIZE_LARGE	3




#define PARTSSIZE	16

//DIRECTION
#define DIR_LEFT	0
#define DIR_UP		1
#define DIR_RIGHT	2
#define DIR_DOWN	3
#define DIR_CENTER	4

//Attribute
//＜非表示 0x00＞
#define ATRB_DISABLE	0x00
//＜奥0x01-0x3F＞
#define ATRB_BACK		0x01
#define ATRB_BACK_W		0x02
#define ATRB_BACK_EB	0x03
#define ATRB_BACK_EB_W	0x04
#define ATRB_BACK_BULTH	0x05 // Bulletだけ通す
//＜前0x40-0x5F＞
#define ATRB_FRONT		0x40
#define ATRB_BLOCK		0x41
#define ATRB_DAMAGE		0x42
#define ATRB_SNACK		0x43
#define ATRB_EBLOCK		0x44
#define ATRB_BLIND		0x45
#define ATRB_BLOCKME    0x46

#define ATRB_TRI_A		0x50
#define ATRB_TRI_B		0x51
#define ATRB_TRI_C		0x52
#define ATRB_TRI_D		0x53
#define ATRB_TRI_E		0x54
#define ATRB_TRI_F		0x55
#define ATRB_TRI_G		0x56
#define ATRB_TRI_H		0x57
//＜水0x60-0x7F＞
#define ATRB_FRONT_W	0x60
#define ATRB_BLOCK_W	0x61
#define ATRB_DAMAGE_W	0x62
#define ATRB_SNACK_W	0x63
#define ATRB_EBLOCK_W	0x64
#define ATRB_BLIND_W	0x65

#define ATRB_TRI_A_W	0x70
#define ATRB_TRI_B_W	0x71
#define ATRB_TRI_C_W	0x72
#define ATRB_TRI_D_W	0x73
#define ATRB_TRI_E_W	0x74
#define ATRB_TRI_F_W	0x75
#define ATRB_TRI_G_W	0x76
#define ATRB_TRI_H_W	0x77
//＜流0x80-0xBF＞
#define ATRB_LEFT		0x80
#define ATRB_UP			0x81
#define ATRB_RIGHT		0x82
#define ATRB_DOWN		0x83

#define ATRB_LEFT_W		0xA0
#define ATRB_UP_W		0xA1
#define ATRB_RIGHT_W	0xA2
#define ATRB_DOWN_W		0xA3
//＜  0xC0-0xFF＞


//あたり判定フラグ
#define FLAG_HIT_LEFT		0x00001
#define FLAG_HIT_TOP		0x00002
#define FLAG_HIT_RIGHT		0x00004
#define FLAG_HIT_BOTTOM		0x00008
#define FLAG_HIT_LDOWN		0x00010
#define FLAG_HIT_RDOWN		0x00020
#define FLAG_HIT_LUP		0x00040
#define FLAG_HIT_RUP		0x00080
#define FLAG_HIT_X			0x000ff
#define FLAG_HIT_WATER		0x00100
#define FLAG_HIT_BLOCK		0x00200
#define FLAG_HIT_DAMAGE		0x00400
#define FLAG_RED_WATER		0x00800
#define FLAG_HIT_VECT_LEFT	0x01000
#define FLAG_HIT_VECT_UP	0x02000
#define FLAG_HIT_VECT_RIGHT	0x04000
#define FLAG_HIT_VECT_DOWN	0x08000

#define FLAG_SET_TRI_E		0x10000
#define FLAG_SET_TRI_F		0x20000
#define FLAG_SET_TRI_G		0x40000
#define FLAG_SET_TRI_H		0x80000

// 装備フラグ
#define EQUIP_BOOST         0x0001
#define EQUIP_MAPSYSTEM     0x0002
#define EQUIP_ARMSBARRIER   0x0004
#define EQUIP_TURBOCHARGE   0x0008
#define EQUIP_AIR           0x0010
#define EQUIP_BOOST2        0x0020
#define EQUIP_MIMIGAH       0x0040
#define EQUIP_STAR          0x0080
#define EQUIP_COUNTER       0x0100

//NPC属性フラグ
#define BITS_BLOCK_MYCHAR	0x0001	//MyCharのすり抜けが不可
#define BITS_THROW_EBLOCK	0x0002	//E-Blockすり抜け可
#define BITS_BLOCK_BULLET	0x0004	//武器のすり抜けが不可
#define BITS_THROW_BLOCK	0x0008	//壁をすり抜けることが出きる。
#define BITS_BOUND_MYCHAR	0x0010	//上に乗ったらバウンド
#define BITS_BANISH_DAMAGE	0x0020	//ダメージにより消滅
#define BITS_BLOCK_MYCHAR2	0x0040	//MyCharのすり抜けが不可
#define BITS_DAMAGE_SIDE	0x0080	//MyCharサイドのみダメージ

#define BITS_EVENT_HIT		0x0100	//触れるとイベントが発生
#define BITS_EVENT_BREAK	0x0200	//破壊するとイベントが発生
#define BITS_EVENT_ALIVE	0x0400	//存在するとイベントが発生
#define BITS_FLAG_ALIVE		0x0800	//対応フラグが立っていれば配置される
#define BITS_DIRECT_RIGHT	0x1000	//右向きスタート
#define BITS_EVENT_CHECK	0x2000	//調べるとイベントが発生
#define BITS_FLAG_DEAD		0x4000	//対応フラグが倒れていれば配置される
#define BITS_VIEWDAMAGE		0x8000	//ダメージが表示される

#define BITS_ACTION			0x80ff	//アクション関係のBITS

//バレット属性フラグ
#define BBITS_BREAKBLOCK	0x0001
#define BBITS_SHOWER		0x0002
#define BBITS_THROWMAP		0x0004
#define BBITS_REPLACE		0x0008
#define BBITS_NOBREAK		0x0010
#define BBITS_BLOCKBUSTER	0x0020
#define BBITS_BLOCKERASE	0x0040

//状態フラグ
#define COND_CHECK				0x00000001//調べている
#define COND_HIDE				0x00000002//表示されない
#define COND_RUN				0x00000004//走りました(効果音)
#define COND_LOSE				0x00000008//→消滅
#define COND_ZEROINDEXDAMAGE	0x00000010//ゼロ番目のキャラにダメージ（ボス用）
#define COND_FLOW				0x00000020//流れ中（地面抵抗をゼロに）
#define COND_ALIVE				0x00000080//存在する

//スクリプトのモード
#define SCRIPT_SLEEP	0//スクリプト無起動
#define SCRIPT_PRIORITY	1//アクションを停止
#define SCRIPT_FREERUN	2//テキストがかってに流れる。

#define MAX_MOVE	( 3 * VS - 1 )
#define MAX_NPC		512
#define MAX_BULLET	64
#define MAX_EXP		9999
#define MAX_AIR		1000
#define MAX_BOSS_PARTS	20

#define MAX_MYSHOPLIST 64

#define MAX_FLAG	( 8*1000 )
#define MAX_ARMSDATA	8
#define MAX_ITEMDATA	(32)
#define MAX_PERMITSTAGE	8
#define MAX_MAPPING		128

//アイテムフラグ
#define ITEMBITS_IMPORTANT	0x01//捨てられない

//マイキャラ
#define MYUNIT_NORMAL		0
#define MYUNIT_STREAM		1


#define SPECIAL_NO			0
#define SPECIAL_ALMOND		1
#define SPECIAL_GRAVITY		2


#define WAIT_NPC_DAMAGE		16

//テキストカラー
#define COLOR_TEXTBACK		0	//文字背景カラー
#define COLOR_TEXTSHADOW	RGB(0x11,0x00,0x22)	//文字影
#define COLOR_TEXT			RGB(0xff,0xff,0xfe) //文字色

#define FLASHMODE_EXPLOSION		1
#define FLASHMODE_FLASH			2
#define FLASHMODE_WHITEOUT		3

// 背景アクション
#define BKACT_FIXED 0
#define BKACT_SCRHF 1
#define BKACT_SCROL 2
#define BKACT_WATER 3
#define BKACT_BLACK 4
#define BKACT_FLOW  5
#define BKACT_CROUD 6
#define BKACT_CROU2 7

// ノイズ
#define NOISE_RIVER   1
#define NOISE_PROPERA 2

#define GAMEFLAG_ACTION     0x0001
#define GAMEFLAG_KEYCONTROL 0x0002
#define GAMEFLAG_TEXTSCRIPT 0x0004
#define GAMEFLAG_ENDING     0x0008


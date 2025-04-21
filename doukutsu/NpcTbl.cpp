#include <windows.h>
#include <stdio.h>
#include "Tags.h"
#include "Define.h"


/*
NPCTABLE gNpcTable[] = {
	//NPC000 ダミー（停止）
	{	BITS_THROW_BLOCK,                            //BITS
		1000, SURF_NPC_SYMBOL, 0, 0, NPCSIZE_MIDDLE, // life, surf, voice1, voice2, size
		0, 0,  4,3,4,8,  8,8,8,8,                    // exp, damage, hit[4], view[4]
	},
	//NPC001 exp object
	{	BITS_EXP,                                   //BITS
		   1, SURF_NPC_SYMBOL, 0, 0, NPCSIZE_MIDDLE,// life, surf, voice1, voice2, size
		1, 0,  4,3,4,3,  8,8,8,8,					// exp, damage, hit[4], view[4]
	},
	//NPC002 ベへモス
	{	BITS_DAMAGE_SIDE|BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR|BITS_BOUND_MYCHAR,
		   8, SURF_NPC_ENEMY, WAVE_NPC_LARGE, WAVE_DESTROY_LARGE, NPCSIZE_LARGE,
		3, 1,     11,14,11,8,    16,16,16,8,
	},

	//NPC003 消滅待機オブジェクト
	{	BITS_THROW_BLOCK,
		   1, SURF_NPC_SYMBOL, 0, 0, NPCSIZE_MIDDLE,
		0, 0,     0,0,0,0,   0,0,0,0,
	},
	//NPC004 爆風
	{	0,
		   1,  SURF_NPC_SYMBOL, 0, 0, NPCSIZE_MIDDLE,
		0, 0,    4,4,4,4,   8,8,8,8,//view
	},

	//NPC005 クリッタ－
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR|BITS_THROW_EBLOCK,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		2,		2,		6,4,6,5,		8,8,8,8,
	},
	//NPC006 ビートル
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_INSECT,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		2,		2,		6,6,5,6,		8,8,8,8,
	},
	//NPC007 バジリスク
	{	BITS_BLOCK_BULLET,
		0,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		5,		100,		8,2,8,8,		16,8,16,8,
	},
	//NPC008 パゴット
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR|BITS_THROW_BLOCK,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_INSECT,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		2,		2,		6,6,5,6,		8,8,8,8,
	},
	//NPC009 バルログ（参上）
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR|BITS_THROW_BLOCK,
		96,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		18,		0,		14,11,14,12,		20,12,20,12,
	},
	//NPC010 バルログ（戦闘）
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		96,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		18,		0,		14,11,14,12,		20,12,20,12,
	},
	//NPC011 ホワイトバブル
	{	BITS_BANISH_DAMAGE,
		1,		SURF_NPC_SYMBOL,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		2,		4,4,4,4,		8,8,8,8,
	},
	//NPC012 バルログ(撤退)
	{	0,
		1,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		18,		0,		14,11,14,12,		20,12,20,12,
	},
	//NPC013 シールド
	{	BITS_BLOCK_BULLET|BITS_BLOCK_MYCHAR2,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		1,		8,8,8,8,		8,8,8,8,
	},
	//NPC014 カギ
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,6,5,6,		8,8,8,8,
	},
	//NPC015 宝箱
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,6,5,8,		8,8,8,8,
	},
	//NPC016 セーブポイント
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,6,5,8,		8,8,8,8,
	},
	//NPC017 エネルギーポイント
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,6,5,8,		8,8,8,8,
	},
	//NPC018 扉
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,8,5,8,		8,16,8,8,
	},
	//NPC019 バルログ（壁破り）
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		96,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		18,		0,		14,11,14,12,		20,12,20,12,
	},

	//NPC020 パソコン
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		16,16,16,8,
	},
	//NPC021 空箱
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,0,5,8,		8,0,8,8,
	},

	//NPC022 転送装置
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		10, 1,10,8,		12,24,12,8,
	},
	//NPC023 転送装置ランプ
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		12, 4,12,0,		12, 4,12,0,
	},
	//NPC024 パワークリッタ－
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		16,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		7,		2,		 9, 6, 9, 7,		12,12,12,12,
	},
	//NPC025 エレベーター
	{	BITS_BLOCK_MYCHAR|BITS_THROW_BLOCK|BITS_BLOCK_BULLET,//BITS
	//	BITS_BLOCK_MYCHAR2|BITS_THROW_BLOCK,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		14, 9,14, 8,		16, 8,16, 8,
	},
	//NPC026 バット（滞空型）
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		2,		4,7,4,4,		8,8,8,8,
	},
	//NPC027 デストラップ
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		127,		8, 8,24,8,		8,16,24,8,
	},
	//NPC028 クリッター２
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		5,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		3,		2,		6,4,6,5,		8,8,8,8,
	},
	//NPC029 クトゥルー
	{	0,
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		4,4,4,4,		8,16,8,8,
	},
	//NPC030 モンク
	{	 0,
		 0,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_MIDDLE,
		 0,		0,		6,6,6,6,		8,8,8,8,
	},
	//NPC031 バット（天井型）
	{	BITS_BANISH_DAMAGE,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		2,		4,7,4,4,		8,8,8,8,
	},
	//NPC032 MAX体力増加
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,8,6,6,		8,8,8,8,
	},
	//NPC033 バブルホッパー
	{	BITS_BANISH_DAMAGE,
		1,		SURF_NPC_BOSS,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		2,		4,4,4,4,		8,8,8,8,
	},
	//NPC034 ベッド
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		16,8,16,8,
	},
	//NPC035 マンナン
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		100,		SURF_NPC_ENEMY,		WAVE_NPC_GOHST,		0,		NPCSIZE_SMALL,
		2,		3,		4,4,4,20,		12,8,12,24,
	},
	//NPC036 バルログ（戦闘LV2）
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		240,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		18,		0,		14,11,14,12,		20,12,20,12,
	},
	//NPC037 看板
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,0,5,8,		8,8,8,8,
	},
	//NPC038 炎
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,0,5,8,		8,8,8,8,
	},
	//NPC039 案内
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,0,5,8,		8,8,8,8,
	},
	//NPC040 サンタ
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC041 ミミガー村の壊れた入り口
	{	0,
		0,		SURF_PARTS,		0,		0,		0,
		0,		0,		5,8,5,8,		24,24,24,8,
	},
	//NPC042 坂本スー
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},

	//NPC043 黒板／作業机
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		12, 8,12,8,		20,24,20,8,
	},
	//NPC044 サンバッグ
	{	BITS_BANISH_DAMAGE,
		120,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		12,		10,		12,12,12,12,		16,16,16,16,
	},
	//NPC045 サンビッツ
	{	BITS_BANISH_DAMAGE,
		2,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		2,		3,		5,5,5,5,		8,8,8,8,
	},
	//NPC046 イベントキーパー
	{	0,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,6,6,6,		8,8,8,8,
	},
	//NPC047 ワニ
	{	BITS_BANISH_DAMAGE,
		30,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_MIDDLE,
		9,		0,		7,24,7, 8,		24,24,24,8,
	},
	//NPC048 オメガクラッカー
	{	BITS_BANISH_DAMAGE,
		1,		SURF_NPC_BOSS,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		1,		4,		4,4,4,4,		8,8,8,8,
	},
	//NPC049 ボーンヘッド
	{	BITS_BANISH_DAMAGE,
		20,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_LARGE,
		10,		8,		14, 8,14,4,		16,16,16,8,
	},

	//NPC50 ボーン
	{	BITS_BANISH_DAMAGE,
		1,		SURF_NPC_ENEMY,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		4,		4, 4, 4, 4,		8, 8, 8, 8,
	},

	//NPC051 クロウ（With ボーンヘッド）
	{	BITS_BANISH_DAMAGE,
		20,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		9,		4,		 8, 8, 8, 8,		16,16,16,16,
	},
	//NPC052 巡回ロボ
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC053 ボーンフット
	{	BITS_BANISH_DAMAGE,//BITS_BLOCK_BULLET,
		10,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_SMALL,		NPCSIZE_MIDDLE,
		2,		8,		10, 4, 8,4,		12, 8,12,8,
	},

	//NPC054 ボーンステップ
	{	BITS_BANISH_DAMAGE,
		20,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_LARGE,
		10,		8,		14, 8,14,4,		16,16,16,8,
	},
	//NPC055 坂本カズマ
	{	0,//ITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		5, 8,5,8,		8,16,8,8,
	},
	//NPC056 ビートル２
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_INSECT,	WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		2,		2,		6,6,5,6,		8,8,8,8,
	},
	//NPC057 クロウ（突撃）
	{	BITS_BANISH_DAMAGE,
		20,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		9,		4,		 8, 8, 8, 8,		16,16,16,16,
	},
	//NPC058 パゴット=バス
	{	BITS_BANISH_DAMAGE,
		15,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,	WAVE_DESTROY_MIDDLE,		NPCSIZE_LARGE,
		9,		0,		6,6,6,6,		12,12,12,12,
	},
	//NPC059 ザ・ドア
	{	BITS_BANISH_DAMAGE,//BITS
		6,		SURF_NPC_SYMBOL,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_MIDDLE,
		2,		4,		5,8,5,8,		8,16,8,8,
	},
	//NPC060 トロ子
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		4,8,4,8,		8,8,8,8,
	},
	//NPC061 キング
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		6,8,5,8,		8,8,8,8,
	},
	//NPC062 PCカズマ
	{	BITS_THROW_BLOCK,//BITS
		0,	SURF_NPC_REGULAR,	0,	0,	0,
		0,	0,	6, 8,6,8,	8,16,8,8,
	},
	//NPC063 トロ子 攻撃
	{	BITS_THROW_BLOCK,//BITS
		100,	SURF_NPC_REGULAR,	WAVE_NPC_SMALL,	WAVE_DESTROY_SMALL,	NPCSIZE_MIDDLE,
		0,	1,	6,8,6,8,	8,8,8,8,
	},
	//NPC064 クリッタ－ LV0
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		2,	SURF_NPC_ENEMY,	WAVE_NPC_MIDDLE,	WAVE_DESTROY_MIDDLE,	NPCSIZE_MIDDLE,
		1,	1,	6,4,6,5,	8,8,8,8,
	},
	//NPC065 バット（滞空型）LV0
	{	BITS_BANISH_DAMAGE,
		1,	SURF_NPC_ENEMY,	WAVE_NPC_SMALL,	WAVE_DESTROY_SMALL,	NPCSIZE_SMALL,
		1,	1,	4,7,4,4,	8,8,8,8,
	},
	//NPC066 トロ子カプセル
	{	BITS_THROW_BLOCK,//BITS
		0,	SURF_NPC_REGULAR,	WAVE_NPC_SMALL,	WAVE_DESTROY_SMALL,	NPCSIZE_MIDDLE,
		0,	1,	5,8,5,8,	12,12,12,12,
	},
	//NPC067 ミザリー
	{	0,//BITS
		0,	SURF_NPC_REGULAR,	WAVE_NPC_SMALL,	WAVE_DESTROY_SMALL,	NPCSIZE_MIDDLE,
		0,	1,	5,8,5,8,	8,8,8,8,
	},
	//NPC068 ポーラ LV0
	{	BITS_BANISH_DAMAGE,//|BITS_BLOCK_MYCHAR,
		60,	SURF_NPC_BOSS,	WAVE_NPC_LARGE,	WAVE_DESTROY_LARGE,	NPCSIZE_LARGE,
		18,	0,	14,11,14,12,	20,12,20,12,
	},

	//NPC069 ピニョン
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		2,	SURF_NPC_ENEMY,	WAVE_NPC_SMALL,	WAVE_DESTROY_MIDDLE,	NPCSIZE_MIDDLE,
		0,	1,	6,5,6,5,
		8,8,8,8,
	},
	//NPC070 ブリンク
	{	0,
		0,	SURF_NPC_SYMBOL,	0,	0,	0,
		0,	0,	5,0,5,8,	8,8,8,8,
	},
	//NPC071 チン魚
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		2,		SURF_NPC_ENEMY,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		1,		6,4,6,4,		8,8,8,8,
	},
	//NPC072 水撒き機
	{	0,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC073 水飛沫
	{	0,//BITS
		0,		SURF_CARET,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		1,1,1,1,		0,0,0,0,
	},
	//NPC74 ジャック
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC75 ルアー
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		 5,8,5, 8,		16,8,8,16,
	},
	//NPC76 プランツ
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC77 アンブレラ
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		 5, 8, 5,8,		20,24,28,8,
	},
	//NPC78 つぼ
	{	0,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC79 ハム
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	  
	//NPC080 包丁ゾンビ
	{	BITS_BLOCK_BULLET,
		20,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		4,		2,		4,12,4,8,		16,16,8,8,
	},
	//NPC081 ジャイアントピニョン
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		12,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		0,		1,		 8, 8, 8,12,		12,12,12,12,
	},
		
	//NPC082 ミザリー
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_MIDDLE,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
		  
	//NPC083 イゴール(イベント)
	{	0,
		10,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_LARGE,
		0,		2,		 8, 1, 8,16,		20,20,20,20,
	},
  	//NPC084 超音波
	{	0,
		1,		SURF_NPC_ENEMY,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		1,		2,2,2,2,		8,8,8,8,
	},
	//NPC085 タマゴモニター
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		 5, 8,5,8,		 8,16,8,8,//view
	},
			  
	//NPC086 bullet object
	{	0,
		1,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		1,		0,		6,6,6,6,		8,8,8,8,
	},
	//NPC087 life object
	{	0,
		2,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		2,		0,		6,6,6,6,		8,8,8,8,
	},
	
	//NPC088 イゴール(戦闘)
	{	BITS_BANISH_DAMAGE,
		300,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		1,		0,		 8,16, 8,16,		20,20,20,20,
	},
	//NPC089 イゴール(やられ)
	{	0,
		300,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		0,		0,		 8,16, 8,16,		20,20,20,20,
	},
	//NPC090 イゴール(うなだれ)
	{	0,
		0,		SURF_NPC_BOSS,		0,		0,		0,		0,		0,		4,8,4,8,		8,16,8,8,
	},
	//NPC091 檻
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		4,8,4,8,		16,16,16,8,
	},
	//NPC092 PCスー
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		5, 8,5,8,		8,16,8,8,
	},
	//NPC093 チャコ
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		5, 8, 5,8,		8, 8, 8,8,
	},
	//NPC094 クララ
	{	BITS_BANISH_DAMAGE,
		50,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		15,		5,		16,7,16,7,		24,12,24,12,
	},
	//NPC095 ヤブクラゲ
	{	BITS_BANISH_DAMAGE,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_INSECT,		WAVE_SPLASH,		NPCSIZE_LARGE,
		2,		5,		6,6,6,6,		8,8,8,8,
	},

	//NPC096 ホバー
	{	BITS_BLOCK_MYCHAR2,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,9,8,8,		8,8,8,8,
	},
	//NPC097 ホバー
	{	BITS_BLOCK_MYCHAR2,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,9,8,8,		8,8,8,8,
	},
	//NPC098 ホバー
	{	BITS_BLOCK_MYCHAR2,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,9,8,8,		8,8,8,8,
	},
	//NPC099 ホバー
	{	BITS_BLOCK_MYCHAR2,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,9,8,8,		8,8,8,8,
	},

	//NPC100 通気口
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//NPC101 電源スイッチ
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//NPC102 電源ビリビリ
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//NPC103 ソニック
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_MIDDLE,
		0,		3,		3,8,3,8,		8,12,8,12,
	},
	//NPC104 カエル
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		10,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		9,		3,		8,9,8,10,		16,16,16,16,
	},
	//NPC105 お～い
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//NPC106 お～いコール
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//NPC107 マルコ
	{	0,		20,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		5,5,5,8,		8,16,8,8,
	},
	//NPC108 フロッギーバブル
	{	BITS_BANISH_DAMAGE,
		1,		SURF_NPC_ENEMY,		WAVE_SPLASH,		WAVE_SPLASH,		NPCSIZE_SMALL,
		0,		4,		4,4,4,4,		8,8,8,8,
	},
	//NPC109 ひしゃげマルコ
	{	0,
		20,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		5,5,5,8,		8,16,8,8,
	},
	//NPC110 小カエル
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		2,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_MIDDLE,
		2,		1,		4,4,4,5,		8,8,8,8,
	},
	//NPC111 転送マイキャラ
	{	0,
		1,		SURF_MYCHAR,		0,		0,		NPCSIZE_LARGE,
		0,		0,		6,6,5,8,		8,8,8,8,
	},
	//NPC112 転送マイキャラ２
	{	0,		1,		SURF_MYCHAR,		0,		0,		NPCSIZE_LARGE,
		0,		0,		6,6,5,16,		8,8,8,8,
	},
	//NPC113 ブースター博士
	{	0,
		1,
		SURF_NPC_REGULAR,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		6,6,5,8,
		8,8,8,8,
	},
	//NPC114 ザ・プレス
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR2,//BITS
		32,
		SURF_NPC_SYMBOL,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		5,
		0,
		8,13,8,12,
		8,12,8,12,
	},
	//NPC115 Ravil
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,//BITS
		40,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_MIDDLE,
		10,
		0,
		8,8,8,8,
		12,16,12,8,
	},
	//NPC116 赤い花びら
	{	0,//BITS
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		0,
		0,
		20,8,20,8,
		24,8,24,8,
	},
	//NPC117 カーリー待機
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		6, 6, 6, 8,
		8, 8, 8, 8,
	},
	//NPC118 カーリー戦闘
	{	BITS_BANISH_DAMAGE,//BITS
		320,
		SURF_NPC_BOSS,
		WAVE_GIRLDMG,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 6, 6, 6, 8,
		16,16,16,16,
	},
	//NPC119 丸テーブル
	{	0,//BITS
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 6, 6, 6, 8,
		12, 8,12, 8,
	},
	//NPC120 コロンズ（後ろ/座り）
	{	0,//BITS
		0,
		SURF_NPC_BOSS,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 6, 6, 6, 8,
		 8, 8, 8, 8,
	},
	//NPC121 コロンズ（ひだり／眠り）
	{	0,//BITS
		0,
		SURF_NPC_BOSS,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 6, 6, 6, 8,
		 8, 8, 8, 8,
	},
	//NPC122 コロンズ戦闘
	{	0,//BITS
		0,
		SURF_NPC_BOSS,
		WAVE_NPC_SMALL,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 6, 6, 6, 8,
		 8, 8, 8, 8,
	},

	//NPC123 カーリーマシンガン
	{	0,//BITS
		0,
		SURF_NPC_BOSS,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		6,
		4, 4, 4, 4,
		8, 8, 8, 8,
	},
	//NPC124 サンブロック
	{	BITS_BLOCK_BULLET|BITS_BLOCK_MYCHAR2,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		16,17,16,16,
		16,16,16,16,
	},

	//NPC125 secret energy
	{	BITS_BANISH_DAMAGE,
		1000,		SURF_NPC_SYMBOL,		WAVE_SECRET,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,6,6,6,		8,8,8,8,
	},
	//NPC126 ゴン太(ラン)
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		4,8,4,8,		8,8,8,8,
	},
	//NPC127 マシンガンLevel2残像
	{	BITS_THROW_BLOCK,//BITS
		1,		SURF_CARET,		0,		0,		0,
		0,		0,		0,0,0,0,		8,8,8,8,
	},
	//NPC128 マシンガンLevel3残像
	{	BITS_THROW_BLOCK,//BITS
		1,		SURF_CARET,		0,		0,		0,
		0,		0,		0,0,0,0,		8,8,8,8,
	},
	//NPC129 ファイアボール残像
	{	BITS_THROW_BLOCK,//BITS
		1,		SURF_CARET,		0,		0,		0,
		0,		0,		0,0,0,0,		8,8,8,8,
	},

	//NPC130 ゴン太（フリフリ）
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		4,8,4,8,		8,8,8,8,
	},
	//NPC131 ゴン太(睡眠)
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		4,8,4,8,		8,8,8,8,
	},

	//NPC132 ゴン太（ワンワン）
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		4,8,4,8,		8,8,8,8,
	},
	//NPC133 ジェンカ
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		6,8,6,8,		8,8,8,8,
	},

	//NPC134 アーマー
	{	BITS_BANISH_DAMAGE,
		20,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_MIDDLE,
		5,		6,		8,4,8,8,		16,8,16,8,
	},
	//NPC135 スケルトン
	{	BITS_BANISH_DAMAGE,
		40,		SURF_NPC_ENEMY,		WAVE_NPC_GOHST,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		20,		6,		8,13,8,8,		16,24,16,8,
	},
	//NPC136 ゴン太（おんぶ）
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		0,0,0,0,		8,8,8,8,
	},
	//NPC137 門
	{	0,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,6,6,6,		16,24,16,8,
	},

	//NPC138 扉
	{	0,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,6,6,6,		8,16,8,8,
	},

	//NPC139 ドクター
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		NPCSIZE_LARGE,
		0,		0,		6,6,6,16,		16,16,8,16,
	},

	//NPC140 トロ子狂暴化
	{	0,//BITS
		500,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		1,		5,		8,8,8,13,		16,16,16,16,
	},
	//NPC141 トロ子岩
	{	BITS_BLOCK_BULLET,//BITS
		1,		SURF_NPC_ENEMY,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		6,
		10,
		8,8,8,8,
		8,8,8,8,
	},
	//NPC142 プチローズ
	{	BITS_BANISH_DAMAGE,
		1,
		SURF_NPC_ENEMY,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		6,
		2,
		6,6,6,8,
		8,8,8,8,
	},
	//NPC143 ジェンカ（ころび）
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		6,8,6,8,
		8,8,8,8,
	},
	//NPC144 トロ子（召還）
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC145 つるぎ
	{	0,//BITS
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		6,6,6,6,
		8,8,8,8,
	},
	//NPC146 いなずま
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_CARET,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,230,6,6,		8,232,8,8,
	},
	//NPC147 クリッターLV3 （垂直ジャンプ）
	{	BITS_BANISH_DAMAGE,
		10,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_MIDDLE,
		4,
		8,
		4,4,4,5,
		8,8,8,8,
	},
  	//NPC148 クリッターバブル
	{	BITS_BANISH_DAMAGE,
		5,
		SURF_NPC_ENEMY,
		WAVE_SPLASH,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		0,
		5,
		4,4,4,4,
		4,4,4,4,
	},

  	//NPC149 悪魔の岩
	{	BITS_BLOCK_MYCHAR2|BITS_BLOCK_BULLET,
		1,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		16,17,16,14,
		16,16,16,16,
	},
	//NPC150 イベント用主人公
	{	0,
		1,
		SURF_MYCHAR,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		6,6,5,8,
		8,8,8,8,
	},
	//NPC151 黒ロボ
	{	0,
		1,
		SURF_NPC_REGULAR,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		6,6,5,8,
		8,8,8,8,
	},
	//NPC152 破壊ダミー
	{	BITS_BANISH_DAMAGE,
		40,
		SURF_NPC_SYMBOL,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_SMALL,
		0,
		0,
		 5,8,5,8,
		 8,8,8,8,//view
	},

  	//NPC153 ガウディ（徒歩）
	{	BITS_BANISH_DAMAGE,
		1000,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		8,
		5,
		8,11,8,5,
		12,16,12,8,
	},

  	//NPC154 ガウディ（やられ）
	{	0,
		1000,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		8,
		5,
		8,11,8,5,
		12,16,12,8,
	},

  	//NPC155 ガウディ（滞空）
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		1000,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		8,
		5,
		8,11,8,5,
		12,16,12,8,
	},
	//NPC156 ガウディバブル
	{	BITS_BANISH_DAMAGE,
		1,
		SURF_NPC_ENEMY,
		WAVE_SPLASH,
		WAVE_SPLASH,
		NPCSIZE_SMALL,
		1,
		3,
		4,4,4,4,
		8,8,8,8,
	},
  	//NPC157 悪魔の岩
	{	BITS_BLOCK_MYCHAR2|BITS_BLOCK_BULLET,
		1,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		16,17,16,14,
		16,16,16,16,
	},
	//NPC158 Ｘミサイル
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		1,
		SURF_NPC_BOSS,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_SMALL,
		5,
		5,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC159 バーストキャット
	{	BITS_THROW_BLOCK,
		1,
		SURF_NPC_BOSS,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_SMALL,
		0,
		0,
		4,4,4,4,
		24,36,24,36,
	},
	//NPC160 ダークマスター
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		300,
		SURF_NPC_BOSS,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		18,
		0,
		14,11,14,12,
		20,12,20,12,
	},

	//NPC161 ダークバブル
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		100,
		SURF_NPC_BOSS,
		WAVE_SPLASH,
		0,
		NPCSIZE_SMALL,
		0,
		5,
		6,6,6,6,
		8,8,8,8,
	},

	//NPC162 ダークマスター（消滅）
	{	BITS_THROW_BLOCK,
		500,
		SURF_NPC_BOSS,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		18,
		0,
		14,11,14,12,
		20,12,20,12,
	},

	//NPC163 ドクター毛呂
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		6,8,6,8,
		8,8,8,8,
	},

	//NPC164 ナース斜河
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		6,8,6,8,
		8,8,8,8,
	},

	//NPC165 ベッドカーリー
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		6,8,6,8,
		8,8,8,8,
	},

	//NPC166 ガウディレジ
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		8,8,8,8,
		20,16,20,8,
	},


	//NPC167 ブースター（座り／落下）
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		6,8,6,8,
		8,8,8,8,
	},

	//NPC168 大石
	{	0,//BITS
		0,
		SURF_NPC_BOSS,
		0,
		0,
		0,
		0,
		0,
		8,8,6,8,
		28,32,28,8,
	},

	//NPC169 バルログ フル
	{	BITS_BANISH_DAMAGE,//|BITS_BLOCK_MYCHAR,
		500, 
		SURF_NPC_BOSS,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		18,
		0,
		14,11,14,12,
		20,12,20,12,
	},
	//NPC170 ミサイル赤
	{	BITS_BANISH_DAMAGE,
		10,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		3,
		5,
		6,4,6,4,
		8,4,8,4,
	},
	//NPC171 魔旋風
	{	BITS_BLOCK_MYCHAR|BITS_BANISH_DAMAGE,
		50,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		20,
		10,
		4,12,4,12,
		16,16,16,16,
	},
	//NPC172 ファイアリング
	{	0,//BITS_BANISH_DAMAGE|BITS_BLOCK_BULLET,
		50,
		SURF_NPC_ENEMY,
		WAVE_FIRE,
		0,
		NPCSIZE_LARGE,
		10,
		5,
		4,12,4,12,
		8,16,8,16,
	},

  	//NPC173 ガウディ（カッター）
	{	BITS_BANISH_DAMAGE,
		1000,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		15,
		5,
		8,11,8,5,
		12,16,12,8,
	},

	//NPC174 ガウディカッター
	{	BITS_BLOCK_BULLET,
		1,
		SURF_NPC_ENEMY,
		WAVE_SPLASH,
		WAVE_SPLASH,
		NPCSIZE_SMALL,
		0,
		3,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC175 ガウディエッグ
	{	BITS_BANISH_DAMAGE,
		100,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		6,
		0,
		8,10,8,10,
		12,12,12,12,
	},
	//NPC176 コケ壷
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		1000,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		9,
		1,
		 4,12, 4,12,
		16, 8,16, 8,
	},
	//NPC177 コケ
	{	BITS_BANISH_DAMAGE,//|BITS_THROW_BLOCK,
		3,
		SURF_NPC_ENEMY,
		WAVE_NPC_INSECT,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		2,
		3,
		6,6,6,6,
		8,8,8,8,
	},
	//NPC178 アーモンドバブル
	{	BITS_THROW_BLOCK|BITS_BANISH_DAMAGE,
		6,
		SURF_NPC_BOSS,
		WAVE_SPLASH,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		1,
		3,
		6,6,6,6,
		8,8,8,8,
	},
	//NPC179 アーモンドオレンジ
	{	BITS_THROW_BLOCK|BITS_BANISH_DAMAGE,
		2,
		SURF_NPC_BOSS,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		5,
		3,
		6,4,6,4,
		8,8,8,8,
	},
	//NPC180 カーリー
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},
	//NPC181 カーリーマシンガン
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},

	//NPC182 カーリーポーラスター
	{	BITS_THROW_BLOCK,//BITS
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC183 カーリーバブル
	{	BITS_THROW_BLOCK,//BITS
		0,
		SURF_CARET,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		12,12,12,12,
	},
	//NPC184 バリケード大
	{	BITS_BLOCK_BULLET|BITS_BLOCK_MYCHAR2,//BITS
		0,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		16,17,16,16,
		16,16,16,16,
	},
	//NPC185 バリケード中
	{	BITS_BLOCK_BULLET|BITS_BLOCK_MYCHAR2,//BITS
		0,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		8,17,8,16,
		8,16,8,16,
	},
	//NPC186 バリケード小
	{	BITS_BLOCK_BULLET|BITS_BLOCK_MYCHAR2,//BITS
		0,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		8,9,8,8,
		8,8,8,8,
	},
	//NPC187 ウィルオウィプス
	{	BITS_BANISH_DAMAGE,
		10,
		SURF_NPC_ENEMY,
		WAVE_NPC_INSECT,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_SMALL,
		7,
		1,
		10,10,10,10,
		16,16,16,16,
	},

	//NPC188 ウィルオウィプス ミニ
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		8,
		SURF_NPC_ENEMY,
		WAVE_SPLASH,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		1,
		1,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC189 魂
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		1,
		1,
		4,4,4,4,
		4,12,4,4,
	},

	//NPC190 壊れロボ
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		1,
		0,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC191 水面操作
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		1,
		0,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC192 バイク
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		1,
		0,
		8,4,8,4,
		16,8,16,8,
	},
	//NPC193 壊れバイク
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		1,
		0,
		20,4,20,4,
		32,8,32,8,
	},
	//NPC194 バラバラロボ
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		1,
		0,
		8,4,8,4,
		16,4,16,4,
	},
	//NPC195 排水檻
	{	BITS_THROW_BLOCK,
		0,
		SURF_PARTS,
		0,
		0,
		0,
		1,
		0,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC196 Stream 上下
	{	BITS_THROW_BLOCK,
		0,
		SURF_PARTS,
		0,
		0,
		0,
		1,
		0,
		4,4,4,4,
		16,8,16,8,
	},
	//NPC197 はりせんぼん
	{	BITS_BANISH_DAMAGE,
		3,
		SURF_NPC_ENEMY,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		5,
		0,
		6,6,6,6,
		8,8,8,8,
	},
	//NPC198 ソニック
	{	0,
		6,
		SURF_NPC_BOSS,
		0,
		0,
		NPCSIZE_SMALL,
		0,
		3,
		6,10,6,10,
		8,12,8,12,
	},
	//NP199 旋風飛沫
	{	BITS_THROW_BLOCK,//BITS
		0,
		SURF_CARET,
		0,
		0,
		NPCSIZE_SMALL,
		0,
		0,
		1,1,1,1,
		1,1,1,1,
	},
	//NPC200 ブルードラゴン
	{	BITS_BANISH_DAMAGE,
		1000,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		20,
		10,
		12,18,12,12,
		20,24,20,16,
	},
	//NPC201 倒れブルードラゴン
	{	0,
		16,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		 0,
		 0,
		10, 16,10,12,
		20,24,20,16,
	},
	//NPC0202 ドラゴンバブル
	{	BITS_BANISH_DAMAGE,
		1,
		SURF_NPC_SYMBOL,
		0,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		0,
		10,
		6, 4,6,4,
		8,16,8,8,
	},
	//NPC203 クリッタ－ LV5
	{	BITS_BANISH_DAMAGE,
		4,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_MIDDLE,
		4,
		5,
		6,5,6,5,
		8,8,8,8,
	},
	//NPC204 落石（小）
	{	BITS_BLOCK_BULLET,
		4,
		SURF_PARTS,
		0,
		0,
		0,
		0,
		5,
		6,5,6,2,
		8,8,8,8,
	},
	//NPC205 落石大
	{	BITS_BLOCK_MYCHAR2|BITS_BLOCK_BULLET,//BITS
		80,
		SURF_PARTS,
		WAVE_SMOKE,
		WAVE_BREAK1,
		NPCSIZE_LARGE,
		0,
		0,
		8,17,8,8,
		8,16,8,8,
	},
	//NPC206 ボムカウンター
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		127,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_SMALL,
		7,
		1,
		10,10,10,10,
		20,20,20,20,
	},
	//NPC207 カウンター
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_SYMBOL,	0,	0,	NPCSIZE_SMALL,
		0,		0,		6,6,6,6,		8,8,8,8,
	},
	//NPC208 パゴット=バスLV2
	{	BITS_BANISH_DAMAGE,
		15,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		9,
		10,
		6,6,6,6,
		12,12,12,12,
	},
  	//NPC209 超音波
	{	0,
		1,
		SURF_NPC_ENEMY,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		0,
		5,
		2,2,2,2,
		8,8,8,8,
	},
	//NPC210 パゴットLV2
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		4,
		SURF_NPC_ENEMY,
		WAVE_NPC_INSECT,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_MIDDLE,
		2,
		5,
		6,6,5,6,
		8,8,8,8,
	},
	//NPC211 デストラップミニ
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		0,
		5,
		6,6,6,6,
		8,8,8,8,
	},
	//NPC212 飛竜
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		10,8,10,12,
		20,24,20,16,
	},
	//NPC213 ムーンゴースト
	{	0,
		64,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		22,
		0,
		 8,16, 8,16,
		24,24,24,24,
	},

	//NPC214 ムーンボール
	{	0,
		0,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_MIDDLE,
		0,
		5,
		6,3, 6,3,
		8,8,24,8,
	},
	//NPC215 ワニ
	{	BITS_BANISH_DAMAGE,
		30,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_MIDDLE,
		9,
		0,
		7,24,7, 8,
		24,24,24,8,
	},

	//NPC216 Pixel
	{	0,//ITS_THROW_BLOCK,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		5, 8,5,8,
		8,16,8,8,
	},
	//NPC217 伊藤
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC218 アーモンド波動
	{	BITS_THROW_BLOCK,//BITS
		 0,
		SURF_NPC_BOSS,
		 0,
		 0,
		 0,
		 0,
		20,
		12,12,12,12,
		16,16,16,16,
	},
	//NPC219 煙装置
	{	BITS_THROW_BLOCK,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		1,1,1,1,
		1,1,1,1,
	},

	//NPC220 ミミガー=スコップ(停止)
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC221 ミミガー=スコップ(歩く)
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC222 檻16x32
	{	BITS_BLOCK_MYCHAR2|BITS_BLOCK_BULLET,//BITS
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		0,
		0,
		5,16,5,16,
		8,16,8,16,
	},

	//NPC223 坂本 百鈴
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		5,16,5,8,
		8,16,8,8,
	},

	//NPC224 チエ
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC225 メガネ・爺
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC226 釣竿イベント
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,16,8,8,
	},

	//NPC227 バケツ
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},
	//NPC228 ドロール
	{	0,//BITS
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_LARGE,
		0,		0,		8,18,8,12,		16,24,16,16,
	},
	//NPC229 赤い花の芽
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 8, 8, 8, 8,
		24, 8,24, 8,
	},
	//NPC230 赤い花の葉
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 8, 8, 8, 8,
		24,24,24, 8,
	},

	//NPC231 ロケット
	{	BITS_BLOCK_MYCHAR2|BITS_BLOCK_BULLET,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		12, 6, 12, 8,
		16, 8,16, 8,
	},

	//NPC232 オレンジタン
	{	BITS_BANISH_DAMAGE,
		64,
		SURF_NPC_BOSS,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		9,
		2,
		10,10,10,10,
		16,16,16,16,
	},
	//NPC233 バット（オレンジタン付き）
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		4,
		SURF_NPC_BOSS,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		2,
		2,
		4,7,4,4,
		8,8,8,8,
	},
	//NPC234 刈られた赤い花
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 8, 8, 8, 8,
		24, 8,24, 8,
	},
	//NPC235 ミドリン
	{	BITS_BLOCK_MYCHAR|BITS_DAMAGE_SIDE|BITS_BANISH_DAMAGE,//BITS
		16,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_MIDDLE,
		2,
		3,
		5,4,5,8,
		8,8,8,8,
	},
	//NPC236 ガンフィッシュ
	{	BITS_BLOCK_MYCHAR|BITS_BANISH_DAMAGE,//BITS
		32,
		SURF_NPC_BOSS,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		20,
		0,
		11,8,11,8,
		12,12,12,12,
	},
	//NPC237 ガンウォーター
	{	0,//BITS
		0,
		SURF_NPC_BOSS,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		0,
		3,
		2,2,2,2,
		4,4,4,4,
	},
	//NPC238 ザ・プレスＨ
	{	BITS_BLOCK_BULLET|BITS_BLOCK_MYCHAR2|BITS_THROW_BLOCK,//BITS
		32,
		SURF_NPC_SYMBOL,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_MIDDLE,
		5,
		0,
		8,8,8,8,
		8,8,8,8,
	},
	//NPC239 折大
	{	BITS_THROW_BLOCK,//BITS
		0,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_SMALL,
		0,
		0,
		 2, 2, 2, 2,
		32, 24,32, 8,
	},

	//NPC240 ミミガー=囚われ(歩く)
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		7,8,7,8,
		8,8,8,8,
	},
	//NPC241 クリッタ－ Red
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		6,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_MIDDLE,
		1,
		6,
		6,4,6,5,
		8,8,8,8,
	},
	//NPC242バット（滞空型）Red
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		1,
		SURF_NPC_ENEMY,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		1,
		2,
		4,7,4,4,
		8,8,8,8,
	},
	//NPC243 バットRed 配置
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_SMALL,
		0,		0,		0, 0, 0, 0,		0, 0, 0, 0,
	},
	//NPC244 赤い水滴
	{	0,
		1,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_SMALL,
		0,		4,		2,7,2,4,		4,8,4,8,
	},
	//NPC245 赤い水滴配置
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_SMALL,
		0,		0,		2,7,2,4,		4,8,4,8,
	},
	//NPC246 ザ・プレス
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR2,//BITS
		32,		SURF_NPC_SYMBOL,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		5,		0,		8,13,8,12,		8,12,8,12,
	},
	//NPC247 ミザリー（戦闘）
	{	0,//BITS
		500,		SURF_NPC_BOSS,		WAVE_GIRLDMG,		0,		NPCSIZE_MIDDLE,
		20,		1,		5,8,5,8,		8,8,8,8,
	},
	//NPC248 ブラックバブル
	{	0,
		1,		SURF_NPC_BOSS,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		6,		4,4,4,4,		8,8,8,8,
	},
	//NPC249 ミザリー残像
	{	0,
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_MIDDLE,
		0,		1,		5,8,5,8,		8,8,8,8,
	},
	//NPC250 ブラックボール
	{	0,
		1,		SURF_NPC_BOSS,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		6,		4,4,4,4,		8,8,8,8,
	},
	//NPC251 ブラックサンダー
	{	0,
		1,		SURF_NPC_BOSS,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		10,		7,7,7,7,		8,16,8,16,
	},
	//NPC252 ブラックサークル→バット
	{	BITS_BLOCK_BULLET|BITS_THROW_BLOCK,
		2,		SURF_NPC_BOSS,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		6,		2,		4,4,4,4,		8,8,8,8,
	},
	//NPC253 EXPカプセル
	{	BITS_BANISH_DAMAGE,
		102,		SURF_NPC_SYMBOL,		WAVE_POP,		WAVE_BUNRET,		NPCSIZE_MIDDLE,
		0,		0,		6,4,6,4,		8,8,8,8,
	},
	//NPC254 ヘリ
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		32, 4,32, 4,		64,56,64, 8,
	},
	//NPC255 プロペラ
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,4,6,4,		56,8,56,8,
	},
	//NPC256 ドクター（背中）
	{	0,//BITS
		0,SURF_NPC_REGULAR,0,	0,	NPCSIZE_LARGE,
		0,	0,	6,6,6,16,	16,16,8,16,
	},
	//NPC257 赤い結晶
	{	BITS_THROW_BLOCK|BITS_BLOCK_BULLET,//BITS
		0,	SURF_NPC_SYMBOL,	0,	0,	NPCSIZE_MIDDLE,
		0,	1,	4,8,4,8,	2,2,2,2,
	},
	//NPC258 ベッドミミガー
	{	0,
		0,	SURF_NPC_ENEMY,	0,	0,	NPCSIZE_MIDDLE,
		0,	0,	6,4,6,4,	8,8,8,8,
	},
	//NPC259 カーリー（おんぶ）
	{	BITS_THROW_BLOCK,//BITS
		0,	SURF_NPC_REGULAR,	0,	0,	0,
		0,	0,	4,8,4,8,	8,8,8,8,
	},
	//NPC260 囚ミミガー(停止)
	{	0,//BITS
		0,	SURF_NPC_ENEMY,	0,	0,	0,
		0,	0,	6,8,6,8,	8,8,8,8,
	},
	//NPC261 囚チエ(停止)
	{	0,//BITS
		0,	SURF_NPC_ENEMY,	0,	0,	0,
		0,	0,	6,8,6,8,	8,8,8,8,
	},
	//NPC262 囚チャコ(停止)
	{	0,//BITS
		0,	SURF_NPC_ENEMY,	0,	0,	0,
		0,	0,	6,8,6,8,	8,8,8,8,
	},
	//NPC263 ドクター（戦闘LV1）
	{	BITS_BANISH_DAMAGE,//BITS
		300,	SURF_NPC_BOSS,	WAVE_NPC_LARGE,	0,	NPCSIZE_LARGE,
		1,	3,	6,12,6,16,	16,16,8,16,
	},
	//NPC264 赤い衝撃
	{	BITS_BLOCK_BULLET|BITS_THROW_BLOCK,
		2,	SURF_NPC_BOSS,	WAVE_NPC_SMALL,	WAVE_DESTROY_SMALL,	NPCSIZE_SMALL,
		0,	5,	4,4,4,4,	8,8,8,8,
	},
	//NPC265 赤い衝撃の尾
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_BOSS,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		6,
		0,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC266 赤い氾濫
	{	BITS_BLOCK_BULLET,
		2,
		SURF_NPC_BOSS,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		0,
		5,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC267 ドクター（戦闘LV2）
	{	BITS_BANISH_DAMAGE,//BITS
		300,
		SURF_NPC_BOSS,
		WAVE_NPC_GOHST,
		0,
		NPCSIZE_LARGE,
		1,
		5,
		10, 8,10,16,
		20,28,20,20,
	},
	//NPC268 ラビル-B
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		300,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		19,
		5,
		 8, 8, 8,16,
		20,20,20,20,
	},
	//NPC269 バットブラスター
	{	BITS_BANISH_DAMAGE,
		2,
		SURF_NPC_BOSS,
		0,
		WAVE_FIRE,
		NPCSIZE_SMALL,
		0,
		2,
		4,4,4,4,
		8,8,8,8,
	},

	//NPC270 ブラッド
	{	0,
		2,
		SURF_NPC_SYMBOL,
		0,
		WAVE_FIRE,
		NPCSIZE_SMALL,
		0,
		0,
		4,4,4,0,
		2,2,2,2,
	},

	//NPC271 流れブロック
	{	BITS_THROW_BLOCK|BITS_BLOCK_BULLET,
		2,		SURF_PARTS,		0,		WAVE_FIRE,		NPCSIZE_SMALL,
		0, 3,		6,6,6,6,		8,8,8,8,
	},

	//NPC272 流れブロック配置
	{	0,
		2,		SURF_PARTS,		0,		WAVE_FIRE,		NPCSIZE_SMALL,
		0,		0,		8,8,8,8,		8,8,8,8,
	},

	//NPC273 ドロールハンマー
	{	0,//BITS
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_LARGE,
		0,		6,		4,4,4,4,		12,12,12,12,
	},
	//NPC274 ドロール
	{	BITS_BANISH_DAMAGE,//BITS
		64,		SURF_NPC_BOSS,		WAVE_NPC_GOHST,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		9,		3,		8,18,8,12,		16,24,16,16,
	},
	//NPC275 ゴン太（大農園）
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		4,8,4,8,
		8,8,8,8,
	},

	//NPC276 あかおに
	{	0,//BITS
		300,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		100,
		10,
		8,18,8,12,
		16,24,16,16,
	},
	//NPC277 赤い金棒
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		6,
		4,4,4,4,
		12,12,12,12,
	},
	//NPC278 リトルおやじ
	{	0,//BITS
		0,
		SURF_ITEMIMAGE,
		0,
		0,
		0,
		0,
		0,
		4,4,4,4,
		4,4,4,4,
	},
	//NPC279 白い落石
	{	BITS_THROW_BLOCK,//BITS
		0,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		16,17,16,16,
		16,16,16,16,
	},
	//NPC280 スー（償還）
	{	0,
		0, SURF_NPC_REGULAR, WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_MIDDLE,
		  0,  0,     5,8,5,8,    8,8,8,8,
	},
	//NPC281 ブラッディドクター
	{	0,
		0, SURF_NPC_REGULAR, WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_MIDDLE,
		  0,  0,     5,8,5,8,    8,8,8,8,
	},
	//NPC282 コアリフト
	{	BITS_THROW_BLOCK | BITS_BLOCK_MYCHAR2,             // BITS
		0, SURF_NPC_BOSS, WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_MIDDLE, // life, surf, voice1, voice2, size
		0,  0,     19,15,19,15,    32,20,32,20,								  // exp, damage, hit[4], view[4]
	},
	//NPC283 ミザリアン
	{	BITS_BANISH_DAMAGE,             // BITS
		1000, SURF_NPC_ENEMY, WAVE_NPC_GOHST, WAVE_DESTROY_LARGE, NPCSIZE_LARGE, // life, surf, voice1, voice2, size
		1,  0,     8,12,8,16,    16,16,16,16,								  // exp, damage, hit[4], view[4]
	},
	//NPC284 バトルスー
	{	BITS_BANISH_DAMAGE,                                                       // BITS
		1000, SURF_NPC_ENEMY, WAVE_GIRLDMG, WAVE_DESTROY_LARGE, NPCSIZE_LARGE, // life, surf, voice1, voice2, size
		1,  0,     8,8,8,13,    16,16,16,16,								      // exp, damage, hit[4], view[4]
	},
	//NPC285 レッドトルネード
	{	BITS_BLOCK_BULLET|BITS_THROW_BLOCK,
		2, SURF_NPC_BOSS, WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_SMALL,
		0, 4,   4,4,4,4,   8,8,8,8,
	},
	//NPC286 レッドトルネードの尾
	{	BITS_THROW_BLOCK,
		0,  SURF_NPC_BOSS, WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_SMALL,
		6, 0,  4,4,4,4,  8,8,8,8,
	},
	//NPC287 レッドスモーク
	{	0,                                          // BITS
		   1,  SURF_NPC_BOSS, 0, 0, NPCSIZE_MIDDLE,	// life, surf, voice1, voice2, size
		0, 0,    4,4,4,4,   8,8,8,8,//view			// exp, damage, hit[4], view[4]
	},
	//NPC288 スモークボール
	{	0,
		   1,  SURF_NPC_BOSS, 0, 0, NPCSIZE_MIDDLE,
		0, 4,    4,4,4,4,   8,8,8,8,//view
	},
	//NPC289 クリッタ－ 召還
	{	0,
		2, SURF_NPC_ENEMY, WAVE_NPC_MIDDLE, WAVE_DESTROY_MIDDLE, NPCSIZE_SMALL,
		1, 0,   6,4,6,5,  8,8,8,8,
	},
	//NPC290 こうもり 召還
	{	BITS_THROW_BLOCK,
		2, SURF_NPC_ENEMY, WAVE_NPC_MIDDLE, WAVE_DESTROY_MIDDLE, NPCSIZE_SMALL,
		1, 0,   6,4,6,5,  8,8,8,8,
	},
	//NPC291 コアリフト(フェイク)
	{	BITS_THROW_BLOCK | BITS_BLOCK_MYCHAR2,             // BITS
		0, SURF_NPC_BOSS, WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_MIDDLE, // life, surf, voice1, voice2, size
		0,  0,     19,15,19,15,    32,20,32,20,								  // exp, damage, hit[4], view[4]
	},

	//NPC292 地震発生装置
	{	0,                                       
		0, SURF_NPC_SYMBOL, WAVE_NPC_SMALL, 0, 0,
		0,  0,     0,0,0,0,    0,0,0,0,	
	},
	//NPC293 波動砲（アンデッドコア）
	{	BITS_THROW_BLOCK,             // BITS
		0, SURF_NPC_BOSS, 0, 0, 0, // life, surf, voice1, voice2, size
		0,  10,     15,15,15,15,    20,20,20,20,								  // exp, damage, hit[4], view[4]
	},
	//NPC294 落石発生装置
	{	0,                                       
		0, SURF_NPC_SYMBOL, WAVE_NPC_SMALL, 0, 0,
		0,  0,     0,0,0,0,    0,0,0,0,	
	},
	//NPC295 雲(Fall)
	{	BITS_THROW_BLOCK,                                       
		0, SURF_PARTS, WAVE_NPC_SMALL, 0, 0,
		0,  0,     0,0,0,0,    0,0,0,0,	
	},
	//NPC296 雲配置
	{	0,                                       
		0, SURF_NPC_SYMBOL, WAVE_NPC_SMALL, 0, 0,
		0,  0,     0,0,0,0,    0,0,0,0,	
	},
	//NPC297 坂本スー(くわえ)
	{	0,//BITS
		0,		SURF_NPC_REGULAR,	0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC298 ドクターすっぴん
	{	0,//BITS
		0,		SURF_NPC_REGULAR,	0,		0,		0,
		0,		0,		5,8,5,8,		8,16,8,16,
	},
	//NPC299 カプセル僕
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		5,8,5,8,		24,24,24,24,
	},
	//NPC300 悪魔の冠
	{	0,//BITS
		0,		SURF_NPC_REGULAR,	0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC301 ミザリーミサイル
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		1,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_LARGE,		NPCSIZE_SMALL,
		1,		3,		4,4,4,4,		8,8,8,8,
	},
	//NPC302 フォーカスマン
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		0,0,0,0,
	},
	//NPC303 マイマシンガン
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//304 ガウディ(credit
	{	0,
		0,		SURF_NPC_BOSS,		WAVE_NPC_MIDDLE,	WAVE_DESTROY_MIDDLE,	NPCSIZE_LARGE,
		8,		5,		8,8,8,8,		12,8,12,8,
	},
	//305 仔ゴン太（フリフリ）
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		4,8,4,8,		8,8,8,8,
	},
	//306 ナースバルログ
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		96,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		18,		0,		14,11,14,12,		20,12,20,12,
	},
	//307 囚サンタ
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		6,8,6,8,		8,8,8,8,
	},
	//308 スタンピー
	{	BITS_BANISH_DAMAGE,
		4,		SURF_NPC_BOSS,		WAVE_NPC_INSECT,	WAVE_DESTROY_MIDDLE,	NPCSIZE_MIDDLE,
		2,		2,		6,6,5,6,		8,8,8,8,
	},
	//309 エンジェル（飛行）
	{	0,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_ENGEL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		0,		6,7,6,8,		8,8,8,8,
	},
	//310 エンジェル（歩行）
	{	0,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_ENGEL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		3,		8,7,8,8,		12,8,12,8,
	},
	//311 エンジェル（弓）
	{	BITS_BANISH_DAMAGE,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_ENGEL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		3,		8,7,8,8,		12,12,12,12,
	},
	//312 エンジェルアロー
	{	BITS_THROW_BLOCK,
		4,		SURF_NPC_SYMBOL,		WAVE_NPC_MIDDLE,	WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		8,		4,2,4,2,		8,8,8,8,
	},
	//313 マピニョン
	{	0,
		300,	SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,	WAVE_DESTROY_MIDDLE,	NPCSIZE_MIDDLE,
		1,		0,		 6, 5, 6, 5,		8,8,8,8,
	},
	//314 CAVE落石
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_PARTS,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		4,4,4,4,		8,8,8,8,
	},
	//315 ダマピニョン
	{	BITS_THROW_BLOCK,
		5,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,	WAVE_DESTROY_MIDDLE,	NPCSIZE_MIDDLE,
		1,		3,		 6, 5, 6, 5,		8,8,8,8,
	},
	//316 エンジェル（やられ）
	{	0,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,	WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		0,		8,7,8,8,		12,12,12,12,
	},
	//317 エンジェル（大男）
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		9,		6,		8,18,8,12,		16,24,16,16,
	},
	//318 エンジェル（大男やられ）
	{	0,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		3,		0,		8,18,8,12,		16,24,16,16,
	},
	//319 エンジェルブロック
	{	BITS_BLOCK_BULLET|BITS_THROW_BLOCK,//BITS
		1,		SURF_PARTS,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		6,		10,		8,8,8,8,		8,8,8,8,
	},
	//320 参戦おんぶカーリー
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//321 カーリーネメシス
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//322 バリケード
	{	BITS_BANISH_DAMAGE,//BITS
		1000,	SURF_NPC_SYMBOL,	WAVE_NPC_GOHST,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		1,		0,	12,12,12,12,		12,12,12,12,
	},
	//323 エンジェル（きりもみ→飛行）
	{	BITS_THROW_BLOCK,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_ENGEL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		0,		6,7,6,8,		8,12,8,8,
	},
	//324 きりもみ配置
	{	BITS_THROW_BLOCK,
		1000,	SURF_NPC_ENEMY,		0,		0,		NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		0,0,0,0,
	},
	//325 プラズマ（へヴィプレス）
	{	BITS_THROW_BLOCK,
		1000,	SURF_NPC_BOSS,		0,		0,		NPCSIZE_SMALL,
		0,		0,		10,8,10,88,		16,16,16,16,
	},

	//326 戻り伊藤/スー
	{	0,
		1000,	SURF_NPC_ENEMY,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		 8,8,8,8,		8,16,8,8,
	},
	//327 くしゃみ
	{	0,
		1000,	SURF_NPC_ENEMY,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	
	//328 変身装置
	{	0,
		1000,	SURF_PARTS,		0,		0,		NPCSIZE_SMALL,
		0,		0,		8,8,8,8,		16,40,16,8,
	},

	//329 換気扇
	{	0,
		1000,	SURF_PARTS,		0,		0,		NPCSIZE_SMALL,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//330 コロコロ
	{	BITS_BLOCK_BULLET,
		1000,	SURF_NPC_SYMBOL,		0,		0,	NPCSIZE_MIDDLE,
		0,		6,		7,7,7,7,		8,8,8,8,
	},
	//331 スモークボーン
	{	BITS_BANISH_DAMAGE,
		   1,  SURF_NPC_ENEMY, 0, 0, NPCSIZE_MIDDLE,
		0, 4,    4,4,4,4,   8,8,8,8,//view
	},
	//332 ボーンスモーク
	{	0,
		   1,  SURF_CARET, 0, 0, NPCSIZE_MIDDLE,
		0, 8,    4,4,4,4,   12,12,12,12,//view
	},
	//333 サンダーシンボル
	{	BITS_THROW_BLOCK,
		   1,  SURF_CARET, 0, 0, NPCSIZE_MIDDLE,
		0, 0,    4,4,4,4,   12,12,12,12,//view
	},
	//334 汗
	{	BITS_THROW_BLOCK,
		0,	SURF_NPC_SYMBOL,		0,		0,	NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		4,8,4,8,
	},
	//335 右泳ぎいか
	{	BITS_THROW_BLOCK,
		0,	SURF_NPC_ENEMY,		0,		0,	NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		8,8,8,8,
	},
	//336 いか配置
	{	BITS_THROW_BLOCK,
		0,	SURF_NPC_SYMBOL,	0,		0,	NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		0,0,0,0,
	},
	//337 ドロール（彫刻）
	{	0,//BITS
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_LARGE,
		0,		0,		8,18,8,12,		16,24,16,16,
	},
	//338 グリーンデビル
	{	BITS_THROW_BLOCK,
		2, SURF_NPC_ENEMY, WAVE_NPC_INSECT, WAVE_DESTROY_MIDDLE, NPCSIZE_SMALL,
		5, 0,   6,4,6,5,  8,8,8,8,
	},
	//339 グリーンデビル配置
	{	BITS_THROW_BLOCK,
		0,	SURF_NPC_SYMBOL,	0,		0,	NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		0,0,0,0,
	},
	//340 神官ボロス
	{	BITS_EVENT_BREAK|BITS_BLOCK_BULLET,
		1000,//1000,
		SURF_NPC_ENEMY,	WAVE_NPC_GOHST,		0,	NPCSIZE_LARGE,
		1,		0,		12,18,12,14,		24,24,24,16,
	},
	//341 神官ボロス表情
	{	0,
		0,	SURF_NPC_ENEMY,	0,		0,	NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		16,8,16,8,
	},
	//342 ボロスサークル
	{	BITS_THROW_BLOCK|BITS_BLOCK_BULLET,//BITS
		0,		SURF_NPC_BOSS,	WAVE_NPC_LARGE, WAVE_DESTROY_MIDDLE, NPCSIZE_MIDDLE,
		1,		0,		12,12,12,12,		20,20,20,20,
	},
	//343 ボロスダミーボディ
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_LARGE,
		0,		0,		0,0,0,0,		60,60,60,60,
	},
	//344 ボロスダミーアイ
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		12,0,12,0,
	},
	//345 ヘッドボーン
	{	BITS_BLOCK_BULLET,//BITS
		0,		SURF_PARTS,		0,		0,		NPCSIZE_SMALL,
		0,		4,		6,6,6,6,		8,8,8,8,
	},
	//346 ボロスリフト
	{	BITS_THROW_BLOCK|BITS_BLOCK_MYCHAR2,//BITS
		0,		SURF_NPC_BOSS,	0, 0, NPCSIZE_MIDDLE,
		1,		0,		16,9,16,2,		16,8,16,8,
	},
	//347 月光虫
	{	BITS_BANISH_DAMAGE,//BITS
		10,		SURF_NPC_ENEMY,	WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_MIDDLE,
		1,		4,		8,6,8,6,		8,8,8,8,
	},
	//348 地獄の槍
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_PARTS,	0, 0, NPCSIZE_SMALL,
		0,		0,		8,8,24,8,		8,16,24,8,
	},

	//NPC000 ダミー（停止）
	{	BITS_THROW_BLOCK,                            //BITS
		1000, SURF_NPC_SYMBOL, 0, 0, NPCSIZE_MIDDLE, // life, surf, voice1, voice2, size
		0, 0,  4,3,4,8,  8,8,8,8,                    // exp, damage, hit[4], view[4]
	},
	//350 エンジェル（翼弓）
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		9,	SURF_NPC_ENEMY,		WAVE_NPC_ENGEL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		5,		3,		8,7,8,8,		12,12,12,12,
	},
	//351 石像
	{	0,
		100, SURF_PARTS,	WAVE_BREAK1, WAVE_BOM, NPCSIZE_LARGE,
		20,		0,		8,12,8,12,		16,20,16,20,
	},
	//352 キャスト
	{	0,
		100, SURF_NPC_REGULAR,	0, 0, NPCSIZE_MIDDLE,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//353 きりもみ飛行赤天使
	{	BITS_THROW_BLOCK,
		9,	SURF_NPC_ENEMY,		WAVE_NPC_ENGEL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		0,		6,7,6,8,		8,12,8,8,
	},
	//354 迫る壁
	{	BITS_THROW_BLOCK,
		0, SURF_NPC_SYMBOL,	0, 0, NPCSIZE_SMALL,
		0,		127,		6,8,6,280,		0,0,0,0,
	},
	//355 くっつきカーリー／クォート
	{	BITS_THROW_BLOCK,
		0,  0,	0, 0, NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		8,8,8,8,
	},
	//356 パタパタバルログ
	{	BITS_THROW_BLOCK,
		0,  SURF_NPC_ENEMY,	0, 0, NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		20,12,20,12,
	},
	//357 ブルードッグ
	{	BITS_THROW_BLOCK,
		0, SURF_NPC_SYMBOL,	0, 0, NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		8,8,8,8,
	},
	//358 ---
	{	0,
		0, 0,	0, 0, 0,
		0,		0,		0,0,0,0,		0,0,0,0,
	},
	//359 ---
	{	0,
		0, 0,	0, 0, 0,
		0,		0,		0,0,0,0,		0,0,0,0,
	},


//BITS
// life, surf, voice1, voice2, size
// exp, damage, hit[4], view[4]


};
/**/

void ActNpc000(NPCHAR*npc);void ActNpc001(NPCHAR*npc);void ActNpc002(NPCHAR*npc);void ActNpc003(NPCHAR*npc);void ActNpc004(NPCHAR*npc);
void ActNpc005(NPCHAR*npc);void ActNpc006(NPCHAR*npc);void ActNpc007(NPCHAR*npc);void ActNpc008(NPCHAR*npc);void ActNpc009(NPCHAR*npc);
void ActNpc010(NPCHAR*npc);void ActNpc011(NPCHAR*npc);void ActNpc012(NPCHAR*npc);void ActNpc013(NPCHAR*npc);void ActNpc014(NPCHAR*npc);
void ActNpc015(NPCHAR*npc);void ActNpc016(NPCHAR*npc);void ActNpc017(NPCHAR*npc);void ActNpc018(NPCHAR*npc);void ActNpc019(NPCHAR*npc);
void ActNpc020(NPCHAR*npc);void ActNpc021(NPCHAR*npc);void ActNpc022(NPCHAR*npc);void ActNpc023(NPCHAR*npc);void ActNpc024(NPCHAR*npc);
void ActNpc025(NPCHAR*npc);void ActNpc026(NPCHAR*npc);void ActNpc027(NPCHAR*npc);void ActNpc028(NPCHAR*npc);void ActNpc029(NPCHAR*npc);
void ActNpc030(NPCHAR*npc);void ActNpc031(NPCHAR*npc);void ActNpc032(NPCHAR*npc);void ActNpc033(NPCHAR*npc);void ActNpc034(NPCHAR*npc);
void ActNpc035(NPCHAR*npc);void ActNpc036(NPCHAR*npc);void ActNpc037(NPCHAR*npc);void ActNpc038(NPCHAR*npc);void ActNpc039(NPCHAR*npc);
void ActNpc040(NPCHAR*npc);void ActNpc041(NPCHAR*npc);void ActNpc042(NPCHAR*npc);void ActNpc043(NPCHAR*npc);void ActNpc044(NPCHAR*npc);
void ActNpc045(NPCHAR*npc);void ActNpc046(NPCHAR*npc);void ActNpc047(NPCHAR*npc);void ActNpc048(NPCHAR*npc);void ActNpc049(NPCHAR*npc);
void ActNpc050(NPCHAR*npc);void ActNpc051(NPCHAR*npc);void ActNpc052(NPCHAR*npc);void ActNpc053(NPCHAR*npc);void ActNpc054(NPCHAR*npc);
void ActNpc055(NPCHAR*npc);void ActNpc056(NPCHAR*npc);void ActNpc057(NPCHAR*npc);void ActNpc058(NPCHAR*npc);void ActNpc059(NPCHAR*npc);
void ActNpc060(NPCHAR*npc);void ActNpc061(NPCHAR*npc);void ActNpc062(NPCHAR*npc);void ActNpc063(NPCHAR*npc);void ActNpc064(NPCHAR*npc);
void ActNpc065(NPCHAR*npc);void ActNpc066(NPCHAR*npc);void ActNpc067(NPCHAR*npc);void ActNpc068(NPCHAR*npc);void ActNpc069(NPCHAR*npc);
void ActNpc070(NPCHAR*npc);void ActNpc071(NPCHAR*npc);void ActNpc072(NPCHAR*npc);void ActNpc073(NPCHAR*npc);void ActNpc074(NPCHAR*npc);
void ActNpc075(NPCHAR*npc);void ActNpc076(NPCHAR*npc);void ActNpc077(NPCHAR*npc);void ActNpc078(NPCHAR*npc);void ActNpc079(NPCHAR*npc);
void ActNpc080(NPCHAR*npc);void ActNpc081(NPCHAR*npc);void ActNpc082(NPCHAR*npc);void ActNpc083(NPCHAR*npc);void ActNpc084(NPCHAR*npc);
void ActNpc085(NPCHAR*npc);void ActNpc086(NPCHAR*npc);void ActNpc087(NPCHAR*npc);void ActNpc088(NPCHAR*npc);void ActNpc089(NPCHAR*npc);
void ActNpc090(NPCHAR*npc);void ActNpc091(NPCHAR*npc);void ActNpc092(NPCHAR*npc);void ActNpc093(NPCHAR*npc);void ActNpc094(NPCHAR*npc);
void ActNpc095(NPCHAR*npc);void ActNpc096(NPCHAR*npc);void ActNpc097(NPCHAR*npc);void ActNpc098(NPCHAR*npc);void ActNpc099(NPCHAR*npc);

void ActNpc100(NPCHAR*npc);void ActNpc101(NPCHAR*npc);void ActNpc102(NPCHAR*npc);void ActNpc103(NPCHAR*npc);void ActNpc104(NPCHAR*npc);
void ActNpc105(NPCHAR*npc);void ActNpc106(NPCHAR*npc);void ActNpc107(NPCHAR*npc);void ActNpc108(NPCHAR*npc);void ActNpc109(NPCHAR*npc);
void ActNpc110(NPCHAR*npc);void ActNpc111(NPCHAR*npc);void ActNpc112(NPCHAR*npc);void ActNpc113(NPCHAR*npc);void ActNpc114(NPCHAR*npc);
void ActNpc115(NPCHAR*npc);void ActNpc116(NPCHAR*npc);void ActNpc117(NPCHAR*npc);void ActNpc118(NPCHAR*npc);void ActNpc119(NPCHAR*npc);
void ActNpc120(NPCHAR*npc);void ActNpc121(NPCHAR*npc);void ActNpc122(NPCHAR*npc);void ActNpc123(NPCHAR*npc);void ActNpc124(NPCHAR*npc);
void ActNpc125(NPCHAR*npc);void ActNpc126(NPCHAR*npc);void ActNpc127(NPCHAR*npc);void ActNpc128(NPCHAR*npc);void ActNpc129(NPCHAR*npc);
void ActNpc130(NPCHAR*npc);void ActNpc131(NPCHAR*npc);void ActNpc132(NPCHAR*npc);void ActNpc133(NPCHAR*npc);void ActNpc134(NPCHAR*npc);
void ActNpc135(NPCHAR*npc);void ActNpc136(NPCHAR*npc);void ActNpc137(NPCHAR*npc);void ActNpc138(NPCHAR*npc);void ActNpc139(NPCHAR*npc);
void ActNpc140(NPCHAR*npc);void ActNpc141(NPCHAR*npc);void ActNpc142(NPCHAR*npc);void ActNpc143(NPCHAR*npc);void ActNpc144(NPCHAR*npc);
void ActNpc145(NPCHAR*npc);void ActNpc146(NPCHAR*npc);void ActNpc147(NPCHAR*npc);void ActNpc148(NPCHAR*npc);void ActNpc149(NPCHAR*npc);
void ActNpc150(NPCHAR*npc);void ActNpc151(NPCHAR*npc);void ActNpc152(NPCHAR*npc);void ActNpc153(NPCHAR*npc);void ActNpc154(NPCHAR*npc);
void ActNpc155(NPCHAR*npc);void ActNpc156(NPCHAR*npc);void ActNpc157(NPCHAR*npc);void ActNpc158(NPCHAR*npc);void ActNpc159(NPCHAR*npc);
void ActNpc160(NPCHAR*npc);void ActNpc161(NPCHAR*npc);void ActNpc162(NPCHAR*npc);void ActNpc163(NPCHAR*npc);void ActNpc164(NPCHAR*npc);
void ActNpc165(NPCHAR*npc);void ActNpc166(NPCHAR*npc);void ActNpc167(NPCHAR*npc);void ActNpc168(NPCHAR*npc);void ActNpc169(NPCHAR*npc);
void ActNpc170(NPCHAR*npc);void ActNpc171(NPCHAR*npc);void ActNpc172(NPCHAR*npc);void ActNpc173(NPCHAR*npc);void ActNpc174(NPCHAR*npc);
void ActNpc175(NPCHAR*npc);void ActNpc176(NPCHAR*npc);void ActNpc177(NPCHAR*npc);void ActNpc178(NPCHAR*npc);void ActNpc179(NPCHAR*npc);
void ActNpc180(NPCHAR*npc);void ActNpc181(NPCHAR*npc);void ActNpc182(NPCHAR*npc);void ActNpc183(NPCHAR*npc);void ActNpc184(NPCHAR*npc);
void ActNpc185(NPCHAR*npc);void ActNpc186(NPCHAR*npc);void ActNpc187(NPCHAR*npc);void ActNpc188(NPCHAR*npc);void ActNpc189(NPCHAR*npc);
void ActNpc190(NPCHAR*npc);void ActNpc191(NPCHAR*npc);void ActNpc192(NPCHAR*npc);void ActNpc193(NPCHAR*npc);void ActNpc194(NPCHAR*npc);
void ActNpc195(NPCHAR*npc);void ActNpc196(NPCHAR*npc);void ActNpc197(NPCHAR*npc);void ActNpc198(NPCHAR*npc);void ActNpc199(NPCHAR*npc);

void ActNpc200(NPCHAR*npc);void ActNpc201(NPCHAR*npc);void ActNpc202(NPCHAR*npc);void ActNpc203(NPCHAR*npc);void ActNpc204(NPCHAR*npc);
void ActNpc205(NPCHAR*npc);void ActNpc206(NPCHAR*npc);void ActNpc207(NPCHAR*npc);void ActNpc208(NPCHAR*npc);void ActNpc209(NPCHAR*npc);
void ActNpc210(NPCHAR*npc);void ActNpc211(NPCHAR*npc);void ActNpc212(NPCHAR*npc);void ActNpc213(NPCHAR*npc);void ActNpc214(NPCHAR*npc);
void ActNpc215(NPCHAR*npc);void ActNpc216(NPCHAR*npc);void ActNpc217(NPCHAR*npc);void ActNpc218(NPCHAR*npc);void ActNpc219(NPCHAR*npc);
void ActNpc220(NPCHAR*npc);void ActNpc221(NPCHAR*npc);void ActNpc222(NPCHAR*npc);void ActNpc223(NPCHAR*npc);void ActNpc224(NPCHAR*npc);
void ActNpc225(NPCHAR*npc);void ActNpc226(NPCHAR*npc);void ActNpc227(NPCHAR*npc);void ActNpc228(NPCHAR*npc);void ActNpc229(NPCHAR*npc);
void ActNpc230(NPCHAR*npc);void ActNpc231(NPCHAR*npc);void ActNpc232(NPCHAR*npc);void ActNpc233(NPCHAR*npc);void ActNpc234(NPCHAR*npc);
void ActNpc235(NPCHAR*npc);void ActNpc236(NPCHAR*npc);void ActNpc237(NPCHAR*npc);void ActNpc238(NPCHAR*npc);void ActNpc239(NPCHAR*npc);
void ActNpc240(NPCHAR*npc);void ActNpc241(NPCHAR*npc);void ActNpc242(NPCHAR*npc);void ActNpc243(NPCHAR*npc);void ActNpc244(NPCHAR*npc);
void ActNpc245(NPCHAR*npc);void ActNpc246(NPCHAR*npc);void ActNpc247(NPCHAR*npc);void ActNpc248(NPCHAR*npc);void ActNpc249(NPCHAR*npc);
void ActNpc250(NPCHAR*npc);void ActNpc251(NPCHAR*npc);void ActNpc252(NPCHAR*npc);void ActNpc253(NPCHAR*npc);void ActNpc254(NPCHAR*npc);
void ActNpc255(NPCHAR*npc);void ActNpc256(NPCHAR*npc);void ActNpc257(NPCHAR*npc);void ActNpc258(NPCHAR*npc);void ActNpc259(NPCHAR*npc);
void ActNpc260(NPCHAR*npc);void ActNpc261(NPCHAR*npc);void ActNpc262(NPCHAR*npc);void ActNpc263(NPCHAR*npc);void ActNpc264(NPCHAR*npc);
void ActNpc265(NPCHAR*npc);void ActNpc266(NPCHAR*npc);void ActNpc267(NPCHAR*npc);void ActNpc268(NPCHAR*npc);void ActNpc269(NPCHAR*npc);
void ActNpc270(NPCHAR*npc);void ActNpc271(NPCHAR*npc);void ActNpc272(NPCHAR*npc);void ActNpc273(NPCHAR*npc);void ActNpc274(NPCHAR*npc);
void ActNpc275(NPCHAR*npc);void ActNpc276(NPCHAR*npc);void ActNpc277(NPCHAR*npc);void ActNpc278(NPCHAR*npc);void ActNpc279(NPCHAR*npc);
void ActNpc280(NPCHAR*npc);void ActNpc281(NPCHAR*npc);void ActNpc282(NPCHAR*npc);void ActNpc283(NPCHAR*npc);void ActNpc284(NPCHAR*npc);
void ActNpc285(NPCHAR*npc);void ActNpc286(NPCHAR*npc);void ActNpc287(NPCHAR*npc);void ActNpc288(NPCHAR*npc);void ActNpc289(NPCHAR*npc);
void ActNpc290(NPCHAR*npc);void ActNpc291(NPCHAR*npc);void ActNpc292(NPCHAR*npc);void ActNpc293(NPCHAR*npc);void ActNpc294(NPCHAR*npc);
void ActNpc295(NPCHAR*npc);void ActNpc296(NPCHAR*npc);void ActNpc297(NPCHAR*npc);void ActNpc298(NPCHAR*npc);void ActNpc299(NPCHAR*npc);
	
void ActNpc300(NPCHAR*npc);void ActNpc301(NPCHAR*npc);void ActNpc302(NPCHAR*npc);void ActNpc303(NPCHAR*npc);void ActNpc304(NPCHAR*npc);
void ActNpc305(NPCHAR*npc);void ActNpc306(NPCHAR*npc);void ActNpc307(NPCHAR*npc);void ActNpc308(NPCHAR*npc);void ActNpc309(NPCHAR*npc);
void ActNpc310(NPCHAR*npc);void ActNpc311(NPCHAR*npc);void ActNpc312(NPCHAR*npc);void ActNpc313(NPCHAR*npc);void ActNpc314(NPCHAR*npc);
void ActNpc315(NPCHAR*npc);void ActNpc316(NPCHAR*npc);void ActNpc317(NPCHAR*npc);void ActNpc318(NPCHAR*npc);void ActNpc319(NPCHAR*npc);
void ActNpc320(NPCHAR*npc);void ActNpc321(NPCHAR*npc);void ActNpc322(NPCHAR*npc);void ActNpc323(NPCHAR*npc);void ActNpc324(NPCHAR*npc);
void ActNpc325(NPCHAR*npc);void ActNpc326(NPCHAR*npc);void ActNpc327(NPCHAR*npc);void ActNpc328(NPCHAR*npc);void ActNpc329(NPCHAR*npc);
void ActNpc330(NPCHAR*npc);void ActNpc331(NPCHAR*npc);void ActNpc332(NPCHAR*npc);void ActNpc333(NPCHAR*npc);void ActNpc334(NPCHAR*npc);
void ActNpc335(NPCHAR*npc);void ActNpc336(NPCHAR*npc);void ActNpc337(NPCHAR*npc);void ActNpc338(NPCHAR*npc);void ActNpc339(NPCHAR*npc);
void ActNpc340(NPCHAR*npc);void ActNpc341(NPCHAR*npc);void ActNpc342(NPCHAR*npc);void ActNpc343(NPCHAR*npc);void ActNpc344(NPCHAR*npc);
void ActNpc345(NPCHAR*npc);void ActNpc346(NPCHAR*npc);void ActNpc347(NPCHAR*npc);void ActNpc348(NPCHAR*npc);void ActNpc349(NPCHAR*npc);
void ActNpc350(NPCHAR*npc);void ActNpc351(NPCHAR*npc);void ActNpc352(NPCHAR*npc);void ActNpc353(NPCHAR*npc);void ActNpc354(NPCHAR*npc);
void ActNpc355(NPCHAR*npc);void ActNpc356(NPCHAR*npc);void ActNpc357(NPCHAR*npc);void ActNpc358(NPCHAR*npc);void ActNpc359(NPCHAR*npc);
void ActNpc360(NPCHAR*npc);

NPCFUNCTION gpNpcFuncTbl[] = {

	ActNpc000,ActNpc001,ActNpc002,ActNpc003,ActNpc004,ActNpc005,ActNpc006,ActNpc007,ActNpc008,ActNpc009,
	ActNpc010,ActNpc011,ActNpc012,ActNpc013,ActNpc014,ActNpc015,ActNpc016,ActNpc017,ActNpc018,ActNpc019,
	ActNpc020,ActNpc021,ActNpc022,ActNpc023,ActNpc024,ActNpc025,ActNpc026,ActNpc027,ActNpc028,ActNpc029,
	ActNpc030,ActNpc031,ActNpc032,ActNpc033,ActNpc034,ActNpc035,ActNpc036,ActNpc037,ActNpc038,ActNpc039,
	ActNpc040,ActNpc041,ActNpc042,ActNpc043,ActNpc044,ActNpc045,ActNpc046,ActNpc047,ActNpc048,ActNpc049,
	ActNpc050,ActNpc051,ActNpc052,ActNpc053,ActNpc054,ActNpc055,ActNpc056,ActNpc057,ActNpc058,ActNpc059,
	ActNpc060,ActNpc061,ActNpc062,ActNpc063,ActNpc064,ActNpc065,ActNpc066,ActNpc067,ActNpc068,ActNpc069,
	ActNpc070,ActNpc071,ActNpc072,ActNpc073,ActNpc074,ActNpc075,ActNpc076,ActNpc077,ActNpc078,ActNpc079,
	ActNpc080,ActNpc081,ActNpc082,ActNpc083,ActNpc084,ActNpc085,ActNpc086,ActNpc087,ActNpc088,ActNpc089,
	ActNpc090,ActNpc091,ActNpc092,ActNpc093,ActNpc094,ActNpc095,ActNpc096,ActNpc097,ActNpc098,ActNpc099,
	
	ActNpc100,ActNpc101,ActNpc102,ActNpc103,ActNpc104,ActNpc105,ActNpc106,ActNpc107,ActNpc108,ActNpc109,
	ActNpc110,ActNpc111,ActNpc112,ActNpc113,ActNpc114,ActNpc115,ActNpc116,ActNpc117,ActNpc118,ActNpc119,
	ActNpc120,ActNpc121,ActNpc122,ActNpc123,ActNpc124,ActNpc125,ActNpc126,ActNpc127,ActNpc128,ActNpc129,
	ActNpc130,ActNpc131,ActNpc132,ActNpc133,ActNpc134,ActNpc135,ActNpc136,ActNpc137,ActNpc138,ActNpc139,
	ActNpc140,ActNpc141,ActNpc142,ActNpc143,ActNpc144,ActNpc145,ActNpc146,ActNpc147,ActNpc148,ActNpc149,
	ActNpc150,ActNpc151,ActNpc152,ActNpc153,ActNpc154,ActNpc155,ActNpc156,ActNpc157,ActNpc158,ActNpc159,
	ActNpc160,ActNpc161,ActNpc162,ActNpc163,ActNpc164,ActNpc165,ActNpc166,ActNpc167,ActNpc168,ActNpc169,
	ActNpc170,ActNpc171,ActNpc172,ActNpc173,ActNpc174,ActNpc175,ActNpc176,ActNpc177,ActNpc178,ActNpc179,
	ActNpc180,ActNpc181,ActNpc182,ActNpc183,ActNpc184,ActNpc185,ActNpc186,ActNpc187,ActNpc188,ActNpc189,
	ActNpc190,ActNpc191,ActNpc192,ActNpc193,ActNpc194,ActNpc195,ActNpc196,ActNpc197,ActNpc198,ActNpc199,
	
	ActNpc200,ActNpc201,ActNpc202,ActNpc203,ActNpc204,ActNpc205,ActNpc206,ActNpc207,ActNpc208,ActNpc209,
	ActNpc210,ActNpc211,ActNpc212,ActNpc213,ActNpc214,ActNpc215,ActNpc216,ActNpc217,ActNpc218,ActNpc219,
	ActNpc220,ActNpc221,ActNpc222,ActNpc223,ActNpc224,ActNpc225,ActNpc226,ActNpc227,ActNpc228,ActNpc229,
	ActNpc230,ActNpc231,ActNpc232,ActNpc233,ActNpc234,ActNpc235,ActNpc236,ActNpc237,ActNpc238,ActNpc239,
	ActNpc240,ActNpc241,ActNpc242,ActNpc243,ActNpc244,ActNpc245,ActNpc246,ActNpc247,ActNpc248,ActNpc249,
	ActNpc250,ActNpc251,ActNpc252,ActNpc253,ActNpc254,ActNpc255,ActNpc256,ActNpc257,ActNpc258,ActNpc259,
	ActNpc260,ActNpc261,ActNpc262,ActNpc263,ActNpc264,ActNpc265,ActNpc266,ActNpc267,ActNpc268,ActNpc269,
	ActNpc270,ActNpc271,ActNpc272,ActNpc273,ActNpc274,ActNpc275,ActNpc276,ActNpc277,ActNpc278,ActNpc279,
	ActNpc280,ActNpc281,ActNpc282,ActNpc283,ActNpc284,ActNpc285,ActNpc286,ActNpc287,ActNpc288,ActNpc289,
	ActNpc290,ActNpc291,ActNpc292,ActNpc293,ActNpc294,ActNpc295,ActNpc296,ActNpc297,ActNpc298,ActNpc299,
	
	ActNpc300,ActNpc301,ActNpc302,ActNpc303,ActNpc304,ActNpc305,ActNpc306,ActNpc307,ActNpc308,ActNpc309,
	ActNpc310,ActNpc311,ActNpc312,ActNpc313,ActNpc314,ActNpc315,ActNpc316,ActNpc317,ActNpc318,ActNpc319,
	ActNpc320,ActNpc321,ActNpc322,ActNpc323,ActNpc324,ActNpc325,ActNpc326,ActNpc327,ActNpc328,ActNpc329,
	ActNpc330,ActNpc331,ActNpc332,ActNpc333,ActNpc334,ActNpc335,ActNpc336,ActNpc337,ActNpc338,ActNpc339,
	ActNpc340,ActNpc341,ActNpc342,ActNpc343,ActNpc344,ActNpc345,ActNpc346,ActNpc347,ActNpc348,ActNpc349,
	ActNpc350,ActNpc351,ActNpc352,ActNpc353,ActNpc354,ActNpc355,ActNpc356,ActNpc357,ActNpc358,ActNpc359,
	ActNpc360,
};


NPCTABLE *gNpcTable = NULL;
long GetFileSizeLong( char *path );

BOOL LoadNpcTable( char *path )
{
	FILE *fp;
	long n;
	long size;
	long num;

	size = GetFileSizeLong( path );
	if( size == -1 ) return FALSE;
	num = size/sizeof( NPCTABLE );

	gNpcTable = (NPCTABLE *)malloc( num * sizeof( NPCTABLE ) );
	if( !gNpcTable ) return FALSE;

	fp = fopen( path, "rb" );
	if( !fp ){
		free( gNpcTable );
		gNpcTable = NULL;
		return FALSE;
	}

	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].bits,          sizeof(unsigned short), 1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].life,          sizeof(unsigned short), 1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].surf,          sizeof(unsigned char),  1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].destroy_voice, sizeof(unsigned char),  1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].hit_voice,     sizeof(unsigned char),  1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].size,          sizeof(unsigned char),  1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].exp,           sizeof(long),           1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].damage,        sizeof(long),           1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].hit,           sizeof(RANGE_CHAR),     1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].view,          sizeof(RANGE_CHAR),     1, fp );

	fclose( fp );
	return TRUE;
}


void ReleaseNpcTable( void )
{
	if( gNpcTable ){
		free( gNpcTable );
		gNpcTable = NULL;
	}
	return;
}

typedef struct{

	long front;//前方
	long top;//上
	long back;//後方
	long bottom;//下

}RANGE;

typedef struct{
	long flag;
	long partsW;
	long partsH;
	long numX;
	long numY;
	long type;

	long _fx;
}BACKDATA;

typedef struct{

	unsigned char cond;
	unsigned long flag;//あたり判定フラグ
	long direct;//向き
	long up;
	long down;
	long unit;//キャラクターインデックス兼サーフェスインデックス。
	long equip;

	long x, y;//実座標
	long tgt_x, tgt_y;//フレーム用
	long index_x, index_y;
	long xm, ym;//移動量
	long ani_wait;
	long ani_no;
	RANGE hit;
	RANGE view;
	RECT rect;
	RECT rect_arms;

	long level;
	long exp_wait;
	long exp_count;

	unsigned char shock;//ダメージを受けた時の点滅
	unsigned char no_life;//体力ゼロの点滅。
	unsigned char rensha;
	unsigned char bubble;

	short life;
	short star;
	short max_life;
	short a;
	long lifeBr;
	long lifeBr_count;
	long air; //10*60
	long air_get;//0になるまでair:100を表示
	char sprash;
	char ques;//クエスチョン出しますか？

	char boost_sw;
	long boost_cnt;

}MYCHAR;

typedef struct NPCHAR{

	unsigned char cond;//状態
	long flag;
	long x, y;//実座標
	long xm, ym;//移動量
	long xm2, ym2;
	long tgt_x, tgt_y;
	long code_char;//キャラコード(要らないかも)
	long code_flag;//固有フラグコード
	long code_event;//イベントコード
	long surf;//サーフェス
	long hit_voice;
	long destroy_voice;
	long life;//体力
	long exp;//消滅時出現マネーコード
	long size;//サイズ（爆風配置やあたり判定に使う）
	long direct;//向き
	unsigned short bits;//属性16フラグ

	//表示RECT
	RECT rect;

	//ゼロで初期化されるもの
	long ani_wait;
	long ani_no;
	long count1;//汎用
	long count2;//汎用
	long act_no;//ほぼ汎用
	long act_wait;//ほぼ汎用

	RANGE hit;
	RANGE view;
	unsigned char shock;//ダメージを受けた時の点滅
	long damage_view;
	long damage;//マイキャラダメージ

	NPCHAR *pNpc;

}NPCHAR;



typedef struct{

	unsigned char front;//前方
	unsigned char top;//上
	unsigned char back;//後方
	unsigned char bottom;//下

}RANGE_CHAR;

//キャラコードによる各パラメータ
typedef struct{

	unsigned short bits;//属性16フラグ
	unsigned short life;
	unsigned char  surf;
	unsigned char  hit_voice;
	unsigned char  destroy_voice;
	unsigned char  size;
	long exp;
	long damage;//マイキャラダメージ
	RANGE_CHAR hit;
	RANGE_CHAR view;

}NPCTABLE;

typedef struct{

	long flag;
	long code_bullet;
	long bbits; // 属性（壁が壊せるとか）
	long cond;  // 状態

	long x,  y; // 実座標
	long xm, ym;// 移動量
	long tgt_x, tgt_y;

	long act_no;
	long act_wait;
	long ani_wait;
	long ani_no;

	unsigned char direct;//向き
	RECT rect;
	long count1;
	long count2;
	long life_count; //最大存在時間。
	long damage;
	long life;

	long enemyXL;
	long enemyYL;
	long blockXL;
	long blockYL;
	RANGE view;

}BULLET;


typedef struct{

	long cond;
	long code;
	long direct;
	long x, y;
	long xm, ym;
	long act_no;
	long act_wait;
	long ani_no;
	long ani_wait;

	long view_left;
	long view_top;

	RECT rect;

}CARET;


typedef struct{

	unsigned char *data;
	unsigned char atrb[256 + 1];
	short width;//マップの幅
	short length;//マップの長さ

}MAPDATA;


typedef struct{
	long code;
	long level;    //レベル
	long exp;      //経験値
	long max_num;
	long num;
}ARMSDATA;

typedef struct{
	long code;
}ITEMDATA;

typedef struct{
	long index;//インデックス
	long event;//イベントNO
}PERMITSTAGE;

typedef void (* NPCFUNCTION)( NPCHAR *npc );
typedef void (* BOSSFUNCTION)( void );



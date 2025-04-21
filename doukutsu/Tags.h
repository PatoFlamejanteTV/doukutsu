typedef struct{

	long front;//�O��
	long top;//��
	long back;//���
	long bottom;//��

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
	unsigned long flag;//�����蔻��t���O
	long direct;//����
	long up;
	long down;
	long unit;//�L�����N�^�[�C���f�b�N�X���T�[�t�F�X�C���f�b�N�X�B
	long equip;

	long x, y;//�����W
	long tgt_x, tgt_y;//�t���[���p
	long index_x, index_y;
	long xm, ym;//�ړ���
	long ani_wait;
	long ani_no;
	RANGE hit;
	RANGE view;
	RECT rect;
	RECT rect_arms;

	long level;
	long exp_wait;
	long exp_count;

	unsigned char shock;//�_���[�W���󂯂����̓_��
	unsigned char no_life;//�̗̓[���̓_�ŁB
	unsigned char rensha;
	unsigned char bubble;

	short life;
	short star;
	short max_life;
	short a;
	long lifeBr;
	long lifeBr_count;
	long air; //10*60
	long air_get;//0�ɂȂ�܂�air:100��\��
	char sprash;
	char ques;//�N�G�X�`�����o���܂����H

	char boost_sw;
	long boost_cnt;

}MYCHAR;

typedef struct NPCHAR{

	unsigned char cond;//���
	long flag;
	long x, y;//�����W
	long xm, ym;//�ړ���
	long xm2, ym2;
	long tgt_x, tgt_y;
	long code_char;//�L�����R�[�h(�v��Ȃ�����)
	long code_flag;//�ŗL�t���O�R�[�h
	long code_event;//�C�x���g�R�[�h
	long surf;//�T�[�t�F�X
	long hit_voice;
	long destroy_voice;
	long life;//�̗�
	long exp;//���Ŏ��o���}�l�[�R�[�h
	long size;//�T�C�Y�i�����z�u�₠���蔻��Ɏg���j
	long direct;//����
	unsigned short bits;//����16�t���O

	//�\��RECT
	RECT rect;

	//�[���ŏ�������������
	long ani_wait;
	long ani_no;
	long count1;//�ėp
	long count2;//�ėp
	long act_no;//�قڔėp
	long act_wait;//�قڔėp

	RANGE hit;
	RANGE view;
	unsigned char shock;//�_���[�W���󂯂����̓_��
	long damage_view;
	long damage;//�}�C�L�����_���[�W

	NPCHAR *pNpc;

}NPCHAR;



typedef struct{

	unsigned char front;//�O��
	unsigned char top;//��
	unsigned char back;//���
	unsigned char bottom;//��

}RANGE_CHAR;

//�L�����R�[�h�ɂ��e�p�����[�^
typedef struct{

	unsigned short bits;//����16�t���O
	unsigned short life;
	unsigned char  surf;
	unsigned char  hit_voice;
	unsigned char  destroy_voice;
	unsigned char  size;
	long exp;
	long damage;//�}�C�L�����_���[�W
	RANGE_CHAR hit;
	RANGE_CHAR view;

}NPCTABLE;

typedef struct{

	long flag;
	long code_bullet;
	long bbits; // �����i�ǂ��󂹂�Ƃ��j
	long cond;  // ���

	long x,  y; // �����W
	long xm, ym;// �ړ���
	long tgt_x, tgt_y;

	long act_no;
	long act_wait;
	long ani_wait;
	long ani_no;

	unsigned char direct;//����
	RECT rect;
	long count1;
	long count2;
	long life_count; //�ő呶�ݎ��ԁB
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
	short width;//�}�b�v�̕�
	short length;//�}�b�v�̒���

}MAPDATA;


typedef struct{
	long code;
	long level;    //���x��
	long exp;      //�o���l
	long max_num;
	long num;
}ARMSDATA;

typedef struct{
	long code;
}ITEMDATA;

typedef struct{
	long index;//�C���f�b�N�X
	long event;//�C�x���gNO
}PERMITSTAGE;

typedef void (* NPCFUNCTION)( NPCHAR *npc );
typedef void (* BOSSFUNCTION)( void );



//キー状態の判定で使用する		 2進数
#define KEY_LEFT	0x0001		//0000_0000_0000_0001
#define KEY_RIGHT	0x0002		//0000_0000_0000_0010
#define KEY_UP		0x0004		//0000_0000_0000_0100
#define KEY_DOWN	0x0008		//0000_0000_0000_1000
#define KEY_W       0x0010		//0000_0000_0001_0000
#define KEY_X	    0x0020		//0000_0000_0010_0000
#define KEY_Z		0x0040		//0000_0000_0100_0000
#define KEY_S		0x0080		//0000_0000_1000_0000
#define KEY_A		0x0100		//0000_0000_1000_0000
#define KEY_SHIFT	0x0200		//0000_0001_0000_0000
#define KEY_F1		0x0400		//0000_0010_0000_0000
#define KEY_F2		0x0800		//0000_0100_0000_0000
#define KEY_ESC		0x8000		//1000_0000_0000_0000
#define KEY_Q		0x1000		//0001_0000_0000_0000

#define KEY_NE		0x00010000		
#define KEY_RU		0x00020000		
#define KEY_ME		0x00040000		
#define KEY_RI		0x00080000		
#define KEY_RE		0x00100000		


extern long gKey;
extern long gKeyTrg;

extern long gKeyArms;
extern long gKeyArmsRev;
extern long gKeyItem;
extern long gKeyJump;
extern long gKeyShot;
extern long gKeyMap;
extern long gKeyLeft;
extern long gKeyUp;   
extern long gKeyRight;
extern long gKeyDown;

extern long gKeyOk;
extern long gKeyCancel;


void GetTrg( void );

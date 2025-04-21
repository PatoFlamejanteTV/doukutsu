#define MAX_PROOF	  32
#define MAX_FONTNAME  64
#define MAX_JOYSTICKBUTTON_USE 8

#define MOVEBUTTONMODE_1       0
#define MOVEBUTTONMODE_2       1
#define ATTACKBUTTONMODE_ZJUMP 0
#define ATTACKBUTTONMODE_XJUMP 1
#define OKBUTTONMODE_JUMPOK    0
#define OKBUTTONMODE_SHOTOK    1

#define BUTTONACTION_JUMP    1
#define BUTTONACTION_SHOT    2
#define BUTTONACTION_ARMS    3
#define BUTTONACTION_ITEM    4
#define BUTTONACTION_MAP     5
#define BUTTONACTION_ARMSREV 6

#define WINDOWMODE_FULL16BIT  0
#define WINDOWMODE_320x240    1
#define WINDOWMODE_640x480    2
#define WINDOWMODE_FULL24BIT  3
#define WINDOWMODE_FULL32BIT  4

typedef struct{

	char proof[    MAX_PROOF   ];
	char font_name[MAX_FONTNAME];
	long move_button_mode;
	long attack_button_mode;
	long ok_button_mode;
	long display_mode;

	BOOL bJoystick;
	long joystick_button[MAX_JOYSTICKBUTTON_USE];

}CONFIGDATA;


BOOL LoadConfigData( CONFIGDATA *conf );
void DefaultConfigData( CONFIGDATA *conf );

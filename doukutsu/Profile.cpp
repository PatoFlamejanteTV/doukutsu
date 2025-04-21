#include <windows.h>
#include <stdio.h>

#include "Define.h"
#include "Tags.h"


static char *gDefaultName = "Profile.dat";
static char *gProfileCode = "Do041220";

BOOL TransferStage( long no, long w, long x, long y );
void InitMyChar( void );
void InitStar( void );
extern HWND ghWnd;

extern MYCHAR gMC;
extern long gStageNo;
extern long gMusicNo;
extern unsigned char gFlagNPC[];
extern ARMSDATA    gArmsData[];
extern ITEMDATA    gItemData[];
extern PERMITSTAGE gPermitStage[];
extern long gSelectedArms;
extern long gSelectedItem;
extern long gCampActive;
extern char gMapping[];

extern char gModulePath[];
extern long gCounter;

void GetStageInfo( long *no_stage, long *no_music );

typedef struct{

	char code[8];     // �v���t�@�C�����ʃR�[�h
	long stage;       // �X�e�[�W�ԍ�
	long music;       // BGM�ԍ�

	long x;           // ���W
	long y;
	long direct;      // ����
	short max_life;    // �ő�̗�
	short star;
	short life;        // ���̗�
	short a;
	long select_arms; // �I�𕐊�
	long select_item; // �I���A�C�e��
	long equip;       // �����t���O
	long unit;
	long counter;     // �v���C�J�E���^

	ARMSDATA    arms[           MAX_ARMSDATA    ];          // ��������
	ITEMDATA    items[          MAX_ITEMDATA    ];         // ��������
	PERMITSTAGE permitstage[    MAX_PERMITSTAGE ];// �]������
	char        permit_mapping[ MAX_MAPPING     ]; // �}�b�s���O����

	char        FLAG[4];//�t���O�N�V���{�� 'FLAG'
	
	unsigned char flags[MAX_FLAG/8];        // �t���O�Q

}PROFILEDATA;

// �v���t�@�C���̑���
BOOL IsProfile()
{
	HANDLE hFile;
	char path[MAX_PATH];
	sprintf( path, "%s\\%s", gModulePath, gDefaultName );

	hFile = CreateFile( path, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )return FALSE;
	CloseHandle( hFile );
	return TRUE;

}
// �v���t�@�C����ۑ�
BOOL SaveProfile( char *name )
{
	FILE *fp;
	PROFILEDATA profile;
	char *FLAG = "FLAG";

	char path[MAX_PATH];
	if( name ) sprintf( path, "%s\\%s", gModulePath, name         );
	else       sprintf( path, "%s\\%s", gModulePath, gDefaultName );
	fp = fopen( path, "wb" );
	if( !fp )return FALSE;

	// �v���t�@�C���\���̂ɓ���
	memset( &profile, 0, sizeof(PROFILEDATA) );
	memcpy( profile.code, gProfileCode, 8 );
	memcpy( profile.FLAG, FLAG, 4 );
	profile.stage       = gStageNo;
	profile.music       = gMusicNo;
	profile.x           = gMC.x;
	profile.y           = gMC.y;
	profile.direct      = gMC.direct;
	profile.max_life    = gMC.max_life;
	profile.life        = gMC.life;
	profile.star        = gMC.star;
	profile.select_arms = gSelectedArms;
	profile.select_item = gSelectedItem;
	profile.equip       = gMC.equip;
	profile.unit        = gMC.unit;
	profile.counter     = gCounter;
	memcpy(  &profile.arms[          0], &gArmsData[   0], sizeof(ARMSDATA)    * MAX_ARMSDATA    );
	memcpy(  &profile.items[         0], &gItemData[   0], sizeof(ITEMDATA)    * MAX_ITEMDATA    );
	memcpy(  &profile.permitstage[   0], &gPermitStage[0], sizeof(PERMITSTAGE) * MAX_PERMITSTAGE );
	memcpy(  &profile.permit_mapping[0], &gMapping[    0], sizeof(char)        * MAX_MAPPING     );
	memcpy(  &profile.flags[         0], &gFlagNPC[    0], sizeof(unsigned char) * MAX_FLAG/8    );

	fwrite( &profile, sizeof(PROFILEDATA), 1, fp );

	fclose( fp );

#ifdef _DEBUG
	// gCounter
	sprintf( path, "%s\\%s", gModulePath, "counter.dat" );
	fp = fopen( path, "wt" );
	fprintf( fp, "count:%d\r\n", gCounter );
#endif

	return TRUE;
}


BOOL ChangeOrganyaVolume( long vol );
void ChangeMusic( long no );
void ClearFade( void );
void SetFrameTargetMyChar( long wait );
void SetFrameMyChar( void );
void InitBossLife(   void );
void CutNoise( void );
void ClearValueView( void );
extern long gCurlyShoot_wait;

// �v���t�@�C����ǂݍ��ށ^����
BOOL LoadProfile( char *name )
{

	FILE        *fp;
	PROFILEDATA profile;
	char        path[MAX_PATH];

	if( name ) sprintf( path, "%s",     name                      );
	else       sprintf( path, "%s\\%s", gModulePath, gDefaultName );

	//�ǂ݂���------------------------------------------
	fp = fopen( path, "rb" );
	if( !fp )return FALSE;
	// �`�F�b�N�R�[�h
	fread( profile.code, 8, 1, fp );
	if( memcmp( profile.code, gProfileCode, 8 ) ) return FALSE;
	fseek( fp, 0, SEEK_SET );
	memset( &profile, 0, sizeof(PROFILEDATA) );
	fread( &profile, sizeof(PROFILEDATA), 1, fp );
	fclose( fp );//------------------------------------------

	gSelectedArms    = profile.select_arms;
	gSelectedItem    = profile.select_item;
	gCounter         = profile.counter;

	memcpy( gArmsData,    profile.arms,           sizeof(ARMSDATA)      * MAX_ARMSDATA    );
	memcpy( gItemData,    profile.items,          sizeof(ITEMDATA)      * MAX_ITEMDATA    );
	memcpy( gPermitStage, profile.permitstage,    sizeof(PERMITSTAGE)   * MAX_PERMITSTAGE );
	memcpy( gMapping,     profile.permit_mapping, sizeof(char)          * MAX_MAPPING     );
	memcpy( gFlagNPC,     profile.flags,          sizeof(unsigned char) * MAX_FLAG/8      );
	ChangeMusic( profile.music );

	InitMyChar();

	if( !TransferStage( profile.stage, 0, 0, 1 ) ){
		return FALSE;
	}

	gMC.equip    = profile.equip;
	gMC.unit     = profile.unit;
	gMC.direct   = profile.direct;
	gMC.max_life = profile.max_life;
	gMC.life     = profile.life;
	gMC.star     = profile.star;
	gMC.cond     = COND_ALIVE;
	gMC.air      = MAX_AIR;
	gMC.lifeBr   = gMC.life;
	gMC.x        = profile.x;
	gMC.y        = profile.y;

	//��������������X�V
	gMC.rect_arms.left   = gArmsData[gSelectedArms].code % 10* 24;
	gMC.rect_arms.right  = gMC.rect_arms.left + 24;
	gMC.rect_arms.top    = gArmsData[gSelectedArms].code / 10* 32;
	gMC.rect_arms.bottom = gMC.rect_arms.top  + 16;

	ClearFade();
	SetFrameMyChar();
	SetFrameTargetMyChar( 16 );
	InitBossLife();
	CutNoise();
	InitStar();
	ClearValueView();
	gCurlyShoot_wait = 0;

	
	return TRUE;
}

// �Q�[���̏������A����̊J�n
void SetFadeMask( void );
void ClearArmsData( void );
void ClearItemData( void );
void ClearPermitStage( void );
void StartMapping( void );
void InitFlags( void );

BOOL InitializeGame( HWND hWnd )
{
	// �v���t�@�C���Ώ�
	InitMyChar();
	gSelectedArms = 0;
	gSelectedItem = 0;
	gCounter      = 0;
	ClearArmsData();
	ClearItemData();
	ClearPermitStage();
	StartMapping();
	InitFlags();

	if( !TransferStage( 13, 200, 10, 8 ) ){
		MessageBox( hWnd, "�X�e�[�W�̓ǂݍ��݂Ɏ��s", "�G���[", MB_OK );
		return FALSE;
	}

	ClearFade();
	SetFrameMyChar();
	SetFrameTargetMyChar( 16 );
	InitBossLife();
	CutNoise();
	ClearValueView();
	gCurlyShoot_wait = 0;

	SetFadeMask();
	SetFrameTargetMyChar( 16 );
	return TRUE;
}

#include <windows.h>
#include <stdio.h>

#include "Define.h"
#include "Draw.h"
#include "Sound.h"
#include "Tags.h"

#include "Escape.h"
#include "KeyControl.h"

//16*4 + 6*2 = 64 +12  = 76
//6*36 + 6*2 = 216 +12 = 228

#define TEXTSCRIPT_BUFFERSIZE 1024*20//�ő�X�N���v�g�T�C�Y

#define TEXTFONT_WIDTH   6 //�t�H���g�̕�
#define TEXTLINE_HEIGHT 16 //�s�̍���
#define NUM_CHAR		36 //��s�ɂ����镶���̐�(1byte)
#define NUM_TEXTLINE     4 //�\���e�L�X�g�s���i���A��\���� -1 �j
#define TEXTSCRIPT_WAIT  4 //���\���E�G�C�g


#define PARAM_TEXTSCROLL	4//�X�N���[���l�i16�̖񐔁j

//RECT
#define TEXTSCRIPT_LEFT		(16 + 12 * 3)
#define TEXTSCRIPT_RIGHT	(TEXTSCRIPT_LEFT + NUM_CHAR * TEXTFONT_WIDTH)
#define TEXTSCRIPT_TOP		(240-48- 8)
#define TEXTSCRIPT_HEIGHT   (TEXTLINE_HEIGHT * ( NUM_TEXTLINE-1 ))

//FACE
#define FACE_X	(TEXTSCRIPT_LEFT)
//#define FACE_Y	(TEXTSCRIPT_TOP-3)

//����
#define MODE_TEXTSCRIPT_SLEEP	0//�ҋ@���
#define MODE_TEXTSCRIPT_READ	1//�e�L�X�g�o��
#define MODE_TEXTSCRIPT_NOD		2//���^�[���҂�
#define MODE_TEXTSCRIPT_SCROLL	3//�X�N���[���҂�
#define MODE_TEXTSCRIPT_WAIT	4//�E�G�C�g
#define MODE_TEXTSCRIPT_FADE	5//�t�F�[�h�҂�
#define MODE_TEXTSCRIPT_YESNO	6//Yes/No���}
#define MODE_TEXTSCRIPT_MYCHARSTEP 7 // ���n�҂�

//GETITEM
#define GETITEM_X  (320/2 - 32-8)
#define GETITEM_Y  128

extern HWND ghWnd;

extern MYCHAR gMC;

extern long g_GameFlags;

extern char gDataPath[];

/*
// ����L�[
extern long gKeyJump;
extern long gKeyShot;
extern long gKeyArms;
extern long gKeyArmsRev;
extern long gKeyItem;
extern long gKeyMap;
extern long gKeyLeft;
extern long gKeyUp;
extern long gKeyRight;
extern long gKeyDown;
*/
#define TSFLAGS_VIEW      0x0001 // ���b�Z�[�W�̕\��
#define TSFLAGS_FRAME     0x0002 // �t���[���̕\��
#define TSFLAGS_TURBO     0x0010 // �ꊇ����(�ꎞ)
#define TSFLAGS_HOIST     0x0020 // ��i�\��
#define TSFLAGS_AUTOTURBO 0x0040 // �����ꊇ����

typedef struct{
	char path[MAX_PATH];
	long size;//�T�C�Y
	char *data;//�X�N���v�g�f�[�^�ւ̃|�C���^

	char mode;
	char flags;


	long p_read;//�ǂݍ��݈ʒu
	long p_write;//�������݈ʒu
	long line;//���s�ڏ������݂��B
	long ypos_line[NUM_TEXTLINE];
	long wait;
	long wait_next;//�C�x���g�̃E�G�C�g�Ŏg��

	long next_event;//�u�������v��I���������̃W�����v���ۑ�
	char select;    // no�X�C�b�`

	//��\��
	long face;
	long face_x;

	//�A�C�e���\��
	long item;
	long item_y;

	//�\�� RECT
	RECT rcText;
	long offsetY;

	unsigned char wait_beam;//�r�[���_�ł̃E�G�C�g
}TEXTSCRIPT;

TEXTSCRIPT gTS;

static char _text[NUM_TEXTLINE][64];

extern RECT grcFull;

//RECT gRect_TS   = {TEXTSCRIPT_LEFT, TEXTSCRIPT_TOP, TEXTSCRIPT_RIGHT, TEXTSCRIPT_BOTTOM};
RECT gRect_line = {0,0,TEXTFONT_WIDTH * NUM_CHAR, TEXTLINE_HEIGHT};


long GetFileSizeLong( char *path );



//TEXTSCRIPT�\���́A�T�[�t�F�X�̏�����
BOOL InitTextScript2( void )
{
	int i;


	gTS.mode     = MODE_TEXTSCRIPT_SLEEP;
	g_GameFlags &= ~GAMEFLAG_TEXTSCRIPT;
	for( i = 0; i < NUM_TEXTLINE; i++ ){
		MakeSurface_Generic( gRect_line.right, gRect_line.bottom, SURF_TEXTSCRIPT + i, FALSE );
	}
	memset( _text, 0, sizeof(char) * 64 * NUM_TEXTLINE );
	//�e�ʂ̊m��
	gTS.data = (char *)malloc( TEXTSCRIPT_BUFFERSIZE );
	if( !gTS.data )return FALSE;
	return TRUE;
}

void EndTextScript( void )
{
	free( gTS.data );
	ReleaseSurface( SURF_TEXTBOX );
	for( long  i = 0; i < NUM_TEXTLINE; i++ ){
		ReleaseSurface( SURF_TEXTSCRIPT + i );
	}
}


void EncryptionBinaryData2( unsigned char *pData, long size )
{
	long i;
	long work;

	long half;
	long val1;

	half = size/2;
	if( !pData[half] ) val1 = (       7       ) * -1;
	else               val1 = (pData[half]%256) * -1;
	
	for( i = 0; i < size; i++ ){

		work = pData[i];
		work += val1;

		if( i != half ) pData[i] = (unsigned char)(work%256);
	}
}


//�X�N���v�g�̃��[�h
BOOL LoadTextScript2( char *name )
{
	FILE *fp;
//	HANDLE hFile;//check size

	char path[MAX_PATH];
	sprintf( path, "%s\\%s", gDataPath, name );

	//�T�C�Y�̊m�F
	gTS.size = GetFileSizeLong( path );
	if( gTS.size == -1 ) return FALSE;

	//�ǂݍ���
	fp = fopen( path, "rb" );
	if( !fp ) return FALSE;
	fread( gTS.data, sizeof(char), gTS.size, fp );
	gTS.data[gTS.size] = '\0';//last
	fclose( fp );
	//�ǂ݂��񂾃t�@�C������ۑ�
	strcpy( gTS.path, name );

	EncryptionBinaryData2( (unsigned char *)gTS.data, gTS.size );
	return TRUE;
}

//�X�N���v�g�̃��[�h(�X�e�[�W�w�b�_�t��)
BOOL LoadTextScript_Stage( char *name )
{
	FILE *fp;

	char path[MAX_PATH];
	long head_size;
	long body_size;
	// �w�b�h ------------------------------------
	sprintf( path, "%s\\%s", gDataPath, "Head.tsc" );

	//�T�C�Y�̊m�F
	head_size = GetFileSizeLong( path );
	if( head_size == -1 ) return FALSE;

	//�ǂݍ���
	fp = fopen( path, "rb" );
	if( !fp )return FALSE;
	fread( &gTS.data[0], sizeof(char), head_size, fp );
	EncryptionBinaryData2( (unsigned char *)&gTS.data[0], head_size );
	gTS.data[head_size] = '\0';//last
	fclose( fp );


	// �{�f�B�[ ----------------------------------
	sprintf( path, "%s\\%s", gDataPath, name );

	//�T�C�Y�̊m�F
	body_size = GetFileSizeLong( path );
	if( body_size == -1 ) return FALSE;

	//�ǂݍ���
	fp = fopen( path, "rb" );
	if( !fp )return FALSE;
	fread( &gTS.data[head_size], sizeof(char), body_size, fp );
	EncryptionBinaryData2( (unsigned char *)&gTS.data[head_size], body_size );
	gTS.data[head_size + body_size] = '\0';//last
	fclose( fp );


	gTS.size = head_size + body_size;


	//�ǂ݂��񂾃t�@�C������ۑ�
	strcpy( gTS.path, name );


//	fp = fopen( "c:\\test.txt", "wb" );
//	fwrite( gTS.data, sizeof(char), gTS.size, fp );
//	fclose( fp );

	return TRUE;
}


void GetTextScriptPath( char *path )
{
	strcpy( path, gTS.path );
}

//0000�̎擾
long GetTextScriptNo( long a )
{
	long b = 0;
	b += 1000 * ( gTS.data[a] - '0' );
	a++;
	b += 100  * ( gTS.data[a] - '0' );
	a++;
	b += 10   * ( gTS.data[a] - '0' );
	a++;
	b += 1    * ( gTS.data[a] - '0' );

	return b;
}


//MODE_TEXTSCRIPT_READ�̊J�n
BOOL StartTextScript( long no )
{
	int i;
	long event_no;

	//�p�����[�^�̃N���A
	gTS.mode      = MODE_TEXTSCRIPT_READ;
	g_GameFlags  |= GAMEFLAG_TEXTSCRIPT|GAMEFLAG_ACTION;
	gTS.line      = 0;
	gTS.p_write   = 0;
	gTS.wait      = TEXTSCRIPT_WAIT;//�����n�߂���
	gTS.flags     = 0;
	gTS.wait_beam = 0;
	gTS.face      = 0;
	gTS.item      = 0;
	gTS.offsetY   = 0;

	gMC.shock     = 0;


//	gTS.rcText        = gRect_TS;
	gTS.rcText.left   = TEXTSCRIPT_LEFT;
	gTS.rcText.top    = TEXTSCRIPT_TOP;
	gTS.rcText.right  = TEXTSCRIPT_RIGHT;
	gTS.rcText.bottom = gTS.rcText.top + TEXTSCRIPT_HEIGHT;


	//�T�[�t�F�X�̃N���A
	for( i = 0; i < NUM_TEXTLINE; i++ ){
		gTS.ypos_line[i] = TEXTLINE_HEIGHT * i;
		CortBox2( &gRect_line, COLOR_TEXTBACK, SURF_TEXTSCRIPT + i );
		memset( _text[ i ], 0, 64 );
	}

	//#�̌���
	gTS.p_read = 0;
	while( 1 ){
		if( gTS.data[gTS.p_read] == '\0' )return FALSE;
		if( gTS.data[gTS.p_read] == '#' ){
			gTS.p_read++;
			event_no = GetTextScriptNo( gTS.p_read );
			if( no == event_no )break;
			if( no < event_no )
				return FALSE;
		}
		gTS.p_read++;
	}

	//���̍s��
	while( gTS.data[gTS.p_read] != 0x0A )gTS.p_read++;
	gTS.p_read++;


	return TRUE;
}

//�X�N���v�g�W�����v
BOOL JumpTextScript( long no )
{
	int i;
	long event_no;

	//�p�����[�^�̃N���A
	gTS.mode      = MODE_TEXTSCRIPT_READ;
	g_GameFlags  |= GAMEFLAG_TEXTSCRIPT;
	gTS.line      = 0;
	gTS.p_write   = 0;
	gTS.wait      = TEXTSCRIPT_WAIT;//�����n�߂���
	gTS.wait_beam = 0;

	//�T�[�t�F�X�̃N���A
	for( i = 0; i < NUM_TEXTLINE; i++ ){
		gTS.ypos_line[i] = TEXTLINE_HEIGHT * i;
		CortBox2( &gRect_line, COLOR_TEXTBACK, SURF_TEXTSCRIPT + i );
		memset( _text[ i ], 0, 64 );
	}

	//#�̌���
	gTS.p_read = 0;
	while( 1 ){
		if( gTS.data[gTS.p_read] == '\0' )return FALSE;
		if( gTS.data[gTS.p_read] == '#' ){
			gTS.p_read++;
			event_no = GetTextScriptNo( gTS.p_read );
			if( no == event_no )break;
			if( no < event_no ){
				return FALSE;
			}
		}
		gTS.p_read++;
	}
	//���̍s��
	while( gTS.data[gTS.p_read] != 0x0A )gTS.p_read++;
	gTS.p_read++;
	return TRUE;
}

//�����I��
void StopTextScript( void )
{
	gTS.mode     = MODE_TEXTSCRIPT_SLEEP;
	g_GameFlags &= ~GAMEFLAG_TEXTSCRIPT;
	g_GameFlags  |= GAMEFLAG_ACTION|GAMEFLAG_KEYCONTROL;
	gTS.flags    = 0;
}

//���s�X�N���[���H
void CheckNewLine( void )
{
	if( gTS.ypos_line[ gTS.line%NUM_TEXTLINE ] == TEXTLINE_HEIGHT * ( NUM_TEXTLINE -1 ) ){

		gTS.mode = MODE_TEXTSCRIPT_SCROLL;
		g_GameFlags |= GAMEFLAG_TEXTSCRIPT;
		CortBox2( &gRect_line, COLOR_TEXTBACK, SURF_TEXTSCRIPT + gTS.line%NUM_TEXTLINE );
		memset( _text[gTS.line%NUM_TEXTLINE], 0, 64 );
	}
}

//���l�\��
long gNumberTextScript[4];
void SetNumberTextScript( long index )
{
	char str[5];
	long bZero;
	long a;
	long b;
	long offset;
	long i;

	long table[] = {1000,100,10};

	a = gNumberTextScript[index];

	// ����������
	bZero  = 0;
	offset = 0;
	for( i = 0; i < 3; i++ ){
		if( a / table[i] || bZero ){
			b = (a/table[i]);
			str[offset] = '0' + (char)b;
			bZero  = 1;
			a -= b*table[i];
			offset++;
		}
	}

	str[offset] = '0' + (char)a;
	str[offset+1] = '\0';

	//�\��
	PutText2( gTS.p_write * TEXTFONT_WIDTH, 0, str,
		COLOR_TEXT, SURF_TEXTSCRIPT + gTS.line%NUM_TEXTLINE );
	strcat( _text[gTS.line%NUM_TEXTLINE], str );

	PlaySoundObject( WAVE_MESSAGE, 1 );
	gTS.wait_beam = 0;//�`�撆�͓_�Ŗ����B
	gTS.p_write += strlen( str );
	//�������s
	if( gTS.p_write >= NUM_CHAR - 1){
		gTS.p_write = 0;
		gTS.line++;
		CheckNewLine();
	}


}

//�e�L�X�g�̈�̃N���A : <CLR
void ClearTextLine( void )
{
	int i;

	gTS.line    = 0;
	gTS.p_write = 0;
	gTS.offsetY = 0;
	//�T�[�t�F�X�̃N���A
	for( i = 0; i < NUM_TEXTLINE; i++ ){
		gTS.ypos_line[i] = TEXTLINE_HEIGHT * i;
		CortBox2( &gRect_line, COLOR_TEXTBACK, SURF_TEXTSCRIPT + i );
		memset( _text[ i ], 0, 64 );
	}
}

//�\��
void PutTextScript( void )
{
	int i;
	RECT rect;
	long text_offset;

	if( gTS.mode == MODE_TEXTSCRIPT_SLEEP )return;
	if( !( gTS.flags & TSFLAGS_VIEW ) )return;

	// �\���ʒu
	if( gTS.flags & TSFLAGS_HOIST ){
		gTS.rcText.top    = 32;
		gTS.rcText.bottom = gTS.rcText.top + TEXTSCRIPT_HEIGHT;
	}else{
		gTS.rcText.top    = TEXTSCRIPT_TOP;
		gTS.rcText.bottom = gTS.rcText.top + TEXTSCRIPT_HEIGHT;
	}

	//�e�L�X�g�{�b�N�X
	if( gTS.flags & TSFLAGS_FRAME ){

		RECT rcFrame1 = {0, 0,244, 8};
		RECT rcFrame2 = {0, 8,244,16};
		RECT rcFrame3 = {0,16,244,24};

		                         PutBitmap3( &grcFull, 38, gTS.rcText.top - 10,       &rcFrame1, SURF_TEXTBOX );
		for( i = 1; i < 7; i++ ) PutBitmap3( &grcFull, 38, gTS.rcText.top - 10 + i*8, &rcFrame2, SURF_TEXTBOX );
		                         PutBitmap3( &grcFull, 38, gTS.rcText.top - 10 + i*8, &rcFrame3, SURF_TEXTBOX );
	}

	//��
	RECT rcFace;
	rcFace.left   = 48 * ( gTS.face % 6 );
	rcFace.top    = 48 * ( gTS.face / 6 );
	rcFace.right  = rcFace.left + 48;
	rcFace.bottom = rcFace.top  + 48;
	if( FACE_X*VS > gTS.face_x ) gTS.face_x += 8*VS;
//	gTS.face_x += (FACE_X*VS - gTS.face_x)/8;

	PutBitmap3( &gTS.rcText, gTS.face_x/VS, gTS.rcText.top-3, &rcFace, SURF_FACE );

	//�e�L�X�g���C���̕\��
	if( gTS.face ) text_offset = 56;
	else           text_offset =  0;

	for( i = 0; i < NUM_TEXTLINE; i++ ){

		PutBitmap3( &gTS.rcText,
			TEXTSCRIPT_LEFT + text_offset,
			gTS.rcText.top + gTS.ypos_line[i] + gTS.offsetY,
			&gRect_line, SURF_TEXTSCRIPT + i );
	}

	//�r�[��
	if( gTS.wait_beam++ % 20 > 12 &&
		gTS.mode == MODE_TEXTSCRIPT_NOD ){

		rect.left   = gTS.p_write * TEXTFONT_WIDTH + TEXTSCRIPT_LEFT + text_offset;
		rect.top    = ( gTS.ypos_line[gTS.line%NUM_TEXTLINE] ) + gTS.rcText.top + gTS.offsetY;
		rect.right  = rect.left + TEXTFONT_WIDTH - 1;
		rect.bottom = rect.top  + 12             - 1;
		CortBox( &rect, COLOR_TEXT );
	}


	//�A�C�e���C���[�W
	RECT rcItemBox1 = {  0,  0, 72, 16};//��
	RECT rcItemBox2 = {  0,  8, 72, 24};//��
	RECT rcItemBox3 = {240,  0,244,  8};//�E��
	RECT rcItemBox4 = {240,  8,244, 16};//�E��
	RECT rcItemBox5 = {240, 16,244, 24};//�E��
	if( gTS.item ){
		PutBitmap3( &grcFull, GETITEM_X,      GETITEM_Y,      &rcItemBox1, SURF_TEXTBOX );
		PutBitmap3( &grcFull, GETITEM_X,      GETITEM_Y + 16, &rcItemBox2, SURF_TEXTBOX );
		PutBitmap3( &grcFull, GETITEM_X + 72, GETITEM_Y,      &rcItemBox3, SURF_TEXTBOX );
		PutBitmap3( &grcFull, GETITEM_X + 72, GETITEM_Y +  8, &rcItemBox4, SURF_TEXTBOX );
		PutBitmap3( &grcFull, GETITEM_X + 72, GETITEM_Y + 16, &rcItemBox4, SURF_TEXTBOX );
		PutBitmap3( &grcFull, GETITEM_X + 72, GETITEM_Y + 24, &rcItemBox5, SURF_TEXTBOX );

		if( gTS.item_y < GETITEM_Y + 8 ) gTS.item_y++;
		if( gTS.item < 1000 ){//����
			rect.left   = (gTS.item%16)  * 16;
			rect.right  = rect.left + 16;
			rect.top    = (gTS.item/16)  * 16;
			rect.bottom = rect.top  + 16;
			PutBitmap3( &grcFull, GETITEM_X + 20+8, gTS.item_y, &rect, SURF_ARMSIMAGE );
		}else{                //�A�C�e��
			rect.left   = ((gTS.item-1000)%8)  * 32;
			rect.right  = rect.left + 32;
			rect.top    = ((gTS.item-1000)/8)  * 16;
			rect.bottom = rect.top  + 16;
			PutBitmap3( &grcFull, GETITEM_X + 20, gTS.item_y, &rect, SURF_ITEMIMAGE );
		}
	}
	//Yes/No
	RECT rect_yesno = { 152,  48, 244,  80 }; 
	RECT rect_cur   = { 112,  88, 128, 104 };
	if( gTS.mode == MODE_TEXTSCRIPT_YESNO ){
		if( gTS.wait < 2 ) i = 144 + (2 - gTS.wait)*4;
		else               i = 144;
		PutBitmap3( &grcFull,  TEXTSCRIPT_RIGHT-52, i, &rect_yesno, SURF_TEXTBOX );//yes_no
		if( gTS.wait == 16 )
			PutBitmap3( &grcFull,  TEXTSCRIPT_RIGHT-52 - 5 + 41* gTS.select, 144 + 10, &rect_cur,   SURF_TEXTBOX );// Cursor
	}
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////



BOOL TransferStage( long no, long w, long x, long y );
void StartMapName( void );

void FullArmsEnergy( void );

extern long gKeyTrg;
extern long gKey;


// �t���[������
void SetFrameTargetMyChar( long wait );
void SetFrameTargetNpChar( long event, long wait );
void SetFrameTargetBoss( long no, long wait );

// �G�t�F�N�g
void SetNoise( long no, long freq );
void CutNoise( void );
void StartFadeIn(  char dir );
void StartFadeOut( char dir );
BOOL GetFadeActive( void );
void SetFlash( long x,long y, long mode );

void ResetCheck( void );

// �t���O�Ǘ�
BOOL GetNPCFlag(  long a );
void SetNPCFlag(  long a );
void CutNPCFlag(  long a );
BOOL GetSkipFlag( long a );
void SetSkipFlag( long a );
void CutSkipFlag( long a );

// npc
BOOL GetNpCharAlive( long code_event );
BOOL IsNpCharCode( long code );
void SetNpCharActionNo( long code_event, long act_no, long dir );
void MoveNpChar( long code_event, long x, long y, long dir );
void DeleteNpCharEvent( long code );
void DeleteNpCharCode(  long code, BOOL bSmoke );
void ChangeNpCharByEvent( long code_event, long code_char, long dir );
void ChangeCheckableNpCharByEvent( long code_event, long code_char, long dir );
void SetNpChar( long code_char, long x, long y, long xm, long ym, long dir, NPCHAR *npc, long start_index );

// �}�b�s���O
BOOL IsMapping( void );
enum_ESCRETURN MiniMapLoop( void );
void SetMapping( long a );

enum_ESCRETURN StageSelectLoop( long *p_event );

// ���y�ύX
void ChangeMusic( long no );
void ReCallMusic( void );
void SetOrganyaFadeout( void );

// Boss
void SetQuake( long time );
BOOL StartBossLife( long code_event );
BOOL StartBossLife2( void );
void SetBossCharActNo( long a );

// �}�C�L����
void SetMyCharDirect( unsigned char dir );
void SetMyCharPosition( long x, long y );
void PitMyChar( void );
void ChangeMyUnit( unsigned char a );
void EquipItem( long flag, BOOL b );
void AddLifeMyChar( long x );
void AddMaxLifeMyChar( long val );
long GetUnitMyChar( void );
void BackStepMyChar( long code_event );
void ZeroMyCharXMove( void );
void ShowMyChar( BOOL bShow );

// �}�b�v�p�[�c����
void ShiftMapParts( long x, long y );
BOOL ChangeMapParts( long x, long y, unsigned char no );

// profile
BOOL LoadProfile( char *name );
BOOL SaveProfile( char *name );
BOOL InitializeGame( HWND hWnd );

//void PutNumber4( RECT *rect_view, long x, long y, long value, long col );

// ����E�A�C�e��
BOOL CheckItem( long a );
BOOL CheckArms( long a );
BOOL AddItemData( long code );
BOOL SubItemData( long code );
BOOL AddArmsData( long code, long max_num );
BOOL SubArmsData( long code );
BOOL TradeArms( long code1, long code2, long max_num );
BOOL AddPermitStage( long index, long event );
void ZeroArmsEnergy_All( void );

BOOL StartCreditScript( void );

// credit
void SetCreditIllust( long a );
void CutCreditIllust( void );
enum_ESCRETURN Scene_DownIsland( HWND hWnd, long mode );

BOOL SaveTimeCounter( void );


extern RECT grcFull;




//TEXTSCRIPT�v���V�[�W��
enum_ESCRETURN TextScriptProc( void )
{
	int i;
	char c[3];
	char str[NUM_CHAR*2];// <-�Ă��Ɓ[
	long w, x, y, z;

	BOOL bExit;
	
	RECT rcSymbol = {64,48,72,56};



	switch( gTS.mode ){
	case MODE_TEXTSCRIPT_SLEEP:
		break;
	//DATA��́�����������������������������������������������������
	case MODE_TEXTSCRIPT_READ:
		//�E�G�C�g
		gTS.wait++;
		if( !( g_GameFlags & (GAMEFLAG_KEYCONTROL) ) && gKey & (gKeyOk|gKeyCancel) ) gTS.wait += 4;
		if( gTS.wait < TEXTSCRIPT_WAIT )break;
		gTS.wait = 0;

		bExit = FALSE;
		while( !bExit ){
			//1BYTE�`�F�b�N
			if( gTS.data[gTS.p_read] == '<' ){//CODE
				if( gTS.data[gTS.p_read + 1] == 'E' &&//���C�x���g�I��
					gTS.data[gTS.p_read + 2] == 'N' &&
					gTS.data[gTS.p_read + 3] == 'D' ){
					gTS.mode = MODE_TEXTSCRIPT_SLEEP;
//					if( !( g_GameFlags & (GAMEFLAG_ACTION|GAMEFLAG_KEYCONTROL) ) ) ResetCheck();
					gMC.cond &= ~COND_CHECK;
					g_GameFlags |= (GAMEFLAG_ACTION|GAMEFLAG_KEYCONTROL);
					gTS.face   = 0;
					bExit      = TRUE;

				// �}�C�L���� �X�e�[�^�X ************************
				}else if( gTS.data[gTS.p_read + 1] == 'L' &&//���̗� +
					      gTS.data[gTS.p_read + 2] == 'I' &&
					      gTS.data[gTS.p_read + 3] == '+' ){
					x = GetTextScriptNo( gTS.p_read + 4 );
					AddLifeMyChar( x );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//���ő�̗� +
					      gTS.data[gTS.p_read + 2] == 'L' &&
					      gTS.data[gTS.p_read + 3] == '+' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					AddMaxLifeMyChar( z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'A' &&//����G�l���M�[��
					      gTS.data[gTS.p_read + 2] == 'E' &&
					      gTS.data[gTS.p_read + 3] == '+' ){
					FullArmsEnergy();
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'I' &&//���A�C�e���ǉ��i�󂫂�������Ζ����j
					      gTS.data[gTS.p_read + 2] == 'T' &&
					      gTS.data[gTS.p_read + 3] == '+' ){
					x = GetTextScriptNo( gTS.p_read + 4 );//NO
					PlaySoundObject( WAVE_GETITEM, 1 );
					AddItemData( x );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'I' &&//���A�C�e���Ȃ��Ȃ�
					      gTS.data[gTS.p_read + 2] == 'T' &&
					      gTS.data[gTS.p_read + 3] == '-' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					SubItemData( z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'E' &&//����������
					      gTS.data[gTS.p_read + 2] == 'Q' &&
					      gTS.data[gTS.p_read + 3] == '+' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					EquipItem( z, TRUE );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'E' &&//�������͂���
					      gTS.data[gTS.p_read + 2] == 'Q' &&
					      gTS.data[gTS.p_read + 3] == '-' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					EquipItem( z, FALSE );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'A' &&//������ǉ��i�󂫂�������Ζ����j
					      gTS.data[gTS.p_read + 2] == 'M' &&
					      gTS.data[gTS.p_read + 3] == '+' ){
					w = GetTextScriptNo( gTS.p_read + 4  );//NO
					x = GetTextScriptNo( gTS.p_read + 9  );//MAX_NUM
					gNumberTextScript[0] = x;
					gNumberTextScript[1] = z;
					PlaySoundObject( WAVE_GETITEM, 1 );
					AddArmsData( w, x );
					gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'A' &&//������Ȃ��Ȃ�
					      gTS.data[gTS.p_read + 2] == 'M' &&
					      gTS.data[gTS.p_read + 3] == '-' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					SubArmsData( z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'Z' &&//���A�C�e���Ȃ��Ȃ�
					      gTS.data[gTS.p_read + 2] == 'A' &&
					      gTS.data[gTS.p_read + 3] == 'M' ){
					ZeroArmsEnergy_All();
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'T' &&//�����������
					      gTS.data[gTS.p_read + 2] == 'A' &&
					      gTS.data[gTS.p_read + 3] == 'M' ){
					x = GetTextScriptNo( gTS.p_read + 4 );//������
					y = GetTextScriptNo( gTS.p_read + 9 );//�V����
					z = GetTextScriptNo( gTS.p_read + 14 );//�V����ő吔
					TradeArms( x, y, z );
					gTS.p_read += 18;
				}else if( gTS.data[gTS.p_read + 1] == 'P' &&//��PermitStage +
					      gTS.data[gTS.p_read + 2] == 'S' &&
					      gTS.data[gTS.p_read + 3] == '+' ){
					x = GetTextScriptNo( gTS.p_read + 4 );
					y = GetTextScriptNo( gTS.p_read + 9 );
					AddPermitStage( x, y );
					gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//���}�b�s���O
					      gTS.data[gTS.p_read + 2] == 'P' &&
					      gTS.data[gTS.p_read + 3] == '+' ){
					x = GetTextScriptNo( gTS.p_read + 4  );//NO
					SetMapping( x );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'U' &&//�����j�b�g�`�F���W
					      gTS.data[gTS.p_read + 2] == 'N' &&
					      gTS.data[gTS.p_read + 3] == 'I' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					ChangeMyUnit( (unsigned char )z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//���Z�[�u�J�E���^�[
					      gTS.data[gTS.p_read + 2] == 'T' &&
					      gTS.data[gTS.p_read + 3] == 'C' ){
					SaveTimeCounter();
					gTS.p_read += 4;
				// ���}�C�L���� �A�N�V���� ****************************
				}else if( gTS.data[gTS.p_read + 1] == 'T' &&//�����̃}�b�v�ֈړ�
					      gTS.data[gTS.p_read + 2] == 'R' &&
					      gTS.data[gTS.p_read + 3] == 'A' ){
					z = GetTextScriptNo( gTS.p_read + 4 );//�}�b�v�e�[�u��
					w = GetTextScriptNo( gTS.p_read + 9 );//���ɋN����C�x���g
					x = GetTextScriptNo( gTS.p_read + 14 );//�����W
					y = GetTextScriptNo( gTS.p_read + 19 );//�����W
					if( !TransferStage( z, w, x, y ) ){
						MessageBox( ghWnd, "�X�e�[�W�̓ǂݍ��݂Ɏ��s", "�G���[", MB_OK );
						return enum_ESCRETURN_exit;
					}
				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//�����}�b�v�̕ʂ̏ꏊ�ֈړ�
					      gTS.data[gTS.p_read + 2] == 'O' &&
					      gTS.data[gTS.p_read + 3] == 'V' ){
					x = GetTextScriptNo( gTS.p_read + 4 );//
					y = GetTextScriptNo( gTS.p_read + 9 );//
					SetMyCharPosition( x*VS*PARTSSIZE, y*VS*PARTSSIZE );
					gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'H' &&//���}�C�L�������B��
					      gTS.data[gTS.p_read + 2] == 'M' &&
					      gTS.data[gTS.p_read + 3] == 'C' ){
					ShowMyChar( FALSE );
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//���}�C�L������������
					      gTS.data[gTS.p_read + 2] == 'M' &&
					      gTS.data[gTS.p_read + 3] == 'C' ){
					ShowMyChar( TRUE );
					gTS.p_read += 4;

				// �C�x���g�R���g���[��
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//���t���O +
					      gTS.data[gTS.p_read + 2] == 'L' &&
					      gTS.data[gTS.p_read + 3] == '+' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					SetNPCFlag( z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//���t���O -
					      gTS.data[gTS.p_read + 2] == 'L' &&
					      gTS.data[gTS.p_read + 3] == '-' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					CutNPCFlag( z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//��skip�t���O +
					      gTS.data[gTS.p_read + 2] == 'K' &&
					      gTS.data[gTS.p_read + 3] == '+' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					SetSkipFlag( z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//��skip�t���O -
					      gTS.data[gTS.p_read + 2] == 'K' &&
					      gTS.data[gTS.p_read + 3] == '-' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					CutSkipFlag( z );
					gTS.p_read += 8;

				// �e�L�X�g�R���g���[��
				}else if( gTS.data[gTS.p_read + 1] == 'K' &&//���L�[���͓Ɛ��]
					      gTS.data[gTS.p_read + 2] == 'E' &&
					      gTS.data[gTS.p_read + 3] == 'Y' ){
					g_GameFlags &= ~GAMEFLAG_KEYCONTROL;
					g_GameFlags |=  GAMEFLAG_ACTION;
					gMC.up      = 0;
					gMC.shock   = 0;
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'P' &&//���C�x���g�v���C�I���e�B
					      gTS.data[gTS.p_read + 2] == 'R' &&
					      gTS.data[gTS.p_read + 3] == 'I' ){
					g_GameFlags &= ~(GAMEFLAG_KEYCONTROL|GAMEFLAG_ACTION);
					gMC.shock   = 0;
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//���C�x���g�v���C�I���e�B����
					      gTS.data[gTS.p_read + 2] == 'R' &&
					      gTS.data[gTS.p_read + 3] == 'E' ){
					g_GameFlags |= (GAMEFLAG_KEYCONTROL|GAMEFLAG_ACTION);
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'N' &&//�����Ƒ҂�
					      gTS.data[gTS.p_read + 2] == 'O' &&
					      gTS.data[gTS.p_read + 3] == 'D' ){
					gTS.mode = MODE_TEXTSCRIPT_NOD;
					gTS.p_read += 4;
					bExit = TRUE;
				}else if( gTS.data[gTS.p_read + 1] == 'C' &&//���e�L�X�g�X�y�[�X�N���A
					      gTS.data[gTS.p_read + 2] == 'L' &&
					      gTS.data[gTS.p_read + 3] == 'R' ){
					ClearTextLine( );
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//�����b�Z�[�W�\��
					      gTS.data[gTS.p_read + 2] == 'S' &&
					      gTS.data[gTS.p_read + 3] == 'G' ){
					ClearTextLine();
					gTS.flags |=  (TSFLAGS_VIEW |TSFLAGS_FRAME);
					gTS.flags &= ~(TSFLAGS_TURBO|TSFLAGS_HOIST);
					if( gTS.flags & TSFLAGS_AUTOTURBO ) gTS.flags |= TSFLAGS_TURBO;
					gTS.p_read   += 4;
					bExit = TRUE;
				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//�����b�Z�[�W�\��
					      gTS.data[gTS.p_read + 2] == 'S' &&
					      gTS.data[gTS.p_read + 3] == '2' ){
					ClearTextLine( );
					gTS.flags        &= ~(TSFLAGS_TURBO|TSFLAGS_FRAME);
					gTS.flags        |=  (TSFLAGS_VIEW|TSFLAGS_HOIST);
					if( gTS.flags & TSFLAGS_AUTOTURBO ) gTS.flags |= TSFLAGS_TURBO;
					gTS.face          =   0;
					gTS.p_read       +=   4;
					bExit       = TRUE;
				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//�����b�Z�[�W�f���\��
					      gTS.data[gTS.p_read + 2] == 'S' &&
					      gTS.data[gTS.p_read + 3] == '3' ){
					ClearTextLine();
					gTS.flags  &= ~TSFLAGS_TURBO;
					gTS.flags |= (TSFLAGS_VIEW|TSFLAGS_FRAME|TSFLAGS_HOIST);
					if( gTS.flags & TSFLAGS_AUTOTURBO ) gTS.flags |= TSFLAGS_TURBO;
					gTS.p_read += 4;
					bExit       = TRUE;
				}else if( gTS.data[gTS.p_read + 1] == 'W' &&//���E�G�C�g
					      gTS.data[gTS.p_read + 2] == 'A' &&
					      gTS.data[gTS.p_read + 3] == 'I' ){
					gTS.mode = MODE_TEXTSCRIPT_WAIT;
					gTS.wait_next = GetTextScriptNo( gTS.p_read + 4 );
					gTS.p_read += 8;
					bExit = TRUE;
				}else if( gTS.data[gTS.p_read + 1] == 'W' &&//�����n�ҋ@
					      gTS.data[gTS.p_read + 2] == 'A' &&
					      gTS.data[gTS.p_read + 3] == 'S' ){
					gTS.mode = MODE_TEXTSCRIPT_MYCHARSTEP;
					gTS.p_read += 4;
					bExit = TRUE;
				}else if( gTS.data[gTS.p_read + 1] == 'T' &&//���^�[�{�\��
					      gTS.data[gTS.p_read + 2] == 'U' &&
					      gTS.data[gTS.p_read + 3] == 'R' ){
					gTS.p_read  += 4;
					gTS.flags |= (TSFLAGS_TURBO);
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//�������^�[�{�X�C�b�` ON
					      gTS.data[gTS.p_read + 2] == 'A' &&
					      gTS.data[gTS.p_read + 3] == 'T' ){
					gTS.p_read  += 4;
					gTS.flags |= (TSFLAGS_AUTOTURBO);
				}else if( gTS.data[gTS.p_read + 1] == 'C' &&//�������^�[�{�X�C�b�` OFF
					      gTS.data[gTS.p_read + 2] == 'A' &&
					      gTS.data[gTS.p_read + 3] == 'T' ){
					gTS.p_read  += 4;
					gTS.flags |= (TSFLAGS_AUTOTURBO);
				}else if( gTS.data[gTS.p_read + 1] == 'C' &&//�����b�Z�[�W�N���[�Y
					      gTS.data[gTS.p_read + 2] == 'L' &&
					      gTS.data[gTS.p_read + 3] == 'O' ){
					gTS.flags &= ~(TSFLAGS_VIEW|TSFLAGS_FRAME|TSFLAGS_TURBO|TSFLAGS_HOIST);
					gTS.p_read += 4;

				// ���W�����v
				}else if( gTS.data[gTS.p_read + 1] == 'E' &&//���P���C�x���g�W�����v
					      gTS.data[gTS.p_read + 2] == 'V' &&
					      gTS.data[gTS.p_read + 3] == 'E' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					JumpTextScript( z );
				}else if( gTS.data[gTS.p_read + 1] == 'Y' &&//��YES/NO�W�����v(No is jump!)
					      gTS.data[gTS.p_read + 2] == 'N' &&
					      gTS.data[gTS.p_read + 3] == 'J' ){
					gTS.next_event = GetTextScriptNo( gTS.p_read + 4 );
					gTS.p_read    += 8;
					gTS.mode       = MODE_TEXTSCRIPT_YESNO;
					PlaySoundObject( WAVE_YESNO, 1 );
					gTS.wait       =  0;
					gTS.select     =  0;
					bExit = TRUE;
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//��NPC�t���O�W�����v
					      gTS.data[gTS.p_read + 2] == 'L' &&
					      gTS.data[gTS.p_read + 3] == 'J' ){
					x = GetTextScriptNo( gTS.p_read + 4 );
					z = GetTextScriptNo( gTS.p_read + 9 );
					if( GetNPCFlag( x ) )JumpTextScript( z );//�W�����v
					else gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//���X�L�b�v�t���O�W�����v
					      gTS.data[gTS.p_read + 2] == 'K' &&
					      gTS.data[gTS.p_read + 3] == 'J' ){
					x = GetTextScriptNo( gTS.p_read + 4 );
					z = GetTextScriptNo( gTS.p_read + 9 );
					if( GetSkipFlag( x ) )JumpTextScript( z );//�W�����v
					else gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'I' &&//���A�C�e���W�����v
					      gTS.data[gTS.p_read + 2] == 'T' &&
					      gTS.data[gTS.p_read + 3] == 'J' ){
					x = GetTextScriptNo( gTS.p_read + 4 );
					z = GetTextScriptNo( gTS.p_read + 9 );
					if( CheckItem( x ) )JumpTextScript( z );//�W�����v
					else gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'A' &&//������W�����v
					      gTS.data[gTS.p_read + 2] == 'M' &&
					      gTS.data[gTS.p_read + 3] == 'J' ){
					x = GetTextScriptNo( gTS.p_read + 4 );
					z = GetTextScriptNo( gTS.p_read + 9 );
					if( CheckArms( x ) )JumpTextScript( z );//�W�����v
					else gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'U' &&//�����j�b�g�W�����v
					      gTS.data[gTS.p_read + 2] == 'N' &&
					      gTS.data[gTS.p_read + 3] == 'J' ){
					x = GetTextScriptNo( gTS.p_read + 4 );
					z = GetTextScriptNo( gTS.p_read + 9 );
					if( GetUnitMyChar() == x )JumpTextScript( z );//�W�����v
					else gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'E' &&//��NP�������ł�����W�����v�i�C�x���g�R�[�h�j
					      gTS.data[gTS.p_read + 2] == 'C' &&
					      gTS.data[gTS.p_read + 3] == 'J' ){
					x = GetTextScriptNo( gTS.p_read + 4 );
					z = GetTextScriptNo( gTS.p_read + 9 );
					if( GetNpCharAlive( x ) )JumpTextScript( z );//�W�����v
					else gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'N' &&//���w�肵���L������������W�����v�iNP�R�[�h�j
					      gTS.data[gTS.p_read + 2] == 'C' &&
					      gTS.data[gTS.p_read + 3] == 'J' ){
					x = GetTextScriptNo( gTS.p_read + 4 );//�L�����R�[�h
					z = GetTextScriptNo( gTS.p_read + 9 );//�W�����v�R�[�h
					if( IsNpCharCode( x ) )JumpTextScript( z );//�W�����v
					else gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//���w�肵���}�b�v������΃W�����v
					      gTS.data[gTS.p_read + 2] == 'P' &&
					      gTS.data[gTS.p_read + 3] == 'J' ){
					x = GetTextScriptNo( gTS.p_read + 4 );//�W�����v�R�[�h
					if( IsMapping() )
						JumpTextScript( x );//�W�����v
					else gTS.p_read += 8;


				// ���G�t�F�N�g **********************
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//���V�����[ON
					      gTS.data[gTS.p_read + 2] == 'S' &&
					      gTS.data[gTS.p_read + 3] == 'S' ){
					x = GetTextScriptNo( gTS.p_read + 4 );
					SetNoise( NOISE_RIVER, x );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'C' &&//���V�����[OFF
					      gTS.data[gTS.p_read + 2] == 'S' &&
					      gTS.data[gTS.p_read + 3] == 'S' ){
					CutNoise();
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//���v���y���n�m
					      gTS.data[gTS.p_read + 2] == 'P' &&
					      gTS.data[gTS.p_read + 3] == 'S' ){
					SetNoise( NOISE_PROPERA, x );
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'C' &&//���v���y��OFF
					      gTS.data[gTS.p_read + 2] == 'P' &&
					      gTS.data[gTS.p_read + 3] == 'S' ){
					CutNoise();
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'Q' &&//���n�k
					      gTS.data[gTS.p_read + 2] == 'U' &&
					      gTS.data[gTS.p_read + 3] == 'A' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					SetQuake( z );
//					PlaySoundObject( WAVE_QUAKE, 1 );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//���t���b�V��
					      gTS.data[gTS.p_read + 2] == 'L' &&
					      gTS.data[gTS.p_read + 3] == 'A' ){
					SetFlash( 0,0,FLASHMODE_FLASH );
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//���t�F�[�h�C��
					      gTS.data[gTS.p_read + 2] == 'A' &&
					      gTS.data[gTS.p_read + 3] == 'I' ){
					z = GetTextScriptNo( gTS.p_read + 4 );//0:��/2:��
					StartFadeIn( (char)z );
					gTS.mode = MODE_TEXTSCRIPT_FADE;
					gTS.p_read += 8;
					bExit = TRUE;
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//���t�F�[�h�A�E�g
					      gTS.data[gTS.p_read + 2] == 'A' &&
					      gTS.data[gTS.p_read + 3] == 'O' ){
					z = GetTextScriptNo( gTS.p_read + 4 );//0:��/2:��
					StartFadeOut( (char)z );
					gTS.mode = MODE_TEXTSCRIPT_FADE;
					gTS.p_read += 8;
					bExit = TRUE;
				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//���}�b�v�̖��O��\��
					      gTS.data[gTS.p_read + 2] == 'N' &&
					      gTS.data[gTS.p_read + 3] == 'A' ){
					StartMapName( );
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//���t�H�[�J�X��MyChar��
					      gTS.data[gTS.p_read + 2] == 'O' &&
					      gTS.data[gTS.p_read + 3] == 'M' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					SetFrameTargetMyChar( z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//���t�H�[�J�X��NPC��
					      gTS.data[gTS.p_read + 2] == 'O' &&
					      gTS.data[gTS.p_read + 3] == 'N' ){
					x = GetTextScriptNo( gTS.p_read + 4 );
					y = GetTextScriptNo( gTS.p_read + 9 );//�t���[�����x
					SetFrameTargetNpChar( x, y );
					gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//���t�H�[�J�X��Boss��
					      gTS.data[gTS.p_read + 2] == 'O' &&
					      gTS.data[gTS.p_read + 3] == 'B' ){
					x = GetTextScriptNo( gTS.p_read + 4 );
					y = GetTextScriptNo( gTS.p_read + 9 );//�t���[�����x
					SetFrameTargetBoss( x, y );
					gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//�����ʉ�
					      gTS.data[gTS.p_read + 2] == 'O' &&
					      gTS.data[gTS.p_read + 3] == 'U' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					PlaySoundObject( z, 1 );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'C' &&//��BGM�`�F���W
					      gTS.data[gTS.p_read + 2] == 'M' &&
					      gTS.data[gTS.p_read + 3] == 'U' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					ChangeMusic( z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//���}�b�v�̖��O��\��
					      gTS.data[gTS.p_read + 2] == 'M' &&
					      gTS.data[gTS.p_read + 3] == 'U' ){
					SetOrganyaFadeout();
					gTS.p_read += 4;


				}else if( gTS.data[gTS.p_read + 1] == 'R' &&//��BGM���R�[��
					      gTS.data[gTS.p_read + 2] == 'M' &&
					      gTS.data[gTS.p_read + 3] == 'U' ){
					ReCallMusic();
					gTS.p_read += 4;

				// �����[�v�Ăяo�� **************************
				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//���}�b�v���[�v
					      gTS.data[gTS.p_read + 2] == 'L' &&
					      gTS.data[gTS.p_read + 3] == 'P' ){
					gTS.p_read += 4;
					bExit      = TRUE;
					switch( MiniMapLoop( ) ){
					case enum_ESCRETURN_exit:     return enum_ESCRETURN_exit;
					case enum_ESCRETURN_restart:  return enum_ESCRETURN_restart;
					case enum_ESCRETURN_continue: break;
					}
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//���X�e�[�W�I�����[�v
					      gTS.data[gTS.p_read + 2] == 'L' &&
					      gTS.data[gTS.p_read + 3] == 'P' ){
					bExit      = TRUE;
//					BackupSurface( SURF_DISPLAY, &grcFull );
					switch( StageSelectLoop( &z ) ){
					case enum_ESCRETURN_exit:     return enum_ESCRETURN_exit;
					case enum_ESCRETURN_restart:  return enum_ESCRETURN_restart;
					case enum_ESCRETURN_continue: break;
					}
					JumpTextScript( z );//�W�����v
					g_GameFlags &= ~(GAMEFLAG_ACTION|GAMEFLAG_KEYCONTROL);

				// ��NPC �R���g���[�� ************************
				}else if( gTS.data[gTS.p_read + 1] == 'D' &&//��NPC���C�x���g�Ō������ď���
					      gTS.data[gTS.p_read + 2] == 'N' &&
					      gTS.data[gTS.p_read + 3] == 'P' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					DeleteNpCharEvent( z );//�폜
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'D' &&//��NPC���R�[�h�Ō������ď���
					      gTS.data[gTS.p_read + 2] == 'N' &&
					      gTS.data[gTS.p_read + 3] == 'A' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					DeleteNpCharCode( z, TRUE );//�폜
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'B' &&//��BOSS
					      gTS.data[gTS.p_read + 2] == 'O' &&
					      gTS.data[gTS.p_read + 3] == 'A' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					SetBossCharActNo( z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'C' &&//���L�����ϐg
					      gTS.data[gTS.p_read + 2] == 'N' &&
					      gTS.data[gTS.p_read + 3] == 'P' ){
					x = GetTextScriptNo( gTS.p_read + 4 );//�����L�[�i�C�x���g�j
					y = GetTextScriptNo( gTS.p_read + 9 );//�L�����R�[�h
					z = GetTextScriptNo( gTS.p_read + 14 );//����
					ChangeNpCharByEvent( x, y, z );
					gTS.p_read += 18;
				}else if( gTS.data[gTS.p_read + 1] == 'A' &&//���L�����A�N�V����
					      gTS.data[gTS.p_read + 2] == 'N' &&
					      gTS.data[gTS.p_read + 3] == 'P' ){
					x = GetTextScriptNo( gTS.p_read + 4 );//�����L�[�i�C�x���g�j
					y = GetTextScriptNo( gTS.p_read + 9 );//�A�N�V����No
					z = GetTextScriptNo( gTS.p_read + 14 );//����
					SetNpCharActionNo( x, y, z );
					gTS.p_read += 18;
				}else if( gTS.data[gTS.p_read + 1] == 'I' &&//���L�����ϐg�������\�L����
					      gTS.data[gTS.p_read + 2] == 'N' &&
					      gTS.data[gTS.p_read + 3] == 'P' ){
					x = GetTextScriptNo( gTS.p_read + 4 );//�����L�[�i�C�x���g�j
					y = GetTextScriptNo( gTS.p_read + 9 );//�L�����R�[�h
					z = GetTextScriptNo( gTS.p_read + 14 );//����
					ChangeCheckableNpCharByEvent( x, y, z );
					gTS.p_read += 18;

				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//���L�����z�u
					      gTS.data[gTS.p_read + 2] == 'N' &&
					      gTS.data[gTS.p_read + 3] == 'P' ){
					w = GetTextScriptNo( gTS.p_read +  4 );// NPC��NO
					x = GetTextScriptNo( gTS.p_read +  9 );// X
					y = GetTextScriptNo( gTS.p_read + 14 );// Y
					z = GetTextScriptNo( gTS.p_read + 19 );// ����
					SetNpChar( w, x*VS*PARTSSIZE, y*VS*PARTSSIZE, 0, 0, z, NULL, MAX_NPC/2 );
					gTS.p_read += 23;

				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//���L�����ړ�
					      gTS.data[gTS.p_read + 2] == 'N' &&
					      gTS.data[gTS.p_read + 3] == 'P' ){
					w = GetTextScriptNo( gTS.p_read +  4 );// NPC�̃C�x���gNO
					x = GetTextScriptNo( gTS.p_read +  9 );// X
					y = GetTextScriptNo( gTS.p_read + 14 );// Y
					z = GetTextScriptNo( gTS.p_read + 19 );// ����
					MoveNpChar( w, x*VS*PARTSSIZE, y*VS*PARTSSIZE, z );
					gTS.p_read += 23;

				// ���}�b�v�R���g���[�� **************************
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//���}�b�v�p�[�c�������V�t�g
					      gTS.data[gTS.p_read + 2] == 'M' &&
					      gTS.data[gTS.p_read + 3] == 'P' ){
					x = GetTextScriptNo( gTS.p_read + 4 );
					y = GetTextScriptNo( gTS.p_read + 9 );
					ShiftMapParts( x, y );
					gTS.p_read += 13;
				}else if( gTS.data[gTS.p_read + 1] == 'C' &&//���}�b�v�p�[�c��ύX
					      gTS.data[gTS.p_read + 2] == 'M' &&
					      gTS.data[gTS.p_read + 3] == 'P' ){
					x = GetTextScriptNo( gTS.p_read +  4 );
					y = GetTextScriptNo( gTS.p_read +  9 );
					z = GetTextScriptNo( gTS.p_read + 14 );
					ChangeMapParts( x, y, (unsigned char)z );
					gTS.p_read += 18;


				}else if( gTS.data[gTS.p_read + 1] == 'B' &&//���{�X���C�t�\��
					      gTS.data[gTS.p_read + 2] == 'S' &&
					      gTS.data[gTS.p_read + 3] == 'L' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					if( z )StartBossLife( z );
					else   StartBossLife2();
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//���}�C�L�����̌�����ݒ�iDIR_DOWN�������������j
					      gTS.data[gTS.p_read + 2] == 'Y' &&
					      gTS.data[gTS.p_read + 3] == 'D' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					SetMyCharDirect( (unsigned char)z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//���}�C�L�����̃o�b�N�X�e�b�v�iNPC���C�x���g�R�[�h�����j
					      gTS.data[gTS.p_read + 2] == 'Y' &&
					      gTS.data[gTS.p_read + 3] == 'B' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					BackStepMyChar( z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'M' &&//���}�C�L�����̈ړ��ʃ[��
					      gTS.data[gTS.p_read + 2] == 'M' &&
					      gTS.data[gTS.p_read + 3] == '0' ){
					ZeroMyCharXMove();
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'I' &&//��������
					      gTS.data[gTS.p_read + 2] == 'N' &&
					      gTS.data[gTS.p_read + 3] == 'I' ){
					InitializeGame( ghWnd );
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//���Z�[�u
					      gTS.data[gTS.p_read + 2] == 'V' &&
					      gTS.data[gTS.p_read + 3] == 'P' ){
					SaveProfile( NULL );
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'L' &&//�����[�h
					      gTS.data[gTS.p_read + 2] == 'D' &&
					      gTS.data[gTS.p_read + 3] == 'P' ){
					if( !LoadProfile( NULL ) ) InitializeGame( ghWnd );
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//����`�F���W
					      gTS.data[gTS.p_read + 2] == 'A' &&
					      gTS.data[gTS.p_read + 3] == 'C' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					if( gTS.face != (char)z ){
						gTS.face = (char)z;
						gTS.face_x = (TEXTSCRIPT_LEFT-48)*VS;
					}
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'F' &&//����`�F���W
					      gTS.data[gTS.p_read + 2] == 'A' &&
					      gTS.data[gTS.p_read + 3] == 'C' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					if( gTS.face != (char)z ){
						gTS.face = (char)z;
						gTS.face_x = (TEXTSCRIPT_LEFT-48)*VS;
					}
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'G' &&//���A�C�e���\��
					      gTS.data[gTS.p_read + 2] == 'I' &&
					      gTS.data[gTS.p_read + 3] == 'T' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					gTS.item   = z;
					gTS.item_y = GETITEM_Y ;
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'N' &&//�����l�\��
					      gTS.data[gTS.p_read + 2] == 'U' &&
					      gTS.data[gTS.p_read + 3] == 'M' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					SetNumberTextScript( z );
					gTS.p_read += 8;
				// ���G���f�B���O =-------------------------------
				}else if( gTS.data[gTS.p_read + 1] == 'C' &&//���G���f�B���O
					      gTS.data[gTS.p_read + 2] == 'R' &&
					      gTS.data[gTS.p_read + 3] == 'E' ){
					g_GameFlags |= GAMEFLAG_ENDING;
					StartCreditScript();
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'S' &&//���C���X�g��\��
					      gTS.data[gTS.p_read + 2] == 'I' &&
					      gTS.data[gTS.p_read + 3] == 'L' ){
					z = GetTextScriptNo( gTS.p_read + 4 );
					SetCreditIllust( z );
					gTS.p_read += 8;
				}else if( gTS.data[gTS.p_read + 1] == 'C' &&//���C���X�g�����܂�
					      gTS.data[gTS.p_read + 2] == 'I' &&
					      gTS.data[gTS.p_read + 3] == 'L' ){
					CutCreditIllust(  );
					gTS.p_read += 4;
				}else if( gTS.data[gTS.p_read + 1] == 'X' &&//���X�e�[�W�I�����[�v
					      gTS.data[gTS.p_read + 2] == 'X' &&
					      gTS.data[gTS.p_read + 3] == '1' ){
					bExit = TRUE;
					z = GetTextScriptNo( gTS.p_read + 4 );
					switch( Scene_DownIsland( ghWnd, z ) ){
					case enum_ESCRETURN_exit:     return enum_ESCRETURN_exit;
					case enum_ESCRETURN_restart:  return enum_ESCRETURN_restart;
					case enum_ESCRETURN_continue: break;
					}
					gTS.p_read += 8;

				// �����̂ق�-------------------------------
				}else if( gTS.data[gTS.p_read + 1] == 'E' &&//�������I��
					      gTS.data[gTS.p_read + 2] == 'S' &&
					      gTS.data[gTS.p_read + 3] == 'C' ){
					return enum_ESCRETURN_restart;
				}else{
					char str[64];
					sprintf( str, "�s���̃R�[�h:<%c%c%c", 
						gTS.data[gTS.p_read + 1],
						gTS.data[gTS.p_read + 2],
						gTS.data[gTS.p_read + 3] );
					MessageBox( NULL, str, "�G���[", MB_OK );
					return enum_ESCRETURN_exit;
				}
			}else if( gTS.data[gTS.p_read] == 0x0D ){//���s�R�[�h
				gTS.p_read += 2;
				gTS.p_write = 0;
				if( gTS.flags & TSFLAGS_VIEW ){
					gTS.line++;
					CheckNewLine();
				}
			}else if( gTS.flags & TSFLAGS_TURBO ){//���� �� �\��-----------------------------
				//������̒����𒲂ׂ�B
				x = gTS.p_read;
				while( gTS.data[x] != '<' &&
					   gTS.data[x] != 0x0D 
				){
					if( gTS.data[x] & 0x80 )x++;
					x++;
				}
				//��������i�[
				y = x - gTS.p_read;
				memcpy( str, &gTS.data[gTS.p_read], y );
				str[y] = '\0';
				//�\��
				gTS.p_write = x;
				PutText2( 0, 0, str, COLOR_TEXT, SURF_TEXTSCRIPT + gTS.line%NUM_TEXTLINE );
				sprintf( _text[gTS.line%NUM_TEXTLINE], str );
				gTS.p_read += y;
				//�������s
				if( gTS.p_write >= NUM_CHAR - 1){
					CheckNewLine();
				}
				bExit = TRUE;

			}else {//�����\��-----------------------------
				c[0] = gTS.data[gTS.p_read];
				if( c[0] & 0x80 ){//�Q�o�C�g����
					c[1] = gTS.data[gTS.p_read + 1];
					c[2] = '\0';
				}
				else{//�P�o�C�g����
					c[1] = '\0';
				}
				//�\��
				if( c[0] == '=' ){
					Surface2Surface( gTS.p_write * TEXTFONT_WIDTH, 2, 
						&rcSymbol, SURF_TEXTSCRIPT + gTS.line%NUM_TEXTLINE, SURF_TEXTBOX );
				}else{
					PutText2( gTS.p_write * TEXTFONT_WIDTH, 0, c,
						COLOR_TEXT, SURF_TEXTSCRIPT + gTS.line%NUM_TEXTLINE ); 
				}
				strcat( _text[gTS.line%NUM_TEXTLINE], c );
				PlaySoundObject( WAVE_MESSAGE, 1 );
				gTS.wait_beam = 0;//�`�撆�͓_�Ŗ����B
				//����
				if( c[0] & 0x80 ){//�Q�o�C�g����
					gTS.p_read += 2;
					gTS.p_write += 2;
				}else{//�P�o�C�g����
					gTS.p_read += 1;
					gTS.p_write += 1;
				}
				//�������s
				if( gTS.p_write >= NUM_CHAR - 1){
					CheckNewLine();
					gTS.p_write = 0;
					gTS.line++;
					CheckNewLine();
				}
				bExit = TRUE;
			}
		}
		break;
	case MODE_TEXTSCRIPT_NOD:
		if( gKeyTrg & (gKeyOk|gKeyCancel) )
			gTS.mode = MODE_TEXTSCRIPT_READ;
		break;
	case MODE_TEXTSCRIPT_SCROLL:
		for( i = 0; i < NUM_TEXTLINE; i++ ){
			gTS.ypos_line[i] -= PARAM_TEXTSCROLL;
			if( gTS.ypos_line[i] == 0 )
				gTS.mode = MODE_TEXTSCRIPT_READ;
			if( gTS.ypos_line[i] == -TEXTLINE_HEIGHT )
				gTS.ypos_line[i] = TEXTLINE_HEIGHT * (NUM_TEXTLINE -1);
		}
		break;
	case MODE_TEXTSCRIPT_WAIT:
		//�E�G�C�g
		if( gTS.wait_next == 9999 ) break;
		if( gTS.wait != 9999 )gTS.wait++;
		if( gTS.wait < gTS.wait_next )break;
		gTS.mode = MODE_TEXTSCRIPT_READ;
		gTS.wait_beam = 0;
		break;
	case MODE_TEXTSCRIPT_FADE:
		//�t�F�[�h�҂�
		if( GetFadeActive() )break;
		gTS.mode = MODE_TEXTSCRIPT_READ;
		gTS.wait_beam = 0;
		break;
	case MODE_TEXTSCRIPT_MYCHARSTEP:
		//�t�F�[�h�҂�
		if( !(gMC.flag & FLAG_HIT_BOTTOM) )break;
		gTS.mode = MODE_TEXTSCRIPT_READ;
		gTS.wait_beam = 0;
		break;
	// �͂�/������
	case MODE_TEXTSCRIPT_YESNO:
		if( gTS.wait < 16 ){
			gTS.wait++;
			break;
		}

		if( gKeyTrg & gKeyOk ){
			PlaySoundObject( WAVE_OK, 1 );
			if( gTS.select == 1){
				JumpTextScript( gTS.next_event );//�W�����v
			}else{
				gTS.mode = MODE_TEXTSCRIPT_READ;
				gTS.wait_beam = 0;
			}
			break;
		}
		if( gKeyTrg & gKeyLeft ){
			gTS.select = 0;
			PlaySoundObject( WAVE_CURSOR, 1 );
		}else  if( gKeyTrg & gKeyRight ){
			gTS.select = 1;
			PlaySoundObject( WAVE_CURSOR, 1 );
		}
		break;
	default:
		break;
	}

	if( gTS.mode == MODE_TEXTSCRIPT_SLEEP ) g_GameFlags &= ~GAMEFLAG_TEXTSCRIPT;
	else                                    g_GameFlags |=  GAMEFLAG_TEXTSCRIPT;

	return enum_ESCRETURN_continue;
}


void RestoreTextScript( void )
{
	long i;
	for( i = 0; i < NUM_TEXTLINE; i++ ){
		CortBox2( &gRect_line, COLOR_TEXTBACK, SURF_TEXTSCRIPT + i );
		PutText2( 0, 0, _text[i], COLOR_TEXT,  SURF_TEXTSCRIPT + i );
	}
}
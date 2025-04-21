#include <windows.h>
#include <stdio.h>

#include "DSOUND.H"
//#pragma comment(lib, "dsound")


#include "Define.h"

BOOL ErrorLog( char *str, long value );

#define MAXNOTELENGTH 255//�����̍ő咷
#define MAXMELODY	    8
#define MAXDRAM		    8
#define MAXTRACK	   16

#define KEYDUMMY	  255//�L�[�̕ύX�͉����Ȃ�����
#define PANDUMMY	  255//�p���̕ύX�͉����Ȃ�����
#define VOLDUMMY	  255//�{�����[���̕ύX�͉����Ȃ�����

#define ALLOCNOTE	  4096



////////////////////////////////////////////////////
// DirectSound for Organya /////////////////////////
////////////////////////////////////////////////////

extern LPDIRECTSOUND       glpDS;            // DirectSound�I�u�W�F�N�g
static LPDIRECTSOUNDBUFFER lpORGANBUFFER[8][8][2] = {NULL};
extern LPDIRECTSOUNDBUFFER lpSECONDARYBUFFER[];   // �񎞃o�b�t�@

typedef struct{
	short wave_size;//�g�`�̃T�C�Y
	short oct_par;//�I�N�^�[�u����������|����(/8)
	short oct_size;//�I�N�^�[�u����������|����(/8)
}OCTWAVE;

OCTWAVE oct_wave[8] = {
	{256,  1,  4},//0 Oct
	{256,  2,  8},//1 Oct
	{128,  4, 12},//2 Oct
	{128,  8, 16},//3 Oct
	{ 64, 16, 20},//4 Oct
	{ 32, 32, 24},
	{ 16, 64, 28},
	{  8,128, 32},
};
BYTE format_tbl2[] = {//22050Hz��Format
	0x01,0x00,0x01,0x00,
	0x22,0x56,0x00,0x00,
	0x22,0x56,0x00,0x00,
	0x01,0x00,0x08,0x00,0x00,0x00};
BOOL MakeSoundObject8(char *wavep,char track, char pipi )
{
	if( !glpDS )return FALSE;

	DWORD i, j, k;
	unsigned long wav_tp;//WAV�e�[�u���������|�C���^
	DWORD wave_size;//256;
	DWORD data_size;
	BYTE  *wp;
	BYTE  *wp_sub;
	int work;
	//�Z�J���_���o�b�t�@�̐���
	DSBUFFERDESC dsbd;

	for( j = 0; j < 8; j++ ){
		for( k = 0; k < 2; k++ ){
			wave_size = oct_wave[j].wave_size;
			if( pipi ) data_size = wave_size * oct_wave[j].oct_size;
			else       data_size = wave_size;
			ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
			dsbd.dwSize        = sizeof(DSBUFFERDESC);
			dsbd.dwBufferBytes = data_size;//file_size;
			dsbd.lpwfxFormat   = (LPWAVEFORMATEX)(&format_tbl2[0]);
			dsbd.dwFlags       =
				DSBCAPS_STATIC|
				DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN|DSBCAPS_CTRLFREQUENCY|
				DSBCAPS_GLOBALFOCUS;
			if( glpDS->CreateSoundBuffer(&dsbd, &lpORGANBUFFER[track][j][k], NULL) != DS_OK ) return FALSE;
			wp     = (BYTE*)malloc(data_size);//�t�@�C���̃��[�N�X�y�[�X�����
			wp_sub = wp;
			wav_tp = 0;
			for( i = 0; i < data_size; i++ ){
				work    = *(wavep+wav_tp);
				work   +=0x80;
				*wp_sub =(BYTE)work;
				wav_tp += 256/wave_size;
				if( wav_tp > 255 ) wav_tp -= 256;
				wp_sub++;
			}
			//�f�[�^�̓]��
			LPVOID lpbuf1, lpbuf2;
			DWORD  dwbuf1, dwbuf2=0;
			HRESULT hr;
			hr = lpORGANBUFFER[track][j][k]->Lock( 0, data_size, &lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0 );		
			if(hr != DS_OK)return (FALSE);	
			CopyMemory( lpbuf1, (BYTE*)wp,dwbuf1 );//+3a�̓f�[�^�̓�
			if(dwbuf2 != 0)	CopyMemory(lpbuf2, (BYTE*)wp+dwbuf1, dwbuf2);
			lpORGANBUFFER[track][j][k]->Unlock(lpbuf1, dwbuf1, lpbuf2, dwbuf2); 
			lpORGANBUFFER[track][j][k]->SetCurrentPosition(0);
			free( wp );
		}
	}
	return(TRUE);
}
//short freq_tbl[12] = {261,278,294,311,329,349,371,391,414,440,466,494};
short freq_tbl[12] = {262,277,294,311,330,349,370,392,415,440,466,494};
void ChangeOrganFrequency(unsigned char key,char track,DWORD a)
{
	if( !glpDS )return;
	for(int j = 0; j < 8; j++)
		for(int i = 0; i < 2; i++)
			lpORGANBUFFER[track][j][i]->SetFrequency(//1000��+���̃f�t�H���g�l�Ƃ���
				((oct_wave[j].wave_size*freq_tbl[key])*oct_wave[j].oct_par)/8 + (a-1000));
}
long g_mute[MAXTRACK] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
static short         _pan_tbl[13] = {0,43,86,129,172,215,256,297,340,383,426,469,512}; 
static unsigned char _old_key[MAXTRACK]  = {255,255,255,255,255,255,255,255};//�Đ����̉�
static unsigned char _key_on[MAXTRACK]   = {  0,  0,  0,  0,  0,  0,  0,  0};//�L�[�X�C�b�`
static unsigned char _key_twin[MAXTRACK] = {  0,  0,  0,  0,  0,  0,  0,  0};//���g���Ă���L�[(�A�����̃m�C�Y�h�~�ׂ̈ɓ�p��)
void ChangeOrganPan(unsigned char key, unsigned char pan,char track)//512��MAX��256��ɰ��
{
	if( !glpDS )return;
	if(_old_key[track] != 255)
		lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->SetPan((_pan_tbl[pan]-256)*10);
}
void ChangeOrganVolume(int no, long volume,char track)//300��MAX��300��ɰ��
{
	if( !glpDS )return;
	if(_old_key[track] != 255)
		lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->SetVolume((volume-255)*8);
}
// �T�E���h�̍Đ� 
void PlayOrganObject(unsigned char key, int mode,char track,DWORD freq)
{
	if( !glpDS )return;
	
    if(lpORGANBUFFER[track][key/12][_key_twin[track]] != NULL){
		switch(mode){
		case 0: // ��~
			if(_old_key[track] != 255){
				lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->Stop();
				lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->SetCurrentPosition(0);
			}
			break;
		case 1: // �Đ�
			break;
		case 2: // ��������~
			if(_old_key[track] != 255){
				lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->Play(0, 0, 0);
				_old_key[track] = 255;
			}
            break;
		case -1:
			if(_old_key[track] == 255){//�V�K�炷
				ChangeOrganFrequency(key%12,track,freq);//���g����ݒ肵��
				lpORGANBUFFER[track][key/12][_key_twin[track]]->Play(0, 0, DSBPLAY_LOOPING);
				_old_key[track] = key;
				_key_on[track]  = 1;
			}else if(_key_on[track] == 1 && _old_key[track] == key){//������
				//���Ȃ��Ă���̂��������~
				lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->Play(0, 0, 0);
				_key_twin[track]++;
				if(_key_twin[track] > 1 )_key_twin[track] = 0; 
				lpORGANBUFFER[track][key/12][_key_twin[track]]->Play(0, 0, DSBPLAY_LOOPING);
			}else{//�Ⴄ����炷�Ȃ�
				lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->Play(0, 0, 0);//���Ȃ��Ă���̂��������~
				_key_twin[track]++;
				if(_key_twin[track] > 1 )_key_twin[track] = 0; 
				ChangeOrganFrequency(key%12,track,freq);//���g����ݒ肵��
				lpORGANBUFFER[track][key/12][_key_twin[track]]->Play(0, 0, DSBPLAY_LOOPING);
				_old_key[track] = key;
			}
			break;
		}
    }
}

//�I���K�[�j���I�u�W�F�N�g���J��
void ReleaseOrganyaObject(char track){
	if( !glpDS )return;
	for(int i = 0; i < 8; i++){
		if(lpORGANBUFFER[track][i][0] != NULL){
			lpORGANBUFFER[track][i][0]->Release();
			lpORGANBUFFER[track][i][0] = NULL;
		}
		if(lpORGANBUFFER[track][i][1] != NULL){
			lpORGANBUFFER[track][i][1]->Release();
			lpORGANBUFFER[track][i][1] = NULL;
		}
	}
}
//�g�`�f�[�^�����[�h
char wave_data[100*256];
BOOL InitWaveData100(void)
{
	if( !glpDS )return FALSE;
    HRSRC hrscr;
    DWORD *lpdword;//���\�[�X�̃A�h���X
    // ���\�[�X�̌���
    if((hrscr = FindResource(NULL, "WAVE100", "WAVE")) == NULL)
                                                    return(FALSE);
    // ���\�[�X�̃A�h���X���擾
    lpdword = (DWORD*)LockResource(LoadResource(NULL, hrscr));
	memcpy(wave_data,lpdword,100*256);
	return TRUE;
}
/*
BOOL LoadWaveData100(char *path_wave)
{
	char path[MAX_PATH];
	sprintf( path, "%s\\%s", gModulePath, path_wave );
	if( !glpDS )return FALSE;
	FILE *fp;
	if( ( fp = fopen(path,"rb") ) == NULL ){
		return FALSE;
	}
//	wave_data = new char[100*256];
	fread(wave_data, sizeof(char), 256*100, fp);
	fclose(fp);
	return TRUE;
}
*/
//�g�`���P�O�O�̒�����I�����č쐬
BOOL MakeOrganyaWave(char track,char wave_no, char pipi)
{
	if( !glpDS )return FALSE;
	if(wave_no > 99)return FALSE;
	ReleaseOrganyaObject(track);
	MakeSoundObject8(&wave_data[0] + wave_no*256,track, pipi);	
	return TRUE;
}
/////////////////////////////////////////////
//���I���K�[�j���h�����X����������������///////
/////////////////////

//�I���K�[�j���I�u�W�F�N�g���J��
/*void ReleaseDramObject(char track){
	if( !glpDS )return;
	for(int i = 0; i < 8; i++){
		if(lpDRAMBUFFER[track] != NULL){
			lpDRAMBUFFER[track]->Release();
			lpDRAMBUFFER[track] = NULL;
		}
	}
}

// �T�E���h�̐ݒ� 
BOOL InitDramObject( LPCSTR resname, int no )
{
 	if( !glpDS ) return FALSE;
	HRSRC hrscr;
    DSBUFFERDESC dsbd;
    DWORD *lpdword;//���\�[�X�̃A�h���X
    // ���\�[�X�̌���
    if((hrscr = FindResource(NULL, resname, "WAVE")) == NULL)
                                                    return(FALSE);
    // ���\�[�X�̃A�h���X���擾
    lpdword = (DWORD*)LockResource(LoadResource(NULL, hrscr));

	// �񎟃o�b�t�@�̐���
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize        = sizeof(DSBUFFERDESC);
	dsbd.dwBufferBytes = *(DWORD*)((BYTE*)lpdword+0x36);
	dsbd.lpwfxFormat   = (LPWAVEFORMATEX)(lpdword+5); 
	dsbd.dwFlags       = 
		DSBCAPS_STATIC|
		DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN|DSBCAPS_CTRLFREQUENCY|
		DSBCAPS_GLOBALFOCUS;
	if(glpDS->CreateSoundBuffer(&dsbd, &lpDRAMBUFFER[no],
								NULL) != DS_OK) return(FALSE);
    LPVOID lpbuf1, lpbuf2;
    DWORD  dwbuf1, dwbuf2;
    // �񎟃o�b�t�@�̃��b�N
    lpDRAMBUFFER[no]->Lock( 0, *(DWORD*)((BYTE*)lpdword+0x36), &lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0 ); 
	// �����f�[�^�̐ݒ�
	CopyMemory( lpbuf1, (BYTE*)lpdword+0x3a, dwbuf1 );
    if( dwbuf2 != 0 ) CopyMemory( lpbuf2, (BYTE*)lpdword+0x3a+dwbuf1, dwbuf2 );
	// �񎟃o�b�t�@�̃��b�N����
	lpDRAMBUFFER[no]->Unlock(lpbuf1, dwbuf1, lpbuf2, dwbuf2); 
	lpDRAMBUFFER[no]->SetCurrentPosition(0);

    return(TRUE);
}

BOOL LoadDramObject(char *name, int no)
{
	if( !glpDS )return FALSE;

	DWORD file_size = 0;
	char path[MAX_PATH];
	sprintf( path, "%s\\%s", gModulePath, name );

	//�T�C�Y�̎擾
	HANDLE hFile;
	hFile = CreateFile( path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	file_size = GetFileSize( hFile, NULL );
	CloseHandle( hFile );
	
	
	
	DWORD i;
	char check_box[58];
	FILE *fp;
	if((fp=fopen(path,"rb"))==NULL){
		return(FALSE);
	}
	for(i = 0; i < 58; i++){
		fread(&check_box[i],sizeof(char),1,fp);
	}
	if(check_box[0] != 'R')return(FALSE);
	if(check_box[1] != 'I')return(FALSE);
	if(check_box[2] != 'F')return(FALSE);
	if(check_box[3] != 'F')return(FALSE);
	
	DWORD *wp;
	wp = (DWORD*)malloc(file_size);//�t�@�C���̃��[�N�X�y�[�X�����
	fseek(fp,0,SEEK_SET);
	for(i = 0; i < file_size; i++){
		fread((BYTE*)wp+i,sizeof(BYTE),1,fp);
	}
	fclose(fp);
	//�Z�J���_���o�b�t�@�̐���
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags =
		DSBCAPS_STATIC|
		DSBCAPS_CTRLDEFAULT|
		DSBCAPS_GLOBALFOCUS;
	dsbd.dwBufferBytes = *(DWORD*)((BYTE*)wp+0x36);//WAVE�f�[�^�̃T�C�Y
	dsbd.lpwfxFormat = (LPWAVEFORMATEX)(wp+5); 
	if(glpDS->CreateSoundBuffer(&dsbd, &lpDRAMBUFFER[no],
								NULL) != DS_OK) return(FALSE);	
    LPVOID lpbuf1, lpbuf2;
    DWORD dwbuf1, dwbuf2;
	HRESULT hr;
	hr = lpDRAMBUFFER[no]->Lock(0, *(DWORD*)((BYTE*)wp+0x36),
							&lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0); 
	if(hr != DS_OK)return (FALSE);
	CopyMemory(lpbuf1, (BYTE*)wp+0x3a, dwbuf1);//+3a�̓f�[�^�̓�
	if(dwbuf2 != 0)	CopyMemory(lpbuf2, (BYTE*)wp+0x3a+dwbuf1, dwbuf2);
	lpDRAMBUFFER[no]->Unlock(lpbuf1, dwbuf1, lpbuf2, dwbuf2); 
	lpDRAMBUFFER[no]->SetCurrentPosition(0);

	free(wp);
	return(TRUE);
}
*/

void ChangeDramFrequency(unsigned char key,char track)
{
	if( !glpDS )return;
	lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->SetFrequency(key*800+100);
}
void ChangeDramPan(unsigned char pan,char track)//512��MAX��256��ɰ��
{
	if( !glpDS )return;
	lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->SetPan((_pan_tbl[pan]-256)*10);
}
void ChangeDramVolume(long volume,char track)//
{
	if( !glpDS )return;
	lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->SetVolume((volume-255)*8);
}

// �T�E���h�̍Đ� 
void PlayDramObject(unsigned char key, int mode,char track)
{
	if( !glpDS )return;
	
    if(lpSECONDARYBUFFER[track+WAVE_ORG_DRAM] != NULL){
		switch(mode){
		case 0: // ��~
			lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->Stop();
			lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->SetCurrentPosition(0);
			break;
		case 1: // �Đ�
			lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->Stop();
			lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->SetCurrentPosition(0);
			ChangeDramFrequency(key,track);//���g����ݒ肵��
			lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->Play(0, 0, 0);
			break;
		case 2: // ��������~
            break;
		case -1:
			break;
		}
    }
}
/*
void PlayOrganKey( unsigned char key,char track, DWORD freq )
{
	if( !glpDS )return;
	if(track < MAXMELODY){
		DWORD wait = GetTickCount();
		ChangeOrganFrequency(key%12,track,freq);//���g����ݒ肵��
		lpORGANBUFFER[track][key/12][0]->SetVolume((160-255)*8);
		lpORGANBUFFER[track][key/12][0]->Play(0, 0, DSBPLAY_LOOPING);
		do{
		}while(GetTickCount() < wait+80);
		lpORGANBUFFER[track][key/12][0]->Play(0, 0, 0);
	}else{
		PlaySoundObject( 
//		lpDRAMBUFFER[track - MAXMELODY]->Stop();
//		lpDRAMBUFFER[track - MAXMELODY]->SetCurrentPosition(0);
//		ChangeDramFrequency(key,track - MAXMELODY);//���g����ݒ肵��
//		lpDRAMBUFFER[track - MAXMELODY]->SetVolume((160-255)*8);
//		lpDRAMBUFFER[track - MAXMELODY]->Play(0, 0, 0);
	}
}

*/
/*
static void _ConvertHzbit( DWORD sps, WORD bps )
{
	if( !glpDS ) return;
	long track, j, k;

	// �v���C�}���o�b�t�@�̃t�H�[�}�b�g���w�肷��B
	WAVEFORMATEX fmt;
	memset( &fmt, 0, sizeof(WAVEFORMATEX) ); 
	fmt.wFormatTag      = WAVE_FORMAT_PCM; // �`��(PCM)
	fmt.nChannels       = 2; // �`�����l��(2ch�X�e���I)
	fmt.nSamplesPerSec  = sps; // �T���v�����O���[�g(44100Hz)
	fmt.wBitsPerSample  = bps; // �ʎq���r�b�g(16bit)

	// 1�T���v�����O������̃o�C�g��(16bit�~2ch)
	fmt.nBlockAlign     = fmt.wBitsPerSample / 8 * fmt.nChannels; 

	// 1�b������̃o�C�g��(16bit�~2ch�~44100Hz)
	fmt.nAvgBytesPerSec = fmt.wBitsPerSample / 8 * fmt.nChannels * fmt.nSamplesPerSec; 

	for( track = 0; track < 8; track++ ){
		if( lpSECONDARYBUFFER[ track + WAVE_ORG_DRAM ] ){
			if( 0 > lpSECONDARYBUFFER[ track + WAVE_ORG_DRAM ]->SetFormat( &fmt ) ){}
		}
	}
	for( track = 0; track < 8; track++ ){
		for( j = 0; j < 8; j++ ){
			for( k = 0; k < 2; k++ ){
				lpORGANBUFFER[ track ][ j ][ k ]->SetFormat( &fmt );
			}
		}
	}

}
*/
////////////////////////////////////////////////////
// Main of Organya /////////////////////////////////
////////////////////////////////////////////////////


//�������X�g�\����
typedef struct NOTELIST{
	NOTELIST *from;//�O�̃A�h���X
	NOTELIST *to;//���̃A�h���X
	long x;//�ʒu
	unsigned char length;//���̒���
	unsigned char y;//���̍���
	unsigned char volume;//���H�����[��
	unsigned char pan;//�p��
}NOTELIST;
//�g���b�N�f�[�^���W
typedef struct{
	unsigned short freq;//+�����g��(1000��Default)
//	unsigned short note_num;//�����̐�
	unsigned char wave_no;//�g�`No
	char pipi;
	NOTELIST *note_p;//NoteData�̈�̐擪�A�h���X
	NOTELIST *note_list;//���X�g�̐擪�A�h���X1
}TRACKDATA;
//�ȒP�ʂŎ��ŗL�̏��
typedef struct MUSICINFO{
	unsigned short wait;
	unsigned char line;//�P���߂̃��C����
	unsigned char dot;//�ꔑ�̃h�b�g��
	unsigned short alloc_note;//�m�ۂ��������̐�
	long repeat_x;//���s�[�g
	long end_x;//�Ȃ̏I���(���s�[�g�ɖ߂�)
	TRACKDATA tdata[16];
}MUSICINFO;
//���C���N���X�B���̃A�v���P�[�V�����̒��S�B�i�N���X���Ă�����߂Ďg���j
typedef struct OrgData{
	OrgData();//�R���X�g���N�^
//	private:
	public:
		MUSICINFO info;
		char track;
		char mute[MAXTRACK];
		unsigned char def_pan;
		unsigned char def_volume;
		void InitOrgData(void);
		void GetMusicInfo(MUSICINFO *mi);//�ȏ����擾
		//�ȏ���ݒ�Bflag�͐ݒ�A�C�e�����w��
		BOOL SetMusicInfo(MUSICINFO *mi,unsigned long flag);
		BOOL NoteAlloc(unsigned short note_num);//�w��̐�����NoteData�̗̈���m��
		void ReleaseNote(void);//NoteData���J��
		NOTELIST *SearchNote(NOTELIST *np);//���g�pNOTE�̌���
		void OrgData::TouchKeyboard(unsigned char y);//���ՂɐG���
		void GetNoteUsed(long *use,long *left,char track);//�����c�ʓ�
		//�ȉ��͍Đ�
		void PlayData(void);
		void SetPlayPointer(long x);//�Đ��|�C���^�[���w��̈ʒu�ɐݒ�
		//�ȉ��̓t�@�C���֌W
		BOOL OrgData::LoadMusicData( char *path );
		BOOL OrgData::InitMusicData( char *path );
		//�ȉ��͕ҏW�֌W
}ORGDATA;
ORGDATA org_data;

//�R���X�g���N�^
OrgData::OrgData()
{
//	memset( &org_data, 0, sizeof(ORGDATA) );
	int i;
	for(i = 0; i < MAXTRACK; i++){
		info.tdata[i].note_list = NULL;
		info.tdata[i].note_p    = NULL;
	}
}
//�ȏ����Z�b�g���鎞�̃t���O
#define SETALL		0xffffffff//�S�Ă��Z�b�g
#define SETWAIT		0x00000001
#define SETGRID		0x00000002
#define SETALLOC	0x00000004
#define SETREPEAT	0x00000008
#define SETFREQ		0x00000010
#define SETWAVE		0x00000020
#define SETPIPI		0x00000040
#define DEFVOLUME	200//255��VOLDUMMY�BMAX��254
#define DEFPAN		6
void OrgData::InitOrgData(void)
{
//	MUSICINFO mi;
	track = 0;
	info.alloc_note = ALLOCNOTE;//�Ƃ肠����4096
	info.dot        = 4;
	info.line       = 4;
	info.wait       = 128;
	info.repeat_x   = info.dot * info.line * 0;
	info.end_x      = info.dot * info.line * 255;
	int i;
	for(i = 0; i < MAXTRACK; i++){
		info.tdata[i].freq = 1000;
		info.tdata[i].wave_no = 0;
		info.tdata[i].pipi = 0;
	}
	NoteAlloc( info.alloc_note );
	SetMusicInfo(&info,SETALL);
	def_pan = DEFPAN;
	def_volume = DEFVOLUME;
}
//�ȏ���ݒ�Bflag�̓A�C�e�����w��
BOOL OrgData::SetMusicInfo(MUSICINFO *mi,unsigned long flag)
{
	char str[32];
	int i;
	if(flag & SETGRID){//�O���b�h��L����
		info.dot = mi->dot;
		info.line = mi->line;

	}
//	if(flag & SETALLOC){//�̈�m��
//		info.alloc_note = mi->alloc_note;
//		ReleaseNote();
//		NoteAlloc(info.alloc_note);
//	}
	if(flag & SETWAIT){
		info.wait = mi->wait;
		itoa(mi->wait,str,10);
	}
	if(flag & SETREPEAT){
		info.repeat_x = mi->repeat_x;
		info.end_x = mi->end_x;
	}
	if(flag & SETFREQ){
		for(i = 0; i < MAXMELODY; i++){
			info.tdata[i].freq = mi->tdata[i].freq;
			info.tdata[i].pipi = info.tdata[i].pipi;
		}
	}
	if(flag & SETWAVE){
		for(i = 0; i < MAXTRACK; i++)
			info.tdata[i].wave_no = mi->tdata[i].wave_no;
	}
	if(flag & SETPIPI){
		for(i = 0; i < MAXTRACK; i++)
			info.tdata[i].pipi = mi->tdata[i].pipi;
	}

	return TRUE;
}

//�w��̐�����NoteData�̗̈���m��(������)
BOOL OrgData::NoteAlloc(unsigned short alloc)
{
	int i,j;
	for(j = 0; j < MAXTRACK; j++){
		info.tdata[j].wave_no = 0;
		info.tdata[j].note_list = NULL;//�R���X�g���N�^�ɂ�点����
		info.tdata[j].note_p    = (NOTELIST *)malloc( sizeof(NOTELIST) * alloc );
		if( !info.tdata[j].note_p ){
			for(i = 0; i < MAXTRACK; i++){
				if( info.tdata[i].note_p ){
					free( info.tdata[i].note_p );
					info.tdata[j].note_p = NULL;
				}
			}
			return FALSE;
		}
		//	info.alloc_note = alloc;
		for(i = 0; i < alloc; i++){
			(info.tdata[j].note_p + i)->from   = NULL;
			(info.tdata[j].note_p + i)->to     = NULL;
			(info.tdata[j].note_p + i)->length = 0;
			(info.tdata[j].note_p + i)->pan    = PANDUMMY;
			(info.tdata[j].note_p + i)->volume = VOLDUMMY;
			(info.tdata[j].note_p + i)->y      = KEYDUMMY;
		}
	}
	for( j = 0; j < MAXMELODY; j++ ) MakeOrganyaWave( j, info.tdata[j].wave_no, info.tdata[j].pipi );
//	for( j = 0; j < MAXDRAM;   j++ ) InitDramObject( "Org\\BASS01", j );

	track = 0;//////////���͂����ɏ����Ă���
	return TRUE;
}
//NoteData���J��
void OrgData::ReleaseNote(void)
{
	for(int i = 0; i < MAXTRACK; i++){
		if( info.tdata[i].note_p ){
			free( info.tdata[i].note_p );
			info.tdata[i].note_p = NULL;
		}
	}
}

char pass[ 7] = "Org-01";
char pass2[7] = "Org-02";//Pipi

////�ȉ��̓I���K�[�j���ȃf�[�^�\���́i�t�B���Ɋ܂܂�鍀�ځj
typedef struct{
	long x;//�ʒu
	unsigned char y;//���̍���
	unsigned char length;//���̒���
	unsigned char volume;//�{�����[��
	unsigned char pan;//�p��
}ORGANYANOTE;

typedef struct{
	unsigned short freq;//+�����g��(1000��Default)
	unsigned char wave_no;//�g�`No
	unsigned char pipi;//��
	unsigned short note_num;//�����̐�
}ORGANYATRACK;

typedef struct{
	unsigned short wait;
	unsigned char line;
	unsigned char dot;
	long repeat_x;//���s�[�g
	long end_x;//�Ȃ̏I���(���s�[�g�ɖ߂�)
	ORGANYATRACK tdata[MAXTRACK];
}ORGANYADATA;
/*
//�ȃf�[�^��ǂݍ���
char *dram_name[] = {
	"BASS01",
	"Bass02",
	"SNARE01",
	"Snare02",
	"TOM01",
	"HICLOSE",
	"HIOPEN",
	"Crash",
	"Per01",
	"Per02",
	"Bass03",
	"Tom02"
};
*/
BOOL OrgData::InitMusicData( char *path )
{
	ORGANYADATA org_data;
	NOTELIST *np;
	int i,j;
	char pass_check[6];
	char ver = 0;

    HRSRC hrscr;
    char * p;

	// ���\�[�X�̌���
    if((hrscr = FindResource(NULL, path, "ORG")) == NULL)
		return(FALSE);
    // ���\�[�X����̓ǂݍ���
    p = (char*)LockResource(LoadResource(NULL, hrscr));

	//�p�X���[�h�`�F�b�N
	memcpy( &pass_check[0], p, sizeof(char)* 6 );
	p += 6;
	if( !memcmp( pass_check, pass,  6 ) )ver = 1;
	if( !memcmp( pass_check, pass2, 6 ) )ver = 2;
	if( !ver ){
		return FALSE;
	}

	//�ȏ��̓ǂݍ���
	memcpy( &org_data, p, sizeof(ORGANYADATA)* 1 );
	p += sizeof(ORGANYADATA);

	//�Ȃ̏���ݒ�
	info.wait     = org_data.wait;// * 3/2;
	info.line     = org_data.line;
	info.dot      = org_data.dot;
	info.repeat_x = org_data.repeat_x;
	info.end_x    = org_data.end_x;
	for(i = 0; i < MAXTRACK; i++){
		info.tdata[i].freq = org_data.tdata[i].freq;
		if( ver == 1 ) info.tdata[i].pipi = 0;
		else           info.tdata[i].pipi = org_data.tdata[i].pipi;
		info.tdata[i].wave_no = org_data.tdata[i].wave_no;
	}

	//�����̃��[�h
	for(j = 0; j < MAXTRACK; j++){
		//�ŏ��̉�����from��NULL�ƂȂ�
		if(org_data.tdata[j].note_num == 0){
			info.tdata[j].note_list = NULL;
			continue;
		}
		//���X�g�����
		np = info.tdata[j].note_p;
		info.tdata[j].note_list = info.tdata[j].note_p;
		np->from = NULL;
		np->to = (np + 1);
		np++;
		for(i = 1; i < org_data.tdata[j].note_num; i++){
			np->from = (np - 1);
			np->to = (np + 1);
			np++;
		}
		//�Ō�̉�����to��NULL
		np--;
		np->to = NULL;

		//���e����
		np = info.tdata[j].note_p;//�w���W
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			memcpy( &np->x, p, sizeof(long)*1 );
			p += sizeof(long);
			
			np++;
		}
		np = info.tdata[j].note_p;//�x���W
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			memcpy( &np->y, p, sizeof(unsigned char)*1 );
			p += sizeof(unsigned char);
			np++;
		}
		np = info.tdata[j].note_p;//����
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			memcpy( &np->length, p, sizeof(unsigned char)* 1 );
			p += sizeof(unsigned char);
			np++;
		}
		np = info.tdata[j].note_p;//�{�����[��
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			memcpy( &np->volume, p, sizeof(unsigned char)* 1 );
			p += sizeof(unsigned char);
			np++;
		}
		np = info.tdata[j].note_p;//�p��
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			memcpy( &np->pan, p, sizeof(unsigned char)* 1 );
			p += sizeof(unsigned char);
			np++;
		}
	}
	//�f�[�^��L����
	for(j = 0; j < MAXMELODY; j++)
		MakeOrganyaWave(j,info.tdata[j].wave_no, info.tdata[j].pipi);
	for(j = MAXMELODY; j < MAXTRACK; j++){
		i = info.tdata[j].wave_no;
//		InitDramObject(dram_name[i],j-MAXMELODY);
	}

//	_ConvertHzbit(8000,8);
	SetPlayPointer(0);//���o��

	return TRUE;
}

/*
BOOL OrgData::LoadMusicData( char *name )
{
	ORGANYADATA org_data;
	NOTELIST *np;
	int i,j;
	char pass_check[7];
	char ver = 0;

	char music_path[MAX_PATH];
	sprintf( music_path, "%s\\%s", gModulePath, name );

	//������������������[�h
	FILE *fp;
	if( ( fp = fopen( music_path, "rb" ) ) == NULL ){
		MessageBox(0,"�t�@�C���ɃA�N�Z�X�ł��܂���","Error (Load)",MB_OK);
		return(FALSE);
	}
	//�p�X���[�h�`�F�b�N
	memset( pass_check, 0, 7 );
	fread(&pass_check[0], sizeof(char), 6, fp);
	if( !memcmp( pass_check, pass, 6 ) )ver = 1;
	if( !memcmp( pass_check, pass2, 6 ) )ver = 2;
	if( !ver ){
		MessageBox(0,"���̃t�@�C���͎g���܂���","Error (Load)",MB_OK);
		return FALSE;
	}

	//�ȏ��̓ǂݍ���
	i = fread(&org_data, sizeof(ORGANYADATA), 1, fp);

	org_data = org_data;

	//�Ȃ̏���ݒ�
	info.wait = org_data.wait;
	info.line = org_data.line;
	info.dot = org_data.dot;
	info.repeat_x = org_data.repeat_x;
	info.end_x = org_data.end_x;
	for(i = 0; i < MAXTRACK; i++){
		info.tdata[i].freq = org_data.tdata[i].freq;
		if( ver == 1 )info.tdata[i].pipi = 0;
		else info.tdata[i].pipi = org_data.tdata[i].pipi;
		info.tdata[i].wave_no = org_data.tdata[i].wave_no;
	}

	//�����̃��[�h
	for(j = 0; j < MAXTRACK; j++){
		//�ŏ��̉�����from��NULL�ƂȂ�
		if(org_data.tdata[j].note_num == 0){
			info.tdata[j].note_list = NULL;
			continue;
		}
		//���X�g�����
		np = info.tdata[j].note_p;
		info.tdata[j].note_list = info.tdata[j].note_p;
		np->from = NULL;
		np->to = (np + 1);
		np++;
		for(i = 1; i < org_data.tdata[j].note_num; i++){
			np->from = (np - 1);
			np->to = (np + 1);
			np++;
		}
		//�Ō�̉�����to��NULL
		np--;
		np->to = NULL;

		//���e����
		np = info.tdata[j].note_p;//�w���W
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			fread(&np->x,      sizeof(long), 1, fp);
			np++;
		}
		np = info.tdata[j].note_p;//�x���W
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			fread(&np->y,      sizeof(unsigned char), 1, fp);
			np++;
		}
		np = info.tdata[j].note_p;//����
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			fread(&np->length, sizeof(unsigned char), 1, fp);
			np++;
		}
		np = info.tdata[j].note_p;//�{�����[��
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			fread(&np->volume, sizeof(unsigned char), 1, fp);
			np++;
		}
		np = info.tdata[j].note_p;//�p��
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			fread(&np->pan,    sizeof(unsigned char), 1, fp);
			np++;
		}
	}
	fclose(fp);
	//�f�[�^��L����
	for( j = 0; j < MAXMELODY; j++ )
		MakeOrganyaWave( j, info.tdata[j].wave_no, info.tdata[j].pipi );
	for( j = MAXMELODY; j < MAXTRACK; j++ ){
		i = info.tdata[j].wave_no;
		LoadDramObject( dram_name[i], j - MAXMELODY );
	}
	SetPlayPointer(0);//���o��

	return TRUE;
}
*/
//�ȏ����擾
void OrgData::GetMusicInfo(MUSICINFO *mi){
	mi->dot = info.dot;
	mi->line = info.line;
	mi->alloc_note = info.alloc_note;
	mi->wait = info.wait;
	mi->repeat_x = info.repeat_x;
	mi->end_x = info.end_x;
	for(int i = 0; i < MAXTRACK; i++){
		mi->tdata[i].freq = info.tdata[i].freq;
		mi->tdata[i].wave_no = info.tdata[i].wave_no;
		mi->tdata[i].pipi = info.tdata[i].pipi;
	}
}




//Import   : WinMM.lib
//Contents : �u�}���`���f�B�A�^�C�}�[�̎g�p���@�v
#include <windows.h>  //Win32n API's
#include <windowsx.h> //mmsystem.h

#pragma comment(lib, "winmm")


BOOL InitMMTimer();
BOOL StartTimer(DWORD dwTimer);
VOID CALLBACK TimerProc(UINT uTID,UINT uMsg,DWORD dwUser,DWORD dwParam1,DWORD dwParam2);
BOOL QuitMMTimer();

static UINT ExactTime   = 13;//�ŏ����x
static UINT TimerID     = NULL;
static BOOL _bTimer     = FALSE;


BOOL InitMMTimer()
{
	TIMECAPS tc;
	MMRESULT ret;

	//�^�C�}�[�̐��x�����擾����
	ret = timeGetDevCaps(&tc,sizeof(TIMECAPS));
	if(ret != TIMERR_NOERROR) return FALSE;
	if(ExactTime < tc.wPeriodMin)ExactTime = tc.wPeriodMin;
	//���̐��x�ŏ���������
	ret = timeBeginPeriod(ExactTime);
	if(ret != TIMERR_NOERROR) return FALSE;
	return TRUE;
}

BOOL StartTimer(DWORD dwTimer)
{
	MMRESULT ret = NULL;
	ExactTime = dwTimer;
	//�^�C�}�[�𐶐�����
	TimerID = timeSetEvent
	(
		dwTimer,       //�^�C�}�[����
		10,             //���e�ł���^�C�}�[���x
		(LPTIMECALLBACK)TimerProc, //�R�[���o�b�N�v���V�[�W��
		NULL,          //���[�U�[���R�[���o�b�N�֐���dwUser�ɑ�����l
		TIME_PERIODIC //�^�C�}�[���Ԗ��ɃC�x���g�𔭐�������
	);
	if(ret != TIMERR_NOERROR) return FALSE;
	_bTimer = TRUE;
	return TRUE;
}

VOID CALLBACK TimerProc(UINT uTID,UINT uMsg,DWORD dwUser,DWORD dwParam1,DWORD dwParam2)
{
	DWORD dwNowTime;
	dwNowTime = timeGetTime();
	org_data.PlayData();
}

BOOL QuitMMTimer( void )
{
	MMRESULT ret;

	if( !_bTimer ) return FALSE;

	if(TimerID != TIMERR_NOERROR)
	{
		//�^�C�}�[���g�p���Ȃ�I��������
		ret = timeKillEvent(TimerID);
		if((ret) != TIMERR_NOERROR) return FALSE;
	}
	//�^�C�}�[���\�[�X���J������
	ret = timeEndPeriod(ExactTime);
	if((ret) != TIMERR_NOERROR) return FALSE;

	_bTimer = FALSE;
	return TRUE;
}





static long _PlayPos;//���ݍĐ��ʒu�i�L�����o�X�j
NOTELIST    *np[MAXTRACK];//���ݍĐ������̉���
static long _now_leng[MAXMELODY] = {NULL};//�Đ��������̒���


//MasterVolume
static long _Volume = 100;
static long _TrackVol[MAXTRACK] = {NULL};
static BOOL _bFadeout = FALSE;

void OrgData::PlayData(void)
{
	int i;
	if( _bFadeout && _Volume ) _Volume -= 2;
	if( _Volume < 0 )          _Volume  = 0;

	//�����f�B�̍Đ�
	for( i = 0; i < MAXMELODY; i++ ){

		if( np[i] != NULL && _PlayPos == np[i]->x ){//���������B
			if( !g_mute[i] ){
				if( np[i]->y != KEYDUMMY ){
					PlayOrganObject(  np[i]->y, -1, i, info.tdata[i].freq );
					_now_leng[i] = np[i]->length;
				}
			}
			if( np[i]->pan    != PANDUMMY ) ChangeOrganPan(np[i]->y,np[i]->pan,i);
			if( np[i]->volume != VOLDUMMY ) _TrackVol[i] = np[i]->volume;
			np[i] = np[i]->to;//���̉������w��
		}
		if( !_now_leng[i] ){
			PlayOrganObject(  NULL, 2, i, info.tdata[i].freq );
		}

		if( _now_leng[i] > 0 ) _now_leng[i]--;
		if( np[i]            ) ChangeOrganVolume( np[i]->y, _TrackVol[i]*_Volume/127, i );

	}

	//�h�����̍Đ�
	for( i = MAXMELODY; i < MAXTRACK; i++ ){
		if( np[i]  && _PlayPos == np[i]->x ){//���������B
			if( np[i]->y      != KEYDUMMY && !g_mute[i] ) PlayDramObject( np[i]->y, 1, i - MAXMELODY );
			if( np[i]->pan    != PANDUMMY             ) ChangeDramPan(  np[i]->pan,  i - MAXMELODY );
			if( np[i]->volume != VOLDUMMY             ) _TrackVol[i] = np[i]->volume;
			np[i] = np[i]->to;//���̉������w��
		}
		if( np[i] ) ChangeDramVolume( _TrackVol[i] * _Volume/127, i - MAXMELODY );
	}

	_PlayPos++;
	if( _PlayPos >= info.end_x ){
		_PlayPos = info.repeat_x;//++�����̂�
		SetPlayPointer(_PlayPos);
	}
}

//�J�n�ʒu��ݒ�
void OrgData::SetPlayPointer(long x)
{
	int i;
	for(i = 0; i < MAXTRACK; i++){
		np[i] = info.tdata[i].note_list;
		while(np[i] != NULL && np[i]->x < x)np[i] = np[i]->to;//����ׂ�������ݒ�		
	}
	_PlayPos = x;
}


//����������������������������������������������������������������������������
////////////////////////////////////////////////////
// Control Organya /////////////////////////////////
////////////////////////////////////////////////////

//�I���K�[�j���̏�����
BOOL StartOrganya( LPDIRECTSOUND lpDS, char *path_wave )
{
	if( !glpDS )return FALSE;
	//�g�`100�̃��[�h
	if( !InitWaveData100( ) )return FALSE;
//	if( !LoadWaveData100( path_wave ) )return FALSE;
	//�I���K�[�j���̏������B
	org_data.InitOrgData();
	return TRUE;
}
//�Ȃ̃��[�h
BOOL LoadOrganya( char *path )
{
	if( !glpDS )return FALSE;
	//�ȃf�[�^�̃��[�h
	if( !org_data.InitMusicData( path ) )return FALSE;
	_Volume   = 100;
	_bFadeout = FALSE;
	return FALSE;
}
//�Đ��ʒu�̐ݒ�
void SetOrganyaPosition( long x )
{
	if( !glpDS )return ;
	org_data.SetPlayPointer( x );
	_Volume   = 100;
	_bFadeout = FALSE;
}
//�Đ��ʒu�̎擾
long GetOrganyaPosition( void )
{
	if( !glpDS )return 0;
	return _PlayPos;
}
//�Ȃ̍Đ�
void PlayOrganyaMusic( void )
{
	if( !glpDS )return;

	QuitMMTimer();
	//�Ȃ̃X�^�[�g
	InitMMTimer();
	StartTimer( org_data.info.wait );
}
//�{�����[���̃`�F���W�i0�`127�j
BOOL ChangeOrganyaVolume( long vol )
{
	if( !glpDS )return FALSE;
	if( vol < 0 || vol > 100 ) return FALSE;
	_Volume = vol;
	return TRUE;
}

//�Ȃ̒�~
void StopOrganyaMusic( void )
{
	if( !glpDS )return;
	QuitMMTimer();

	for( int i = 0; i < MAXMELODY; i++ ){
		PlayOrganObject( NULL, 2, i, NULL );
	}
	memset( _old_key, 255, MAXTRACK );
	memset( _key_on,    0, MAXTRACK );
	memset( _key_twin,  0, MAXTRACK );
	Sleep( 100 );

}


void SetOrganyaFadeout( void ){ _bFadeout = TRUE; };

//�I���K�[�j���̏I��
void EndOrganya( void )
{
	if( !glpDS )return;
	int i;
	QuitMMTimer();
	org_data.ReleaseNote();
	for( i = 0; i < 8; i++ ){
		PlayOrganObject(NULL, 0 ,i,NULL);
		ReleaseOrganyaObject( i );
	}
}
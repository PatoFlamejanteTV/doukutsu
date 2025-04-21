#include <windows.h>
#include <stdio.h>

#include "DirectX5\DxSOUND.H"


#include "Organya.h"
#define SE_MAX	160

LPDIRECTSOUND       glpDS = NULL;            // DirectSound�I�u�W�F�N�g
LPDIRECTSOUNDBUFFER lpPRIMARYBUFFER = NULL; // �ꎞ�o�b�t�@
LPDIRECTSOUNDBUFFER lpSECONDARYBUFFER[SE_MAX];   // �񎞃o�b�t�@

extern char gModulePath[];


// DirectSound�̊J�n 
BOOL InitDirectSound( HWND hWnd )
{
	int i;
	DSBUFFERDESC dsbd;


	// DirectDraw�̏�����
	if(DirectSoundCreate(NULL, &glpDS, NULL) != DS_OK){
		glpDS = NULL;
		//�I���K�[�j��(���s)
		StartOrganya( glpDS, "Org\\Wave.dat" );
		return FALSE;
	}
	glpDS->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE);
	// �ꎟ�o�b�t�@�̏�����
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags =
		DSBCAPS_PRIMARYBUFFER|
		DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN;
	glpDS->CreateSoundBuffer(&dsbd, &lpPRIMARYBUFFER, NULL);

	// �v���C�}���o�b�t�@�̃t�H�[�}�b�g���w�肷��B
//	WAVEFORMATEX fmt;
//	memset( &fmt, 0, sizeof(WAVEFORMATEX) ); 
//	fmt.wFormatTag      = WAVE_FORMAT_PCM; // �`��(PCM)
//	fmt.nChannels       = 2; // �`�����l��(2ch�X�e���I)
//	fmt.nSamplesPerSec  = 44100; // �T���v�����O���[�g(44100Hz)
//	fmt.wBitsPerSample  = 16; // �ʎq���r�b�g(16bit)
//	fmt.nBlockAlign     = fmt.wBitsPerSample / 8 * fmt.nChannels; // 1�T���v�����O������̃o�C�g��(16bit�~2ch)
//	fmt.nAvgBytesPerSec = fmt.wBitsPerSample / 8 * fmt.nChannels * fmt.nSamplesPerSec; // 1�b������̃o�C�g��(16bit�~2ch�~44100Hz)
//	if( 0 > lpPRIMARYBUFFER->SetFormat(&fmt) ) return FALSE;




	for(i = 0; i < SE_MAX; i++) lpSECONDARYBUFFER[i] = NULL;

	//�I���K�[�j��
	StartOrganya( glpDS, "Org\\Wave.dat" );


	return(TRUE);
}

// DirectSound�̏I�� 
void EndDirectSound( void )
{
	int i;
	if( !glpDS )return;
	EndOrganya();
	for(i = 0; i < SE_MAX; i++){
		if(lpSECONDARYBUFFER[i] != NULL) lpSECONDARYBUFFER[i]->Release();
	}
	if(lpPRIMARYBUFFER != NULL) lpPRIMARYBUFFER->Release();
	if(glpDS) glpDS->Release();
	glpDS = NULL;
}

// �T�E���h�̐ݒ� 
BOOL InitSoundObject( LPCSTR resname, int no )
{
    HRSRC hrscr;
    DSBUFFERDESC dsbd;
    DWORD * lpdword;
    LPVOID lpbuf1, lpbuf2;
    DWORD  dwbuf1, dwbuf2;

	if( !glpDS )return TRUE;
	// ���\�[�X�̌���
    if((hrscr = FindResource(NULL, resname, "WAVE")) == NULL)
		return(FALSE);
    // ���\�[�X����̓ǂݍ���
    lpdword = (DWORD*)LockResource( LoadResource( NULL, hrscr ) );
    // �񎟃o�b�t�@�̏�����
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = 
		DSBCAPS_STATIC|
		DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN|DSBCAPS_CTRLFREQUENCY|
		DSBCAPS_GLOBALFOCUS;
		//DSBCAPS_STATIC|DSBCAPS_CTRLDEFAULT; 
    dsbd.dwBufferBytes = *(DWORD*)( (BYTE*)lpdword+0x36 );
    dsbd.lpwfxFormat   = (LPWAVEFORMATEX)( lpdword+5 ); 
    if(glpDS->CreateSoundBuffer(&dsbd, &lpSECONDARYBUFFER[no],
                                    NULL) != DS_OK) return(FALSE);
    // �񎟃o�b�t�@�̃��b�N
    lpSECONDARYBUFFER[no]->Lock(0, *(DWORD*)((BYTE*)lpdword+0x36),
                        &lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0);
	// �����f�[�^�̐ݒ�
    CopyMemory( lpbuf1, ((BYTE*)lpdword+0x3a), dwbuf1 );
    if(dwbuf2 != 0) CopyMemory(lpbuf2, ( (BYTE*)lpdword+0x3a ) + dwbuf1, dwbuf2 );
    // �񎟃o�b�t�@�̃��b�N����
    lpSECONDARYBUFFER[no]->Unlock(lpbuf1, dwbuf1, lpbuf2, dwbuf2); 

    return(TRUE);
}
BOOL LoadSoundObject(char *name, int no)
{
	DWORD i;
	DWORD file_size = 0;
	char check_box[58];

	char path[MAX_PATH];
	sprintf( path, "%s\\%s", gModulePath, name );

	if( !glpDS )return TRUE;

	//�T�C�Y�̎擾
	HANDLE hFile;
	hFile = CreateFile( path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	file_size = GetFileSize( hFile, NULL );
	CloseHandle( hFile );




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
		DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN|DSBCAPS_CTRLFREQUENCY|
		DSBCAPS_GLOBALFOCUS;
		//DSBCAPS_STATIC|DSBCAPS_CTRLDEFAULT;
	dsbd.dwBufferBytes = *(DWORD*)((BYTE*)wp+0x36);//WAVE�f�[�^�̃T�C�Y
	dsbd.lpwfxFormat = (LPWAVEFORMATEX)(wp+5); 
	
	if(glpDS->CreateSoundBuffer(&dsbd, &lpSECONDARYBUFFER[no],
								NULL) != DS_OK) return(FALSE);	
    LPVOID lpbuf1, lpbuf2;
    DWORD dwbuf1, dwbuf2;
	HRESULT hr;
	hr = lpSECONDARYBUFFER[no]->Lock(0, *(DWORD*)((BYTE*)wp+0x36),
							&lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0); 
	if(hr != DS_OK)return (FALSE);
	CopyMemory(lpbuf1, (BYTE*)wp+0x3a, dwbuf1);//+3a�̓f�[�^�̓�
	if(dwbuf2 != 0)	CopyMemory(lpbuf2, (BYTE*)wp+0x3a+dwbuf1, dwbuf2);
	lpSECONDARYBUFFER[no]->Unlock(lpbuf1, dwbuf1, lpbuf2, dwbuf2); 
	free(wp);
	return(TRUE);
}
// �T�E���h�̍Đ� 
void PlaySoundObject(int no, int mode)
{
	if( !glpDS )return;
    if(lpSECONDARYBUFFER[no] != NULL){
		switch(mode){
		case 0: // ��~
			lpSECONDARYBUFFER[no]->Stop();
			break;
		case 1: // �Đ�
			lpSECONDARYBUFFER[no]->Stop();
			lpSECONDARYBUFFER[no]->SetCurrentPosition(0);
            lpSECONDARYBUFFER[no]->Play(0, 0, 0);
            break;
		case -1: // ���[�v�Đ�
			lpSECONDARYBUFFER[no]->Play(0, 0, DSBPLAY_LOOPING);
			break;
		}
    }
}

void ChangeSoundFrequency(int no, DWORD rate)//9999��MAX��2195?��ɰ��
{
	if( !glpDS )return;
	lpSECONDARYBUFFER[no]->SetFrequency(rate*10+100);
}
void ChangeSoundVolume(int no, long volume)//300��MAX��300��ɰ��
{
	if( !glpDS )return;
	lpSECONDARYBUFFER[no]->SetVolume((volume-300)*8);
}
void ChangeSoundPan(int no, long pan)//512��MAX��256��ɰ��
{
	if( !glpDS )return;
	lpSECONDARYBUFFER[no]->SetPan((pan-256)*10);
}







#include "PixTone.h"

// �T�E���h�̐ݒ� 
long MakePixToneObject( PIXTONEPARAMETER *ptp, long ptp_num, int no )
{
	//DirectSound Buffer
    DSBUFFERDESC dsbd;
    LPVOID lpBuf1, lpBuf2;
    DWORD  dwBuf1, dwBuf2;

	//PixTone 
	PIXTONEPARAMETER *pPtp;
	unsigned char *pData;
	unsigned char *pMixData;
	long mixSize;
	long i, d;

	if( !glpDS )return 0;

	
	//�w�b�_�[�̍쐬-----------------------	
  	char *code_RIFF = "RIFF";
	char *code_fmt_ = "fmt ";
	char *code_WAVE = "WAVE";
	char *code_data = "data";

	WAVEHEADER wh;

	//�w��
	wh.bit_per_sample = 8;
	wh.sampling_rate  = 22050;
	wh.channel_num    = 1;

	//�Œ�
	wh.format = 1;
	wh.fmt_byte = 0x10;
	memcpy( wh.code_RIFF, code_RIFF, 4 );
	memcpy( wh.code_fmt_, code_fmt_, 4 );
	memcpy( wh.code_WAVE, code_WAVE, 4 );
	memcpy( wh.code_data, code_data, 4 );

	//�Z�o
	wh.block_size      = wh.bit_per_sample/8 * wh.channel_num;
	wh.byte_per_second = wh.bit_per_sample/8 * wh.channel_num * wh.sampling_rate;

	//�ŏI�Z�o
	wh.data_size       = wh.block_size * ptp->size;
	wh.next_size       = 36 + wh.data_size;
	
	
	//�ő�T�C�Y��p��
	pPtp = ptp;
	mixSize = 0;
	for( i = 0; i < ptp_num; i++ ){
		if( pPtp->size > mixSize ) mixSize = pPtp->size;
		pPtp += 1;//sizeof(PIXTONEPARAMETER);
	}
	
    // �񎟃o�b�t�@�̏�����--------------------------
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       =
		DSBCAPS_STATIC|
		DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN|DSBCAPS_CTRLFREQUENCY|
		DSBCAPS_GLOBALFOCUS;
		//DSBCAPS_STATIC|DSBCAPS_CTRLDEFAULT; 
    dsbd.dwBufferBytes = mixSize;
    dsbd.lpwfxFormat   = (LPWAVEFORMATEX)( (char *)&wh + 5*4 ); 
    if( glpDS->CreateSoundBuffer( &dsbd, &lpSECONDARYBUFFER[no], NULL) != DS_OK ){
		return(-1);
	}

	// PixTone -------------------------------


	//���������m��
	pData = pMixData = 0;
	pData    = (unsigned char *)malloc( mixSize );
	pMixData = (unsigned char *)malloc( mixSize );

	if( !pData || !pMixData ){
		if( pData    ) free( pData    );
		if( pMixData ) free( pMixData );
		return -1;
	}
	memset( pData,    128, mixSize );
	memset( pMixData, 128, mixSize );

	pPtp = ptp;
	for( i = 0; i < ptp_num; i++ ){
		if( FALSE == MakePixelWaveData( pPtp, pData ) ){
			if( pData    ) free( pData    );
			if( pMixData ) free( pMixData );
			return(-1);
		}
		for( d = 0; d < pPtp->size; d++ ){
			if(      ((long)pData[d]-128) + ((long)pMixData[d]-128) < -127 ) pMixData[d] = 0; 
			else if( ((long)pData[d]-128) + ((long)pMixData[d]-128) >  127 ) pMixData[d] = 255;
			else pMixData[d] = (unsigned char)(  ((long)pData[d]-128) + ((long)pMixData[d]-128) + 128 );
		}
		pPtp += 1;//sizeof(PIXTONEPARAMETER);
	}

	pMixData[0] = pMixData[0];
	pMixData[mixSize-1] = pMixData[mixSize-1];
	// �o�b�t�@�֏�������-----------------------
    lpSECONDARYBUFFER[no]->Lock( 0, mixSize, &lpBuf1, &dwBuf1, &lpBuf2, &dwBuf2, 0 );
    CopyMemory( lpBuf1, pMixData, dwBuf1 );
    if( dwBuf2 != 0 ) CopyMemory(lpBuf2, pMixData + dwBuf1, dwBuf2 );
    lpSECONDARYBUFFER[no]->Unlock(lpBuf1, dwBuf1, lpBuf2, dwBuf2 ); 

//	lpSECONDARYBUFFER[no]->SetVolume( -500 );

	if( pData    ) free( pData    );
	if( pMixData ) free( pMixData );

	return mixSize;
}

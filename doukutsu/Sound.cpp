#include <windows.h>
#include <stdio.h>

#include "DirectX5\DxSOUND.H"


#include "Organya.h"
#define SE_MAX	160

LPDIRECTSOUND       glpDS = NULL;            // DirectSoundオブジェクト
LPDIRECTSOUNDBUFFER lpPRIMARYBUFFER = NULL; // 一時バッファ
LPDIRECTSOUNDBUFFER lpSECONDARYBUFFER[SE_MAX];   // 二時バッファ

extern char gModulePath[];


// DirectSoundの開始 
BOOL InitDirectSound( HWND hWnd )
{
	int i;
	DSBUFFERDESC dsbd;


	// DirectDrawの初期化
	if(DirectSoundCreate(NULL, &glpDS, NULL) != DS_OK){
		glpDS = NULL;
		//オルガーニャ(失敗)
		StartOrganya( glpDS, "Org\\Wave.dat" );
		return FALSE;
	}
	glpDS->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE);
	// 一次バッファの初期化
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags =
		DSBCAPS_PRIMARYBUFFER|
		DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN;
	glpDS->CreateSoundBuffer(&dsbd, &lpPRIMARYBUFFER, NULL);

	// プライマリバッファのフォーマットを指定する。
//	WAVEFORMATEX fmt;
//	memset( &fmt, 0, sizeof(WAVEFORMATEX) ); 
//	fmt.wFormatTag      = WAVE_FORMAT_PCM; // 形式(PCM)
//	fmt.nChannels       = 2; // チャンネル(2chステレオ)
//	fmt.nSamplesPerSec  = 44100; // サンプリングレート(44100Hz)
//	fmt.wBitsPerSample  = 16; // 量子化ビット(16bit)
//	fmt.nBlockAlign     = fmt.wBitsPerSample / 8 * fmt.nChannels; // 1サンプリングあたりのバイト数(16bit×2ch)
//	fmt.nAvgBytesPerSec = fmt.wBitsPerSample / 8 * fmt.nChannels * fmt.nSamplesPerSec; // 1秒あたりのバイト数(16bit×2ch×44100Hz)
//	if( 0 > lpPRIMARYBUFFER->SetFormat(&fmt) ) return FALSE;




	for(i = 0; i < SE_MAX; i++) lpSECONDARYBUFFER[i] = NULL;

	//オルガーニャ
	StartOrganya( glpDS, "Org\\Wave.dat" );


	return(TRUE);
}

// DirectSoundの終了 
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

// サウンドの設定 
BOOL InitSoundObject( LPCSTR resname, int no )
{
    HRSRC hrscr;
    DSBUFFERDESC dsbd;
    DWORD * lpdword;
    LPVOID lpbuf1, lpbuf2;
    DWORD  dwbuf1, dwbuf2;

	if( !glpDS )return TRUE;
	// リソースの検索
    if((hrscr = FindResource(NULL, resname, "WAVE")) == NULL)
		return(FALSE);
    // リソースからの読み込み
    lpdword = (DWORD*)LockResource( LoadResource( NULL, hrscr ) );
    // 二次バッファの初期化
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
    // 二次バッファのロック
    lpSECONDARYBUFFER[no]->Lock(0, *(DWORD*)((BYTE*)lpdword+0x36),
                        &lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0);
	// 音源データの設定
    CopyMemory( lpbuf1, ((BYTE*)lpdword+0x3a), dwbuf1 );
    if(dwbuf2 != 0) CopyMemory(lpbuf2, ( (BYTE*)lpdword+0x3a ) + dwbuf1, dwbuf2 );
    // 二次バッファのロック解除
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

	//サイズの取得
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
	wp = (DWORD*)malloc(file_size);//ファイルのワークスペースを作る
	fseek(fp,0,SEEK_SET);
	for(i = 0; i < file_size; i++){
		fread((BYTE*)wp+i,sizeof(BYTE),1,fp);
	}
	fclose(fp);
	//セカンダリバッファの生成
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = 
		DSBCAPS_STATIC|
		DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN|DSBCAPS_CTRLFREQUENCY|
		DSBCAPS_GLOBALFOCUS;
		//DSBCAPS_STATIC|DSBCAPS_CTRLDEFAULT;
	dsbd.dwBufferBytes = *(DWORD*)((BYTE*)wp+0x36);//WAVEデータのサイズ
	dsbd.lpwfxFormat = (LPWAVEFORMATEX)(wp+5); 
	
	if(glpDS->CreateSoundBuffer(&dsbd, &lpSECONDARYBUFFER[no],
								NULL) != DS_OK) return(FALSE);	
    LPVOID lpbuf1, lpbuf2;
    DWORD dwbuf1, dwbuf2;
	HRESULT hr;
	hr = lpSECONDARYBUFFER[no]->Lock(0, *(DWORD*)((BYTE*)wp+0x36),
							&lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0); 
	if(hr != DS_OK)return (FALSE);
	CopyMemory(lpbuf1, (BYTE*)wp+0x3a, dwbuf1);//+3aはデータの頭
	if(dwbuf2 != 0)	CopyMemory(lpbuf2, (BYTE*)wp+0x3a+dwbuf1, dwbuf2);
	lpSECONDARYBUFFER[no]->Unlock(lpbuf1, dwbuf1, lpbuf2, dwbuf2); 
	free(wp);
	return(TRUE);
}
// サウンドの再生 
void PlaySoundObject(int no, int mode)
{
	if( !glpDS )return;
    if(lpSECONDARYBUFFER[no] != NULL){
		switch(mode){
		case 0: // 停止
			lpSECONDARYBUFFER[no]->Stop();
			break;
		case 1: // 再生
			lpSECONDARYBUFFER[no]->Stop();
			lpSECONDARYBUFFER[no]->SetCurrentPosition(0);
            lpSECONDARYBUFFER[no]->Play(0, 0, 0);
            break;
		case -1: // ループ再生
			lpSECONDARYBUFFER[no]->Play(0, 0, DSBPLAY_LOOPING);
			break;
		}
    }
}

void ChangeSoundFrequency(int no, DWORD rate)//9999がMAXで2195?がﾉｰﾏﾙ
{
	if( !glpDS )return;
	lpSECONDARYBUFFER[no]->SetFrequency(rate*10+100);
}
void ChangeSoundVolume(int no, long volume)//300がMAXで300がﾉｰﾏﾙ
{
	if( !glpDS )return;
	lpSECONDARYBUFFER[no]->SetVolume((volume-300)*8);
}
void ChangeSoundPan(int no, long pan)//512がMAXで256がﾉｰﾏﾙ
{
	if( !glpDS )return;
	lpSECONDARYBUFFER[no]->SetPan((pan-256)*10);
}







#include "PixTone.h"

// サウンドの設定 
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

	
	//ヘッダーの作成-----------------------	
  	char *code_RIFF = "RIFF";
	char *code_fmt_ = "fmt ";
	char *code_WAVE = "WAVE";
	char *code_data = "data";

	WAVEHEADER wh;

	//指定
	wh.bit_per_sample = 8;
	wh.sampling_rate  = 22050;
	wh.channel_num    = 1;

	//固定
	wh.format = 1;
	wh.fmt_byte = 0x10;
	memcpy( wh.code_RIFF, code_RIFF, 4 );
	memcpy( wh.code_fmt_, code_fmt_, 4 );
	memcpy( wh.code_WAVE, code_WAVE, 4 );
	memcpy( wh.code_data, code_data, 4 );

	//算出
	wh.block_size      = wh.bit_per_sample/8 * wh.channel_num;
	wh.byte_per_second = wh.bit_per_sample/8 * wh.channel_num * wh.sampling_rate;

	//最終算出
	wh.data_size       = wh.block_size * ptp->size;
	wh.next_size       = 36 + wh.data_size;
	
	
	//最大サイズを用意
	pPtp = ptp;
	mixSize = 0;
	for( i = 0; i < ptp_num; i++ ){
		if( pPtp->size > mixSize ) mixSize = pPtp->size;
		pPtp += 1;//sizeof(PIXTONEPARAMETER);
	}
	
    // 二次バッファの初期化--------------------------
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


	//メモリを確保
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
	// バッファへ書き込み-----------------------
    lpSECONDARYBUFFER[no]->Lock( 0, mixSize, &lpBuf1, &dwBuf1, &lpBuf2, &dwBuf2, 0 );
    CopyMemory( lpBuf1, pMixData, dwBuf1 );
    if( dwBuf2 != 0 ) CopyMemory(lpBuf2, pMixData + dwBuf1, dwBuf2 );
    lpSECONDARYBUFFER[no]->Unlock(lpBuf1, dwBuf1, lpBuf2, dwBuf2 ); 

//	lpSECONDARYBUFFER[no]->SetVolume( -500 );

	if( pData    ) free( pData    );
	if( pMixData ) free( pMixData );

	return mixSize;
}

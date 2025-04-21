#include <windows.h>
#include <stdio.h>

#include "DSOUND.H"
//#pragma comment(lib, "dsound")


#include "Define.h"

BOOL ErrorLog( char *str, long value );

#define MAXNOTELENGTH 255//音符の最大長
#define MAXMELODY	    8
#define MAXDRAM		    8
#define MAXTRACK	   16

#define KEYDUMMY	  255//キーの変更は加えない音符
#define PANDUMMY	  255//パンの変更は加えない音符
#define VOLDUMMY	  255//ボリュームの変更は加えない音符

#define ALLOCNOTE	  4096



////////////////////////////////////////////////////
// DirectSound for Organya /////////////////////////
////////////////////////////////////////////////////

extern LPDIRECTSOUND       glpDS;            // DirectSoundオブジェクト
static LPDIRECTSOUNDBUFFER lpORGANBUFFER[8][8][2] = {NULL};
extern LPDIRECTSOUNDBUFFER lpSECONDARYBUFFER[];   // 二時バッファ

typedef struct{
	short wave_size;//波形のサイズ
	short oct_par;//オクターブを実現する掛け率(/8)
	short oct_size;//オクターブを実現する掛け率(/8)
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
BYTE format_tbl2[] = {//22050HzのFormat
	0x01,0x00,0x01,0x00,
	0x22,0x56,0x00,0x00,
	0x22,0x56,0x00,0x00,
	0x01,0x00,0x08,0x00,0x00,0x00};
BOOL MakeSoundObject8(char *wavep,char track, char pipi )
{
	if( !glpDS )return FALSE;

	DWORD i, j, k;
	unsigned long wav_tp;//WAVテーブルをさすポインタ
	DWORD wave_size;//256;
	DWORD data_size;
	BYTE  *wp;
	BYTE  *wp_sub;
	int work;
	//セカンダリバッファの生成
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
			wp     = (BYTE*)malloc(data_size);//ファイルのワークスペースを作る
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
			//データの転送
			LPVOID lpbuf1, lpbuf2;
			DWORD  dwbuf1, dwbuf2=0;
			HRESULT hr;
			hr = lpORGANBUFFER[track][j][k]->Lock( 0, data_size, &lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0 );		
			if(hr != DS_OK)return (FALSE);	
			CopyMemory( lpbuf1, (BYTE*)wp,dwbuf1 );//+3aはデータの頭
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
			lpORGANBUFFER[track][j][i]->SetFrequency(//1000を+αのデフォルト値とする
				((oct_wave[j].wave_size*freq_tbl[key])*oct_wave[j].oct_par)/8 + (a-1000));
}
long g_mute[MAXTRACK] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
static short         _pan_tbl[13] = {0,43,86,129,172,215,256,297,340,383,426,469,512}; 
static unsigned char _old_key[MAXTRACK]  = {255,255,255,255,255,255,255,255};//再生中の音
static unsigned char _key_on[MAXTRACK]   = {  0,  0,  0,  0,  0,  0,  0,  0};//キースイッチ
static unsigned char _key_twin[MAXTRACK] = {  0,  0,  0,  0,  0,  0,  0,  0};//今使っているキー(連続時のノイズ防止の為に二つ用意)
void ChangeOrganPan(unsigned char key, unsigned char pan,char track)//512がMAXで256がﾉｰﾏﾙ
{
	if( !glpDS )return;
	if(_old_key[track] != 255)
		lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->SetPan((_pan_tbl[pan]-256)*10);
}
void ChangeOrganVolume(int no, long volume,char track)//300がMAXで300がﾉｰﾏﾙ
{
	if( !glpDS )return;
	if(_old_key[track] != 255)
		lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->SetVolume((volume-255)*8);
}
// サウンドの再生 
void PlayOrganObject(unsigned char key, int mode,char track,DWORD freq)
{
	if( !glpDS )return;
	
    if(lpORGANBUFFER[track][key/12][_key_twin[track]] != NULL){
		switch(mode){
		case 0: // 停止
			if(_old_key[track] != 255){
				lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->Stop();
				lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->SetCurrentPosition(0);
			}
			break;
		case 1: // 再生
			break;
		case 2: // 歩かせ停止
			if(_old_key[track] != 255){
				lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->Play(0, 0, 0);
				_old_key[track] = 255;
			}
            break;
		case -1:
			if(_old_key[track] == 255){//新規鳴らす
				ChangeOrganFrequency(key%12,track,freq);//周波数を設定して
				lpORGANBUFFER[track][key/12][_key_twin[track]]->Play(0, 0, DSBPLAY_LOOPING);
				_old_key[track] = key;
				_key_on[track]  = 1;
			}else if(_key_on[track] == 1 && _old_key[track] == key){//同じ音
				//今なっているのを歩かせ停止
				lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->Play(0, 0, 0);
				_key_twin[track]++;
				if(_key_twin[track] > 1 )_key_twin[track] = 0; 
				lpORGANBUFFER[track][key/12][_key_twin[track]]->Play(0, 0, DSBPLAY_LOOPING);
			}else{//違う音を鳴らすなら
				lpORGANBUFFER[track][_old_key[track]/12][_key_twin[track]]->Play(0, 0, 0);//今なっているのを歩かせ停止
				_key_twin[track]++;
				if(_key_twin[track] > 1 )_key_twin[track] = 0; 
				ChangeOrganFrequency(key%12,track,freq);//周波数を設定して
				lpORGANBUFFER[track][key/12][_key_twin[track]]->Play(0, 0, DSBPLAY_LOOPING);
				_old_key[track] = key;
			}
			break;
		}
    }
}

//オルガーニャオブジェクトを開放
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
//波形データをロード
char wave_data[100*256];
BOOL InitWaveData100(void)
{
	if( !glpDS )return FALSE;
    HRSRC hrscr;
    DWORD *lpdword;//リソースのアドレス
    // リソースの検索
    if((hrscr = FindResource(NULL, "WAVE100", "WAVE")) == NULL)
                                                    return(FALSE);
    // リソースのアドレスを取得
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
//波形を１００個の中から選択して作成
BOOL MakeOrganyaWave(char track,char wave_no, char pipi)
{
	if( !glpDS )return FALSE;
	if(wave_no > 99)return FALSE;
	ReleaseOrganyaObject(track);
	MakeSoundObject8(&wave_data[0] + wave_no*256,track, pipi);	
	return TRUE;
}
/////////////////////////////////////////////
//■オルガーニャドラムス■■■■■■■■///////
/////////////////////

//オルガーニャオブジェクトを開放
/*void ReleaseDramObject(char track){
	if( !glpDS )return;
	for(int i = 0; i < 8; i++){
		if(lpDRAMBUFFER[track] != NULL){
			lpDRAMBUFFER[track]->Release();
			lpDRAMBUFFER[track] = NULL;
		}
	}
}

// サウンドの設定 
BOOL InitDramObject( LPCSTR resname, int no )
{
 	if( !glpDS ) return FALSE;
	HRSRC hrscr;
    DSBUFFERDESC dsbd;
    DWORD *lpdword;//リソースのアドレス
    // リソースの検索
    if((hrscr = FindResource(NULL, resname, "WAVE")) == NULL)
                                                    return(FALSE);
    // リソースのアドレスを取得
    lpdword = (DWORD*)LockResource(LoadResource(NULL, hrscr));

	// 二次バッファの生成
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
    // 二次バッファのロック
    lpDRAMBUFFER[no]->Lock( 0, *(DWORD*)((BYTE*)lpdword+0x36), &lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0 ); 
	// 音源データの設定
	CopyMemory( lpbuf1, (BYTE*)lpdword+0x3a, dwbuf1 );
    if( dwbuf2 != 0 ) CopyMemory( lpbuf2, (BYTE*)lpdword+0x3a+dwbuf1, dwbuf2 );
	// 二次バッファのロック解除
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

	//サイズの取得
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
		DSBCAPS_CTRLDEFAULT|
		DSBCAPS_GLOBALFOCUS;
	dsbd.dwBufferBytes = *(DWORD*)((BYTE*)wp+0x36);//WAVEデータのサイズ
	dsbd.lpwfxFormat = (LPWAVEFORMATEX)(wp+5); 
	if(glpDS->CreateSoundBuffer(&dsbd, &lpDRAMBUFFER[no],
								NULL) != DS_OK) return(FALSE);	
    LPVOID lpbuf1, lpbuf2;
    DWORD dwbuf1, dwbuf2;
	HRESULT hr;
	hr = lpDRAMBUFFER[no]->Lock(0, *(DWORD*)((BYTE*)wp+0x36),
							&lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0); 
	if(hr != DS_OK)return (FALSE);
	CopyMemory(lpbuf1, (BYTE*)wp+0x3a, dwbuf1);//+3aはデータの頭
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
void ChangeDramPan(unsigned char pan,char track)//512がMAXで256がﾉｰﾏﾙ
{
	if( !glpDS )return;
	lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->SetPan((_pan_tbl[pan]-256)*10);
}
void ChangeDramVolume(long volume,char track)//
{
	if( !glpDS )return;
	lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->SetVolume((volume-255)*8);
}

// サウンドの再生 
void PlayDramObject(unsigned char key, int mode,char track)
{
	if( !glpDS )return;
	
    if(lpSECONDARYBUFFER[track+WAVE_ORG_DRAM] != NULL){
		switch(mode){
		case 0: // 停止
			lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->Stop();
			lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->SetCurrentPosition(0);
			break;
		case 1: // 再生
			lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->Stop();
			lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->SetCurrentPosition(0);
			ChangeDramFrequency(key,track);//周波数を設定して
			lpSECONDARYBUFFER[track+WAVE_ORG_DRAM]->Play(0, 0, 0);
			break;
		case 2: // 歩かせ停止
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
		ChangeOrganFrequency(key%12,track,freq);//周波数を設定して
		lpORGANBUFFER[track][key/12][0]->SetVolume((160-255)*8);
		lpORGANBUFFER[track][key/12][0]->Play(0, 0, DSBPLAY_LOOPING);
		do{
		}while(GetTickCount() < wait+80);
		lpORGANBUFFER[track][key/12][0]->Play(0, 0, 0);
	}else{
		PlaySoundObject( 
//		lpDRAMBUFFER[track - MAXMELODY]->Stop();
//		lpDRAMBUFFER[track - MAXMELODY]->SetCurrentPosition(0);
//		ChangeDramFrequency(key,track - MAXMELODY);//周波数を設定して
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

	// プライマリバッファのフォーマットを指定する。
	WAVEFORMATEX fmt;
	memset( &fmt, 0, sizeof(WAVEFORMATEX) ); 
	fmt.wFormatTag      = WAVE_FORMAT_PCM; // 形式(PCM)
	fmt.nChannels       = 2; // チャンネル(2chステレオ)
	fmt.nSamplesPerSec  = sps; // サンプリングレート(44100Hz)
	fmt.wBitsPerSample  = bps; // 量子化ビット(16bit)

	// 1サンプリングあたりのバイト数(16bit×2ch)
	fmt.nBlockAlign     = fmt.wBitsPerSample / 8 * fmt.nChannels; 

	// 1秒あたりのバイト数(16bit×2ch×44100Hz)
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


//音符リスト構造体
typedef struct NOTELIST{
	NOTELIST *from;//前のアドレス
	NOTELIST *to;//次のアドレス
	long x;//位置
	unsigned char length;//音の長さ
	unsigned char y;//音の高さ
	unsigned char volume;//ヴォリューム
	unsigned char pan;//パン
}NOTELIST;
//トラックデータ＊８
typedef struct{
	unsigned short freq;//+α周波数(1000がDefault)
//	unsigned short note_num;//音符の数
	unsigned char wave_no;//波形No
	char pipi;
	NOTELIST *note_p;//NoteData領域の先頭アドレス
	NOTELIST *note_list;//リストの先頭アドレス1
}TRACKDATA;
//曲単位で持つ固有の情報
typedef struct MUSICINFO{
	unsigned short wait;
	unsigned char line;//１小節のライン数
	unsigned char dot;//一泊のドット数
	unsigned short alloc_note;//確保した音符の数
	long repeat_x;//リピート
	long end_x;//曲の終わり(リピートに戻る)
	TRACKDATA tdata[16];
}MUSICINFO;
//メインクラス。このアプリケーションの中心。（クラスってやつを初めて使う）
typedef struct OrgData{
	OrgData();//コンストラクタ
//	private:
	public:
		MUSICINFO info;
		char track;
		char mute[MAXTRACK];
		unsigned char def_pan;
		unsigned char def_volume;
		void InitOrgData(void);
		void GetMusicInfo(MUSICINFO *mi);//曲情報を取得
		//曲情報を設定。flagは設定アイテムを指定
		BOOL SetMusicInfo(MUSICINFO *mi,unsigned long flag);
		BOOL NoteAlloc(unsigned short note_num);//指定の数だけNoteDataの領域を確保
		void ReleaseNote(void);//NoteDataを開放
		NOTELIST *SearchNote(NOTELIST *np);//未使用NOTEの検索
		void OrgData::TouchKeyboard(unsigned char y);//鍵盤に触れる
		void GetNoteUsed(long *use,long *left,char track);//音譜残量等
		//以下は再生
		void PlayData(void);
		void SetPlayPointer(long x);//再生ポインターを指定の位置に設定
		//以下はファイル関係
		BOOL OrgData::LoadMusicData( char *path );
		BOOL OrgData::InitMusicData( char *path );
		//以下は編集関係
}ORGDATA;
ORGDATA org_data;

//コンストラクタ
OrgData::OrgData()
{
//	memset( &org_data, 0, sizeof(ORGDATA) );
	int i;
	for(i = 0; i < MAXTRACK; i++){
		info.tdata[i].note_list = NULL;
		info.tdata[i].note_p    = NULL;
	}
}
//曲情報をセットする時のフラグ
#define SETALL		0xffffffff//全てをセット
#define SETWAIT		0x00000001
#define SETGRID		0x00000002
#define SETALLOC	0x00000004
#define SETREPEAT	0x00000008
#define SETFREQ		0x00000010
#define SETWAVE		0x00000020
#define SETPIPI		0x00000040
#define DEFVOLUME	200//255はVOLDUMMY。MAXは254
#define DEFPAN		6
void OrgData::InitOrgData(void)
{
//	MUSICINFO mi;
	track = 0;
	info.alloc_note = ALLOCNOTE;//とりあえず4096
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
//曲情報を設定。flagはアイテムを指定
BOOL OrgData::SetMusicInfo(MUSICINFO *mi,unsigned long flag)
{
	char str[32];
	int i;
	if(flag & SETGRID){//グリッドを有効に
		info.dot = mi->dot;
		info.line = mi->line;

	}
//	if(flag & SETALLOC){//領域確保
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

//指定の数だけNoteDataの領域を確保(初期化)
BOOL OrgData::NoteAlloc(unsigned short alloc)
{
	int i,j;
	for(j = 0; j < MAXTRACK; j++){
		info.tdata[j].wave_no = 0;
		info.tdata[j].note_list = NULL;//コンストラクタにやらせたい
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

	track = 0;//////////今はここに書いておく
	return TRUE;
}
//NoteDataを開放
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

////以下はオルガーニャ曲データ構造体（フィルに含まれる項目）
typedef struct{
	long x;//位置
	unsigned char y;//音の高さ
	unsigned char length;//音の長さ
	unsigned char volume;//ボリューム
	unsigned char pan;//パン
}ORGANYANOTE;

typedef struct{
	unsigned short freq;//+α周波数(1000がDefault)
	unsigned char wave_no;//波形No
	unsigned char pipi;//☆
	unsigned short note_num;//音符の数
}ORGANYATRACK;

typedef struct{
	unsigned short wait;
	unsigned char line;
	unsigned char dot;
	long repeat_x;//リピート
	long end_x;//曲の終わり(リピートに戻る)
	ORGANYATRACK tdata[MAXTRACK];
}ORGANYADATA;
/*
//曲データを読み込み
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

	// リソースの検索
    if((hrscr = FindResource(NULL, path, "ORG")) == NULL)
		return(FALSE);
    // リソースからの読み込み
    p = (char*)LockResource(LoadResource(NULL, hrscr));

	//パスワードチェック
	memcpy( &pass_check[0], p, sizeof(char)* 6 );
	p += 6;
	if( !memcmp( pass_check, pass,  6 ) )ver = 1;
	if( !memcmp( pass_check, pass2, 6 ) )ver = 2;
	if( !ver ){
		return FALSE;
	}

	//曲情報の読み込み
	memcpy( &org_data, p, sizeof(ORGANYADATA)* 1 );
	p += sizeof(ORGANYADATA);

	//曲の情報を設定
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

	//音符のロード
	for(j = 0; j < MAXTRACK; j++){
		//最初の音符はfromがNULLとなる
		if(org_data.tdata[j].note_num == 0){
			info.tdata[j].note_list = NULL;
			continue;
		}
		//リストを作る
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
		//最後の音符のtoはNULL
		np--;
		np->to = NULL;

		//内容を代入
		np = info.tdata[j].note_p;//Ｘ座標
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			memcpy( &np->x, p, sizeof(long)*1 );
			p += sizeof(long);
			
			np++;
		}
		np = info.tdata[j].note_p;//Ｙ座標
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			memcpy( &np->y, p, sizeof(unsigned char)*1 );
			p += sizeof(unsigned char);
			np++;
		}
		np = info.tdata[j].note_p;//長さ
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			memcpy( &np->length, p, sizeof(unsigned char)* 1 );
			p += sizeof(unsigned char);
			np++;
		}
		np = info.tdata[j].note_p;//ボリューム
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			memcpy( &np->volume, p, sizeof(unsigned char)* 1 );
			p += sizeof(unsigned char);
			np++;
		}
		np = info.tdata[j].note_p;//パン
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			memcpy( &np->pan, p, sizeof(unsigned char)* 1 );
			p += sizeof(unsigned char);
			np++;
		}
	}
	//データを有効に
	for(j = 0; j < MAXMELODY; j++)
		MakeOrganyaWave(j,info.tdata[j].wave_no, info.tdata[j].pipi);
	for(j = MAXMELODY; j < MAXTRACK; j++){
		i = info.tdata[j].wave_no;
//		InitDramObject(dram_name[i],j-MAXMELODY);
	}

//	_ConvertHzbit(8000,8);
	SetPlayPointer(0);//頭出し

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

	//｣｣｣｣｣｣｣｣｣｣｣｣｣｣｣ロード
	FILE *fp;
	if( ( fp = fopen( music_path, "rb" ) ) == NULL ){
		MessageBox(0,"ファイルにアクセスできません","Error (Load)",MB_OK);
		return(FALSE);
	}
	//パスワードチェック
	memset( pass_check, 0, 7 );
	fread(&pass_check[0], sizeof(char), 6, fp);
	if( !memcmp( pass_check, pass, 6 ) )ver = 1;
	if( !memcmp( pass_check, pass2, 6 ) )ver = 2;
	if( !ver ){
		MessageBox(0,"このファイルは使えません","Error (Load)",MB_OK);
		return FALSE;
	}

	//曲情報の読み込み
	i = fread(&org_data, sizeof(ORGANYADATA), 1, fp);

	org_data = org_data;

	//曲の情報を設定
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

	//音符のロード
	for(j = 0; j < MAXTRACK; j++){
		//最初の音符はfromがNULLとなる
		if(org_data.tdata[j].note_num == 0){
			info.tdata[j].note_list = NULL;
			continue;
		}
		//リストを作る
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
		//最後の音符のtoはNULL
		np--;
		np->to = NULL;

		//内容を代入
		np = info.tdata[j].note_p;//Ｘ座標
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			fread(&np->x,      sizeof(long), 1, fp);
			np++;
		}
		np = info.tdata[j].note_p;//Ｙ座標
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			fread(&np->y,      sizeof(unsigned char), 1, fp);
			np++;
		}
		np = info.tdata[j].note_p;//長さ
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			fread(&np->length, sizeof(unsigned char), 1, fp);
			np++;
		}
		np = info.tdata[j].note_p;//ボリューム
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			fread(&np->volume, sizeof(unsigned char), 1, fp);
			np++;
		}
		np = info.tdata[j].note_p;//パン
		for(i = 0; i < org_data.tdata[j].note_num; i++){
			fread(&np->pan,    sizeof(unsigned char), 1, fp);
			np++;
		}
	}
	fclose(fp);
	//データを有効に
	for( j = 0; j < MAXMELODY; j++ )
		MakeOrganyaWave( j, info.tdata[j].wave_no, info.tdata[j].pipi );
	for( j = MAXMELODY; j < MAXTRACK; j++ ){
		i = info.tdata[j].wave_no;
		LoadDramObject( dram_name[i], j - MAXMELODY );
	}
	SetPlayPointer(0);//頭出し

	return TRUE;
}
*/
//曲情報を取得
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
//Contents : 「マルチメディアタイマーの使用方法」
#include <windows.h>  //Win32n API's
#include <windowsx.h> //mmsystem.h

#pragma comment(lib, "winmm")


BOOL InitMMTimer();
BOOL StartTimer(DWORD dwTimer);
VOID CALLBACK TimerProc(UINT uTID,UINT uMsg,DWORD dwUser,DWORD dwParam1,DWORD dwParam2);
BOOL QuitMMTimer();

static UINT ExactTime   = 13;//最小精度
static UINT TimerID     = NULL;
static BOOL _bTimer     = FALSE;


BOOL InitMMTimer()
{
	TIMECAPS tc;
	MMRESULT ret;

	//タイマーの精度情報を取得する
	ret = timeGetDevCaps(&tc,sizeof(TIMECAPS));
	if(ret != TIMERR_NOERROR) return FALSE;
	if(ExactTime < tc.wPeriodMin)ExactTime = tc.wPeriodMin;
	//この精度で初期化する
	ret = timeBeginPeriod(ExactTime);
	if(ret != TIMERR_NOERROR) return FALSE;
	return TRUE;
}

BOOL StartTimer(DWORD dwTimer)
{
	MMRESULT ret = NULL;
	ExactTime = dwTimer;
	//タイマーを生成する
	TimerID = timeSetEvent
	(
		dwTimer,       //タイマー時間
		10,             //許容できるタイマー精度
		(LPTIMECALLBACK)TimerProc, //コールバックプロシージャ
		NULL,          //ユーザーがコールバック関数のdwUserに送る情報値
		TIME_PERIODIC //タイマー時間毎にイベントを発生させる
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
		//タイマーを使用中なら終了させる
		ret = timeKillEvent(TimerID);
		if((ret) != TIMERR_NOERROR) return FALSE;
	}
	//タイマーリソースを開放する
	ret = timeEndPeriod(ExactTime);
	if((ret) != TIMERR_NOERROR) return FALSE;

	_bTimer = FALSE;
	return TRUE;
}





static long _PlayPos;//現在再生位置（キャンバス）
NOTELIST    *np[MAXTRACK];//現在再生準備の音符
static long _now_leng[MAXMELODY] = {NULL};//再生中音符の長さ


//MasterVolume
static long _Volume = 100;
static long _TrackVol[MAXTRACK] = {NULL};
static BOOL _bFadeout = FALSE;

void OrgData::PlayData(void)
{
	int i;
	if( _bFadeout && _Volume ) _Volume -= 2;
	if( _Volume < 0 )          _Volume  = 0;

	//メロディの再生
	for( i = 0; i < MAXMELODY; i++ ){

		if( np[i] != NULL && _PlayPos == np[i]->x ){//音が来た。
			if( !g_mute[i] ){
				if( np[i]->y != KEYDUMMY ){
					PlayOrganObject(  np[i]->y, -1, i, info.tdata[i].freq );
					_now_leng[i] = np[i]->length;
				}
			}
			if( np[i]->pan    != PANDUMMY ) ChangeOrganPan(np[i]->y,np[i]->pan,i);
			if( np[i]->volume != VOLDUMMY ) _TrackVol[i] = np[i]->volume;
			np[i] = np[i]->to;//次の音符を指す
		}
		if( !_now_leng[i] ){
			PlayOrganObject(  NULL, 2, i, info.tdata[i].freq );
		}

		if( _now_leng[i] > 0 ) _now_leng[i]--;
		if( np[i]            ) ChangeOrganVolume( np[i]->y, _TrackVol[i]*_Volume/127, i );

	}

	//ドラムの再生
	for( i = MAXMELODY; i < MAXTRACK; i++ ){
		if( np[i]  && _PlayPos == np[i]->x ){//音が来た。
			if( np[i]->y      != KEYDUMMY && !g_mute[i] ) PlayDramObject( np[i]->y, 1, i - MAXMELODY );
			if( np[i]->pan    != PANDUMMY             ) ChangeDramPan(  np[i]->pan,  i - MAXMELODY );
			if( np[i]->volume != VOLDUMMY             ) _TrackVol[i] = np[i]->volume;
			np[i] = np[i]->to;//次の音符を指す
		}
		if( np[i] ) ChangeDramVolume( _TrackVol[i] * _Volume/127, i - MAXMELODY );
	}

	_PlayPos++;
	if( _PlayPos >= info.end_x ){
		_PlayPos = info.repeat_x;//++されるので
		SetPlayPointer(_PlayPos);
	}
}

//開始位置を設定
void OrgData::SetPlayPointer(long x)
{
	int i;
	for(i = 0; i < MAXTRACK; i++){
		np[i] = info.tdata[i].note_list;
		while(np[i] != NULL && np[i]->x < x)np[i] = np[i]->to;//見るべき音符を設定		
	}
	_PlayPos = x;
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
////////////////////////////////////////////////////
// Control Organya /////////////////////////////////
////////////////////////////////////////////////////

//オルガーニャの初期化
BOOL StartOrganya( LPDIRECTSOUND lpDS, char *path_wave )
{
	if( !glpDS )return FALSE;
	//波形100のロード
	if( !InitWaveData100( ) )return FALSE;
//	if( !LoadWaveData100( path_wave ) )return FALSE;
	//オルガーニャの初期化。
	org_data.InitOrgData();
	return TRUE;
}
//曲のロード
BOOL LoadOrganya( char *path )
{
	if( !glpDS )return FALSE;
	//曲データのロード
	if( !org_data.InitMusicData( path ) )return FALSE;
	_Volume   = 100;
	_bFadeout = FALSE;
	return FALSE;
}
//再生位置の設定
void SetOrganyaPosition( long x )
{
	if( !glpDS )return ;
	org_data.SetPlayPointer( x );
	_Volume   = 100;
	_bFadeout = FALSE;
}
//再生位置の取得
long GetOrganyaPosition( void )
{
	if( !glpDS )return 0;
	return _PlayPos;
}
//曲の再生
void PlayOrganyaMusic( void )
{
	if( !glpDS )return;

	QuitMMTimer();
	//曲のスタート
	InitMMTimer();
	StartTimer( org_data.info.wait );
}
//ボリュームのチェンジ（0～127）
BOOL ChangeOrganyaVolume( long vol )
{
	if( !glpDS )return FALSE;
	if( vol < 0 || vol > 100 ) return FALSE;
	_Volume = vol;
	return TRUE;
}

//曲の停止
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

//オルガーニャの終了
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
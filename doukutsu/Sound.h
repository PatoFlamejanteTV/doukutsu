/*****							  *****/
/*****   DirectSoundサポート関数   *****/
/*****							  *****/


// DirectSoundの開始
BOOL InitDirectSound(HWND hwnd);
BOOL LoadSoundObject( char *file_name, int no);
// DirectSoundの終了
void EndDirectSound(void);
void ReleaseSoundObject(int no);//バッファの開放
//BOOL InitSoundObject(LPCSTR resname, int no);
// サウンドの再生
void PlaySoundObject(int no, int mode);
void ChangeSoundVolume(int no, long volume);//9999がMAX2195?がﾉｰﾏﾙ
void ChangeSoundFrequency(int no, DWORD rate);//300がMAX300がﾉｰﾏﾙ
void ChangeSoundPan(int no, long pan);




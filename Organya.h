// ＜ オルガーニャ必要ファイル＞
// Organya.cpp/Organya.h
// Orgフォルダ

// ＜ オルガーニャ導入手順 ＞
// 1、プロジェクトに Organya.cpp と Organya.h を組み込みます。
// 2、プロジェクトに dsound.lib と winmm.lib を組み込みます。
// 3、カレントディレクトリに Orgフォルダ をいれておきます。

//補足：StartOrganya()の引数 LPDIRECTSOUND に NULLを入れると、
//      それ以降のオルガーニャ関数は全て無効にできます。


// ＜オルガーニャ関数群＞

//●オルガーニャの初期化●
//必ず最初にこの関数を読んでください。
//引数は、DirectSound へのポインタと、Orgフォルダ内の Wave.dat のパスです。
//  注：DirectSoundのポインタ、及びプライマリバッファは、
//      あらかじめ作っておいてください。
//（SetCooperativeLevel:DSSCL_EXCLUSIVE）
BOOL StartOrganya( LPDIRECTSOUND lpDS, char *path_wave );

//●曲のロード●
//再生する前に曲をロードします。
//引数は曲データのパスです。
BOOL LoadOrganya( char *path );

//●再生位置の設定●
//曲の再生中には呼ばないでください。
//引数はオルガーニャの最小再生単位です。
void SetOrganyaPosition( long x );

//●再生位置の取得●
//戻り値はオルガーニャの最小再生単位です。
long GetOrganyaPosition( void );

//●曲の再生●
void PlayOrganyaMusic( void );

//●マスターボリュームの変更●
//0～127に変更可能です。
//再生中でも可能です。
BOOL ChangeOrganyaVolume( long vol );

//●曲の停止●
void StopOrganyaMusic( void );

//●オルガーニャの終了●
//終了する前に曲の停止を呼び出してください。
void EndOrganya( void );

//●フェードアウト設定●
void SetOrganyaFadeout( void );
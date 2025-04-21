#include "Escape.h"

//TEXTSCRIPT構造体、サーフェスの初期化
BOOL InitTextScript2( void );
void EndTextScript( void );
//スクリプトのロード
BOOL LoadTextScript2( char *path );
BOOL LoadTextScript_Stage( char *path );

//MODE_TEXTSCRIPT_READの開始
BOOL StartTextScript( long no );
void StopTextScript( void );
//TEXTSCRIPTプロシージャ
enum_ESCRETURN TextScriptProc( void );
//表示
void PutTextScript( void );
//最後に読んだスクリプトファイル名を取得
void GetTextScriptPath( char *path );

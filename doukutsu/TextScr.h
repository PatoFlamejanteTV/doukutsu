#include "Escape.h"

//TEXTSCRIPT�\���́A�T�[�t�F�X�̏�����
BOOL InitTextScript2( void );
void EndTextScript( void );
//�X�N���v�g�̃��[�h
BOOL LoadTextScript2( char *path );
BOOL LoadTextScript_Stage( char *path );

//MODE_TEXTSCRIPT_READ�̊J�n
BOOL StartTextScript( long no );
void StopTextScript( void );
//TEXTSCRIPT�v���V�[�W��
enum_ESCRETURN TextScriptProc( void );
//�\��
void PutTextScript( void );
//�Ō�ɓǂ񂾃X�N���v�g�t�@�C�������擾
void GetTextScriptPath( char *path );

#include <windows.h>
#include <stdio.h>
#include "Draw.h"
#include "Define.h"
#include "Tags.h"


MAPDATA gMap;

char *code_pxma = "PXM";
extern RECT grcGame;

#define MAX_WIDTH_MAP	640
#define MAX_LENGTH_MAP	480

extern char gDataPath[];

void SetNpChar( long code_char, long x, long y, long xm, long ym, long dir, NPCHAR *npc, long start_index );

//�}�b�v���̏�����
BOOL InitMapData2( void )
{
	//allocate
	gMap.data = (unsigned char *)malloc( MAX_WIDTH_MAP*	MAX_LENGTH_MAP );
	return TRUE;
}

//�}�b�v���̏������iLoad�j(2:�e�ʊm�ۂ����Ȃ�)
BOOL LoadMapData2( char *path_map )
{
	FILE *fp;
	char check[3];
	char path[MAX_PATH];
	sprintf( path, "%s\\%s", gDataPath, path_map );

	fp = fopen( path, "rb" );
	if( !fp )return FALSE;
	fread( check, sizeof(unsigned char), 3, fp );//CODE(3)
	if( memcmp(  check, code_pxma, 3 ) ){
		//error : code
		fclose( fp );
		return FALSE;
	}

	unsigned char dum;
	fread( &dum, sizeof(unsigned char), 1, fp );//parts size
	fread( &gMap.width, sizeof(short), 1, fp );//width
	fread( &gMap.length, sizeof(short), 1, fp );//length

	if( !gMap.data ){
		//error : allocate memory
		fclose( fp );
		return FALSE;
	}
	//�}�b�v�C���[�W�̃��[�h
	fread( gMap.data, sizeof(unsigned char), gMap.width * gMap.length, fp );//data
	fclose( fp );
	
		
	return TRUE;
}


//�}�b�v�A�g���r���[�g
BOOL LoadAttributeData( char *path_atrb )
{
	FILE *fp;
	char path[MAX_PATH];
	sprintf( path, "%s\\%s", gDataPath, path_atrb );
	fp = fopen( path, "rb" );
	if( !fp ) return FALSE;
	fread( gMap.atrb, sizeof(char), 256, fp );
	fclose( fp );
	return TRUE;
}

//�}�b�v�f�[�^�̊J��(�Q�[���I�����ɂ̂݊J��)
void EndMapData( void )
{
	free( gMap.data );
}

void ReleasePartsImage( void )
{
	ReleaseSurface( SURF_PARTS );
}


//�}�b�v�̏��
void GetMapData( unsigned char **data, short *mw, short *ml )
{
	if( data )*data = gMap.data;
	if( mw )*mw = gMap.width;
	if( ml )*ml = gMap.length;
}

//�w��̃p�[�c�̑�����Ԃ�
unsigned char GetAttribute( long x, long y )
{
	long a;

	if( x < 0 || y < 0 || x >= gMap.width || y >= gMap.length )
		return ATRB_DISABLE;

	//�p�[�c
	a = *( gMap.data + x + ( y * gMap.width ) );
	return gMap.atrb[a];
}

//�w��̃p�[�c������
void DeleteMapParts( long x, long y )
{
//	long a;
	//�p�[�c
	*( gMap.data + x + ( y * gMap.width ) ) = 0;
}
//�w��̃p�[�c���V�t�g�i�~�T�C���ł̔j��j
void ShiftMapParts( long x, long y )
{
	//�p�[�c
	*( gMap.data + x + ( y * gMap.width ) ) -= 1;
}
//�w��̃p�[�c��ύX
BOOL ChangeMapParts( long x, long y, unsigned char no )
{
	//�p�[�c
	if( *( gMap.data + x + ( y * gMap.width ) ) == no ) return FALSE;

	*( gMap.data + x + ( y * gMap.width ) ) = no;
	for( long i = 0; i < 3; i++ )
		SetNpChar( 4, x*VS*PARTSSIZE, y*VS*PARTSSIZE, 0, 0, DIR_LEFT,  NULL, 0 );
	return TRUE;
}

//�}�b�v�\��(��)
void PutStage_Back( long fx, long fy )
{
	int i,j;
	RECT rect;
	long offset;
	long num_x;
	long num_y;
	long put_x;
	long put_y;
	//���[�v��
	num_x = SURFACE_WIDTH / PARTSSIZE + 1;
	num_y = SURFACE_HEIGHT / PARTSSIZE + 1;
	//�z�u�X�^�[�g
	put_x = (fx/VS + PARTSSIZE/2) / PARTSSIZE;
	put_y = (fy/VS + PARTSSIZE/2) / PARTSSIZE;

	long atrb;

	for( j = put_y; j < put_y + num_y; j++ ){
		for( i = put_x; i < put_x + num_x; i++ ){

			offset = j * gMap.width + i;//�I�t�Z�b�g

			atrb = GetAttribute( i, j );

			//���ɕ\�����Ȃ����̂��Ȃ�
			if( atrb >= 0x20 )continue;

			//RECT����
			rect.left = PARTSSIZE * ( *(gMap.data+offset)%16 );
			rect.top = PARTSSIZE * ( *(gMap.data+offset)/16 );
			rect.right = rect.left + PARTSSIZE;
			rect.bottom = rect.top + PARTSSIZE;
			//�\��
			PutBitmap3( &grcGame,
				(i * PARTSSIZE - PARTSSIZE/2) - fx/VS,
				(j * PARTSSIZE - PARTSSIZE/2) - fy/VS,
				&rect, SURF_PARTS);
		}
	}
}


//�}�b�v�\��(��O)
void PutStage_Front( long fx, long fy )
{
	RECT rcSnack = {256, 48, 272, 64};
	int i,j;
	RECT rect;
	long offset;
	long num_x;
	long num_y;
	long put_x;
	long put_y;
	//���[�v��
	num_x = SURFACE_WIDTH / PARTSSIZE + 1;
	num_y = SURFACE_HEIGHT / PARTSSIZE + 1;
	//�z�u�X�^�[�g
	put_x = (fx/VS + PARTSSIZE/2) / PARTSSIZE;
	put_y = (fy/VS + PARTSSIZE/2) / PARTSSIZE;

	long atrb;

	for( j = put_y; j < put_y + num_y; j++ ){
		for( i = put_x; i < put_x + num_x; i++ ){

			offset = j * gMap.width + i;//�I�t�Z�b�g

			atrb = GetAttribute( i, j );

			//��O�ɕ\�����Ȃ����̂͏Ȃ�

			if( atrb < 0x40 || atrb >= 0x80)continue;


			//RECT����
			rect.left = PARTSSIZE * ( *(gMap.data+offset)%16 );
			rect.top = PARTSSIZE * ( *(gMap.data+offset)/16 );
			rect.right = rect.left + PARTSSIZE;
			rect.bottom = rect.top + PARTSSIZE;
			//�\��
			PutBitmap3( &grcGame,
				(i * PARTSSIZE - PARTSSIZE/2) - fx/VS,
				(j * PARTSSIZE - PARTSSIZE/2) - fy/VS,
				&rect, SURF_PARTS);
			if( atrb == ATRB_SNACK ){
				PutBitmap3( &grcGame,
					(i * PARTSSIZE - PARTSSIZE/2) - fx/VS,
					(j * PARTSSIZE - PARTSSIZE/2) - fy/VS,
					&rcSnack, SURF_NPC_SYMBOL);
			}
		}
	}
}

//�}�b�v�\��(�ړ�)
void PutMapDataVector( long fx, long fy )
{
	int i,j;
	RECT rect;
	long offset;
	long num_x;
	long num_y;
	long put_x;
	long put_y;

	static unsigned char count = 0;//�X�N���[���A�j���[�V����
	count+=2;

	//���[�v��
	num_x = SURFACE_WIDTH  / PARTSSIZE + 1;
	num_y = SURFACE_HEIGHT / PARTSSIZE + 1;
	//�z�u�X�^�[�g
	put_x = (fx/VS + PARTSSIZE/2) / PARTSSIZE;
	put_y = (fy/VS + PARTSSIZE/2) / PARTSSIZE;

	long atrb;

	for( j = put_y; j < put_y + num_y; j++ ){
		for( i = put_x; i < put_x + num_x; i++ ){

			//ATTRIBUTE�̎擾
			offset = j * gMap.width + i;//�I�t�Z�b�g
			atrb = GetAttribute( i, j );

			//�㉺���E�ȊO�͏Ȃ�
			if( atrb != ATRB_LEFT &&
				atrb != ATRB_UP &&
				atrb != ATRB_RIGHT &&
				atrb != ATRB_DOWN &&
				atrb != ATRB_LEFT_W &&
				atrb != ATRB_UP_W &&
				atrb != ATRB_RIGHT_W &&
				atrb != ATRB_DOWN_W )continue;

			//RECT����
			switch( atrb ){
			case ATRB_LEFT://��
			case ATRB_LEFT_W://��
				rect.left   = 224 + count%16;
				rect.right  = rect.left + PARTSSIZE;
				rect.top    = 48;//����͒��l
				rect.bottom = rect.top  + PARTSSIZE;
				break;
			case ATRB_UP://��
			case ATRB_UP_W://��
				rect.left   = 224;
				rect.right  = rect.left + PARTSSIZE;
				rect.top    = 48 + count%16;
				rect.bottom = rect.top  + PARTSSIZE;
				break;
			case ATRB_RIGHT://��
			case ATRB_RIGHT_W://��
				rect.left   = 224 + 16 - count%16;
				rect.right  = rect.left + PARTSSIZE;
				rect.top    = 48;
				rect.bottom = rect.top  + PARTSSIZE;
				break;
			case ATRB_DOWN://��
			case ATRB_DOWN_W://��
				rect.left   = 224;
				rect.right  = rect.left + PARTSSIZE;
				rect.top    = 48 + 16 - count%16;
				rect.bottom = rect.top  + PARTSSIZE;
				break;
			}
			//�\��
			PutBitmap3( &grcGame,
				(i * PARTSSIZE - PARTSSIZE/2) - fx/VS,
				(j * PARTSSIZE - PARTSSIZE/2) - fy/VS,
				&rect, SURF_CARET);
		}
	}
}


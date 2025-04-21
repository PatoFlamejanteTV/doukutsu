#include <windows.h>
#include <stdio.h>
#include <math.h>

#include "PixTone.h"

#define TOPVOLUME	64
#define TABLESIZE	256
#define WAVEWIDTH	256

static char gWaveModelTable[6][TABLESIZE];
static char gEnvelope[TABLESIZE];

void MakeWaveTables( void )
{
	long i;

	long a;
	//�T�C���g------------------------------------------------
	for( i = 0; i < TABLESIZE; i++ ){
		gWaveModelTable[WAVEMODEL_SIN][i] = (char)(sin((double)3.141592*2*i/TABLESIZE)*64);
		a = gWaveModelTable[WAVEMODEL_SIN][i];
	}
	//�O�p�g--------------------------------------------------
	//���
	a = 0;
	for( i = 0; i < 64; i++ ){
		gWaveModelTable[WAVEMODEL_TRIANGLE][i] = (char)( TOPVOLUME*a/64 );
		a++;
	}
	//����
	a = 0;
	for(      ; i < 64*3; i++ ){
		gWaveModelTable[WAVEMODEL_TRIANGLE][i] = (char)( 64 - TOPVOLUME*a/64 );
		a++;
	}
	//���
	a = 0;
	for(      ; i < 64*4; i++ ){
		gWaveModelTable[WAVEMODEL_TRIANGLE][i] = (char)( -64 + TOPVOLUME*a/64 );
		a++;
	}

	//�̂����聪�g---------------------------------------------
	for( i = 0; i < TABLESIZE; i++ ){
		gWaveModelTable[WAVEMODEL_SAWUP][i]   = (char)( -64+(i/2) );
	}
	//�̂����聫�g---------------------------------------------
	for( i = 0; i < TABLESIZE; i++ ){
		gWaveModelTable[WAVEMODEL_SAWDOWN][i] = (char)( +64-(i/2) );
	}
	//��`�g---------------------------------------------------
	for( i = 0; i < TABLESIZE/2; i++ ){
		gWaveModelTable[WAVEMODEL_RECTANGLE][i] =  TOPVOLUME;
	}
	for(      ; i < TABLESIZE; i++ ){
		gWaveModelTable[WAVEMODEL_RECTANGLE][i] = -TOPVOLUME;
	}
	//�����_��-------------------------------------------------
	srand( 0 );
	for( i = 0; i < TABLESIZE; i++ ){
		gWaveModelTable[WAVEMODEL_RANDOM][i] = (char)( (0x00ff&rand()) )/2;
	}

	
}












//BOOL PrintBinaryGraph( HWND hWnd, unsigned char *pData, long size );



BOOL MakePixelWaveData( PIXTONEPARAMETER *ptp, unsigned char *pData )
{



	long i;
	long a,b,c,d;

	double dPitch;
	double dMain;
	double dVolume;

	double dEnvelope;
	char envelopeTable[TABLESIZE];

	//�t���l
	double d1,d2,d3;

	//�g�`�e�[�u���𐶐�
	MakeWaveTables( );


	//�G���x���[�v�e�[�u�����쐬
	memset( envelopeTable, 0, TABLESIZE );
	i = 0;
	dEnvelope = (double)ptp->initial;
	while( i < ptp->pointAx ){
		envelopeTable[i] = (char)dEnvelope;
		dEnvelope += ((double)ptp->pointAy - ptp->initial)/ptp->pointAx;
		i++;
	}
	dEnvelope = (double)ptp->pointAy;
	while( i < ptp->pointBx ){
		envelopeTable[i] = (char)dEnvelope;
		dEnvelope += ((double)ptp->pointBy - ptp->pointAy)/(ptp->pointBx - ptp->pointAx);
		i++;
	}
	dEnvelope = (double)ptp->pointBy;
	while( i < ptp->pointCx ){
		envelopeTable[i] = (char)dEnvelope;
		dEnvelope += ((double)ptp->pointCy - ptp->pointBy)/(ptp->pointCx - ptp->pointBx);
		i++;
	}
	dEnvelope = (double)ptp->pointCy;
	while( i < TABLESIZE ){
		envelopeTable[i] = (char)dEnvelope;
		dEnvelope -= (double)(ptp->pointCy)/(TABLESIZE - ptp->pointCx);
		i++;
	}


	//�����e�[�u���C���f�b�N�X
	dPitch  = (double)ptp->oPitch.offset;
	dMain   = (double)ptp->oMain.offset;
	dVolume = (double)ptp->oVolume.offset;

	//�t���l�𐶐�
	if( ptp->oMain.num   == 0 ) d1 = 0;
	else                        d1 = TABLESIZE / ( (double)ptp->size / ptp->oMain.num   );
	if( ptp->oPitch.num  == 0 ) d2 = 0;
	else                        d2 = TABLESIZE / ( (double)ptp->size / ptp->oPitch.num  );
	if( ptp->oVolume.num == 0 ) d3 = 0;
	else                        d3 = TABLESIZE / ( (double)ptp->size / ptp->oVolume.num );

//	double xxx;
	for( i = 0; i < ptp->size; i++ ){
		//�e�[�u���C���f�b�N�X
		a = ((long)( dMain   ))%TABLESIZE; 
		b = ((long)( dPitch  ))%TABLESIZE;
		c = ((long)( dVolume ))%TABLESIZE; 
		d = (long)( TABLESIZE * i / (double)ptp->size );

		//���C���ƃ{�����[��
		pData[i] = 
			( unsigned char )( 128 +
				(
					( gWaveModelTable[ptp->oMain.model][a] * ptp->oMain.top / TOPVOLUME ) *
					( TOPVOLUME + gWaveModelTable[ptp->oVolume.model][c] * ptp->oVolume.top / TOPVOLUME ) /TOPVOLUME
				) *  envelopeTable[d] /TOPVOLUME
			);

//		xxx = envelopeTable[d] /TOPVOLUME;
		//���C���̃C���N�������g
		if( gWaveModelTable[ptp->oPitch.model][b] < 0 ){
			dMain   += ( d1 -( d1 * 0.50f * -gWaveModelTable[ptp->oPitch.model][b]*ptp->oPitch.top/TOPVOLUME /TOPVOLUME ) );
		}else{
			dMain   += ( d1 +( d1 *   2   *  gWaveModelTable[ptp->oPitch.model][b]*ptp->oPitch.top/TOPVOLUME /TOPVOLUME ) );
		}
		//�s�b�`�̃C���N�������g
		dPitch    += d2;
		//�{�����[���̃C���N�������g
		dVolume   += d3;
	}

	//�\��
//	PrintBinaryGraph( hWnd, data, DATASIZE ); 
//	PrintBinaryGraph( hWnd, (unsigned char *)envelopeTable, TABLESIZE ); 
	return TRUE;

}



/*


BOOL MakePixelWaveFile( char *path, unsigned long block_num, unsigned char *pData )
{

  	char *code_RIFF = "RIFF";
	char *code_fmt_ = "fmt ";
	char *code_WAVE = "WAVE";
	char *code_data = "data";

	WAVEHEADER wh;
	FILE *fp;

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
	wh.data_size       = wh.block_size * block_num;
	wh.next_size       = 36 + wh.data_size;


	fp = fopen( path, "wb" );
	if( !fp )return FALSE;

	fwrite( &wh, sizeof(WAVEHEADER), 1, fp );
	fwrite( pData, wh.block_size, block_num, fp );
	
	fclose( fp );
	return TRUE;

}
*/

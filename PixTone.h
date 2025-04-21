
#define WAVEMODEL_SIN		0
#define WAVEMODEL_TRIANGLE	1
#define WAVEMODEL_SAWUP		2
#define WAVEMODEL_SAWDOWN	3
#define WAVEMODEL_RECTANGLE	4
#define WAVEMODEL_RANDOM	5


typedef struct{

	unsigned char  code_RIFF[4];    //"RIFF"
	unsigned long  next_size;	    //filesize ( 36 + data_size )
	unsigned char  code_WAVE[4];    //"WAVE"
	unsigned char  code_fmt_[4];    //"fmt "
	unsigned long  fmt_byte;        //0x10
	unsigned short format;          //0x01PCM(linear)
	unsigned short channel_num;     //1:monaural / 2:stereo
	unsigned long  sampling_rate;   //44100/22050
	unsigned long  byte_per_second; //44100stereo16bit : 44100*2*2
	unsigned short block_size;      //stereo16bit : 2*2
	unsigned short bit_per_sample;  //16bit : 16

	unsigned char  code_data[4];    //"data"
	unsigned long  data_size;       //

}WAVEHEADER;


typedef struct{
	long model;
	double num;
	long top;
	long offset;
}PIXOSCILLATOR;

typedef struct{
	long use;
	long size;
	PIXOSCILLATOR oMain;
	PIXOSCILLATOR oPitch;
	PIXOSCILLATOR oVolume;


	long initial;
	long pointAx;
	long pointAy;
	long pointBx;
	long pointBy;
	long pointCx;
	long pointCy;

}PIXTONEPARAMETER;

//BOOL MakePixelWaveFile( char *path, unsigned long block_freq, unsigned char *pData );
BOOL MakePixelWaveData( PIXTONEPARAMETER *ptp, unsigned char *pData );

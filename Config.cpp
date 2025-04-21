#include <windows.h>
#include <stdio.h>

#include "resource.h"
#include "Config.h"

extern char gModulePath[];

static char *gProof      = "DOUKUTSU20041206";
static char *gConfigName = "Config.dat";

// コンフィグをロード
BOOL LoadConfigData( CONFIGDATA *conf )
{
	FILE *fp;
	char path[MAX_PATH];
	long read_size;

	memset( conf, 0, sizeof(CONFIGDATA) );
	sprintf( path, "%s\\%s", gModulePath, gConfigName );

	fp = fopen( path, "rb" );
	if( !fp )return FALSE;
	read_size = fread( conf, sizeof(CONFIGDATA), 1, fp );
	fclose( fp );

	if( read_size != 1 ||
		strcmp( conf->proof, gProof ) ){
		memset( conf, 0, sizeof(CONFIGDATA) );
		return FALSE;
	}

	return TRUE;
}

void DefaultConfigData( CONFIGDATA *conf )
{
	memset( conf, 0, sizeof(CONFIGDATA) );

	conf->bJoystick = TRUE;
	conf->joystick_button[0] = BUTTONACTION_SHOT;
	conf->joystick_button[1] = BUTTONACTION_JUMP;
	conf->joystick_button[2] = BUTTONACTION_MAP;
	conf->joystick_button[3] = BUTTONACTION_ARMSREV;
	conf->joystick_button[4] = BUTTONACTION_ARMS;
	conf->joystick_button[5] = BUTTONACTION_ITEM;
	conf->joystick_button[6] = BUTTONACTION_ARMSREV;
	conf->joystick_button[7] = BUTTONACTION_ARMS;

}

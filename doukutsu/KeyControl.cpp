#include <windows.h>
#include <stdio.h>

#include "KeyControl.h"

long gKey    = 0;
long gKeyTrg = 0;

long gKeyJump    = KEY_Z;
long gKeyShot    = KEY_X;
long gKeyArms    = KEY_S;
long gKeyArmsRev = KEY_A;
long gKeyItem    = KEY_Q;
long gKeyMap     = KEY_W;

long gKeyOk      = KEY_Z;
long gKeyCancel  = KEY_X;

long gKeyLeft    = KEY_LEFT;
long gKeyUp      = KEY_UP;
long gKeyRight   = KEY_RIGHT;
long gKeyDown    = KEY_DOWN;

//ÉgÉäÉKÇÃê∂ê¨
void GetTrg(void)		//âüÇµÇΩèuä‘(ÉgÉäÉKÅ[)ÇÃéÊìæ
{
	static long _key_old   = 0;
//	static long _click_old = 0;
	gKeyTrg = gKey ^ _key_old;
	gKeyTrg = gKey & gKeyTrg;
	_key_old = gKey;
//	gClickTrg = gClickTrg ^ _click_old;
//	gClickTrg = gClickTrg & gClickTrg;
//	_click_old = gClickTrg;
}

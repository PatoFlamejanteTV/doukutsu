#include <windows.h>
#include "Draw.h"
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "KeyControl.h"

extern MYCHAR gMC;
//extern long gKeyTrg;
//extern long gKey;
//extern long gKeyShot;


long CountArmsBullet(  long arms_code );
void DeleteBullet( long code );
void SetBullet(    long no, long x, long y, long dir );

void SetCaret( long x, long y, long code, long dir );
void AddExpMyChar( long x );
void ZeroExpMyChar( void );
BOOL IsMaxExpMyChar( void );
BOOL UseArmsEnergy( long a );

extern ARMSDATA gArmsData[];
extern long gSelectedArms;
void ChangeToFirstArms( void );

static long _empty = 0;


//スネーク
void ShootBullet_Frontia1( long level )
{
	long bul_no;

	switch( level ){
	case 1: bul_no = BULLET_FRONTIA1; break;
	case 2: bul_no = BULLET_FRONTIA2; break;
	case 3: bul_no = BULLET_FRONTIA3; break;
	}

	if( CountArmsBullet( 1 ) > 3 )return;
	if( gKeyTrg & gKeyShot ){
		if( UseArmsEnergy( 1 ) == FALSE ){
			ChangeToFirstArms();
			return;
		}
		if( gMC.up ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 3*VS, gMC.y - 10*VS, DIR_UP );
				SetCaret(     gMC.x - 3*VS, gMC.y - 10*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x + 3*VS, gMC.y - 10*VS, DIR_UP );
				SetCaret(     gMC.x + 3*VS, gMC.y - 10*VS, CARET_FLASH, 0 );
			}
		}else if( gMC.down ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 3*VS, gMC.y + 10*VS, DIR_DOWN );
				SetCaret(     gMC.x - 3*VS, gMC.y + 10*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x + 3*VS, gMC.y + 10*VS, DIR_DOWN );
				SetCaret(     gMC.x + 3*VS, gMC.y + 10*VS, CARET_FLASH, 0 );
			}
		}else{
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 6*VS, gMC.y + 2*VS, DIR_LEFT );
				SetCaret( gMC.x - 12*VS, gMC.y + 2*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x + 6*VS, gMC.y + 2*VS, DIR_RIGHT );
				SetCaret( gMC.x + 12*VS, gMC.y + 2*VS, CARET_FLASH, 0 );
			}
		}
		PlaySoundObject( WAVE_FRONTIA, 1 );
	}
}

//PoleStar
void ShootBullet_PoleStar( long level )
{
	long bul_no;

	switch( level ){
	case 1: bul_no = BULLET_POLESTAR1; break;
	case 2: bul_no = BULLET_POLESTAR2; break;
	case 3: bul_no = BULLET_POLESTAR3; break;
	}

	if( CountArmsBullet( 2 ) > 1 )return;
	if( gKeyTrg & gKeyShot ){
		if( UseArmsEnergy( 1 ) == FALSE ){
			PlaySoundObject( WAVE_ARMS_EMPTY, 1 );
			return;
		}
		if( gMC.up ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 1*VS, gMC.y - 8*VS, DIR_UP );
				SetCaret(          gMC.x - 1*VS, gMC.y - 8*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x + 1*VS, gMC.y - 8*VS, DIR_UP );
				SetCaret(          gMC.x + 1*VS, gMC.y - 8*VS, CARET_FLASH, 0 );
			}
		}else if( gMC.down ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 1*VS, gMC.y + 8*VS, DIR_DOWN );
				SetCaret(          gMC.x - 1*VS, gMC.y + 8*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x + 1*VS, gMC.y + 8*VS, DIR_DOWN );
				SetCaret(          gMC.x + 1*VS, gMC.y + 8*VS, CARET_FLASH, 0 );
			}
		}else{
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x -  6*VS, gMC.y + 3*VS, DIR_LEFT );
				SetCaret(          gMC.x - 12*VS, gMC.y + 3*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x +  6*VS, gMC.y + 3*VS, DIR_RIGHT );
				SetCaret(          gMC.x + 12*VS, gMC.y + 3*VS, CARET_FLASH, 0 );
			}
		}
		if( level == 3 ) PlaySoundObject( WAVE_POLEST2,  1 );
		else             PlaySoundObject( WAVE_POLESTAR, 1 );
	}
}

//FireBall
void ShootBullet_FireBall( long level )
{
	long bul_no;

	switch( level ){
	case 1:
		if( CountArmsBullet( 3 ) > 1 )return;
		bul_no = BULLET_FIREBALL1;
		break;
	case 2:
		if( CountArmsBullet( 3 ) > 2 )return;
		bul_no = BULLET_FIREBALL2;
		break;
	case 3:
		if( CountArmsBullet( 3 ) > 3 )return;
		bul_no = BULLET_FIREBALL3;
		break;
	}

	if( gKeyTrg & gKeyShot ){
		if( UseArmsEnergy( 1 ) == FALSE ){
			ChangeToFirstArms();
			return;
		}
		if( gMC.up ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x -4*VS, gMC.y - 8*VS, DIR_UP );
				SetCaret(          gMC.x -4*VS, gMC.y - 8*VS,  CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x +4*VS, gMC.y - 8*VS, DIR_UP );
				SetCaret(          gMC.x +4*VS, gMC.y - 8*VS,  CARET_FLASH, 0 );
			}
		}else if( gMC.down ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x -4*VS, gMC.y + 8*VS, DIR_DOWN );
				SetCaret(          gMC.x -4*VS, gMC.y + 8*VS,  CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x +4*VS, gMC.y + 8*VS, DIR_DOWN );
				SetCaret(          gMC.x +4*VS, gMC.y + 8*VS,  CARET_FLASH, 0 );
			}
		}else{
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 6*VS,  gMC.y + 2*VS, DIR_LEFT );
				SetCaret(          gMC.x - 12*VS, gMC.y + 2*VS,  CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x + 6*VS,  gMC.y + 2*VS, DIR_RIGHT );
				SetCaret(          gMC.x + 12*VS, gMC.y + 2*VS,  CARET_FLASH, 0 );
			}
		}
		PlaySoundObject( WAVE_FIRE, 1 );
	}
}
// Machinegun
BOOL ChargeArmsEnergy( long num );
void ShootBullet_Machinegun1( long level )
{
	long bul_no;
	static long wait = 0;
	if( CountArmsBullet( 4 ) > 4 )return;

	switch( level ){
	case 1: bul_no = BULLET_MACHINEGUN1; break;
	case 2: bul_no = BULLET_MACHINEGUN2; break;
	case 3: bul_no = BULLET_MACHINEGUN3; break;
	}


	if( !(gKey & gKeyShot) )gMC.rensha = 6;
	if( gKey & gKeyShot ){
		gMC.rensha++;
		if( gMC.rensha < 6 )return;
		gMC.rensha = 0;
		
		if( UseArmsEnergy( 1 ) == FALSE ){
			PlaySoundObject( WAVE_ARMS_EMPTY, 1 );
			if( !_empty ){
				SetCaret( gMC.x, gMC.y, CARET_EMPTY, DIR_LEFT );
				_empty = 50;
			}
			return;
		}
		if( gMC.up ){
			if( level == 3 )gMC.ym += VS/2;
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x -3*VS, gMC.y - 8*VS, DIR_UP );
				SetCaret(          gMC.x -3*VS, gMC.y - 8*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x +3*VS, gMC.y - 8*VS, DIR_UP );
				SetCaret(          gMC.x +3*VS, gMC.y - 8*VS, CARET_FLASH, 0 );
			}
		}else if( gMC.down ){
			if( level == 3 ){
				if( gMC.ym > 0 ) gMC.ym = gMC.ym / 2;
				if( gMC.ym > -VS*2 ){
					gMC.ym -= VS;
					if( gMC.ym < -2*VS ) gMC.ym = -2*VS;
//					if( gMC.ym < -2*VS ) gMC.ym = gMC.ym/2;//-2*VS;
				}
			}
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x -3*VS, gMC.y + 8*VS, DIR_DOWN );
				SetCaret(          gMC.x -3*VS, gMC.y + 8*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x +3*VS, gMC.y + 8*VS, DIR_DOWN );
				SetCaret(          gMC.x +3*VS, gMC.y + 8*VS, CARET_FLASH, 0 );
			}
		}else{
			if( gMC.direct == DIR_LEFT ){
//				if( level == 3 )gMC.xm += VS/2;
				SetBullet( bul_no, gMC.x - 12*VS, gMC.y + 3*VS, DIR_LEFT );
				SetCaret(          gMC.x - 12*VS, gMC.y + 3*VS, CARET_FLASH, 0 );
			}else{
//				if( level == 3 )gMC.xm -= VS/2;
				SetBullet( bul_no, gMC.x + 12*VS, gMC.y + 3*VS, DIR_RIGHT );
				SetCaret(          gMC.x + 12*VS, gMC.y + 3*VS, CARET_FLASH, 0 );
			}
		}
		if( level == 3 ) PlaySoundObject( WAVE_POLEST2,  1 );
		else             PlaySoundObject( WAVE_POLESTAR, 1 );
	}else{
		wait++;
		if( gMC.equip & EQUIP_TURBOCHARGE ){
			if( wait > 1 ){
				wait = 0;
				ChargeArmsEnergy( 1 );
			}
		}else{
			if( wait > 4 ){
				wait = 0;
				ChargeArmsEnergy( 1 );
			}
		}
	}
}

// Missile
void ShootBullet_Missile( long level, BOOL bSuper )
{
	long bul_no;

	if( bSuper ){
		switch( level ){
		case 1: bul_no = BULLET_SUPERMISSILE1; break;
		case 2: bul_no = BULLET_SUPERMISSILE2; break;
		case 3: bul_no = BULLET_SUPERMISSILE3; break;
		}
		switch( level ){
		case 1:
			if( CountArmsBullet( 10 ) > 0 )return;
			if( CountArmsBullet( 11 ) > 0 )return;
			break;
		case 2:
			if( CountArmsBullet( 10 ) > 1 )return;
			if( CountArmsBullet( 11 ) > 1 )return;
			break;
		case 3:
			if( CountArmsBullet( 10 ) > 3 )return;
			if( CountArmsBullet( 11 ) > 3 )return;
			break;
		}
	}else{
		switch( level ){
		case 1: bul_no = BULLET_MISSILE1; break;
		case 2: bul_no = BULLET_MISSILE2; break;
		case 3: bul_no = BULLET_MISSILE3; break;
		}
		switch( level ){
		case 1:
			if( CountArmsBullet( 5 ) > 0 )return;
			if( CountArmsBullet( 6 ) > 0 )return;
			break;
		case 2:
			if( CountArmsBullet( 5 ) > 1 )return;
			if( CountArmsBullet( 6 ) > 1 )return;
			break;
		case 3:
			if( CountArmsBullet( 5 ) > 3 )return;
			if( CountArmsBullet( 6 ) > 3 )return;
			break;
		}
	}


	

	if( gKeyTrg & gKeyShot ){
		if( level < 3 ){
			if( UseArmsEnergy( 1 ) == FALSE ){
//				ChangeToFirstArms();
				PlaySoundObject( WAVE_ARMS_EMPTY, 1 );
				if( !_empty ){
					SetCaret( gMC.x, gMC.y, CARET_EMPTY, DIR_LEFT );
					_empty = 50;
				}

				return;
			}
			if( gMC.up ){
				if( gMC.direct == DIR_LEFT ){
					SetBullet( bul_no, gMC.x - 1*VS, gMC.y - 8*VS, DIR_UP );
					SetCaret(          gMC.x - 1*VS, gMC.y - 8*VS, CARET_FLASH, 0 );
				}else{
					SetBullet( bul_no, gMC.x + 1*VS, gMC.y - 8*VS, DIR_UP );
					SetCaret(          gMC.x + 1*VS, gMC.y - 8*VS, CARET_FLASH, 0 );
				}
			}else if( gMC.down ){
				if( gMC.direct == DIR_LEFT ){
					SetBullet( bul_no, gMC.x - 1*VS, gMC.y + 8*VS, DIR_DOWN );
					SetCaret(          gMC.x - 1*VS, gMC.y + 8*VS, CARET_FLASH, 0 );
				}else{
					SetBullet( bul_no, gMC.x + 1*VS, gMC.y + 8*VS, DIR_DOWN );
					SetCaret(          gMC.x + 1*VS, gMC.y + 8*VS, CARET_FLASH, 0 );
				}
			}else{
				if( gMC.direct == DIR_LEFT ){
					SetBullet( bul_no, gMC.x -  6*VS, gMC.y + 0*VS, DIR_LEFT );
					SetCaret(          gMC.x - 12*VS, gMC.y + 0*VS, CARET_FLASH, 0 );
				}else{
					SetBullet( bul_no, gMC.x +  6*VS, gMC.y + 0*VS, DIR_RIGHT );
					SetCaret(          gMC.x + 12*VS, gMC.y + 0*VS, CARET_FLASH, 0 );
				}
			}
		}else{
			if( UseArmsEnergy( 1 ) == FALSE ){
//				ChangeToFirstArms();
//				PlaySoundObject( WAVE_EMPTY, 1 );
				PlaySoundObject( WAVE_ARMS_EMPTY, 1 );
				if( !_empty ){
					SetCaret( gMC.x, gMC.y, CARET_EMPTY, DIR_LEFT );
					_empty = 50;
				}
				return;
			}
			if( gMC.up ){
				if( gMC.direct == DIR_LEFT ){
					SetBullet( bul_no, gMC.x - 1*VS, gMC.y - 8*VS, DIR_UP );
					SetCaret(          gMC.x - 1*VS, gMC.y - 8*VS, CARET_FLASH, 0 );
					SetBullet( bul_no, gMC.x + 3*VS, gMC.y - 0*VS, DIR_UP );
					SetBullet( bul_no, gMC.x - 3*VS, gMC.y - 0*VS, DIR_UP );
				}else{
					SetBullet( bul_no, gMC.x + 1*VS, gMC.y - 8*VS, DIR_UP );
					SetCaret(          gMC.x + 1*VS, gMC.y - 8*VS, CARET_FLASH, 0 );
					SetBullet( bul_no, gMC.x + 3*VS, gMC.y - 0*VS, DIR_UP );
					SetBullet( bul_no, gMC.x - 3*VS, gMC.y - 0*VS, DIR_UP );
				}
			}else if( gMC.down ){
				if( gMC.direct == DIR_LEFT ){
					SetBullet( bul_no, gMC.x - 1*VS, gMC.y + 8*VS, DIR_DOWN );
					SetCaret(          gMC.x - 1*VS, gMC.y + 8*VS, CARET_FLASH, 0 );
					SetBullet( bul_no, gMC.x + 3*VS, gMC.y + 0*VS, DIR_DOWN );
					SetBullet( bul_no, gMC.x - 3*VS, gMC.y + 0*VS, DIR_DOWN );
				}else{
					SetBullet( bul_no, gMC.x + 1*VS, gMC.y + 8*VS, DIR_DOWN );
					SetCaret(          gMC.x + 1*VS, gMC.y + 8*VS, CARET_FLASH, 0 );
					SetBullet( bul_no, gMC.x - 3*VS, gMC.y + 0*VS, DIR_DOWN );
					SetBullet( bul_no, gMC.x + 3*VS, gMC.y + 0*VS, DIR_DOWN );
				}
			}else{
				if( gMC.direct == DIR_LEFT ){
					SetBullet( bul_no, gMC.x -  6*VS, gMC.y + 1*VS, DIR_LEFT );
					SetCaret(          gMC.x - 12*VS, gMC.y + 1*VS, CARET_FLASH, 0 );
					SetBullet( bul_no, gMC.x -  0*VS, gMC.y - 8*VS, DIR_LEFT );
					SetBullet( bul_no, gMC.x +  4*VS, gMC.y - 1*VS, DIR_LEFT );
				}else{
					SetBullet( bul_no, gMC.x +  6*VS, gMC.y + 1*VS, DIR_RIGHT );
					SetCaret(          gMC.x + 12*VS, gMC.y + 1*VS, CARET_FLASH, 0 );
					SetBullet( bul_no, gMC.x +  0*VS, gMC.y - 8*VS, DIR_RIGHT );
					SetBullet( bul_no, gMC.x -  4*VS, gMC.y - 1*VS, DIR_RIGHT );
				}
			}
		}
		PlaySoundObject( WAVE_POLESTAR, 1 );
	}
}

// バブリン
void ShootBullet_Bubblin1( void )
{
	static long wait = 0;

	if( CountArmsBullet( 7 ) > 3 )return;
	if( gKeyTrg & gKeyShot ){
		if( UseArmsEnergy( 1 ) == FALSE ){
			PlaySoundObject( WAVE_ARMS_EMPTY, 1 );
			if( !_empty ){
				SetCaret( gMC.x, gMC.y, CARET_EMPTY, DIR_LEFT );
				_empty = 50;
			}
			return;
		}
		if( gMC.up ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( BULLET_BUBBLIN1, gMC.x - 1*VS, gMC.y - 2*VS, DIR_UP );
				SetCaret(                   gMC.x - 1*VS, gMC.y - 2*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( BULLET_BUBBLIN1, gMC.x + 1*VS, gMC.y - 2*VS, DIR_UP );
				SetCaret(                   gMC.x + 1*VS, gMC.y - 2*VS, CARET_FLASH, 0 );
			}
		}else if( gMC.down ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( BULLET_BUBBLIN1, gMC.x - 1*VS, gMC.y + 2*VS, DIR_DOWN );
				SetCaret(                   gMC.x - 1*VS, gMC.y + 2*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( BULLET_BUBBLIN1, gMC.x + 1*VS, gMC.y + 2*VS, DIR_DOWN );
				SetCaret(                   gMC.x + 1*VS, gMC.y + 2*VS, CARET_FLASH, 0 );
			}
		}else{
			if( gMC.direct == DIR_LEFT ){
				SetBullet( BULLET_BUBBLIN1, gMC.x -  6*VS, gMC.y + 3*VS, DIR_LEFT );
				SetCaret(                   gMC.x - 12*VS, gMC.y + 3*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( BULLET_BUBBLIN1, gMC.x +  6*VS, gMC.y + 3*VS, DIR_RIGHT );
				SetCaret(                   gMC.x + 12*VS, gMC.y + 3*VS, CARET_FLASH, 0 );
			}
		}
		PlaySoundObject( WAVE_BUBBLIN, 1 );
	}else{
		wait++;
		if( wait > 20 ){
			wait = 0;
			ChargeArmsEnergy( 1 );
		}
	}

}
void ShootBullet_Bubblin2( long level )
{
	static long wait = 0;

	if( CountArmsBullet( 7 ) > 15 )return;
	level += BULLET_BUBBLIN2 - 2;

	if( !(gKey & gKeyShot) )gMC.rensha = 6;
	if( gKey & gKeyShot ){
		gMC.rensha++;
		if( gMC.rensha < 7 )return;
		gMC.rensha = 0;
		
		if( UseArmsEnergy( 1 ) == FALSE ){
			PlaySoundObject( WAVE_ARMS_EMPTY, 1 );
			if( !_empty ){
				SetCaret( gMC.x, gMC.y, CARET_EMPTY, DIR_LEFT );
				_empty = 50;
			}
			return;
		}
		if( gMC.up ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( level, gMC.x -3*VS, gMC.y -  8*VS, DIR_UP );
				SetCaret(         gMC.x -3*VS, gMC.y - 16*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( level, gMC.x +3*VS, gMC.y -  8*VS, DIR_UP );
				SetCaret(         gMC.x +3*VS, gMC.y - 16*VS, CARET_FLASH, 0 );
			}
		}else if( gMC.down ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( level, gMC.x -3*VS, gMC.y +  8*VS, DIR_DOWN );
				SetCaret(         gMC.x -3*VS, gMC.y + 16*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( level, gMC.x +3*VS, gMC.y +  8*VS, DIR_DOWN );
				SetCaret(         gMC.x +3*VS, gMC.y + 16*VS, CARET_FLASH, 0 );
			}
		}else{
			if( gMC.direct == DIR_LEFT ){
				SetBullet( level, gMC.x -  6*VS, gMC.y + 3*VS, DIR_LEFT );
				SetCaret(         gMC.x - 12*VS, gMC.y + 3*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( level, gMC.x +  6*VS, gMC.y + 3*VS, DIR_RIGHT );
				SetCaret(         gMC.x + 12*VS, gMC.y + 3*VS, CARET_FLASH, 0 );
			}
		}
		PlaySoundObject( WAVE_BUBBLIN, 1 );
	}else{
		wait++;
		if( wait > 1 ){
			wait = 0;
			ChargeArmsEnergy( 1 );
		}
	}
}



//ブレード
void ShootBullet_Sword( long level )
{
	long bul_no;
	if( CountArmsBullet( 9 ) > 0 ){
		return;
	}
	switch( level ){
	case 1: bul_no = BULLET_SWORD1; break;
	case 2: bul_no = BULLET_SWORD2; break;
	case 3: bul_no = BULLET_SWORD3; break;
	}
	if( gKeyTrg & gKeyShot ){
//		DeleteBullet( 9 );
		if( gMC.up ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 1*VS, gMC.y + 4*VS, DIR_UP );
			}else{
				SetBullet( bul_no, gMC.x + 1*VS, gMC.y + 4*VS, DIR_UP );
			}
		}else if( gMC.down ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 1*VS, gMC.y - 6*VS, DIR_DOWN );
			}else{
				SetBullet( bul_no, gMC.x + 1*VS, gMC.y - 6*VS, DIR_DOWN );
			}
		}else{
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x + 6*VS, gMC.y - 3*VS, DIR_LEFT );
			}else{
				SetBullet( bul_no, gMC.x - 6*VS, gMC.y - 3*VS, DIR_RIGHT );
			}
		}
		PlaySoundObject( WAVE_FIRE,  1 );
		
	}
}

// Nemesis
void ShootBullet_Nemesis( long level )
{
	long bul_no;

	switch( level ){
	case 1: bul_no = BULLET_NEMESIS1; break;
	case 2: bul_no = BULLET_NEMESIS2; break;
	case 3: bul_no = BULLET_NEMESIS3; break;
	}

	if( CountArmsBullet( 12 ) > 1 ) return;
	if( gKeyTrg & gKeyShot ){
		if( UseArmsEnergy( 1 ) == FALSE ){
			PlaySoundObject( WAVE_ARMS_EMPTY, 1 );
			return;
		}
		if( gMC.up ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 1*VS, gMC.y - 12*VS, DIR_UP );
				SetCaret(          gMC.x - 1*VS, gMC.y - 12*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x + 1*VS, gMC.y - 12*VS, DIR_UP );
				SetCaret(          gMC.x + 1*VS, gMC.y - 12*VS, CARET_FLASH, 0 );
			}
		}else if( gMC.down ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 1*VS, gMC.y + 12*VS, DIR_DOWN );
				SetCaret(          gMC.x - 1*VS, gMC.y + 12*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x + 1*VS, gMC.y + 12*VS, DIR_DOWN );
				SetCaret(          gMC.x + 1*VS, gMC.y + 12*VS, CARET_FLASH, 0 );
			}
		}else{
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 22*VS, gMC.y + 3*VS, DIR_LEFT );
				SetCaret(          gMC.x - 16*VS, gMC.y + 3*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x + 22*VS, gMC.y + 3*VS, DIR_RIGHT );
				SetCaret(          gMC.x + 16*VS, gMC.y + 3*VS, CARET_FLASH, 0 );
			}
		}
		switch( level ){
		case 1:  PlaySoundObject( WAVE_NEMESIS,    1 ); break;
		case 2:  PlaySoundObject( WAVE_POLEST2,    1 ); break;
		case 3:  PlaySoundObject( WAVE_CHARGE_LV2, 1 ); break;
		}
	}
}


static long _spur_charge = 0;
void ResetSpurCharge( void ){
	_spur_charge = 0;
	if( gArmsData[gSelectedArms].code == 13 ) ZeroExpMyChar();
}
// Spur
void ShootBullet_Spur( long level )
{
	long        bul_no;
	BOOL        bShot   = FALSE;
	static BOOL _bMax   = FALSE;

	if( gKey & gKeyShot ){
		if( gMC.equip&EQUIP_TURBOCHARGE ) AddExpMyChar( 3 );
		else                              AddExpMyChar( 2 );
		_spur_charge++;
		if( _spur_charge/2%2 ){
			switch( level ){
			case 1: PlaySoundObject( WAVE_CHARGE_LV1, 1 ); break;
			case 2: PlaySoundObject( WAVE_CHARGE_LV2, 1 ); break;
			case 3:
				if( !IsMaxExpMyChar() ){
					PlaySoundObject( WAVE_CHARGE_LV3, 1 );
				}
				break;
			}
		}
	}else{
		if( _spur_charge ) bShot = TRUE;
		_spur_charge = 0;
	}

	if( IsMaxExpMyChar() ){
		if( !_bMax ){
			_bMax = TRUE;
			PlaySoundObject( WAVE_CHARGE_MAX, 1 );
		}
	}else{
		_bMax = FALSE;
	}

	if( !(gKey & gKeyShot) ) ZeroExpMyChar();

	switch( level ){
	case 1: bul_no = BULLET_POLESTAR3; bShot = FALSE;break;
	case 2: bul_no = BULLET_SPUR1;     break;
	case 3:
		if( _bMax ) bul_no = BULLET_SPUR3;
		else        bul_no = BULLET_SPUR2;
		break;
	}

	if( CountArmsBullet( 13 ) > 0 || CountArmsBullet( 14 ) > 0 )return;
	if( gKeyTrg & gKeyShot || bShot ){
		if( UseArmsEnergy( 1 ) == FALSE ){
			PlaySoundObject( WAVE_ARMS_EMPTY, 1 );
			return;
		}
		if( gMC.up ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 1*VS, gMC.y - 8*VS, DIR_UP );
				SetCaret(          gMC.x - 1*VS, gMC.y - 8*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x + 1*VS, gMC.y - 8*VS, DIR_UP );
				SetCaret(          gMC.x + 1*VS, gMC.y - 8*VS, CARET_FLASH, 0 );
			}
		}else if( gMC.down ){
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x - 1*VS, gMC.y + 8*VS, DIR_DOWN );
				SetCaret(          gMC.x - 1*VS, gMC.y + 8*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x + 1*VS, gMC.y + 8*VS, DIR_DOWN );
				SetCaret(          gMC.x + 1*VS, gMC.y + 8*VS, CARET_FLASH, 0 );
			}
		}else{
			if( gMC.direct == DIR_LEFT ){
				SetBullet( bul_no, gMC.x -  6*VS, gMC.y + 3*VS, DIR_LEFT );
				SetCaret(          gMC.x - 12*VS, gMC.y + 3*VS, CARET_FLASH, 0 );
			}else{
				SetBullet( bul_no, gMC.x +  6*VS, gMC.y + 3*VS, DIR_RIGHT );
				SetCaret(          gMC.x + 12*VS, gMC.y + 3*VS, CARET_FLASH, 0 );
			}
		}
		switch( bul_no ){
		case BULLET_POLESTAR3: PlaySoundObject( WAVE_POLEST2,  1 ); break;
		case BULLET_SPUR1:     PlaySoundObject( WAVE_SPUR_LV1, 1 ); break;
		case BULLET_SPUR2:     PlaySoundObject( WAVE_SPUR_LV2, 1 ); break;
		case BULLET_SPUR3:     PlaySoundObject( WAVE_SPUR_LV3, 1 ); break;
		}
	}
}

void ShootBullet( void )
{
	static long soft_rensha = 0;

	if( _empty ) _empty--;

	if( soft_rensha ){
		soft_rensha--;
	}
	if( gKeyTrg & gKeyShot ){
		if( soft_rensha ) return;
		soft_rensha = 4;//2;
	}

//	if( gMC.cond & COND_CHECK || gMC.cond & COND_HIDE )return;
	if(  gMC.cond & COND_HIDE )return;
	switch( gArmsData[gSelectedArms].code ){

	case 1: ShootBullet_Frontia1(    gArmsData[gSelectedArms].level        ); break;
	case 2: ShootBullet_PoleStar(    gArmsData[gSelectedArms].level        ); break;
	case 3: ShootBullet_FireBall(    gArmsData[gSelectedArms].level        ); break;
	case 4: ShootBullet_Machinegun1( gArmsData[gSelectedArms].level        ); break;
	case 5: ShootBullet_Missile(     gArmsData[gSelectedArms].level, FALSE ); break;
	case 6: break;
	case 7:// バブリン
		switch( gArmsData[gSelectedArms].level ){
		case 1:ShootBullet_Bubblin1( ); break;
		case 2:ShootBullet_Bubblin2(2); break;
		case 3:ShootBullet_Bubblin2(3); break;
		}
		break;
	case 9:// ブレード
		switch( gArmsData[gSelectedArms].level ){
		case 1:ShootBullet_Sword( 1 ); break;
		case 2:ShootBullet_Sword( 2 ); break;
		case 3:ShootBullet_Sword( 3 ); break;
		}
		break;
	case 10: ShootBullet_Missile( gArmsData[gSelectedArms].level, TRUE ); break;
	case 11: break;
	case 12: ShootBullet_Nemesis( gArmsData[gSelectedArms].level       ); break;
	case 13: ShootBullet_Spur(    gArmsData[gSelectedArms].level       ); break;
	}
}

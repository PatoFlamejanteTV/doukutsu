void ChangeAllNpCharByCode( long code1, long code2 );
BOOL ChangeMapParts( long x, long y, unsigned char no );
long CountArmsBullet( long arms_code );
long CountBulletNum( long bullet_code );
void DamageMyChar( long a );
void DeleteNpCharCode( long code, BOOL bSmoke );
unsigned char GetArktan( long x,long y );
int  GetCos( unsigned char deg );
BOOL GetNPCFlag( long a );
int  GetSin( unsigned char deg );
void LoseNpChar( NPCHAR *npc, BOOL bVanish );
long Random( int min, int max );
void SetBullet( long no, long x, long y, long dir );
void SetCaret(         long x, long y, long code, long dir );
void SetDestroyNpChar( long x, long y, long w, long num );
void SetExpObjects(    long x, long y, long exp );
void SetFlash(         long x, long y, long mode );
void SetNPCFlag( long a );
void SetNpChar( long code_char, long x, long y, long xm, long ym, long dir, NPCHAR *npc, long start_index );
void SetQuake(  long time );
void SetQuake2( long time );
void VanishNpChar( NPCHAR *npc );

void SetDestroyNpCharUp( long x, long y, long w, long num );

extern BACKDATA gBack;
extern MAPDATA  gMap;
extern MYCHAR   gMC;
extern NPCHAR   gNPC[];
extern NPCHAR   gBoss[];
/*
// ëÄçÏÉLÅ[
extern long gKeyJump;
extern long gKeyShot;
extern long gKeyArms;
extern long gKeyArmsRev;
extern long gKeyItem;
extern long gKeyMap;
extern long gKeyLeft;
extern long gKeyUp;
extern long gKeyRight;
extern long gKeyDown;
*/
extern long gKeyTrg;

extern long gCurlyShoot_wait;
extern long gCurlyShoot_x;
extern long gCurlyShoot_y;
extern long gSuperXpos;
extern long gSuperYpos;

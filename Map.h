//マップ情報の初期化
BOOL InitMapData2( void );
//マップ情報の初期化（Load）
BOOL LoadMapData2( char *path_map );
void EndMapData( void );
//パーツのロード（マップ情報を指定しておく必要がある）
void ReleasePartsImage( void );
//マップ表示
void PutStage_Back(    long fx, long fy );
void PutStage_Front(   long fx, long fy );
void PutMapDataVector( long fx, long fy );

BOOL LoadAttributeData( char *path_atrb );
unsigned char GetAttribute( long x, long y );
void DeleteMapParts( long x, long y );
void ShiftMapParts( long x, long y );

//�}�b�v���̏�����
BOOL InitMapData2( void );
//�}�b�v���̏������iLoad�j
BOOL LoadMapData2( char *path_map );
void EndMapData( void );
//�p�[�c�̃��[�h�i�}�b�v�����w�肵�Ă����K�v������j
void ReleasePartsImage( void );
//�}�b�v�\��
void PutStage_Back(    long fx, long fy );
void PutStage_Front(   long fx, long fy );
void PutMapDataVector( long fx, long fy );

BOOL LoadAttributeData( char *path_atrb );
unsigned char GetAttribute( long x, long y );
void DeleteMapParts( long x, long y );
void ShiftMapParts( long x, long y );

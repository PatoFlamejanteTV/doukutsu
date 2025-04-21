BOOL StartDirectDraw( HWND hWnd, long mode, long bit );
void EndDirectDraw(   HWND hWnd );
void SetClientOffset( long offset_x, long offset_y );
BOOL Flip_SystemTask( HWND hWnd );

BOOL MakeSurface_Resource(  char *name, long surf_no );
BOOL MakeSurface_File(      char *name, long surf_no );
BOOL MakeSurface_Generic( long bxsize, long bysize, long surf_no, BOOL bSystem );
BOOL ReloadBitmap_Resource( char *name, long surf_no );
BOOL ReloadBitmap_File(     char *name, long surf_no );
void ReleaseSurface(                    long surf_no );


//void PutBitmap(             long x, long y, RECT* rect, long surf_no );
void PutBitmap3( RECT *rcView,long x, long y, RECT* rect, long surf_no );
void PutBitmap4( RECT *rcView,long x, long y, RECT* rect, long surf_no );
void PutMakerFrame(           long x, long y, RECT *rect, long surf_no );
void Surface2Surface(         long x, long y, RECT* rect, long to, long from );

void BackupSurface( long surf_no, RECT *rect );

unsigned long GetCortBoxColor( COLORREF rgb );
void CortBox(  RECT *rect, long col);
void CortBox2( RECT *rect, long col, long surf);

//void RestoreAllSurface( void );

//テキストの表示
void InitTextObject( char *name );
void PutText( long x, long y, LPSTR text, DWORD color );
void PutText2(long x, long y, LPSTR text, DWORD color, long surf );
void EndTextObject(void);

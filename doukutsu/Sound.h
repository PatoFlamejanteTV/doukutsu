/*****							  *****/
/*****   DirectSound�T�|�[�g�֐�   *****/
/*****							  *****/


// DirectSound�̊J�n
BOOL InitDirectSound(HWND hwnd);
BOOL LoadSoundObject( char *file_name, int no);
// DirectSound�̏I��
void EndDirectSound(void);
void ReleaseSoundObject(int no);//�o�b�t�@�̊J��
//BOOL InitSoundObject(LPCSTR resname, int no);
// �T�E���h�̍Đ�
void PlaySoundObject(int no, int mode);
void ChangeSoundVolume(int no, long volume);//9999��MAX2195?��ɰ��
void ChangeSoundFrequency(int no, DWORD rate);//300��MAX300��ɰ��
void ChangeSoundPan(int no, long pan);




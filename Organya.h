// �� �I���K�[�j���K�v�t�@�C����
// Organya.cpp/Organya.h
// Org�t�H���_

// �� �I���K�[�j�������菇 ��
// 1�A�v���W�F�N�g�� Organya.cpp �� Organya.h ��g�ݍ��݂܂��B
// 2�A�v���W�F�N�g�� dsound.lib �� winmm.lib ��g�ݍ��݂܂��B
// 3�A�J�����g�f�B���N�g���� Org�t�H���_ ������Ă����܂��B

//�⑫�FStartOrganya()�̈��� LPDIRECTSOUND �� NULL������ƁA
//      ����ȍ~�̃I���K�[�j���֐��͑S�Ė����ɂł��܂��B


// ���I���K�[�j���֐��Q��

//���I���K�[�j���̏�������
//�K���ŏ��ɂ��̊֐���ǂ�ł��������B
//�����́ADirectSound �ւ̃|�C���^�ƁAOrg�t�H���_���� Wave.dat �̃p�X�ł��B
//  ���FDirectSound�̃|�C���^�A�y�уv���C�}���o�b�t�@�́A
//      ���炩���ߍ���Ă����Ă��������B
//�iSetCooperativeLevel:DSSCL_EXCLUSIVE�j
BOOL StartOrganya( LPDIRECTSOUND lpDS, char *path_wave );

//���Ȃ̃��[�h��
//�Đ�����O�ɋȂ����[�h���܂��B
//�����͋ȃf�[�^�̃p�X�ł��B
BOOL LoadOrganya( char *path );

//���Đ��ʒu�̐ݒ聜
//�Ȃ̍Đ����ɂ͌Ă΂Ȃ��ł��������B
//�����̓I���K�[�j���̍ŏ��Đ��P�ʂł��B
void SetOrganyaPosition( long x );

//���Đ��ʒu�̎擾��
//�߂�l�̓I���K�[�j���̍ŏ��Đ��P�ʂł��B
long GetOrganyaPosition( void );

//���Ȃ̍Đ���
void PlayOrganyaMusic( void );

//���}�X�^�[�{�����[���̕ύX��
//0�`127�ɕύX�\�ł��B
//�Đ����ł��\�ł��B
BOOL ChangeOrganyaVolume( long vol );

//���Ȃ̒�~��
void StopOrganyaMusic( void );

//���I���K�[�j���̏I����
//�I������O�ɋȂ̒�~���Ăяo���Ă��������B
void EndOrganya( void );

//���t�F�[�h�A�E�g�ݒ聜
void SetOrganyaFadeout( void );
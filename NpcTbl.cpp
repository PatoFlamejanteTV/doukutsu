#include <windows.h>
#include <stdio.h>
#include "Tags.h"
#include "Define.h"


/*
NPCTABLE gNpcTable[] = {
	//NPC000 �_�~�[�i��~�j
	{	BITS_THROW_BLOCK,                            //BITS
		1000, SURF_NPC_SYMBOL, 0, 0, NPCSIZE_MIDDLE, // life, surf, voice1, voice2, size
		0, 0,  4,3,4,8,  8,8,8,8,                    // exp, damage, hit[4], view[4]
	},
	//NPC001 exp object
	{	BITS_EXP,                                   //BITS
		   1, SURF_NPC_SYMBOL, 0, 0, NPCSIZE_MIDDLE,// life, surf, voice1, voice2, size
		1, 0,  4,3,4,3,  8,8,8,8,					// exp, damage, hit[4], view[4]
	},
	//NPC002 �x�փ��X
	{	BITS_DAMAGE_SIDE|BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR|BITS_BOUND_MYCHAR,
		   8, SURF_NPC_ENEMY, WAVE_NPC_LARGE, WAVE_DESTROY_LARGE, NPCSIZE_LARGE,
		3, 1,     11,14,11,8,    16,16,16,8,
	},

	//NPC003 ���őҋ@�I�u�W�F�N�g
	{	BITS_THROW_BLOCK,
		   1, SURF_NPC_SYMBOL, 0, 0, NPCSIZE_MIDDLE,
		0, 0,     0,0,0,0,   0,0,0,0,
	},
	//NPC004 ����
	{	0,
		   1,  SURF_NPC_SYMBOL, 0, 0, NPCSIZE_MIDDLE,
		0, 0,    4,4,4,4,   8,8,8,8,//view
	},

	//NPC005 �N���b�^�|
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR|BITS_THROW_EBLOCK,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		2,		2,		6,4,6,5,		8,8,8,8,
	},
	//NPC006 �r�[�g��
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_INSECT,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		2,		2,		6,6,5,6,		8,8,8,8,
	},
	//NPC007 �o�W���X�N
	{	BITS_BLOCK_BULLET,
		0,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		5,		100,		8,2,8,8,		16,8,16,8,
	},
	//NPC008 �p�S�b�g
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR|BITS_THROW_BLOCK,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_INSECT,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		2,		2,		6,6,5,6,		8,8,8,8,
	},
	//NPC009 �o�����O�i�Q��j
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR|BITS_THROW_BLOCK,
		96,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		18,		0,		14,11,14,12,		20,12,20,12,
	},
	//NPC010 �o�����O�i�퓬�j
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		96,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		18,		0,		14,11,14,12,		20,12,20,12,
	},
	//NPC011 �z���C�g�o�u��
	{	BITS_BANISH_DAMAGE,
		1,		SURF_NPC_SYMBOL,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		2,		4,4,4,4,		8,8,8,8,
	},
	//NPC012 �o�����O(�P��)
	{	0,
		1,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		18,		0,		14,11,14,12,		20,12,20,12,
	},
	//NPC013 �V�[���h
	{	BITS_BLOCK_BULLET|BITS_BLOCK_MYCHAR2,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		1,		8,8,8,8,		8,8,8,8,
	},
	//NPC014 �J�M
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,6,5,6,		8,8,8,8,
	},
	//NPC015 ��
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,6,5,8,		8,8,8,8,
	},
	//NPC016 �Z�[�u�|�C���g
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,6,5,8,		8,8,8,8,
	},
	//NPC017 �G�l���M�[�|�C���g
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,6,5,8,		8,8,8,8,
	},
	//NPC018 ��
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,8,5,8,		8,16,8,8,
	},
	//NPC019 �o�����O�i�ǔj��j
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		96,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		18,		0,		14,11,14,12,		20,12,20,12,
	},

	//NPC020 �p�\�R��
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		16,16,16,8,
	},
	//NPC021 ��
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,0,5,8,		8,0,8,8,
	},

	//NPC022 �]�����u
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		10, 1,10,8,		12,24,12,8,
	},
	//NPC023 �]�����u�����v
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		12, 4,12,0,		12, 4,12,0,
	},
	//NPC024 �p���[�N���b�^�|
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		16,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		7,		2,		 9, 6, 9, 7,		12,12,12,12,
	},
	//NPC025 �G���x�[�^�[
	{	BITS_BLOCK_MYCHAR|BITS_THROW_BLOCK|BITS_BLOCK_BULLET,//BITS
	//	BITS_BLOCK_MYCHAR2|BITS_THROW_BLOCK,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		14, 9,14, 8,		16, 8,16, 8,
	},
	//NPC026 �o�b�g�i�؋�^�j
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		2,		4,7,4,4,		8,8,8,8,
	},
	//NPC027 �f�X�g���b�v
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		127,		8, 8,24,8,		8,16,24,8,
	},
	//NPC028 �N���b�^�[�Q
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		5,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		3,		2,		6,4,6,5,		8,8,8,8,
	},
	//NPC029 �N�g�D���[
	{	0,
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		4,4,4,4,		8,16,8,8,
	},
	//NPC030 �����N
	{	 0,
		 0,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_MIDDLE,
		 0,		0,		6,6,6,6,		8,8,8,8,
	},
	//NPC031 �o�b�g�i�V��^�j
	{	BITS_BANISH_DAMAGE,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		2,		4,7,4,4,		8,8,8,8,
	},
	//NPC032 MAX�̗͑���
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,8,6,6,		8,8,8,8,
	},
	//NPC033 �o�u���z�b�p�[
	{	BITS_BANISH_DAMAGE,
		1,		SURF_NPC_BOSS,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		2,		4,4,4,4,		8,8,8,8,
	},
	//NPC034 �x�b�h
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		16,8,16,8,
	},
	//NPC035 �}���i��
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		100,		SURF_NPC_ENEMY,		WAVE_NPC_GOHST,		0,		NPCSIZE_SMALL,
		2,		3,		4,4,4,20,		12,8,12,24,
	},
	//NPC036 �o�����O�i�퓬LV2�j
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		240,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		18,		0,		14,11,14,12,		20,12,20,12,
	},
	//NPC037 �Ŕ�
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,0,5,8,		8,8,8,8,
	},
	//NPC038 ��
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,0,5,8,		8,8,8,8,
	},
	//NPC039 �ē�
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,0,5,8,		8,8,8,8,
	},
	//NPC040 �T���^
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC041 �~�~�K�[���̉�ꂽ�����
	{	0,
		0,		SURF_PARTS,		0,		0,		0,
		0,		0,		5,8,5,8,		24,24,24,8,
	},
	//NPC042 ��{�X�[
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},

	//NPC043 ���^��Ɗ�
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		12, 8,12,8,		20,24,20,8,
	},
	//NPC044 �T���o�b�O
	{	BITS_BANISH_DAMAGE,
		120,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		12,		10,		12,12,12,12,		16,16,16,16,
	},
	//NPC045 �T���r�b�c
	{	BITS_BANISH_DAMAGE,
		2,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		2,		3,		5,5,5,5,		8,8,8,8,
	},
	//NPC046 �C�x���g�L�[�p�[
	{	0,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,6,6,6,		8,8,8,8,
	},
	//NPC047 ���j
	{	BITS_BANISH_DAMAGE,
		30,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_MIDDLE,
		9,		0,		7,24,7, 8,		24,24,24,8,
	},
	//NPC048 �I���K�N���b�J�[
	{	BITS_BANISH_DAMAGE,
		1,		SURF_NPC_BOSS,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		1,		4,		4,4,4,4,		8,8,8,8,
	},
	//NPC049 �{�[���w�b�h
	{	BITS_BANISH_DAMAGE,
		20,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_LARGE,
		10,		8,		14, 8,14,4,		16,16,16,8,
	},

	//NPC50 �{�[��
	{	BITS_BANISH_DAMAGE,
		1,		SURF_NPC_ENEMY,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		4,		4, 4, 4, 4,		8, 8, 8, 8,
	},

	//NPC051 �N���E�iWith �{�[���w�b�h�j
	{	BITS_BANISH_DAMAGE,
		20,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		9,		4,		 8, 8, 8, 8,		16,16,16,16,
	},
	//NPC052 ���񃍃{
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC053 �{�[���t�b�g
	{	BITS_BANISH_DAMAGE,//BITS_BLOCK_BULLET,
		10,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_SMALL,		NPCSIZE_MIDDLE,
		2,		8,		10, 4, 8,4,		12, 8,12,8,
	},

	//NPC054 �{�[���X�e�b�v
	{	BITS_BANISH_DAMAGE,
		20,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_LARGE,
		10,		8,		14, 8,14,4,		16,16,16,8,
	},
	//NPC055 ��{�J�Y�}
	{	0,//ITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		5, 8,5,8,		8,16,8,8,
	},
	//NPC056 �r�[�g���Q
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_INSECT,	WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		2,		2,		6,6,5,6,		8,8,8,8,
	},
	//NPC057 �N���E�i�ˌ��j
	{	BITS_BANISH_DAMAGE,
		20,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		9,		4,		 8, 8, 8, 8,		16,16,16,16,
	},
	//NPC058 �p�S�b�g=�o�X
	{	BITS_BANISH_DAMAGE,
		15,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,	WAVE_DESTROY_MIDDLE,		NPCSIZE_LARGE,
		9,		0,		6,6,6,6,		12,12,12,12,
	},
	//NPC059 �U�E�h�A
	{	BITS_BANISH_DAMAGE,//BITS
		6,		SURF_NPC_SYMBOL,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_MIDDLE,
		2,		4,		5,8,5,8,		8,16,8,8,
	},
	//NPC060 �g���q
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		4,8,4,8,		8,8,8,8,
	},
	//NPC061 �L���O
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		6,8,5,8,		8,8,8,8,
	},
	//NPC062 PC�J�Y�}
	{	BITS_THROW_BLOCK,//BITS
		0,	SURF_NPC_REGULAR,	0,	0,	0,
		0,	0,	6, 8,6,8,	8,16,8,8,
	},
	//NPC063 �g���q �U��
	{	BITS_THROW_BLOCK,//BITS
		100,	SURF_NPC_REGULAR,	WAVE_NPC_SMALL,	WAVE_DESTROY_SMALL,	NPCSIZE_MIDDLE,
		0,	1,	6,8,6,8,	8,8,8,8,
	},
	//NPC064 �N���b�^�| LV0
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		2,	SURF_NPC_ENEMY,	WAVE_NPC_MIDDLE,	WAVE_DESTROY_MIDDLE,	NPCSIZE_MIDDLE,
		1,	1,	6,4,6,5,	8,8,8,8,
	},
	//NPC065 �o�b�g�i�؋�^�jLV0
	{	BITS_BANISH_DAMAGE,
		1,	SURF_NPC_ENEMY,	WAVE_NPC_SMALL,	WAVE_DESTROY_SMALL,	NPCSIZE_SMALL,
		1,	1,	4,7,4,4,	8,8,8,8,
	},
	//NPC066 �g���q�J�v�Z��
	{	BITS_THROW_BLOCK,//BITS
		0,	SURF_NPC_REGULAR,	WAVE_NPC_SMALL,	WAVE_DESTROY_SMALL,	NPCSIZE_MIDDLE,
		0,	1,	5,8,5,8,	12,12,12,12,
	},
	//NPC067 �~�U���[
	{	0,//BITS
		0,	SURF_NPC_REGULAR,	WAVE_NPC_SMALL,	WAVE_DESTROY_SMALL,	NPCSIZE_MIDDLE,
		0,	1,	5,8,5,8,	8,8,8,8,
	},
	//NPC068 �|�[�� LV0
	{	BITS_BANISH_DAMAGE,//|BITS_BLOCK_MYCHAR,
		60,	SURF_NPC_BOSS,	WAVE_NPC_LARGE,	WAVE_DESTROY_LARGE,	NPCSIZE_LARGE,
		18,	0,	14,11,14,12,	20,12,20,12,
	},

	//NPC069 �s�j����
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		2,	SURF_NPC_ENEMY,	WAVE_NPC_SMALL,	WAVE_DESTROY_MIDDLE,	NPCSIZE_MIDDLE,
		0,	1,	6,5,6,5,
		8,8,8,8,
	},
	//NPC070 �u�����N
	{	0,
		0,	SURF_NPC_SYMBOL,	0,	0,	0,
		0,	0,	5,0,5,8,	8,8,8,8,
	},
	//NPC071 �`����
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		2,		SURF_NPC_ENEMY,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		1,		6,4,6,4,		8,8,8,8,
	},
	//NPC072 ���T���@
	{	0,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC073 ����
	{	0,//BITS
		0,		SURF_CARET,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		1,1,1,1,		0,0,0,0,
	},
	//NPC74 �W���b�N
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC75 ���A�[
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		 5,8,5, 8,		16,8,8,16,
	},
	//NPC76 �v�����c
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC77 �A���u����
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		 5, 8, 5,8,		20,24,28,8,
	},
	//NPC78 ��
	{	0,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC79 �n��
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	  
	//NPC080 ��]���r
	{	BITS_BLOCK_BULLET,
		20,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_MIDDLE,		NPCSIZE_MIDDLE,
		4,		2,		4,12,4,8,		16,16,8,8,
	},
	//NPC081 �W���C�A���g�s�j����
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		12,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		0,		1,		 8, 8, 8,12,		12,12,12,12,
	},
		
	//NPC082 �~�U���[
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_MIDDLE,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
		  
	//NPC083 �C�S�[��(�C�x���g)
	{	0,
		10,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_LARGE,
		0,		2,		 8, 1, 8,16,		20,20,20,20,
	},
  	//NPC084 �����g
	{	0,
		1,		SURF_NPC_ENEMY,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		1,		2,2,2,2,		8,8,8,8,
	},
	//NPC085 �^�}�S���j�^�[
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		 5, 8,5,8,		 8,16,8,8,//view
	},
			  
	//NPC086 bullet object
	{	0,
		1,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		1,		0,		6,6,6,6,		8,8,8,8,
	},
	//NPC087 life object
	{	0,
		2,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		2,		0,		6,6,6,6,		8,8,8,8,
	},
	
	//NPC088 �C�S�[��(�퓬)
	{	BITS_BANISH_DAMAGE,
		300,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		1,		0,		 8,16, 8,16,		20,20,20,20,
	},
	//NPC089 �C�S�[��(����)
	{	0,
		300,		SURF_NPC_BOSS,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		0,		0,		 8,16, 8,16,		20,20,20,20,
	},
	//NPC090 �C�S�[��(���Ȃ���)
	{	0,
		0,		SURF_NPC_BOSS,		0,		0,		0,		0,		0,		4,8,4,8,		8,16,8,8,
	},
	//NPC091 �B
	{	0,
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		4,8,4,8,		16,16,16,8,
	},
	//NPC092 PC�X�[
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		5, 8,5,8,		8,16,8,8,
	},
	//NPC093 �`���R
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		5, 8, 5,8,		8, 8, 8,8,
	},
	//NPC094 �N����
	{	BITS_BANISH_DAMAGE,
		50,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		15,		5,		16,7,16,7,		24,12,24,12,
	},
	//NPC095 ���u�N���Q
	{	BITS_BANISH_DAMAGE,
		4,		SURF_NPC_ENEMY,		WAVE_NPC_INSECT,		WAVE_SPLASH,		NPCSIZE_LARGE,
		2,		5,		6,6,6,6,		8,8,8,8,
	},

	//NPC096 �z�o�[
	{	BITS_BLOCK_MYCHAR2,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,9,8,8,		8,8,8,8,
	},
	//NPC097 �z�o�[
	{	BITS_BLOCK_MYCHAR2,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,9,8,8,		8,8,8,8,
	},
	//NPC098 �z�o�[
	{	BITS_BLOCK_MYCHAR2,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,9,8,8,		8,8,8,8,
	},
	//NPC099 �z�o�[
	{	BITS_BLOCK_MYCHAR2,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,9,8,8,		8,8,8,8,
	},

	//NPC100 �ʋC��
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//NPC101 �d���X�C�b�`
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//NPC102 �d���r���r��
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//NPC103 �\�j�b�N
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_MIDDLE,
		0,		3,		3,8,3,8,		8,12,8,12,
	},
	//NPC104 �J�G��
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		10,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		9,		3,		8,9,8,10,		16,16,16,16,
	},
	//NPC105 ���`��
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//NPC106 ���`���R�[��
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//NPC107 �}���R
	{	0,		20,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		5,5,5,8,		8,16,8,8,
	},
	//NPC108 �t���b�M�[�o�u��
	{	BITS_BANISH_DAMAGE,
		1,		SURF_NPC_ENEMY,		WAVE_SPLASH,		WAVE_SPLASH,		NPCSIZE_SMALL,
		0,		4,		4,4,4,4,		8,8,8,8,
	},
	//NPC109 �Ђ��Ⴐ�}���R
	{	0,
		20,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		5,5,5,8,		8,16,8,8,
	},
	//NPC110 ���J�G��
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		2,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_MIDDLE,
		2,		1,		4,4,4,5,		8,8,8,8,
	},
	//NPC111 �]���}�C�L����
	{	0,
		1,		SURF_MYCHAR,		0,		0,		NPCSIZE_LARGE,
		0,		0,		6,6,5,8,		8,8,8,8,
	},
	//NPC112 �]���}�C�L�����Q
	{	0,		1,		SURF_MYCHAR,		0,		0,		NPCSIZE_LARGE,
		0,		0,		6,6,5,16,		8,8,8,8,
	},
	//NPC113 �u�[�X�^�[���m
	{	0,
		1,
		SURF_NPC_REGULAR,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		6,6,5,8,
		8,8,8,8,
	},
	//NPC114 �U�E�v���X
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR2,//BITS
		32,
		SURF_NPC_SYMBOL,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		5,
		0,
		8,13,8,12,
		8,12,8,12,
	},
	//NPC115 Ravil
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,//BITS
		40,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_MIDDLE,
		10,
		0,
		8,8,8,8,
		12,16,12,8,
	},
	//NPC116 �Ԃ��Ԃт�
	{	0,//BITS
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		0,
		0,
		20,8,20,8,
		24,8,24,8,
	},
	//NPC117 �J�[���[�ҋ@
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		6, 6, 6, 8,
		8, 8, 8, 8,
	},
	//NPC118 �J�[���[�퓬
	{	BITS_BANISH_DAMAGE,//BITS
		320,
		SURF_NPC_BOSS,
		WAVE_GIRLDMG,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 6, 6, 6, 8,
		16,16,16,16,
	},
	//NPC119 �ۃe�[�u��
	{	0,//BITS
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 6, 6, 6, 8,
		12, 8,12, 8,
	},
	//NPC120 �R�����Y�i���/����j
	{	0,//BITS
		0,
		SURF_NPC_BOSS,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 6, 6, 6, 8,
		 8, 8, 8, 8,
	},
	//NPC121 �R�����Y�i�Ђ���^����j
	{	0,//BITS
		0,
		SURF_NPC_BOSS,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 6, 6, 6, 8,
		 8, 8, 8, 8,
	},
	//NPC122 �R�����Y�퓬
	{	0,//BITS
		0,
		SURF_NPC_BOSS,
		WAVE_NPC_SMALL,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 6, 6, 6, 8,
		 8, 8, 8, 8,
	},

	//NPC123 �J�[���[�}�V���K��
	{	0,//BITS
		0,
		SURF_NPC_BOSS,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		6,
		4, 4, 4, 4,
		8, 8, 8, 8,
	},
	//NPC124 �T���u���b�N
	{	BITS_BLOCK_BULLET|BITS_BLOCK_MYCHAR2,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		16,17,16,16,
		16,16,16,16,
	},

	//NPC125 secret energy
	{	BITS_BANISH_DAMAGE,
		1000,		SURF_NPC_SYMBOL,		WAVE_SECRET,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,6,6,6,		8,8,8,8,
	},
	//NPC126 �S����(����)
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		4,8,4,8,		8,8,8,8,
	},
	//NPC127 �}�V���K��Level2�c��
	{	BITS_THROW_BLOCK,//BITS
		1,		SURF_CARET,		0,		0,		0,
		0,		0,		0,0,0,0,		8,8,8,8,
	},
	//NPC128 �}�V���K��Level3�c��
	{	BITS_THROW_BLOCK,//BITS
		1,		SURF_CARET,		0,		0,		0,
		0,		0,		0,0,0,0,		8,8,8,8,
	},
	//NPC129 �t�@�C�A�{�[���c��
	{	BITS_THROW_BLOCK,//BITS
		1,		SURF_CARET,		0,		0,		0,
		0,		0,		0,0,0,0,		8,8,8,8,
	},

	//NPC130 �S�����i�t���t���j
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		4,8,4,8,		8,8,8,8,
	},
	//NPC131 �S����(����)
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		4,8,4,8,		8,8,8,8,
	},

	//NPC132 �S�����i���������j
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		4,8,4,8,		8,8,8,8,
	},
	//NPC133 �W�F���J
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		6,8,6,8,		8,8,8,8,
	},

	//NPC134 �A�[�}�[
	{	BITS_BANISH_DAMAGE,
		20,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_MIDDLE,
		5,		6,		8,4,8,8,		16,8,16,8,
	},
	//NPC135 �X�P���g��
	{	BITS_BANISH_DAMAGE,
		40,		SURF_NPC_ENEMY,		WAVE_NPC_GOHST,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		20,		6,		8,13,8,8,		16,24,16,8,
	},
	//NPC136 �S�����i����ԁj
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		0,0,0,0,		8,8,8,8,
	},
	//NPC137 ��
	{	0,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,6,6,6,		16,24,16,8,
	},

	//NPC138 ��
	{	0,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,6,6,6,		8,16,8,8,
	},

	//NPC139 �h�N�^�[
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		NPCSIZE_LARGE,
		0,		0,		6,6,6,16,		16,16,8,16,
	},

	//NPC140 �g���q���\��
	{	0,//BITS
		500,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		1,		5,		8,8,8,13,		16,16,16,16,
	},
	//NPC141 �g���q��
	{	BITS_BLOCK_BULLET,//BITS
		1,		SURF_NPC_ENEMY,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		6,
		10,
		8,8,8,8,
		8,8,8,8,
	},
	//NPC142 �v�`���[�Y
	{	BITS_BANISH_DAMAGE,
		1,
		SURF_NPC_ENEMY,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		6,
		2,
		6,6,6,8,
		8,8,8,8,
	},
	//NPC143 �W�F���J�i����сj
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		6,8,6,8,
		8,8,8,8,
	},
	//NPC144 �g���q�i���ҁj
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC145 �邬
	{	0,//BITS
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		6,6,6,6,
		8,8,8,8,
	},
	//NPC146 ���Ȃ���
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_CARET,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,230,6,6,		8,232,8,8,
	},
	//NPC147 �N���b�^�[LV3 �i�����W�����v�j
	{	BITS_BANISH_DAMAGE,
		10,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_MIDDLE,
		4,
		8,
		4,4,4,5,
		8,8,8,8,
	},
  	//NPC148 �N���b�^�[�o�u��
	{	BITS_BANISH_DAMAGE,
		5,
		SURF_NPC_ENEMY,
		WAVE_SPLASH,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		0,
		5,
		4,4,4,4,
		4,4,4,4,
	},

  	//NPC149 �����̊�
	{	BITS_BLOCK_MYCHAR2|BITS_BLOCK_BULLET,
		1,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		16,17,16,14,
		16,16,16,16,
	},
	//NPC150 �C�x���g�p��l��
	{	0,
		1,
		SURF_MYCHAR,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		6,6,5,8,
		8,8,8,8,
	},
	//NPC151 �����{
	{	0,
		1,
		SURF_NPC_REGULAR,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		6,6,5,8,
		8,8,8,8,
	},
	//NPC152 �j��_�~�[
	{	BITS_BANISH_DAMAGE,
		40,
		SURF_NPC_SYMBOL,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_SMALL,
		0,
		0,
		 5,8,5,8,
		 8,8,8,8,//view
	},

  	//NPC153 �K�E�f�B�i�k���j
	{	BITS_BANISH_DAMAGE,
		1000,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		8,
		5,
		8,11,8,5,
		12,16,12,8,
	},

  	//NPC154 �K�E�f�B�i����j
	{	0,
		1000,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		8,
		5,
		8,11,8,5,
		12,16,12,8,
	},

  	//NPC155 �K�E�f�B�i�؋�j
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		1000,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		8,
		5,
		8,11,8,5,
		12,16,12,8,
	},
	//NPC156 �K�E�f�B�o�u��
	{	BITS_BANISH_DAMAGE,
		1,
		SURF_NPC_ENEMY,
		WAVE_SPLASH,
		WAVE_SPLASH,
		NPCSIZE_SMALL,
		1,
		3,
		4,4,4,4,
		8,8,8,8,
	},
  	//NPC157 �����̊�
	{	BITS_BLOCK_MYCHAR2|BITS_BLOCK_BULLET,
		1,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		16,17,16,14,
		16,16,16,16,
	},
	//NPC158 �w�~�T�C��
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		1,
		SURF_NPC_BOSS,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_SMALL,
		5,
		5,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC159 �o�[�X�g�L���b�g
	{	BITS_THROW_BLOCK,
		1,
		SURF_NPC_BOSS,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_SMALL,
		0,
		0,
		4,4,4,4,
		24,36,24,36,
	},
	//NPC160 �_�[�N�}�X�^�[
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		300,
		SURF_NPC_BOSS,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		18,
		0,
		14,11,14,12,
		20,12,20,12,
	},

	//NPC161 �_�[�N�o�u��
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		100,
		SURF_NPC_BOSS,
		WAVE_SPLASH,
		0,
		NPCSIZE_SMALL,
		0,
		5,
		6,6,6,6,
		8,8,8,8,
	},

	//NPC162 �_�[�N�}�X�^�[�i���Łj
	{	BITS_THROW_BLOCK,
		500,
		SURF_NPC_BOSS,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		18,
		0,
		14,11,14,12,
		20,12,20,12,
	},

	//NPC163 �h�N�^�[�јC
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		6,8,6,8,
		8,8,8,8,
	},

	//NPC164 �i�[�X�Ή�
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		6,8,6,8,
		8,8,8,8,
	},

	//NPC165 �x�b�h�J�[���[
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		6,8,6,8,
		8,8,8,8,
	},

	//NPC166 �K�E�f�B���W
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		8,8,8,8,
		20,16,20,8,
	},


	//NPC167 �u�[�X�^�[�i����^�����j
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		6,8,6,8,
		8,8,8,8,
	},

	//NPC168 ���
	{	0,//BITS
		0,
		SURF_NPC_BOSS,
		0,
		0,
		0,
		0,
		0,
		8,8,6,8,
		28,32,28,8,
	},

	//NPC169 �o�����O �t��
	{	BITS_BANISH_DAMAGE,//|BITS_BLOCK_MYCHAR,
		500, 
		SURF_NPC_BOSS,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		18,
		0,
		14,11,14,12,
		20,12,20,12,
	},
	//NPC170 �~�T�C����
	{	BITS_BANISH_DAMAGE,
		10,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		3,
		5,
		6,4,6,4,
		8,4,8,4,
	},
	//NPC171 ������
	{	BITS_BLOCK_MYCHAR|BITS_BANISH_DAMAGE,
		50,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		20,
		10,
		4,12,4,12,
		16,16,16,16,
	},
	//NPC172 �t�@�C�A�����O
	{	0,//BITS_BANISH_DAMAGE|BITS_BLOCK_BULLET,
		50,
		SURF_NPC_ENEMY,
		WAVE_FIRE,
		0,
		NPCSIZE_LARGE,
		10,
		5,
		4,12,4,12,
		8,16,8,16,
	},

  	//NPC173 �K�E�f�B�i�J�b�^�[�j
	{	BITS_BANISH_DAMAGE,
		1000,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		15,
		5,
		8,11,8,5,
		12,16,12,8,
	},

	//NPC174 �K�E�f�B�J�b�^�[
	{	BITS_BLOCK_BULLET,
		1,
		SURF_NPC_ENEMY,
		WAVE_SPLASH,
		WAVE_SPLASH,
		NPCSIZE_SMALL,
		0,
		3,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC175 �K�E�f�B�G�b�O
	{	BITS_BANISH_DAMAGE,
		100,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		6,
		0,
		8,10,8,10,
		12,12,12,12,
	},
	//NPC176 �R�P��
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		1000,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		9,
		1,
		 4,12, 4,12,
		16, 8,16, 8,
	},
	//NPC177 �R�P
	{	BITS_BANISH_DAMAGE,//|BITS_THROW_BLOCK,
		3,
		SURF_NPC_ENEMY,
		WAVE_NPC_INSECT,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		2,
		3,
		6,6,6,6,
		8,8,8,8,
	},
	//NPC178 �A�[�����h�o�u��
	{	BITS_THROW_BLOCK|BITS_BANISH_DAMAGE,
		6,
		SURF_NPC_BOSS,
		WAVE_SPLASH,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		1,
		3,
		6,6,6,6,
		8,8,8,8,
	},
	//NPC179 �A�[�����h�I�����W
	{	BITS_THROW_BLOCK|BITS_BANISH_DAMAGE,
		2,
		SURF_NPC_BOSS,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		5,
		3,
		6,4,6,4,
		8,8,8,8,
	},
	//NPC180 �J�[���[
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},
	//NPC181 �J�[���[�}�V���K��
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},

	//NPC182 �J�[���[�|�[���X�^�[
	{	BITS_THROW_BLOCK,//BITS
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC183 �J�[���[�o�u��
	{	BITS_THROW_BLOCK,//BITS
		0,
		SURF_CARET,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		12,12,12,12,
	},
	//NPC184 �o���P�[�h��
	{	BITS_BLOCK_BULLET|BITS_BLOCK_MYCHAR2,//BITS
		0,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		16,17,16,16,
		16,16,16,16,
	},
	//NPC185 �o���P�[�h��
	{	BITS_BLOCK_BULLET|BITS_BLOCK_MYCHAR2,//BITS
		0,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		8,17,8,16,
		8,16,8,16,
	},
	//NPC186 �o���P�[�h��
	{	BITS_BLOCK_BULLET|BITS_BLOCK_MYCHAR2,//BITS
		0,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		8,9,8,8,
		8,8,8,8,
	},
	//NPC187 �E�B���I�E�B�v�X
	{	BITS_BANISH_DAMAGE,
		10,
		SURF_NPC_ENEMY,
		WAVE_NPC_INSECT,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_SMALL,
		7,
		1,
		10,10,10,10,
		16,16,16,16,
	},

	//NPC188 �E�B���I�E�B�v�X �~�j
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		8,
		SURF_NPC_ENEMY,
		WAVE_SPLASH,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		1,
		1,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC189 ��
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		1,
		1,
		4,4,4,4,
		4,12,4,4,
	},

	//NPC190 ��ꃍ�{
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		1,
		0,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC191 ���ʑ���
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		1,
		0,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC192 �o�C�N
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		1,
		0,
		8,4,8,4,
		16,8,16,8,
	},
	//NPC193 ���o�C�N
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		1,
		0,
		20,4,20,4,
		32,8,32,8,
	},
	//NPC194 �o���o�����{
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		1,
		0,
		8,4,8,4,
		16,4,16,4,
	},
	//NPC195 �r���B
	{	BITS_THROW_BLOCK,
		0,
		SURF_PARTS,
		0,
		0,
		0,
		1,
		0,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC196 Stream �㉺
	{	BITS_THROW_BLOCK,
		0,
		SURF_PARTS,
		0,
		0,
		0,
		1,
		0,
		4,4,4,4,
		16,8,16,8,
	},
	//NPC197 �͂肹��ڂ�
	{	BITS_BANISH_DAMAGE,
		3,
		SURF_NPC_ENEMY,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		5,
		0,
		6,6,6,6,
		8,8,8,8,
	},
	//NPC198 �\�j�b�N
	{	0,
		6,
		SURF_NPC_BOSS,
		0,
		0,
		NPCSIZE_SMALL,
		0,
		3,
		6,10,6,10,
		8,12,8,12,
	},
	//NP199 ������
	{	BITS_THROW_BLOCK,//BITS
		0,
		SURF_CARET,
		0,
		0,
		NPCSIZE_SMALL,
		0,
		0,
		1,1,1,1,
		1,1,1,1,
	},
	//NPC200 �u���[�h���S��
	{	BITS_BANISH_DAMAGE,
		1000,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		20,
		10,
		12,18,12,12,
		20,24,20,16,
	},
	//NPC201 �|��u���[�h���S��
	{	0,
		16,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		 0,
		 0,
		10, 16,10,12,
		20,24,20,16,
	},
	//NPC0202 �h���S���o�u��
	{	BITS_BANISH_DAMAGE,
		1,
		SURF_NPC_SYMBOL,
		0,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		0,
		10,
		6, 4,6,4,
		8,16,8,8,
	},
	//NPC203 �N���b�^�| LV5
	{	BITS_BANISH_DAMAGE,
		4,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_MIDDLE,
		4,
		5,
		6,5,6,5,
		8,8,8,8,
	},
	//NPC204 ���΁i���j
	{	BITS_BLOCK_BULLET,
		4,
		SURF_PARTS,
		0,
		0,
		0,
		0,
		5,
		6,5,6,2,
		8,8,8,8,
	},
	//NPC205 ���Α�
	{	BITS_BLOCK_MYCHAR2|BITS_BLOCK_BULLET,//BITS
		80,
		SURF_PARTS,
		WAVE_SMOKE,
		WAVE_BREAK1,
		NPCSIZE_LARGE,
		0,
		0,
		8,17,8,8,
		8,16,8,8,
	},
	//NPC206 �{���J�E���^�[
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		127,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_SMALL,
		7,
		1,
		10,10,10,10,
		20,20,20,20,
	},
	//NPC207 �J�E���^�[
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_SYMBOL,	0,	0,	NPCSIZE_SMALL,
		0,		0,		6,6,6,6,		8,8,8,8,
	},
	//NPC208 �p�S�b�g=�o�XLV2
	{	BITS_BANISH_DAMAGE,
		15,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		9,
		10,
		6,6,6,6,
		12,12,12,12,
	},
  	//NPC209 �����g
	{	0,
		1,
		SURF_NPC_ENEMY,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		0,
		5,
		2,2,2,2,
		8,8,8,8,
	},
	//NPC210 �p�S�b�gLV2
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		4,
		SURF_NPC_ENEMY,
		WAVE_NPC_INSECT,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_MIDDLE,
		2,
		5,
		6,6,5,6,
		8,8,8,8,
	},
	//NPC211 �f�X�g���b�v�~�j
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		0,
		5,
		6,6,6,6,
		8,8,8,8,
	},
	//NPC212 ��
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		10,8,10,12,
		20,24,20,16,
	},
	//NPC213 ���[���S�[�X�g
	{	0,
		64,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		22,
		0,
		 8,16, 8,16,
		24,24,24,24,
	},

	//NPC214 ���[���{�[��
	{	0,
		0,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_MIDDLE,
		0,
		5,
		6,3, 6,3,
		8,8,24,8,
	},
	//NPC215 ���j
	{	BITS_BANISH_DAMAGE,
		30,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_MIDDLE,
		9,
		0,
		7,24,7, 8,
		24,24,24,8,
	},

	//NPC216 Pixel
	{	0,//ITS_THROW_BLOCK,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		5, 8,5,8,
		8,16,8,8,
	},
	//NPC217 �ɓ�
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC218 �A�[�����h�g��
	{	BITS_THROW_BLOCK,//BITS
		 0,
		SURF_NPC_BOSS,
		 0,
		 0,
		 0,
		 0,
		20,
		12,12,12,12,
		16,16,16,16,
	},
	//NPC219 �����u
	{	BITS_THROW_BLOCK,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		1,1,1,1,
		1,1,1,1,
	},

	//NPC220 �~�~�K�[=�X�R�b�v(��~)
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC221 �~�~�K�[=�X�R�b�v(����)
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC222 �B16x32
	{	BITS_BLOCK_MYCHAR2|BITS_BLOCK_BULLET,//BITS
		0,
		SURF_NPC_SYMBOL,
		0,
		0,
		0,
		0,
		0,
		5,16,5,16,
		8,16,8,16,
	},

	//NPC223 ��{ �S��
	{	0,//BITS
		0,
		SURF_NPC_REGULAR,
		0,
		0,
		0,
		0,
		0,
		5,16,5,8,
		8,16,8,8,
	},

	//NPC224 �`�G
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC225 ���K�l�E��
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},

	//NPC226 �ފƃC�x���g
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,16,8,8,
	},

	//NPC227 �o�P�c
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		5,8,5,8,
		8,8,8,8,
	},
	//NPC228 �h���[��
	{	0,//BITS
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_LARGE,
		0,		0,		8,18,8,12,		16,24,16,16,
	},
	//NPC229 �Ԃ��Ԃ̉�
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 8, 8, 8, 8,
		24, 8,24, 8,
	},
	//NPC230 �Ԃ��Ԃ̗t
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 8, 8, 8, 8,
		24,24,24, 8,
	},

	//NPC231 ���P�b�g
	{	BITS_BLOCK_MYCHAR2|BITS_BLOCK_BULLET,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		12, 6, 12, 8,
		16, 8,16, 8,
	},

	//NPC232 �I�����W�^��
	{	BITS_BANISH_DAMAGE,
		64,
		SURF_NPC_BOSS,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		9,
		2,
		10,10,10,10,
		16,16,16,16,
	},
	//NPC233 �o�b�g�i�I�����W�^���t���j
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		4,
		SURF_NPC_BOSS,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		2,
		2,
		4,7,4,4,
		8,8,8,8,
	},
	//NPC234 ����ꂽ�Ԃ���
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		NPCSIZE_MIDDLE,
		0,
		0,
		 8, 8, 8, 8,
		24, 8,24, 8,
	},
	//NPC235 �~�h����
	{	BITS_BLOCK_MYCHAR|BITS_DAMAGE_SIDE|BITS_BANISH_DAMAGE,//BITS
		16,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_MIDDLE,
		2,
		3,
		5,4,5,8,
		8,8,8,8,
	},
	//NPC236 �K���t�B�b�V��
	{	BITS_BLOCK_MYCHAR|BITS_BANISH_DAMAGE,//BITS
		32,
		SURF_NPC_BOSS,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		20,
		0,
		11,8,11,8,
		12,12,12,12,
	},
	//NPC237 �K���E�H�[�^�[
	{	0,//BITS
		0,
		SURF_NPC_BOSS,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_LARGE,
		0,
		3,
		2,2,2,2,
		4,4,4,4,
	},
	//NPC238 �U�E�v���X�g
	{	BITS_BLOCK_BULLET|BITS_BLOCK_MYCHAR2|BITS_THROW_BLOCK,//BITS
		32,
		SURF_NPC_SYMBOL,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_MIDDLE,
		5,
		0,
		8,8,8,8,
		8,8,8,8,
	},
	//NPC239 �ܑ�
	{	BITS_THROW_BLOCK,//BITS
		0,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_SMALL,
		0,
		0,
		 2, 2, 2, 2,
		32, 24,32, 8,
	},

	//NPC240 �~�~�K�[=�����(����)
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		7,8,7,8,
		8,8,8,8,
	},
	//NPC241 �N���b�^�| Red
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		6,
		SURF_NPC_ENEMY,
		WAVE_NPC_MIDDLE,
		WAVE_DESTROY_MIDDLE,
		NPCSIZE_MIDDLE,
		1,
		6,
		6,4,6,5,
		8,8,8,8,
	},
	//NPC242�o�b�g�i�؋�^�jRed
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		1,
		SURF_NPC_ENEMY,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		1,
		2,
		4,7,4,4,
		8,8,8,8,
	},
	//NPC243 �o�b�gRed �z�u
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_SMALL,
		0,		0,		0, 0, 0, 0,		0, 0, 0, 0,
	},
	//NPC244 �Ԃ����H
	{	0,
		1,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_SMALL,
		0,		4,		2,7,2,4,		4,8,4,8,
	},
	//NPC245 �Ԃ����H�z�u
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		NPCSIZE_SMALL,
		0,		0,		2,7,2,4,		4,8,4,8,
	},
	//NPC246 �U�E�v���X
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR2,//BITS
		32,		SURF_NPC_SYMBOL,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		5,		0,		8,13,8,12,		8,12,8,12,
	},
	//NPC247 �~�U���[�i�퓬�j
	{	0,//BITS
		500,		SURF_NPC_BOSS,		WAVE_GIRLDMG,		0,		NPCSIZE_MIDDLE,
		20,		1,		5,8,5,8,		8,8,8,8,
	},
	//NPC248 �u���b�N�o�u��
	{	0,
		1,		SURF_NPC_BOSS,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		6,		4,4,4,4,		8,8,8,8,
	},
	//NPC249 �~�U���[�c��
	{	0,
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_MIDDLE,
		0,		1,		5,8,5,8,		8,8,8,8,
	},
	//NPC250 �u���b�N�{�[��
	{	0,
		1,		SURF_NPC_BOSS,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		6,		4,4,4,4,		8,8,8,8,
	},
	//NPC251 �u���b�N�T���_�[
	{	0,
		1,		SURF_NPC_BOSS,		0,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		10,		7,7,7,7,		8,16,8,16,
	},
	//NPC252 �u���b�N�T�[�N�����o�b�g
	{	BITS_BLOCK_BULLET|BITS_THROW_BLOCK,
		2,		SURF_NPC_BOSS,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		6,		2,		4,4,4,4,		8,8,8,8,
	},
	//NPC253 EXP�J�v�Z��
	{	BITS_BANISH_DAMAGE,
		102,		SURF_NPC_SYMBOL,		WAVE_POP,		WAVE_BUNRET,		NPCSIZE_MIDDLE,
		0,		0,		6,4,6,4,		8,8,8,8,
	},
	//NPC254 �w��
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		32, 4,32, 4,		64,56,64, 8,
	},
	//NPC255 �v���y��
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		6,4,6,4,		56,8,56,8,
	},
	//NPC256 �h�N�^�[�i�w���j
	{	0,//BITS
		0,SURF_NPC_REGULAR,0,	0,	NPCSIZE_LARGE,
		0,	0,	6,6,6,16,	16,16,8,16,
	},
	//NPC257 �Ԃ�����
	{	BITS_THROW_BLOCK|BITS_BLOCK_BULLET,//BITS
		0,	SURF_NPC_SYMBOL,	0,	0,	NPCSIZE_MIDDLE,
		0,	1,	4,8,4,8,	2,2,2,2,
	},
	//NPC258 �x�b�h�~�~�K�[
	{	0,
		0,	SURF_NPC_ENEMY,	0,	0,	NPCSIZE_MIDDLE,
		0,	0,	6,4,6,4,	8,8,8,8,
	},
	//NPC259 �J�[���[�i����ԁj
	{	BITS_THROW_BLOCK,//BITS
		0,	SURF_NPC_REGULAR,	0,	0,	0,
		0,	0,	4,8,4,8,	8,8,8,8,
	},
	//NPC260 ���~�~�K�[(��~)
	{	0,//BITS
		0,	SURF_NPC_ENEMY,	0,	0,	0,
		0,	0,	6,8,6,8,	8,8,8,8,
	},
	//NPC261 ���`�G(��~)
	{	0,//BITS
		0,	SURF_NPC_ENEMY,	0,	0,	0,
		0,	0,	6,8,6,8,	8,8,8,8,
	},
	//NPC262 ���`���R(��~)
	{	0,//BITS
		0,	SURF_NPC_ENEMY,	0,	0,	0,
		0,	0,	6,8,6,8,	8,8,8,8,
	},
	//NPC263 �h�N�^�[�i�퓬LV1�j
	{	BITS_BANISH_DAMAGE,//BITS
		300,	SURF_NPC_BOSS,	WAVE_NPC_LARGE,	0,	NPCSIZE_LARGE,
		1,	3,	6,12,6,16,	16,16,8,16,
	},
	//NPC264 �Ԃ��Ռ�
	{	BITS_BLOCK_BULLET|BITS_THROW_BLOCK,
		2,	SURF_NPC_BOSS,	WAVE_NPC_SMALL,	WAVE_DESTROY_SMALL,	NPCSIZE_SMALL,
		0,	5,	4,4,4,4,	8,8,8,8,
	},
	//NPC265 �Ԃ��Ռ��̔�
	{	BITS_THROW_BLOCK,
		0,
		SURF_NPC_BOSS,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		6,
		0,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC266 �Ԃ��×�
	{	BITS_BLOCK_BULLET,
		2,
		SURF_NPC_BOSS,
		WAVE_NPC_SMALL,
		WAVE_DESTROY_SMALL,
		NPCSIZE_SMALL,
		0,
		5,
		4,4,4,4,
		8,8,8,8,
	},
	//NPC267 �h�N�^�[�i�퓬LV2�j
	{	BITS_BANISH_DAMAGE,//BITS
		300,
		SURF_NPC_BOSS,
		WAVE_NPC_GOHST,
		0,
		NPCSIZE_LARGE,
		1,
		5,
		10, 8,10,16,
		20,28,20,20,
	},
	//NPC268 ���r��-B
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		300,
		SURF_NPC_ENEMY,
		WAVE_NPC_LARGE,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		19,
		5,
		 8, 8, 8,16,
		20,20,20,20,
	},
	//NPC269 �o�b�g�u���X�^�[
	{	BITS_BANISH_DAMAGE,
		2,
		SURF_NPC_BOSS,
		0,
		WAVE_FIRE,
		NPCSIZE_SMALL,
		0,
		2,
		4,4,4,4,
		8,8,8,8,
	},

	//NPC270 �u���b�h
	{	0,
		2,
		SURF_NPC_SYMBOL,
		0,
		WAVE_FIRE,
		NPCSIZE_SMALL,
		0,
		0,
		4,4,4,0,
		2,2,2,2,
	},

	//NPC271 ����u���b�N
	{	BITS_THROW_BLOCK|BITS_BLOCK_BULLET,
		2,		SURF_PARTS,		0,		WAVE_FIRE,		NPCSIZE_SMALL,
		0, 3,		6,6,6,6,		8,8,8,8,
	},

	//NPC272 ����u���b�N�z�u
	{	0,
		2,		SURF_PARTS,		0,		WAVE_FIRE,		NPCSIZE_SMALL,
		0,		0,		8,8,8,8,		8,8,8,8,
	},

	//NPC273 �h���[���n���}�[
	{	0,//BITS
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_LARGE,
		0,		6,		4,4,4,4,		12,12,12,12,
	},
	//NPC274 �h���[��
	{	BITS_BANISH_DAMAGE,//BITS
		64,		SURF_NPC_BOSS,		WAVE_NPC_GOHST,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		9,		3,		8,18,8,12,		16,24,16,16,
	},
	//NPC275 �S�����i��_���j
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		0,
		0,
		0,
		4,8,4,8,
		8,8,8,8,
	},

	//NPC276 ��������
	{	0,//BITS
		300,
		SURF_NPC_ENEMY,
		WAVE_NPC_GOHST,
		WAVE_DESTROY_LARGE,
		NPCSIZE_LARGE,
		100,
		10,
		8,18,8,12,
		16,24,16,16,
	},
	//NPC277 �Ԃ����_
	{	0,//BITS
		0,
		SURF_NPC_ENEMY,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		6,
		4,4,4,4,
		12,12,12,12,
	},
	//NPC278 ���g�����₶
	{	0,//BITS
		0,
		SURF_ITEMIMAGE,
		0,
		0,
		0,
		0,
		0,
		4,4,4,4,
		4,4,4,4,
	},
	//NPC279 ��������
	{	BITS_THROW_BLOCK,//BITS
		0,
		SURF_PARTS,
		0,
		0,
		NPCSIZE_LARGE,
		0,
		0,
		16,17,16,16,
		16,16,16,16,
	},
	//NPC280 �X�[�i���ҁj
	{	0,
		0, SURF_NPC_REGULAR, WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_MIDDLE,
		  0,  0,     5,8,5,8,    8,8,8,8,
	},
	//NPC281 �u���b�f�B�h�N�^�[
	{	0,
		0, SURF_NPC_REGULAR, WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_MIDDLE,
		  0,  0,     5,8,5,8,    8,8,8,8,
	},
	//NPC282 �R�A���t�g
	{	BITS_THROW_BLOCK | BITS_BLOCK_MYCHAR2,             // BITS
		0, SURF_NPC_BOSS, WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_MIDDLE, // life, surf, voice1, voice2, size
		0,  0,     19,15,19,15,    32,20,32,20,								  // exp, damage, hit[4], view[4]
	},
	//NPC283 �~�U���A��
	{	BITS_BANISH_DAMAGE,             // BITS
		1000, SURF_NPC_ENEMY, WAVE_NPC_GOHST, WAVE_DESTROY_LARGE, NPCSIZE_LARGE, // life, surf, voice1, voice2, size
		1,  0,     8,12,8,16,    16,16,16,16,								  // exp, damage, hit[4], view[4]
	},
	//NPC284 �o�g���X�[
	{	BITS_BANISH_DAMAGE,                                                       // BITS
		1000, SURF_NPC_ENEMY, WAVE_GIRLDMG, WAVE_DESTROY_LARGE, NPCSIZE_LARGE, // life, surf, voice1, voice2, size
		1,  0,     8,8,8,13,    16,16,16,16,								      // exp, damage, hit[4], view[4]
	},
	//NPC285 ���b�h�g���l�[�h
	{	BITS_BLOCK_BULLET|BITS_THROW_BLOCK,
		2, SURF_NPC_BOSS, WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_SMALL,
		0, 4,   4,4,4,4,   8,8,8,8,
	},
	//NPC286 ���b�h�g���l�[�h�̔�
	{	BITS_THROW_BLOCK,
		0,  SURF_NPC_BOSS, WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_SMALL,
		6, 0,  4,4,4,4,  8,8,8,8,
	},
	//NPC287 ���b�h�X���[�N
	{	0,                                          // BITS
		   1,  SURF_NPC_BOSS, 0, 0, NPCSIZE_MIDDLE,	// life, surf, voice1, voice2, size
		0, 0,    4,4,4,4,   8,8,8,8,//view			// exp, damage, hit[4], view[4]
	},
	//NPC288 �X���[�N�{�[��
	{	0,
		   1,  SURF_NPC_BOSS, 0, 0, NPCSIZE_MIDDLE,
		0, 4,    4,4,4,4,   8,8,8,8,//view
	},
	//NPC289 �N���b�^�| ����
	{	0,
		2, SURF_NPC_ENEMY, WAVE_NPC_MIDDLE, WAVE_DESTROY_MIDDLE, NPCSIZE_SMALL,
		1, 0,   6,4,6,5,  8,8,8,8,
	},
	//NPC290 �������� ����
	{	BITS_THROW_BLOCK,
		2, SURF_NPC_ENEMY, WAVE_NPC_MIDDLE, WAVE_DESTROY_MIDDLE, NPCSIZE_SMALL,
		1, 0,   6,4,6,5,  8,8,8,8,
	},
	//NPC291 �R�A���t�g(�t�F�C�N)
	{	BITS_THROW_BLOCK | BITS_BLOCK_MYCHAR2,             // BITS
		0, SURF_NPC_BOSS, WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_MIDDLE, // life, surf, voice1, voice2, size
		0,  0,     19,15,19,15,    32,20,32,20,								  // exp, damage, hit[4], view[4]
	},

	//NPC292 �n�k�������u
	{	0,                                       
		0, SURF_NPC_SYMBOL, WAVE_NPC_SMALL, 0, 0,
		0,  0,     0,0,0,0,    0,0,0,0,	
	},
	//NPC293 �g���C�i�A���f�b�h�R�A�j
	{	BITS_THROW_BLOCK,             // BITS
		0, SURF_NPC_BOSS, 0, 0, 0, // life, surf, voice1, voice2, size
		0,  10,     15,15,15,15,    20,20,20,20,								  // exp, damage, hit[4], view[4]
	},
	//NPC294 ���Δ������u
	{	0,                                       
		0, SURF_NPC_SYMBOL, WAVE_NPC_SMALL, 0, 0,
		0,  0,     0,0,0,0,    0,0,0,0,	
	},
	//NPC295 �_(Fall)
	{	BITS_THROW_BLOCK,                                       
		0, SURF_PARTS, WAVE_NPC_SMALL, 0, 0,
		0,  0,     0,0,0,0,    0,0,0,0,	
	},
	//NPC296 �_�z�u
	{	0,                                       
		0, SURF_NPC_SYMBOL, WAVE_NPC_SMALL, 0, 0,
		0,  0,     0,0,0,0,    0,0,0,0,	
	},
	//NPC297 ��{�X�[(���킦)
	{	0,//BITS
		0,		SURF_NPC_REGULAR,	0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC298 �h�N�^�[�����҂�
	{	0,//BITS
		0,		SURF_NPC_REGULAR,	0,		0,		0,
		0,		0,		5,8,5,8,		8,16,8,16,
	},
	//NPC299 �J�v�Z���l
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		5,8,5,8,		24,24,24,24,
	},
	//NPC300 �����̊�
	{	0,//BITS
		0,		SURF_NPC_REGULAR,	0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//NPC301 �~�U���[�~�T�C��
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		1,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,		WAVE_DESTROY_LARGE,		NPCSIZE_SMALL,
		1,		3,		4,4,4,4,		8,8,8,8,
	},
	//NPC302 �t�H�[�J�X�}��
	{	BITS_THROW_BLOCK,
		0,		SURF_NPC_SYMBOL,		0,		0,		NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		0,0,0,0,
	},
	//NPC303 �}�C�}�V���K��
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//304 �K�E�f�B(credit
	{	0,
		0,		SURF_NPC_BOSS,		WAVE_NPC_MIDDLE,	WAVE_DESTROY_MIDDLE,	NPCSIZE_LARGE,
		8,		5,		8,8,8,8,		12,8,12,8,
	},
	//305 �e�S�����i�t���t���j
	{	0,//BITS
		0,		SURF_NPC_ENEMY,		0,		0,		0,
		0,		0,		4,8,4,8,		8,8,8,8,
	},
	//306 �i�[�X�o�����O
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		96,		SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		18,		0,		14,11,14,12,		20,12,20,12,
	},
	//307 ���T���^
	{	0,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		6,8,6,8,		8,8,8,8,
	},
	//308 �X�^���s�[
	{	BITS_BANISH_DAMAGE,
		4,		SURF_NPC_BOSS,		WAVE_NPC_INSECT,	WAVE_DESTROY_MIDDLE,	NPCSIZE_MIDDLE,
		2,		2,		6,6,5,6,		8,8,8,8,
	},
	//309 �G���W�F���i��s�j
	{	0,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_ENGEL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		0,		6,7,6,8,		8,8,8,8,
	},
	//310 �G���W�F���i���s�j
	{	0,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_ENGEL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		3,		8,7,8,8,		12,8,12,8,
	},
	//311 �G���W�F���i�|�j
	{	BITS_BANISH_DAMAGE,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_ENGEL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		3,		8,7,8,8,		12,12,12,12,
	},
	//312 �G���W�F���A���[
	{	BITS_THROW_BLOCK,
		4,		SURF_NPC_SYMBOL,		WAVE_NPC_MIDDLE,	WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		0,		8,		4,2,4,2,		8,8,8,8,
	},
	//313 �}�s�j����
	{	0,
		300,	SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,	WAVE_DESTROY_MIDDLE,	NPCSIZE_MIDDLE,
		1,		0,		 6, 5, 6, 5,		8,8,8,8,
	},
	//314 CAVE����
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_PARTS,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		4,4,4,4,		8,8,8,8,
	},
	//315 �_�}�s�j����
	{	BITS_THROW_BLOCK,
		5,		SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,	WAVE_DESTROY_MIDDLE,	NPCSIZE_MIDDLE,
		1,		3,		 6, 5, 6, 5,		8,8,8,8,
	},
	//316 �G���W�F���i����j
	{	0,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_MIDDLE,	WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		0,		8,7,8,8,		12,12,12,12,
	},
	//317 �G���W�F���i��j�j
	{	BITS_BANISH_DAMAGE|BITS_BLOCK_MYCHAR,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		9,		6,		8,18,8,12,		16,24,16,16,
	},
	//318 �G���W�F���i��j����j
	{	0,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_LARGE,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		3,		0,		8,18,8,12,		16,24,16,16,
	},
	//319 �G���W�F���u���b�N
	{	BITS_BLOCK_BULLET|BITS_THROW_BLOCK,//BITS
		1,		SURF_PARTS,		WAVE_NPC_SMALL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		6,		10,		8,8,8,8,		8,8,8,8,
	},
	//320 �Q�킨��ԃJ�[���[
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_REGULAR,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//321 �J�[���[�l���V�X
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_SYMBOL,		0,		0,		0,
		0,		0,		5,8,5,8,		8,8,8,8,
	},
	//322 �o���P�[�h
	{	BITS_BANISH_DAMAGE,//BITS
		1000,	SURF_NPC_SYMBOL,	WAVE_NPC_GOHST,		WAVE_DESTROY_LARGE,		NPCSIZE_LARGE,
		1,		0,	12,12,12,12,		12,12,12,12,
	},
	//323 �G���W�F���i������݁���s�j
	{	BITS_THROW_BLOCK,
		1000,	SURF_NPC_ENEMY,		WAVE_NPC_ENGEL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		0,		6,7,6,8,		8,12,8,8,
	},
	//324 ������ݔz�u
	{	BITS_THROW_BLOCK,
		1000,	SURF_NPC_ENEMY,		0,		0,		NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		0,0,0,0,
	},
	//325 �v���Y�}�i�փ��B�v���X�j
	{	BITS_THROW_BLOCK,
		1000,	SURF_NPC_BOSS,		0,		0,		NPCSIZE_SMALL,
		0,		0,		10,8,10,88,		16,16,16,16,
	},

	//326 �߂�ɓ�/�X�[
	{	0,
		1000,	SURF_NPC_ENEMY,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		 8,8,8,8,		8,16,8,8,
	},
	//327 �������
	{	0,
		1000,	SURF_NPC_ENEMY,		0,		0,		NPCSIZE_MIDDLE,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	
	//328 �ϐg���u
	{	0,
		1000,	SURF_PARTS,		0,		0,		NPCSIZE_SMALL,
		0,		0,		8,8,8,8,		16,40,16,8,
	},

	//329 ���C��
	{	0,
		1000,	SURF_PARTS,		0,		0,		NPCSIZE_SMALL,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//330 �R���R��
	{	BITS_BLOCK_BULLET,
		1000,	SURF_NPC_SYMBOL,		0,		0,	NPCSIZE_MIDDLE,
		0,		6,		7,7,7,7,		8,8,8,8,
	},
	//331 �X���[�N�{�[��
	{	BITS_BANISH_DAMAGE,
		   1,  SURF_NPC_ENEMY, 0, 0, NPCSIZE_MIDDLE,
		0, 4,    4,4,4,4,   8,8,8,8,//view
	},
	//332 �{�[���X���[�N
	{	0,
		   1,  SURF_CARET, 0, 0, NPCSIZE_MIDDLE,
		0, 8,    4,4,4,4,   12,12,12,12,//view
	},
	//333 �T���_�[�V���{��
	{	BITS_THROW_BLOCK,
		   1,  SURF_CARET, 0, 0, NPCSIZE_MIDDLE,
		0, 0,    4,4,4,4,   12,12,12,12,//view
	},
	//334 ��
	{	BITS_THROW_BLOCK,
		0,	SURF_NPC_SYMBOL,		0,		0,	NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		4,8,4,8,
	},
	//335 �E�j������
	{	BITS_THROW_BLOCK,
		0,	SURF_NPC_ENEMY,		0,		0,	NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		8,8,8,8,
	},
	//336 �����z�u
	{	BITS_THROW_BLOCK,
		0,	SURF_NPC_SYMBOL,	0,		0,	NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		0,0,0,0,
	},
	//337 �h���[���i�����j
	{	0,//BITS
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_LARGE,
		0,		0,		8,18,8,12,		16,24,16,16,
	},
	//338 �O���[���f�r��
	{	BITS_THROW_BLOCK,
		2, SURF_NPC_ENEMY, WAVE_NPC_INSECT, WAVE_DESTROY_MIDDLE, NPCSIZE_SMALL,
		5, 0,   6,4,6,5,  8,8,8,8,
	},
	//339 �O���[���f�r���z�u
	{	BITS_THROW_BLOCK,
		0,	SURF_NPC_SYMBOL,	0,		0,	NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		0,0,0,0,
	},
	//340 �_���{���X
	{	BITS_EVENT_BREAK|BITS_BLOCK_BULLET,
		1000,//1000,
		SURF_NPC_ENEMY,	WAVE_NPC_GOHST,		0,	NPCSIZE_LARGE,
		1,		0,		12,18,12,14,		24,24,24,16,
	},
	//341 �_���{���X�\��
	{	0,
		0,	SURF_NPC_ENEMY,	0,		0,	NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		16,8,16,8,
	},
	//342 �{���X�T�[�N��
	{	BITS_THROW_BLOCK|BITS_BLOCK_BULLET,//BITS
		0,		SURF_NPC_BOSS,	WAVE_NPC_LARGE, WAVE_DESTROY_MIDDLE, NPCSIZE_MIDDLE,
		1,		0,		12,12,12,12,		20,20,20,20,
	},
	//343 �{���X�_�~�[�{�f�B
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_LARGE,
		0,		0,		0,0,0,0,		60,60,60,60,
	},
	//344 �{���X�_�~�[�A�C
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_NPC_BOSS,		0,		0,		NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		12,0,12,0,
	},
	//345 �w�b�h�{�[��
	{	BITS_BLOCK_BULLET,//BITS
		0,		SURF_PARTS,		0,		0,		NPCSIZE_SMALL,
		0,		4,		6,6,6,6,		8,8,8,8,
	},
	//346 �{���X���t�g
	{	BITS_THROW_BLOCK|BITS_BLOCK_MYCHAR2,//BITS
		0,		SURF_NPC_BOSS,	0, 0, NPCSIZE_MIDDLE,
		1,		0,		16,9,16,2,		16,8,16,8,
	},
	//347 ������
	{	BITS_BANISH_DAMAGE,//BITS
		10,		SURF_NPC_ENEMY,	WAVE_NPC_SMALL, WAVE_DESTROY_SMALL, NPCSIZE_MIDDLE,
		1,		4,		8,6,8,6,		8,8,8,8,
	},
	//348 �n���̑�
	{	BITS_THROW_BLOCK,//BITS
		0,		SURF_PARTS,	0, 0, NPCSIZE_SMALL,
		0,		0,		8,8,24,8,		8,16,24,8,
	},

	//NPC000 �_�~�[�i��~�j
	{	BITS_THROW_BLOCK,                            //BITS
		1000, SURF_NPC_SYMBOL, 0, 0, NPCSIZE_MIDDLE, // life, surf, voice1, voice2, size
		0, 0,  4,3,4,8,  8,8,8,8,                    // exp, damage, hit[4], view[4]
	},
	//350 �G���W�F���i���|�j
	{	BITS_BANISH_DAMAGE|BITS_THROW_BLOCK,
		9,	SURF_NPC_ENEMY,		WAVE_NPC_ENGEL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		5,		3,		8,7,8,8,		12,12,12,12,
	},
	//351 �Α�
	{	0,
		100, SURF_PARTS,	WAVE_BREAK1, WAVE_BOM, NPCSIZE_LARGE,
		20,		0,		8,12,8,12,		16,20,16,20,
	},
	//352 �L���X�g
	{	0,
		100, SURF_NPC_REGULAR,	0, 0, NPCSIZE_MIDDLE,
		0,		0,		8,8,8,8,		8,8,8,8,
	},
	//353 ������ݔ�s�ԓV�g
	{	BITS_THROW_BLOCK,
		9,	SURF_NPC_ENEMY,		WAVE_NPC_ENGEL,		WAVE_DESTROY_SMALL,		NPCSIZE_SMALL,
		3,		0,		6,7,6,8,		8,12,8,8,
	},
	//354 �����
	{	BITS_THROW_BLOCK,
		0, SURF_NPC_SYMBOL,	0, 0, NPCSIZE_SMALL,
		0,		127,		6,8,6,280,		0,0,0,0,
	},
	//355 �������J�[���[�^�N�H�[�g
	{	BITS_THROW_BLOCK,
		0,  0,	0, 0, NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		8,8,8,8,
	},
	//356 �p�^�p�^�o�����O
	{	BITS_THROW_BLOCK,
		0,  SURF_NPC_ENEMY,	0, 0, NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		20,12,20,12,
	},
	//357 �u���[�h�b�O
	{	BITS_THROW_BLOCK,
		0, SURF_NPC_SYMBOL,	0, 0, NPCSIZE_SMALL,
		0,		0,		0,0,0,0,		8,8,8,8,
	},
	//358 ---
	{	0,
		0, 0,	0, 0, 0,
		0,		0,		0,0,0,0,		0,0,0,0,
	},
	//359 ---
	{	0,
		0, 0,	0, 0, 0,
		0,		0,		0,0,0,0,		0,0,0,0,
	},


//BITS
// life, surf, voice1, voice2, size
// exp, damage, hit[4], view[4]


};
/**/

void ActNpc000(NPCHAR*npc);void ActNpc001(NPCHAR*npc);void ActNpc002(NPCHAR*npc);void ActNpc003(NPCHAR*npc);void ActNpc004(NPCHAR*npc);
void ActNpc005(NPCHAR*npc);void ActNpc006(NPCHAR*npc);void ActNpc007(NPCHAR*npc);void ActNpc008(NPCHAR*npc);void ActNpc009(NPCHAR*npc);
void ActNpc010(NPCHAR*npc);void ActNpc011(NPCHAR*npc);void ActNpc012(NPCHAR*npc);void ActNpc013(NPCHAR*npc);void ActNpc014(NPCHAR*npc);
void ActNpc015(NPCHAR*npc);void ActNpc016(NPCHAR*npc);void ActNpc017(NPCHAR*npc);void ActNpc018(NPCHAR*npc);void ActNpc019(NPCHAR*npc);
void ActNpc020(NPCHAR*npc);void ActNpc021(NPCHAR*npc);void ActNpc022(NPCHAR*npc);void ActNpc023(NPCHAR*npc);void ActNpc024(NPCHAR*npc);
void ActNpc025(NPCHAR*npc);void ActNpc026(NPCHAR*npc);void ActNpc027(NPCHAR*npc);void ActNpc028(NPCHAR*npc);void ActNpc029(NPCHAR*npc);
void ActNpc030(NPCHAR*npc);void ActNpc031(NPCHAR*npc);void ActNpc032(NPCHAR*npc);void ActNpc033(NPCHAR*npc);void ActNpc034(NPCHAR*npc);
void ActNpc035(NPCHAR*npc);void ActNpc036(NPCHAR*npc);void ActNpc037(NPCHAR*npc);void ActNpc038(NPCHAR*npc);void ActNpc039(NPCHAR*npc);
void ActNpc040(NPCHAR*npc);void ActNpc041(NPCHAR*npc);void ActNpc042(NPCHAR*npc);void ActNpc043(NPCHAR*npc);void ActNpc044(NPCHAR*npc);
void ActNpc045(NPCHAR*npc);void ActNpc046(NPCHAR*npc);void ActNpc047(NPCHAR*npc);void ActNpc048(NPCHAR*npc);void ActNpc049(NPCHAR*npc);
void ActNpc050(NPCHAR*npc);void ActNpc051(NPCHAR*npc);void ActNpc052(NPCHAR*npc);void ActNpc053(NPCHAR*npc);void ActNpc054(NPCHAR*npc);
void ActNpc055(NPCHAR*npc);void ActNpc056(NPCHAR*npc);void ActNpc057(NPCHAR*npc);void ActNpc058(NPCHAR*npc);void ActNpc059(NPCHAR*npc);
void ActNpc060(NPCHAR*npc);void ActNpc061(NPCHAR*npc);void ActNpc062(NPCHAR*npc);void ActNpc063(NPCHAR*npc);void ActNpc064(NPCHAR*npc);
void ActNpc065(NPCHAR*npc);void ActNpc066(NPCHAR*npc);void ActNpc067(NPCHAR*npc);void ActNpc068(NPCHAR*npc);void ActNpc069(NPCHAR*npc);
void ActNpc070(NPCHAR*npc);void ActNpc071(NPCHAR*npc);void ActNpc072(NPCHAR*npc);void ActNpc073(NPCHAR*npc);void ActNpc074(NPCHAR*npc);
void ActNpc075(NPCHAR*npc);void ActNpc076(NPCHAR*npc);void ActNpc077(NPCHAR*npc);void ActNpc078(NPCHAR*npc);void ActNpc079(NPCHAR*npc);
void ActNpc080(NPCHAR*npc);void ActNpc081(NPCHAR*npc);void ActNpc082(NPCHAR*npc);void ActNpc083(NPCHAR*npc);void ActNpc084(NPCHAR*npc);
void ActNpc085(NPCHAR*npc);void ActNpc086(NPCHAR*npc);void ActNpc087(NPCHAR*npc);void ActNpc088(NPCHAR*npc);void ActNpc089(NPCHAR*npc);
void ActNpc090(NPCHAR*npc);void ActNpc091(NPCHAR*npc);void ActNpc092(NPCHAR*npc);void ActNpc093(NPCHAR*npc);void ActNpc094(NPCHAR*npc);
void ActNpc095(NPCHAR*npc);void ActNpc096(NPCHAR*npc);void ActNpc097(NPCHAR*npc);void ActNpc098(NPCHAR*npc);void ActNpc099(NPCHAR*npc);

void ActNpc100(NPCHAR*npc);void ActNpc101(NPCHAR*npc);void ActNpc102(NPCHAR*npc);void ActNpc103(NPCHAR*npc);void ActNpc104(NPCHAR*npc);
void ActNpc105(NPCHAR*npc);void ActNpc106(NPCHAR*npc);void ActNpc107(NPCHAR*npc);void ActNpc108(NPCHAR*npc);void ActNpc109(NPCHAR*npc);
void ActNpc110(NPCHAR*npc);void ActNpc111(NPCHAR*npc);void ActNpc112(NPCHAR*npc);void ActNpc113(NPCHAR*npc);void ActNpc114(NPCHAR*npc);
void ActNpc115(NPCHAR*npc);void ActNpc116(NPCHAR*npc);void ActNpc117(NPCHAR*npc);void ActNpc118(NPCHAR*npc);void ActNpc119(NPCHAR*npc);
void ActNpc120(NPCHAR*npc);void ActNpc121(NPCHAR*npc);void ActNpc122(NPCHAR*npc);void ActNpc123(NPCHAR*npc);void ActNpc124(NPCHAR*npc);
void ActNpc125(NPCHAR*npc);void ActNpc126(NPCHAR*npc);void ActNpc127(NPCHAR*npc);void ActNpc128(NPCHAR*npc);void ActNpc129(NPCHAR*npc);
void ActNpc130(NPCHAR*npc);void ActNpc131(NPCHAR*npc);void ActNpc132(NPCHAR*npc);void ActNpc133(NPCHAR*npc);void ActNpc134(NPCHAR*npc);
void ActNpc135(NPCHAR*npc);void ActNpc136(NPCHAR*npc);void ActNpc137(NPCHAR*npc);void ActNpc138(NPCHAR*npc);void ActNpc139(NPCHAR*npc);
void ActNpc140(NPCHAR*npc);void ActNpc141(NPCHAR*npc);void ActNpc142(NPCHAR*npc);void ActNpc143(NPCHAR*npc);void ActNpc144(NPCHAR*npc);
void ActNpc145(NPCHAR*npc);void ActNpc146(NPCHAR*npc);void ActNpc147(NPCHAR*npc);void ActNpc148(NPCHAR*npc);void ActNpc149(NPCHAR*npc);
void ActNpc150(NPCHAR*npc);void ActNpc151(NPCHAR*npc);void ActNpc152(NPCHAR*npc);void ActNpc153(NPCHAR*npc);void ActNpc154(NPCHAR*npc);
void ActNpc155(NPCHAR*npc);void ActNpc156(NPCHAR*npc);void ActNpc157(NPCHAR*npc);void ActNpc158(NPCHAR*npc);void ActNpc159(NPCHAR*npc);
void ActNpc160(NPCHAR*npc);void ActNpc161(NPCHAR*npc);void ActNpc162(NPCHAR*npc);void ActNpc163(NPCHAR*npc);void ActNpc164(NPCHAR*npc);
void ActNpc165(NPCHAR*npc);void ActNpc166(NPCHAR*npc);void ActNpc167(NPCHAR*npc);void ActNpc168(NPCHAR*npc);void ActNpc169(NPCHAR*npc);
void ActNpc170(NPCHAR*npc);void ActNpc171(NPCHAR*npc);void ActNpc172(NPCHAR*npc);void ActNpc173(NPCHAR*npc);void ActNpc174(NPCHAR*npc);
void ActNpc175(NPCHAR*npc);void ActNpc176(NPCHAR*npc);void ActNpc177(NPCHAR*npc);void ActNpc178(NPCHAR*npc);void ActNpc179(NPCHAR*npc);
void ActNpc180(NPCHAR*npc);void ActNpc181(NPCHAR*npc);void ActNpc182(NPCHAR*npc);void ActNpc183(NPCHAR*npc);void ActNpc184(NPCHAR*npc);
void ActNpc185(NPCHAR*npc);void ActNpc186(NPCHAR*npc);void ActNpc187(NPCHAR*npc);void ActNpc188(NPCHAR*npc);void ActNpc189(NPCHAR*npc);
void ActNpc190(NPCHAR*npc);void ActNpc191(NPCHAR*npc);void ActNpc192(NPCHAR*npc);void ActNpc193(NPCHAR*npc);void ActNpc194(NPCHAR*npc);
void ActNpc195(NPCHAR*npc);void ActNpc196(NPCHAR*npc);void ActNpc197(NPCHAR*npc);void ActNpc198(NPCHAR*npc);void ActNpc199(NPCHAR*npc);

void ActNpc200(NPCHAR*npc);void ActNpc201(NPCHAR*npc);void ActNpc202(NPCHAR*npc);void ActNpc203(NPCHAR*npc);void ActNpc204(NPCHAR*npc);
void ActNpc205(NPCHAR*npc);void ActNpc206(NPCHAR*npc);void ActNpc207(NPCHAR*npc);void ActNpc208(NPCHAR*npc);void ActNpc209(NPCHAR*npc);
void ActNpc210(NPCHAR*npc);void ActNpc211(NPCHAR*npc);void ActNpc212(NPCHAR*npc);void ActNpc213(NPCHAR*npc);void ActNpc214(NPCHAR*npc);
void ActNpc215(NPCHAR*npc);void ActNpc216(NPCHAR*npc);void ActNpc217(NPCHAR*npc);void ActNpc218(NPCHAR*npc);void ActNpc219(NPCHAR*npc);
void ActNpc220(NPCHAR*npc);void ActNpc221(NPCHAR*npc);void ActNpc222(NPCHAR*npc);void ActNpc223(NPCHAR*npc);void ActNpc224(NPCHAR*npc);
void ActNpc225(NPCHAR*npc);void ActNpc226(NPCHAR*npc);void ActNpc227(NPCHAR*npc);void ActNpc228(NPCHAR*npc);void ActNpc229(NPCHAR*npc);
void ActNpc230(NPCHAR*npc);void ActNpc231(NPCHAR*npc);void ActNpc232(NPCHAR*npc);void ActNpc233(NPCHAR*npc);void ActNpc234(NPCHAR*npc);
void ActNpc235(NPCHAR*npc);void ActNpc236(NPCHAR*npc);void ActNpc237(NPCHAR*npc);void ActNpc238(NPCHAR*npc);void ActNpc239(NPCHAR*npc);
void ActNpc240(NPCHAR*npc);void ActNpc241(NPCHAR*npc);void ActNpc242(NPCHAR*npc);void ActNpc243(NPCHAR*npc);void ActNpc244(NPCHAR*npc);
void ActNpc245(NPCHAR*npc);void ActNpc246(NPCHAR*npc);void ActNpc247(NPCHAR*npc);void ActNpc248(NPCHAR*npc);void ActNpc249(NPCHAR*npc);
void ActNpc250(NPCHAR*npc);void ActNpc251(NPCHAR*npc);void ActNpc252(NPCHAR*npc);void ActNpc253(NPCHAR*npc);void ActNpc254(NPCHAR*npc);
void ActNpc255(NPCHAR*npc);void ActNpc256(NPCHAR*npc);void ActNpc257(NPCHAR*npc);void ActNpc258(NPCHAR*npc);void ActNpc259(NPCHAR*npc);
void ActNpc260(NPCHAR*npc);void ActNpc261(NPCHAR*npc);void ActNpc262(NPCHAR*npc);void ActNpc263(NPCHAR*npc);void ActNpc264(NPCHAR*npc);
void ActNpc265(NPCHAR*npc);void ActNpc266(NPCHAR*npc);void ActNpc267(NPCHAR*npc);void ActNpc268(NPCHAR*npc);void ActNpc269(NPCHAR*npc);
void ActNpc270(NPCHAR*npc);void ActNpc271(NPCHAR*npc);void ActNpc272(NPCHAR*npc);void ActNpc273(NPCHAR*npc);void ActNpc274(NPCHAR*npc);
void ActNpc275(NPCHAR*npc);void ActNpc276(NPCHAR*npc);void ActNpc277(NPCHAR*npc);void ActNpc278(NPCHAR*npc);void ActNpc279(NPCHAR*npc);
void ActNpc280(NPCHAR*npc);void ActNpc281(NPCHAR*npc);void ActNpc282(NPCHAR*npc);void ActNpc283(NPCHAR*npc);void ActNpc284(NPCHAR*npc);
void ActNpc285(NPCHAR*npc);void ActNpc286(NPCHAR*npc);void ActNpc287(NPCHAR*npc);void ActNpc288(NPCHAR*npc);void ActNpc289(NPCHAR*npc);
void ActNpc290(NPCHAR*npc);void ActNpc291(NPCHAR*npc);void ActNpc292(NPCHAR*npc);void ActNpc293(NPCHAR*npc);void ActNpc294(NPCHAR*npc);
void ActNpc295(NPCHAR*npc);void ActNpc296(NPCHAR*npc);void ActNpc297(NPCHAR*npc);void ActNpc298(NPCHAR*npc);void ActNpc299(NPCHAR*npc);
	
void ActNpc300(NPCHAR*npc);void ActNpc301(NPCHAR*npc);void ActNpc302(NPCHAR*npc);void ActNpc303(NPCHAR*npc);void ActNpc304(NPCHAR*npc);
void ActNpc305(NPCHAR*npc);void ActNpc306(NPCHAR*npc);void ActNpc307(NPCHAR*npc);void ActNpc308(NPCHAR*npc);void ActNpc309(NPCHAR*npc);
void ActNpc310(NPCHAR*npc);void ActNpc311(NPCHAR*npc);void ActNpc312(NPCHAR*npc);void ActNpc313(NPCHAR*npc);void ActNpc314(NPCHAR*npc);
void ActNpc315(NPCHAR*npc);void ActNpc316(NPCHAR*npc);void ActNpc317(NPCHAR*npc);void ActNpc318(NPCHAR*npc);void ActNpc319(NPCHAR*npc);
void ActNpc320(NPCHAR*npc);void ActNpc321(NPCHAR*npc);void ActNpc322(NPCHAR*npc);void ActNpc323(NPCHAR*npc);void ActNpc324(NPCHAR*npc);
void ActNpc325(NPCHAR*npc);void ActNpc326(NPCHAR*npc);void ActNpc327(NPCHAR*npc);void ActNpc328(NPCHAR*npc);void ActNpc329(NPCHAR*npc);
void ActNpc330(NPCHAR*npc);void ActNpc331(NPCHAR*npc);void ActNpc332(NPCHAR*npc);void ActNpc333(NPCHAR*npc);void ActNpc334(NPCHAR*npc);
void ActNpc335(NPCHAR*npc);void ActNpc336(NPCHAR*npc);void ActNpc337(NPCHAR*npc);void ActNpc338(NPCHAR*npc);void ActNpc339(NPCHAR*npc);
void ActNpc340(NPCHAR*npc);void ActNpc341(NPCHAR*npc);void ActNpc342(NPCHAR*npc);void ActNpc343(NPCHAR*npc);void ActNpc344(NPCHAR*npc);
void ActNpc345(NPCHAR*npc);void ActNpc346(NPCHAR*npc);void ActNpc347(NPCHAR*npc);void ActNpc348(NPCHAR*npc);void ActNpc349(NPCHAR*npc);
void ActNpc350(NPCHAR*npc);void ActNpc351(NPCHAR*npc);void ActNpc352(NPCHAR*npc);void ActNpc353(NPCHAR*npc);void ActNpc354(NPCHAR*npc);
void ActNpc355(NPCHAR*npc);void ActNpc356(NPCHAR*npc);void ActNpc357(NPCHAR*npc);void ActNpc358(NPCHAR*npc);void ActNpc359(NPCHAR*npc);
void ActNpc360(NPCHAR*npc);

NPCFUNCTION gpNpcFuncTbl[] = {

	ActNpc000,ActNpc001,ActNpc002,ActNpc003,ActNpc004,ActNpc005,ActNpc006,ActNpc007,ActNpc008,ActNpc009,
	ActNpc010,ActNpc011,ActNpc012,ActNpc013,ActNpc014,ActNpc015,ActNpc016,ActNpc017,ActNpc018,ActNpc019,
	ActNpc020,ActNpc021,ActNpc022,ActNpc023,ActNpc024,ActNpc025,ActNpc026,ActNpc027,ActNpc028,ActNpc029,
	ActNpc030,ActNpc031,ActNpc032,ActNpc033,ActNpc034,ActNpc035,ActNpc036,ActNpc037,ActNpc038,ActNpc039,
	ActNpc040,ActNpc041,ActNpc042,ActNpc043,ActNpc044,ActNpc045,ActNpc046,ActNpc047,ActNpc048,ActNpc049,
	ActNpc050,ActNpc051,ActNpc052,ActNpc053,ActNpc054,ActNpc055,ActNpc056,ActNpc057,ActNpc058,ActNpc059,
	ActNpc060,ActNpc061,ActNpc062,ActNpc063,ActNpc064,ActNpc065,ActNpc066,ActNpc067,ActNpc068,ActNpc069,
	ActNpc070,ActNpc071,ActNpc072,ActNpc073,ActNpc074,ActNpc075,ActNpc076,ActNpc077,ActNpc078,ActNpc079,
	ActNpc080,ActNpc081,ActNpc082,ActNpc083,ActNpc084,ActNpc085,ActNpc086,ActNpc087,ActNpc088,ActNpc089,
	ActNpc090,ActNpc091,ActNpc092,ActNpc093,ActNpc094,ActNpc095,ActNpc096,ActNpc097,ActNpc098,ActNpc099,
	
	ActNpc100,ActNpc101,ActNpc102,ActNpc103,ActNpc104,ActNpc105,ActNpc106,ActNpc107,ActNpc108,ActNpc109,
	ActNpc110,ActNpc111,ActNpc112,ActNpc113,ActNpc114,ActNpc115,ActNpc116,ActNpc117,ActNpc118,ActNpc119,
	ActNpc120,ActNpc121,ActNpc122,ActNpc123,ActNpc124,ActNpc125,ActNpc126,ActNpc127,ActNpc128,ActNpc129,
	ActNpc130,ActNpc131,ActNpc132,ActNpc133,ActNpc134,ActNpc135,ActNpc136,ActNpc137,ActNpc138,ActNpc139,
	ActNpc140,ActNpc141,ActNpc142,ActNpc143,ActNpc144,ActNpc145,ActNpc146,ActNpc147,ActNpc148,ActNpc149,
	ActNpc150,ActNpc151,ActNpc152,ActNpc153,ActNpc154,ActNpc155,ActNpc156,ActNpc157,ActNpc158,ActNpc159,
	ActNpc160,ActNpc161,ActNpc162,ActNpc163,ActNpc164,ActNpc165,ActNpc166,ActNpc167,ActNpc168,ActNpc169,
	ActNpc170,ActNpc171,ActNpc172,ActNpc173,ActNpc174,ActNpc175,ActNpc176,ActNpc177,ActNpc178,ActNpc179,
	ActNpc180,ActNpc181,ActNpc182,ActNpc183,ActNpc184,ActNpc185,ActNpc186,ActNpc187,ActNpc188,ActNpc189,
	ActNpc190,ActNpc191,ActNpc192,ActNpc193,ActNpc194,ActNpc195,ActNpc196,ActNpc197,ActNpc198,ActNpc199,
	
	ActNpc200,ActNpc201,ActNpc202,ActNpc203,ActNpc204,ActNpc205,ActNpc206,ActNpc207,ActNpc208,ActNpc209,
	ActNpc210,ActNpc211,ActNpc212,ActNpc213,ActNpc214,ActNpc215,ActNpc216,ActNpc217,ActNpc218,ActNpc219,
	ActNpc220,ActNpc221,ActNpc222,ActNpc223,ActNpc224,ActNpc225,ActNpc226,ActNpc227,ActNpc228,ActNpc229,
	ActNpc230,ActNpc231,ActNpc232,ActNpc233,ActNpc234,ActNpc235,ActNpc236,ActNpc237,ActNpc238,ActNpc239,
	ActNpc240,ActNpc241,ActNpc242,ActNpc243,ActNpc244,ActNpc245,ActNpc246,ActNpc247,ActNpc248,ActNpc249,
	ActNpc250,ActNpc251,ActNpc252,ActNpc253,ActNpc254,ActNpc255,ActNpc256,ActNpc257,ActNpc258,ActNpc259,
	ActNpc260,ActNpc261,ActNpc262,ActNpc263,ActNpc264,ActNpc265,ActNpc266,ActNpc267,ActNpc268,ActNpc269,
	ActNpc270,ActNpc271,ActNpc272,ActNpc273,ActNpc274,ActNpc275,ActNpc276,ActNpc277,ActNpc278,ActNpc279,
	ActNpc280,ActNpc281,ActNpc282,ActNpc283,ActNpc284,ActNpc285,ActNpc286,ActNpc287,ActNpc288,ActNpc289,
	ActNpc290,ActNpc291,ActNpc292,ActNpc293,ActNpc294,ActNpc295,ActNpc296,ActNpc297,ActNpc298,ActNpc299,
	
	ActNpc300,ActNpc301,ActNpc302,ActNpc303,ActNpc304,ActNpc305,ActNpc306,ActNpc307,ActNpc308,ActNpc309,
	ActNpc310,ActNpc311,ActNpc312,ActNpc313,ActNpc314,ActNpc315,ActNpc316,ActNpc317,ActNpc318,ActNpc319,
	ActNpc320,ActNpc321,ActNpc322,ActNpc323,ActNpc324,ActNpc325,ActNpc326,ActNpc327,ActNpc328,ActNpc329,
	ActNpc330,ActNpc331,ActNpc332,ActNpc333,ActNpc334,ActNpc335,ActNpc336,ActNpc337,ActNpc338,ActNpc339,
	ActNpc340,ActNpc341,ActNpc342,ActNpc343,ActNpc344,ActNpc345,ActNpc346,ActNpc347,ActNpc348,ActNpc349,
	ActNpc350,ActNpc351,ActNpc352,ActNpc353,ActNpc354,ActNpc355,ActNpc356,ActNpc357,ActNpc358,ActNpc359,
	ActNpc360,
};


NPCTABLE *gNpcTable = NULL;
long GetFileSizeLong( char *path );

BOOL LoadNpcTable( char *path )
{
	FILE *fp;
	long n;
	long size;
	long num;

	size = GetFileSizeLong( path );
	if( size == -1 ) return FALSE;
	num = size/sizeof( NPCTABLE );

	gNpcTable = (NPCTABLE *)malloc( num * sizeof( NPCTABLE ) );
	if( !gNpcTable ) return FALSE;

	fp = fopen( path, "rb" );
	if( !fp ){
		free( gNpcTable );
		gNpcTable = NULL;
		return FALSE;
	}

	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].bits,          sizeof(unsigned short), 1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].life,          sizeof(unsigned short), 1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].surf,          sizeof(unsigned char),  1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].destroy_voice, sizeof(unsigned char),  1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].hit_voice,     sizeof(unsigned char),  1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].size,          sizeof(unsigned char),  1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].exp,           sizeof(long),           1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].damage,        sizeof(long),           1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].hit,           sizeof(RANGE_CHAR),     1, fp );
	for( n = 0; n < num; n++ ) fread( &gNpcTable[n].view,          sizeof(RANGE_CHAR),     1, fp );

	fclose( fp );
	return TRUE;
}


void ReleaseNpcTable( void )
{
	if( gNpcTable ){
		free( gNpcTable );
		gNpcTable = NULL;
	}
	return;
}

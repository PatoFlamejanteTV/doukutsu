#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"
#include "KeyControl.h"


//�R�����Y�i���^����j
void ActNpc120( NPCHAR *npc )
{
	RECT rect[] = {
		{ 64, 0, 80, 16}, // ���
		{ 64,16, 80, 32},// ����
	};
	if( npc->direct == DIR_LEFT ) npc->rect = rect[0];
	else                          npc->rect = rect[1];
}
//�R�����Y�i�����������^����j
void ActNpc121( NPCHAR *npc )
{
	RECT rect[] = {
		{  0,  0, 16, 16}, // ����
		{ 16,  0, 32, 16}, // �u��
		{112,  0,128, 16}, // ����
	};

	if( npc->direct == DIR_LEFT ){
		switch( npc->act_no ){
		case 0:
			npc->act_no   = 1;
			npc->ani_no   = 0;
			npc->ani_wait = 0;
		case 1://�ҋ@
			if( Random( 0, 120 ) == 10 ){
				npc->act_no   = 2;
				npc->act_wait = 0;
				npc->ani_no   = 1;
			}
			break;
		case 2://�u��
			npc->act_wait++;
			if( npc->act_wait > 8 ){
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;
		}
		npc->rect = rect[npc->ani_no];
	}else{
		npc->rect = rect[2];
		npc->act_wait++;
		if( npc->act_wait > 100 ){
			npc->act_wait = 0;
			SetCaret( npc->x, npc->y, CARET_ZZZ, DIR_LEFT );
		}
	}
}


// �R�����Y(�퓬)
void ActNpc122( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{  0,  0, 16, 16}, // ����
		{ 16,  0, 32, 16}, // �u��

		{ 32,  0, 48, 16},//����
		{  0,  0, 16, 16},
		{ 48,  0, 64, 16},
		{  0,  0, 16, 16},

		{ 80,  0, 96, 16},//�Ƃ� ���Ԃ� 6
		{ 96,  0,112, 16},//�Ƃ� �̂����� 7
		{112,  0,128, 16},//���� ���Ԃ� 8
		{128,  0,144, 16},//���� �̂����� 9
	};

	RECT rcRight[] = {

		{  0, 16, 16, 32}, // ����
		{ 16, 16, 32, 32}, // �u��

		{ 32, 16, 48, 32},//����
		{  0, 16, 16, 32},
		{ 48, 16, 64, 32},
		{  0, 16, 16, 32},

		{ 80, 16, 96, 32},//�Ƃ� ���Ԃ� 6
		{ 96, 16,112, 32},//�Ƃ� �̂����� 7
		{112, 16,128, 32},//���� ���Ԃ� 8
		{128, 16,144, 32},//���� �̂����� 9
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1://�ҋ@
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		//�߂Â��Ƃӂ�����c�B
		if( npc->x - PARTSSIZE*2*VS < gMC.x &&
			npc->x + PARTSSIZE*2*VS > gMC.x &&
			npc->y - PARTSSIZE*2*VS < gMC.y &&
			npc->y + PARTSSIZE*1*VS > gMC.y ){

			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}

		break;

	case 2://�u��
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;

	case 10:// �퓬
		npc->life = 1000;
		npc->act_no   = 11;
		npc->act_wait =  Random( 0, 50 );
		npc->ani_no   =  0;
		npc->damage   =  0;
	case 11:
		if( npc->act_wait ) npc->act_wait--;
		else                npc->act_no   = 13;

		break;

	case 13: //����
		npc->act_no   = 14;
		npc->act_wait = Random( 0, 50 );
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	case 14:


		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 2;
		if( npc->direct == DIR_LEFT ) npc->xm -= VS/8;
		else                          npc->xm += VS/8;

		if( npc->act_wait ) npc->act_wait--;
		else{
			npc->bits |= BITS_BANISH_DAMAGE;
			npc->act_no = 15;// �W�����v��
			npc->ani_no =  2;
			npc->ym     = -VS;
			npc->damage = 2;
		}
		break;
	case 15:// �W�����v

		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->bits |= BITS_BANISH_DAMAGE;
			npc->xm     =  0;
			npc->act_no = 10;
			npc->damage =  0;
		}
		break;

	case 20:// �Ƃ�
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->xm     =  0;
			npc->act_no = 21;
			npc->damage =  0;
			if( npc->ani_no == 6 ) npc->ani_no = 8;
			else                   npc->ani_no = 9;
			npc->act_wait = Random( 300, 400 );
		}
		break;
	case 21:// ����
		if( npc->act_wait ) npc->act_wait--;
		else{
			npc->bits |= BITS_BANISH_DAMAGE;
			npc->life = 1000;
			npc->act_no   = 11;
			npc->act_wait =  Random( 0, 50 );
			npc->ani_no   =  0;
		}
		break;
	}

	//������
	if( npc->act_no > 10 &&
		npc->act_no < 20 &&
		npc->life != 1000 ){

		npc->act_no  = 20;
		npc->ym      = -VS;
		npc->ani_no  = Random( 6,7 );
		npc->bits   &= ~BITS_BANISH_DAMAGE;
	}

	npc->ym += VS/16;
	if( npc->xm >  MAX_MOVE/3 ) npc->xm =  MAX_MOVE/3;
	if( npc->xm < -MAX_MOVE/3 ) npc->xm = -MAX_MOVE/3;
	if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;
	npc->y += npc->ym;
	npc->x += npc->xm;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}



// �}�V���K��
void ActNpc123( NPCHAR *npc )
{
	RECT rect[] = {
		{192, 0,208, 16},
		{208, 0,224, 16},
		{224, 0,240, 16},
		{240, 0,256, 16},
	};

	BOOL bBreak;

	bBreak = FALSE;

	switch( npc->act_no ){
	case 0:

		npc->act_no = 1;
		SetCaret( npc->x, npc->y, CARET_FLASH, 0 );
		PlaySoundObject( WAVE_POLESTAR, 1 );
		switch( npc->direct ){
		case 0:// ��
			npc->xm = -8*VS;
			npc->ym = Random( -VS/4, VS/4 );
			break;
		case 1:// ��
			npc->ym = -8*VS;
			npc->xm = Random( -VS/4, VS/4 );
			break;
		case 2:// ��
			npc->xm = 8*VS;
			npc->ym = Random( -VS/4, VS/4 );
			break;
		case 3:// ��
			npc->ym = 8*VS;
			npc->xm = Random( -VS/4, VS/4 );
			break;
		}
		break;

	case 1:

		switch( npc->direct ){
		case 0: if( npc->flag & FLAG_HIT_LEFT   ) bBreak = TRUE; break;
		case 1: if( npc->flag & FLAG_HIT_TOP    ) bBreak = TRUE; break;
		case 2: if( npc->flag & FLAG_HIT_RIGHT  ) bBreak = TRUE; break;
		case 3: if( npc->flag & FLAG_HIT_BOTTOM ) bBreak = TRUE; break;
		}
		npc->x += npc->xm;
		npc->y += npc->ym;

		break;
		
	}

	if( bBreak ){
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_RIGHT );
		PlaySoundObject( WAVE_SMOKE, 1 );
		npc->cond = 0;
	}

	npc->rect = rect[npc->direct];
}




// �T���u���b�N
void ActNpc124( NPCHAR *npc )
{
	RECT rect[] = {
		{160, 0, 192, 32},
		{192, 0, 224, 32},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->x += VS*8;
		npc->y += VS*8;
	case 1:
		npc->bits &= ~BITS_THROW_BLOCK;
		npc->ani_no = 0;
		break;
	case 10:
		npc->act_no   = 11;
		npc->ani_no   = 1;
		npc->act_wait = 0;
		npc->bits |= BITS_THROW_BLOCK;
	case 11:
		switch( npc->direct ){
		case DIR_LEFT:  npc->x -= VS/4; break;
		case DIR_UP:    npc->y -= VS/4; break;
		case DIR_RIGHT: npc->x += VS/4; break;
		case DIR_DOWN:  npc->y += VS/4; break;
		}
		npc->act_wait++;
		if( npc->act_wait%8 == 0 ) PlaySoundObject( WAVE_QUAKE, 1 );

		SetQuake( 20 );
		break;
	}

	npc->rect = rect[npc->ani_no];
}


// �V�[�N���b�g�G�i�W�[
void ActNpc125( NPCHAR *npc )
{
	if( npc->life < 990 ){
		SetDestroyNpChar( npc->x, npc->y, npc->view.back,  8 );
		PlaySoundObject( WAVE_DESTROY_SMALL, 1 );

		if( npc->direct == DIR_LEFT ) SetNpChar( 87, npc->x, npc->y, 0, 0, DIR_RIGHT, NULL, 0 ); // life
		else                          SetNpChar( 86, npc->x, npc->y, 0, 0, DIR_RIGHT, NULL, 0 ); // missile
		npc->cond = 0;
	}


	RECT rc[] = {
		{  0, 96, 16,112},
		{ 16, 96, 32,112},
	};
	
	if( npc->direct == DIR_LEFT ) npc->rect = rc[0]; // life
	else                          npc->rect = rc[1]; // missile


}

// �S����
void ActNpc126( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{ 48,144, 64,160},
		{ 64,144, 80,160},// �܂΂���

		{ 48,144, 64,160},// �V�b�|�t���t��
		{ 80,144, 96,160},


		{ 96,144,112,160},// ���� 7
		{112,144,128,160},

	};

	RECT rcRight[] = {

		{ 48,160, 64,176},
		{ 64,160, 80,176},// �܂΂���

		{ 48,160, 64,176},// �V�b�|�t���t��
		{ 80,160, 96,176},

		{ 96,160,112,176},// ����
		{112,160,128,176},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1:// �ҋ@ ---
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		// �߂Â��Ƃӂ�����c�B
		if( npc->x - PARTSSIZE*6*VS < gMC.x &&
			npc->x + PARTSSIZE*6*VS > gMC.x &&
			npc->y - PARTSSIZE*2*VS < gMC.y &&
			npc->y + PARTSSIZE*1*VS > gMC.y ){

			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}

		// ����ɋ߂Â��ƃ���
		if( npc->x - PARTSSIZE*2*VS < gMC.x &&
			npc->x + PARTSSIZE*2*VS > gMC.x &&
			npc->y - PARTSSIZE*2*VS < gMC.y &&
			npc->y + PARTSSIZE*1*VS > gMC.y ){

			if( npc->x > gMC.x ) npc->direct = DIR_RIGHT;
			else                 npc->direct = DIR_LEFT;
			npc->act_no = 10;
		}
		break;

	case 2:// �u��
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;


	case 10: //���� ----
		npc->act_no   = 11;
		npc->ani_no   = 4;
		npc->ani_wait = 0;
	case 11:
		// anime
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ani_wait++;
			if( npc->ani_wait > 2 ){
				npc->ani_wait = 0;
				npc->ani_no++;
			}
			if( npc->ani_no > 5 )
				npc->ani_no = 4;
		}else{
			npc->ani_no   = 5;
			npc->ani_wait = 0;
		}

		// hit
		if( npc->xm < 0 && npc->flag & FLAG_HIT_LEFT ){
			npc->xm = npc->xm/(-2);
			npc->direct = DIR_RIGHT;
		}
		if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT ){
			npc->xm = npc->xm/(-2);
			npc->direct = DIR_LEFT;
		}


		// move
		if( npc->direct == DIR_LEFT ) npc->xm -= VS/8;
		else                          npc->xm += VS/8;

		if( npc->xm >  MAX_MOVE ) npc->xm =  VS*2;
		if( npc->xm < -MAX_MOVE ) npc->xm = -VS*2;

		break;
	}

	if( gKeyTrg & gKeyDown ) npc->bits |=  BITS_EVENT_CHECK;
	else                     npc->bits &= ~BITS_EVENT_CHECK;

	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// �}�V���K���c��Level3
void ActNpc127( NPCHAR *npc )
{
	RECT rcV[] = {
		{112, 48,128, 64},// ��
		{112, 64,128, 80},
		{112, 80,128, 96},

	};

	RECT rcH[] = {
		{ 64, 80, 80, 96},// �c
		{ 80, 80, 96, 96},
		{ 96, 80,112, 96},

	};
	//Animation
	npc->ani_wait++;
	if( npc->ani_wait > 0 ){
		npc->ani_wait = 0;
		npc->ani_no++;
		if( npc->ani_no > 2 )
			npc->cond = 0;
	}

	//Rect
	if( npc->direct == DIR_LEFT ) npc->rect = rcH[npc->ani_no];
	else                          npc->rect = rcV[npc->ani_no];

}

// �}�V���K���c��Level3
void ActNpc128( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{0,0,0,0},
		{176, 16,184, 32},
		{184, 16,192, 32},
		{192, 16,200, 32},
		{200, 16,208, 32},
	};
	RECT rcRight[] = {
		{0,0,0,0},
		{232, 16,240, 32},
		{224, 16,232, 32},
		{216, 16,224, 32},
		{208, 16,216, 32},
	};
	RECT rcUp[] = {
		{0,0,0,0},
		{176, 32,192, 40},
		{176, 40,192, 48},
		{192, 32,208, 40},
		{192, 40,208, 48},
	};
	RECT rcDown[] = {
		{0,0,0,0},
		{208, 32,224, 40},
		{208, 40,224, 48},
		{224, 32,232, 40},
		{224, 40,232, 48},
	};

	if( !npc->act_no ){
		npc->act_no = 1;
		if( npc->direct == DIR_LEFT ||
			npc->direct == DIR_RIGHT ){
			npc->view.front = 4*VS;
			npc->view.top   = 8*VS;
		}else{
			npc->view.front = 8*VS;
			npc->view.top   = 4*VS;
		}

	}

	//Animation
	npc->ani_no++;
	if( npc->ani_no > 4 )
		npc->cond = 0;

	//Rect
	switch( npc->direct ){
	case DIR_LEFT:
		npc->rect = rcLeft[npc->ani_no];
		break;
	case DIR_UP:
		npc->rect = rcUp[npc->ani_no];
		break;
	case DIR_RIGHT:
		npc->rect = rcRight[npc->ani_no];
		break;
	case DIR_DOWN:
		npc->rect = rcDown[npc->ani_no];
		break;
	}

}

// �t�@�C�A�{�[���c��
void ActNpc129( NPCHAR *npc )
{
	RECT rect[] = {
		{128, 48,144, 64},
		{144, 48,160, 64},
		{160, 48,176, 64},
		
		{128, 64,144, 80},
		{144, 64,160, 80},
		{160, 64,176, 80},
		
		{128, 80,144, 96},
		{144, 80,160, 96},
		{160, 80,176, 96},

		// level3
		{176, 48,192, 64},
		{192, 48,208, 64},
		{208, 48,224, 64},
		
		{176, 64,192, 80},
		{192, 64,208, 80},
		{208, 64,224, 80},
		
		{176, 80,192, 96},
		{192, 80,208, 96},
		{208, 80,224, 96},
	
	};

	//Animation
	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
		if( npc->ani_no > 2 )
			npc->cond = 0;
	}
	npc->y += npc->ym;

	//Rect
	npc->rect = rect[ npc->direct*3 + npc->ani_no ];
}




// �S����(�t���t��)
void ActNpc130( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 48,144, 64,160},
		{ 64,144, 80,160},// �܂΂���
		{ 48,144, 64,160},// �V�b�|�t���t��
		{ 80,144, 96,160},
	};

	RECT rcRight[] = {
		{ 48,160, 64,176},
		{ 64,160, 80,176},// �܂΂���
		{ 48,160, 64,176},// �V�b�|�t���t��
		{ 80,160, 96,176},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->bits    |= BITS_EVENT_CHECK;
	case 1:// �ҋ@ ---
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		// ����ɋ߂Â��ƃt���t��
		if( npc->x - PARTSSIZE*4*VS < gMC.x && npc->x + PARTSSIZE*4*VS > gMC.x &&
			npc->y - PARTSSIZE*2*VS < gMC.y && npc->y + PARTSSIZE*1*VS > gMC.y ){

			npc->ani_wait++;
			if( npc->ani_wait > 3 ){
				npc->ani_wait = 0;
				npc->ani_no++;
			}
			if( npc->ani_no > 3 ){
				npc->ani_no = 2;
			}
		}
		// �߂Â��Ƃӂ�����c�B
		if( npc->x - PARTSSIZE*6*VS < gMC.x && npc->x + PARTSSIZE*6*VS > gMC.x &&
			npc->y - PARTSSIZE*2*VS < gMC.y && npc->y + PARTSSIZE*1*VS > gMC.y ){

			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		break;

	case 2:// �u��
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}
		break;
	}

	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

//�S�����i����j
void ActNpc131( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{144,144,160,160}, // ����
	};
	RECT rcRight[] = {
		{144,160,160,176}, // ����
	};
	npc->act_wait++;
	if( npc->act_wait > 100 ){
		npc->act_wait = 0;
		SetCaret( npc->x, npc->y, CARET_ZZZ, DIR_LEFT );
	}
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}


// �S����(��������)
void ActNpc132( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{ 48,144, 64,160},
		{ 64,144, 80,160},// �܂΂���

		{ 96,144,112,160},
		{ 96,144,112,160},
		{128,144,144,160},// ���������I


	};

	RECT rcRight[] = {

		{ 48,160, 64,176},
		{ 64,160, 80,176},// �܂΂���

		{ 96,160,112,176},
		{ 96,160,112,176},
		{128,160,144,176},// ���������I

	};


	if( npc->act_no < 100 ){
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	}

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1:// �ҋ@ ---
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		// ����ɋ߂Â��ƃ�������
		if( npc->x - PARTSSIZE*4*VS < gMC.x &&
			npc->x + PARTSSIZE*4*VS > gMC.x &&
			npc->y - PARTSSIZE*1*VS < gMC.y &&
			npc->y + PARTSSIZE*1*VS > gMC.y ){

			npc->ani_wait++;
			if( npc->ani_wait > 4 ){
				npc->ani_wait = 0;
				npc->ani_no++;
			}
			if( npc->ani_no > 4 ){
				npc->ani_no = 2;
			}
			if( npc->ani_no == 4 && npc->ani_wait == 0 )
				PlaySoundObject( WAVE_BOWWOW, 1 );
		}else{
			if( npc->ani_no == 4 )npc->ani_no = 2;
		}
		break;
	case 2:// �u��
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;

	case 10:
		npc->act_no   = 11;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 11:// �ҋ@ ---
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 12;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		break;
	case 12:// �u��
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 11;
			npc->ani_no = 0;
		}

		break;

	case 100://�Œ胏������
		npc->act_no = 101;
		npc->count1 = 0;
	case 101:
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 4 ){
			if( npc->count1 < 3 ){
				npc->ani_no = 2;
				npc->count1++;
			}else{
				npc->ani_no = 0;
				npc->count1 = 0;
			}
		}
		if( npc->ani_no == 4 && npc->ani_wait == 0 ){
			PlaySoundObject( WAVE_BOWWOW, 1 );
		}
		break;
	case 120://�Œ肨����
		npc->ani_no = 0;
		break;
	}

	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}


//�W�F���J
void ActNpc133( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{176, 32,192, 48}, // ����
		{192, 32,208, 48}, // �u��
	};
	RECT rcRight[] = {
		{176, 48,192, 64}, // ����
		{192, 48,208, 64}, // �u��
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1://�ҋ@
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		break;
	case 2://�u��
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}


// �A�[�}�[
void ActNpc134( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{224,  0,256, 16},
		{256,  0,288, 16},
		{288,  0,320, 16},// �ӂ�
	};
	RECT rcRight[] = {
		{224, 16,256, 32},
		{256, 16,288, 32},
		{288, 16,320, 32},// �ӂ�
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->ani_no = 2;
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->bits |=  BITS_BLOCK_BULLET;
	case 1:
		if( gMC.x > npc->x -VS*PARTSSIZE*20 &&
			gMC.x < npc->x +VS*PARTSSIZE*20 &&
			gMC.y > npc->y -VS*PARTSSIZE*10 &&
			gMC.y < npc->y +VS*PARTSSIZE* 4 ){
			npc->act_no = 10;
			npc->bits |=  BITS_BANISH_DAMAGE;
			npc->bits &= ~BITS_BLOCK_BULLET;

		}
		break;

	case 10:// ����
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;

		// �����]��
		if( npc->direct == DIR_LEFT && 
			npc->flag & FLAG_HIT_LEFT ) npc->direct = DIR_RIGHT;
		if( npc->direct == DIR_RIGHT && 
			npc->flag & FLAG_HIT_RIGHT ) npc->direct = DIR_LEFT;

		if( npc->direct == DIR_LEFT ) npc->x += -VS/2;
		else                          npc->x +=  VS/2;

		if( CountArmsBullet( 6 ) ){
			npc->act_no   = 20;
			npc->act_wait = 0;
			npc->ani_no   = 2;
			npc->bits &= ~BITS_BANISH_DAMAGE;
			npc->bits |=  BITS_BLOCK_BULLET;
		}

		break;
	case 20:// �ӂ�
		npc->act_wait++;
		if( npc->act_wait > 100 ){
			npc->act_no   = 10;
			npc->ani_no   =  0;
			npc->ani_wait =  0;
			npc->bits |=  BITS_BANISH_DAMAGE;
			npc->bits &= ~BITS_BLOCK_BULLET;
		}
		break;
	}

	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}


// �X�P���g��
void ActNpc135( NPCHAR *npc )
{
	unsigned char deg;
	long xm, ym;
	RECT rcLeft[] = {
		{256, 32,288, 64},
		{288, 32,320, 64},// �Ƃ�
	};
	RECT rcRight[] = {
		{256, 64,288, 96},
		{288, 64,320, 96},// �Ƃ�
	};


	if( gMC.x < npc->x -VS*PARTSSIZE*22 ||
		gMC.x > npc->x +VS*PARTSSIZE*22 ||
		gMC.y < npc->y -VS*PARTSSIZE*10 ||
		gMC.y > npc->y +VS*PARTSSIZE* 4 ){
		npc->act_no = 0;
	}



	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->xm = 0;
	case 1:
		if( gMC.x > npc->x -VS*PARTSSIZE*20 &&
			gMC.x < npc->x +VS*PARTSSIZE*20 &&
			gMC.y > npc->y -VS*PARTSSIZE*10 &&
			gMC.y < npc->y +VS*PARTSSIZE* 4 ){
			npc->act_no = 10;
		}
		if( npc->flag & FLAG_HIT_BOTTOM ) npc->ani_no = 0;
		break;

	case 10:
		npc->xm = 0;
		npc->act_no   = 11; 
		npc->act_wait = 0;
		npc->ani_no   = 0;
	case 11:
		npc->act_wait++;
		if( npc->act_wait >= 5 && npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 20;
			npc->ani_no = 1;
			npc->count1 = 0;
			npc->ym = -VS*Random( 1, 3 );
			if( npc->shock ){
				if( npc->x < gMC.x ) npc->xm -= VS/2;
				else                 npc->xm += VS/2;
			}else{
				if( npc->x < gMC.x ) npc->xm += VS/2;
				else                 npc->xm -= VS/2;
			}
		}
		break;

	case 20:
		if( npc->ym > 0 && npc->count1 == 0 ){
			npc->count1++;
			deg = GetArktan( npc->x - gMC.x, npc->y+4*VS - gMC.y );

			ym = GetSin( deg ) * 2;
			xm = GetCos( deg ) * 2;
			SetNpChar( 50, npc->x, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC*3/4 );
			PlaySoundObject( WAVE_POP, 1 );
		}

		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 10;
			npc->ani_no = 0;
		}
		break;
	}
	if( npc->act_no >= 10 ){
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	}

	npc->ym += VS/10;
	if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;
	if( npc->xm >  MAX_MOVE ) npc->xm =  MAX_MOVE;
	if( npc->xm < -MAX_MOVE ) npc->xm = -MAX_MOVE;
	
	npc->y += npc->ym;
	npc->x += npc->xm;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}





// �S����(�����)
void ActNpc136( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 48,144, 64,160},
		{ 64,144, 80,160},// �܂΂���
	};

	RECT rcRight[] = {
		{ 48,160, 64,176},
		{ 64,160, 80,176},// �܂΂���
	};

	switch( npc->act_no ){
	case 0:
		npc->bits &= ~BITS_EVENT_CHECK;
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1:// �ҋ@ ---
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		break;
	case 2:// �u��
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}
		break;

	}


	if( gMC.direct == DIR_LEFT ) npc->direct = DIR_LEFT;
	else                         npc->direct = DIR_RIGHT;

	npc->y = gMC.y -VS*10;

	if( npc->direct == DIR_LEFT ){
		npc->x = gMC.x + 4*VS;
		npc->rect = rcLeft[ npc->ani_no];
	}else{
		npc->x = gMC.x - 4*VS;
		npc->rect = rcRight[npc->ani_no];
	}
	if( gMC.ani_no%2 )npc->rect.top += 1;

}

// ��
void ActNpc137( NPCHAR *npc )
{
	RECT rc = {96,136,128,188};
	npc->rect = rc;
}

// ��
void ActNpc138( NPCHAR *npc )
{
	RECT rcLeft  = { 96,112,112,136};
	RECT rcRight = {112,112,128,136};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		if( npc->direct == DIR_LEFT ){
			npc->rect = rcLeft;
			npc->x += VS*8;
		}else{
			npc->rect = rcRight;
			npc->x -= VS*8;
		}
		npc->tgt_x = npc->x;
	case 1:
		break;
	case 10:
		npc->act_no   = 11;
		npc->ani_no   = 1;
		npc->act_wait = 0;
		npc->bits |= BITS_THROW_BLOCK;
	case 11:
		npc->act_wait++;
		if( npc->act_wait%8 == 0 ) PlaySoundObject( WAVE_QUAKE, 1 );
	//	SetQuake( 20 );

		if( npc->direct == DIR_LEFT ){
			npc->rect = rcLeft;
			npc->rect.left += npc->act_wait/8;
		}else{
			npc->x = npc->tgt_x + (npc->act_wait/8)*VS;
			npc->rect = rcRight;
			npc->rect.right -= npc->act_wait/8;
		}
		if( npc->act_wait == 8*13 ){
			npc->cond = 0;
		}
		
		break;
	}
}

// �h�N�^�[�i�C�x���g�j
void ActNpc139( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,128, 24,160},// �Î~
		{ 24,128, 48,160},// �΂�
		{ 48,128, 72,160},// ���V
	};
	RECT rcRight[] = {
		{  0,160, 24,192},// �Î~
		{ 24,160, 48,192},// �΂�
		{ 48,160, 72,192},// ���V
	};

	switch( npc->act_no ){
	case 0://�Î~ -------------
		npc->act_no = 1;
		npc->xm = 0;
		npc->ym = 0;
		npc->y -= VS*8;
	case 1:
		if( npc->flag & FLAG_HIT_BOTTOM ) npc->ani_no = 0;
		else                              npc->ani_no = 2;
		npc->ym += VS/8;
		break;

	case 10://�΂� -------------
		npc->act_no   = 11;
		npc->ani_no   = 1;
		npc->ani_wait = 0;
		npc->count1 = 0;
	case 11:
		npc->ani_wait++;
		if( npc->ani_wait > 6 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 ){
			npc->ani_no = 0;
			npc->count1++;
		}
		if( npc->count1 > 8 ){
			npc->ani_no = 0;
			npc->act_no = 1;
		}
		break;

	case 20://���V ---------------
		npc->act_no   =  21;
		npc->act_wait =   0;
		npc->ani_no   =   2;
		npc->tgt_y    = npc->y - 32*VS;
	case 21:
		if( npc->y < npc->tgt_y ) npc->ym += VS/16;
		else                      npc->ym -= VS/16;
		if( npc->ym >  VS ) npc->ym =  VS;
		if( npc->ym < -VS ) npc->ym = -VS;
		break;

	case 30:// �t�F�[�h�A�E�g-------
		npc->act_no = 31;
		npc->xm = 0;
		npc->ym = 0;
		npc->act_wait = (npc->rect.bottom - npc->rect.top)*2;
		PlaySoundObject( WAVE_WARP, 1 );
	case 31:// 
		npc->act_wait--;
		npc->ani_no   = 0;
		if( !npc->act_wait ) npc->cond = 0;
		break;

	case 40:// �t�F�[�h�C��-------
		npc->act_no = 41;
		npc->act_wait = 0;
		npc->xm = 0;
		npc->ym = 0;
		PlaySoundObject( WAVE_WARP, 1 );
	case 41:// 
		npc->ani_no   = 2;
		npc->act_wait++;
		if( npc->act_wait < 32*2 ){
		}else npc->act_no = 20;
		break;
	}
//	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];


	if( npc->act_no == 31 || npc->act_no == 41 ){
		npc->rect.bottom = npc->rect.top + npc->act_wait/2;
		if( npc->act_wait/2%2 ) npc->rect.left++;
	}


}
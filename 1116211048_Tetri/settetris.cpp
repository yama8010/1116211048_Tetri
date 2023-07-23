#include "settetris.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

SHAPE shapes[SHAPE_MAX] = {
	// SHAPE_I
	{
		4,4,//int width, height
		// int pattrn[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX]
		{
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		}
	},
	// SHAPE_O
	{
		2,2,
		{
			{1,1},
			{1,1}
		}
	},
	// SHAPE_S
	{
		3,3,//int width, height
		// int pattrn[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX]
		{
			{0,1,1},
			{1,1,0},
			{0,0,0}
		}
	},
	// SHAPE_Z
	{
		3,3,//int width, height
		// int pattrn[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX]
		{
			{1,1,0},
			{0,1,1},
			{0,0,0}
		}
	},
	// SHAPE_J
	{
		3,3,//int width, height
		// int pattrn[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX]
		{
			{1,0,0},
			{1,1,1},
			{0,0,0}
		}
	},
	// SHAPE_L
	{
		3,3,//int width, height
		// int pattrn[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX]
		{
			{0,0,1},
			{1,1,1},
			{0,0,0}
		}
	},
	// SHAPE_T
	{
		3,3,//int width, height
		// int pattrn[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX]
		{
			{0,1,0},
			{1,1,1},
			{0,0,0}
		}
	}
};



/* mino��field�̓����蔻��֐�*/
bool MinoIntersectField() {
	for (int y = 0; y < mino.shape.height; y++)
		for (int x = 0; x < mino.shape.width; x++)  //mino�̌`��
			if (mino.shape.pattern[y][x]) {
				if (mino.y + y < 0 || (mino.y + y >= FIELD_HEIGHT)
					|| (mino.x + x < 0) || (mino.x + x >= FIELD_WIDTH))
					return true;

				if (field[mino.y + y][mino.x + x]) //�s���̐�Ƀu���b�N������Ȃ��true��Ԃ�
					return true;
			}

	return false;
}

void DrawScreen() {

	COORD consoleSize = GetConsoleSize();

	// �e�g���X�̃t�B�[���h�̍�����W���v�Z
	//int fieldStartX = (consoleSize.X - FIELD_WIDTH) / 2;
	int fieldStartX = (consoleSize.X - 20) / 2;
	int fieldStartY = (consoleSize.Y - FIELD_HEIGHT) / 2;

	memcpy(screen, field, sizeof field);

	for (int y = 0; y < mino.shape.height; y++)
		for (int x = 0; x < mino.shape.width; x++)
			if (mino.shape.pattern[y][x])
				screen[mino.y + y][mino.x + x] |= 1;

	system("cls");



	for (int y = 0; y < FIELD_HEIGHT; y++) {
		printf("%*s", fieldStartX, ""); // ���[�̗]�����o��
		printf("��");
		for (int x = 0; x < FIELD_WIDTH; x++)
			printf("%s", screen[y][x] ? "��" : "�@"); //�X�N���[���ɕ��������邩�ǂ����m�F
		printf("��");
		printf("\n");
	}
	printf("%*s", fieldStartX, "");

	for (int x = 0; x < FIELD_WIDTH + 2; x++) {
		printf("��");
	}

	printf("\n\n");
	printf("%*s%d", fieldStartX, "SCORE : ", score);
	printf("\t�o�ߎ��� : %d �b", lastClock / 1000);
	printf("\t  Level : %d", fps);
	printf("\n\n");
	printf("%*s", fieldStartX + 30, "���L�[: ����  \n");
	printf("%*s", fieldStartX + 30, "���L�[: �E    \n");
	printf("%*s", fieldStartX + 30, "���L�[: ��    \n");
	printf("%*s", fieldStartX + 30, "���L�[: ��  �@\n");
	printf("%*s", fieldStartX + 30, " f    : ��]  \n");
}

void InitMino() {
	mino.shape = shapes[rand() % SHAPE_MAX];
	mino.x = (FIELD_WIDTH - mino.shape.width) / 2; //mino��x���W���^�񒆕ӂ�ɗ���
	mino.y = 0;
}





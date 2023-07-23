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



/* minoとfieldの当たり判定関数*/
bool MinoIntersectField() {
	for (int y = 0; y < mino.shape.height; y++)
		for (int x = 0; x < mino.shape.width; x++)  //minoの形状
			if (mino.shape.pattern[y][x]) {
				if (mino.y + y < 0 || (mino.y + y >= FIELD_HEIGHT)
					|| (mino.x + x < 0) || (mino.x + x >= FIELD_WIDTH))
					return true;

				if (field[mino.y + y][mino.x + x]) //行動の先にブロックがあるならばtrueを返す
					return true;
			}

	return false;
}

void DrawScreen() {

	COORD consoleSize = GetConsoleSize();

	// テトリスのフィールドの左上座標を計算
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
		printf("%*s", fieldStartX, ""); // 左端の余白を出力
		printf("□");
		for (int x = 0; x < FIELD_WIDTH; x++)
			printf("%s", screen[y][x] ? "■" : "　"); //スクリーンに文字があるかどうか確認
		printf("□");
		printf("\n");
	}
	printf("%*s", fieldStartX, "");

	for (int x = 0; x < FIELD_WIDTH + 2; x++) {
		printf("□");
	}

	printf("\n\n");
	printf("%*s%d", fieldStartX, "SCORE : ", score);
	printf("\t経過時間 : %d 秒", lastClock / 1000);
	printf("\t  Level : %d", fps);
	printf("\n\n");
	printf("%*s", fieldStartX + 30, "↑キー: 落下  \n");
	printf("%*s", fieldStartX + 30, "→キー: 右    \n");
	printf("%*s", fieldStartX + 30, "←キー: 左    \n");
	printf("%*s", fieldStartX + 30, "↓キー: 下  　\n");
	printf("%*s", fieldStartX + 30, " f    : 回転  \n");
}

void InitMino() {
	mino.shape = shapes[rand() % SHAPE_MAX];
	mino.x = (FIELD_WIDTH - mino.shape.width) / 2; //minoのx座標が真ん中辺りに来る
	mino.y = 0;
}





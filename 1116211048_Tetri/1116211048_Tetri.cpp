#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include "drawingteto.h"

#define CHARBUFF 124

#define FIELD_WIDTH  10
#define FIELD_HEIGHT 20

#define SHAPE_WIDTH_MAX  4
#define SHAPE_HEIGHT_MAX 4

void getGurrentDirectory(char* currentDirectory) {
	GetCurrentDirectory(CHARBUFF, currentDirectory);
}
// コンソールのサイズを取得する関数
COORD GetConsoleSize() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	COORD size = { consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1,
				   consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1 };
	return size;
}

enum {
	SHAPE_I,
	SHAPE_O,
	SHAPE_S,
	SHAPE_Z,
	SHAPE_J,
	SHAPE_L,
	SHAPE_T,
	SHAPE_MAX
};

struct SHAPE {
	int width;
	int height;
	int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
};

struct MINO {
	int x;
	int y;
	SHAPE shape;
};
/* minoの形状定義 */
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

int score = 0;
int fps = 1;
int interval = 1000 / fps;
int field[FIELD_HEIGHT][FIELD_WIDTH];
int screen[FIELD_HEIGHT][FIELD_WIDTH];

clock_t lastClock = clock(); //gameが起動してから何ms経過したか

MINO mino;

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
void Init() {
	memset(field, 0, sizeof field);
	InitMino();
	DrawScreen();
}

void MinoFall() {
	while (!MinoIntersectField()) {
		mino.y++;
	}
	mino.y--;
}


void LevelUp() {
	static int prevScore = 0; // 前回のスコアを保持する変数
    static int targetScore = 500; // 次にスコアを増やす目標のスコアを保持する変数
	static int initialInterval = 1000; // ミノの初期落下間隔（ミリ秒）
	int prevtargetScore = 0;

	if (score >= targetScore) {
		fps++; // FPSを増やす
		targetScore += 500; // 次の目標スコアを更新
		initialInterval -= 50; // ミノの初期落下間隔を減らす（ミリ秒単位）
	}

	prevtargetScore = targetScore;
	//prevScore = score; // 前回のスコアを更新
	interval = initialInterval; // ミノの落下間隔を初期間隔に戻す
}

int main() {

	drawLargeTETRI();
	drawStart();
	

	while (1) {
		if (_kbhit()) {
			char ch = _getch();
			if (ch == '\r') { // Enterキーが押されたら終了
				break;
			}
		}
	}
	srand((unsigned int)time(NULL));
	Init();
	while (1) {

		clock_t nowClock = clock();

		if (nowClock >= lastClock + interval) {
			lastClock = nowClock;
			LevelUp();
			MINO lastMino = mino;
			mino.y++;

			if (MinoIntersectField()) {
				mino = lastMino;

				for (int y = 0; y < mino.shape.height; y++)
					for (int x = 0; x < mino.shape.width; x++)
						if (mino.shape.pattern[y][x])
							field[mino.y + y][mino.x + x] |= 1;

				for (int y = 0; y < FIELD_HEIGHT; y++) {
					bool completed = true;
					for (int x = 0; x < FIELD_WIDTH; x++)
						if (!field[y][x]) {
							completed = false;
							break;
						}
					/* 1列揃えた時の処理 */
					if (completed) {
						for (int x = 0; x < FIELD_WIDTH; x++)
							field[y][x] = 0;

						/*1列消すと下に落とす*/
						for (int y2 = y; y2 >= 1; y2--)
							for (int x = 0; x < FIELD_WIDTH; x++) {
								field[y2][x] = field[y2 - 1][x];
								field[y2 - 1][x] = 0;
							}
						score += 100;
					}
				}
				InitMino();
			}

			DrawScreen();
		}

		if (_kbhit()) {
			MINO lastMino = mino;
			switch (_getch()) {
			case 72: //上矢印キー
				MinoFall();
				break;
			case 80: //下矢印キー
				mino.y++;
				break;
			case 75: //左矢印キー
				mino.x--;
				break;
			case 77: //右矢印キー
				mino.x++;
				break;
			case 'f': //回転
			{
				MINO newMino = mino;
				for (int y = 0; y < mino.shape.height; y++)
					for (int x = 0; x < mino.shape.width; x++)
						newMino.shape.pattern[mino.shape.width - 1 - x][y] = mino.shape.pattern[y][x];
				mino = newMino;
			}
			break;
			}

			if (MinoIntersectField()) //minoが当たったら前の位置にする
				mino = lastMino;

			DrawScreen();
		}
		if (field[0][4] == 1) {
			break;
		}
	}

	system("cls");

	drawGameover();
	printf("\n\n");
	printf("\tSCORE : %d\n", score);
	printf("\t経過時間 : %d 秒", lastClock / 1000);
	printf("\tLEVEL : %d\n", fps);

	char currentDirectory[CHARBUFF];
	getGurrentDirectory(currentDirectory);

	//inifileでのcsvファイルの読み込み
	char section[CHARBUFF];
	sprintf_s(section, "section1");
	char keyWord[CHARBUFF];
	sprintf_s(keyWord, "keyword1");
	char settingFile[CHARBUFF];
	sprintf_s(settingFile, "%s\\score.ini", currentDirectory);
	char keyValue[CHARBUFF];
	if (GetPrivateProfileString(section, keyWord, "none", keyValue, CHARBUFF, settingFile) != 0) {
		//fprintf(stdout, "%s\n%s\n", settingFile, keyValue); // iniファイルの読み込み確認
	}
	else {
		//fprintf(stdout, "%s doesn't contain [$s] %s\n", settingFile, section, keyWord);
	}

	//== csvファイルに書き込み ==
	FILE* fp;
	errno_t error;

	error = fopen_s(&fp, keyValue, "w");
	if (error != 0) {
		fprintf_s(stderr, "failed to open");
	}
	else {
		fputs("score\n", fp);
		fprintf_s(fp, "%d\n", score);
		fputs("経過時間\n", fp);
		fprintf_s(fp, "%d秒\n", lastClock / 1000);
		fputs("level\n", fp);
		fprintf_s(fp, "%d\n", fps);
		fclose(fp);
	}
};
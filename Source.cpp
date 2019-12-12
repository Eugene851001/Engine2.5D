#include "Engine.h"
#include "Enemy.h"
#include "EnemySoldier.h"
#include "Bullet.h"
#include "EventHadler.h"
#include "Menu.h"
#include "Levels.h"
#include "Pause.h"
#include "GameOver.h"
#include "Records.h"
#include "EDS.h"
#include "Sound.h"
#define _CRT_SECURE_NO_WARNINGS

//TODO: add to Engine.h
int MapWidth = 16;
int MapHeigth = 16;
float DepthBuf[ScreenWidth];
Player player;
Sound *sound;
int KillsCounter;
float TotalTime;
int Siganture;
Records records;

CONSOLE_FONT_INFOEX font;
HANDLE hConsole;
char* sndBuf;
const char* SoundFileName = "gun-gunshot-01.wav";
const char* RecordsFileName = "Records.txt";
GameState gameState;
//---------------------------

using namespace std;

string map = "";
list<Enemy*> enemyes;
list<Bullet> bullets;
DWORD note;

void CreateMap()
{
	map += "################";
	map += "#...#..........#";
	map += "#...#..........#";
	map += "#...#..........#";
	map += "#..............#";
	map += "#..............#";
	map += "#.....###......#";
	map += "#..............#";
	map += "#..............#";
	map += "#.......#......#";
	map += "#..............#";
	map += "#.........#....#";
	map += "#.........#....#";
	map += "#.........######";
	map += "#..............f";
	map += "################";
}

void DrawMap(char* screen, int MapHeight, int MapWidth, int ScreenWidth)
{
	int y, x, i;
	i = 0;
	for (y = 0; y < MapHeight; y++)
	{
		for (x = 0; x < MapWidth; x++)
		{
			screen[i] = map[y * MapWidth + x];
			i++;
		}
		i = y * ScreenWidth;
	}
	screen[(int)player.fY * ScreenWidth - ScreenWidth + (int)player.fX] = 'P';
}

bool isCollision(float x, float y)
{
	bool isCollision = 0;
	list<Enemy*>::iterator p = enemyes.begin();
	while ((p != enemyes.end()) && (!isCollision) && (enemyes.size() > 0))
	{
		float size = (*p)->getSize();
		if (x > (*p)->getX() - size && x < (*p)->getX() + size
			&& y >(*p)->getY() - size && y < (*p)->getY() + size)
			isCollision = true;
		p++;
	}
	return isCollision;
}

bool IsWall(float x, float y)
{
	return map[(int)y * MapWidth + (int)x] == '#' 
		|| map[(int)y * MapWidth + (int)x] == 'f';
}

void MovePlayer(float time)
{
	if (GetAsyncKeyState('F'))
	{
		float testX = player.fX + 0.1f * cosf(player.fAngle);
		float testY = player.fY + 0.1f * sinf(player.fAngle);
		if (map[(int)testY * MapWidth + (int)testX] == 'f')
		{
			gameState = gsGameOver;
		}
	}
	if (GetAsyncKeyState(VK_ESCAPE))
		gameState = gsPause;
	if (GetAsyncKeyState('A'))
		player.fAngle -= player.fAngleSpeed * time;
	if (GetAsyncKeyState('D'))
		player.fAngle += player.fAngleSpeed * time;
	if (GetAsyncKeyState('W'))
	{
		float testX = player.fX + player.fSpeed * cosf(player.fAngle) * time;
		float testY = player.fY + player.fSpeed * sinf(player.fAngle) * time;
		if (!IsWall(testX, testY) && !isCollision(testX, testY))
		{
			player.fX = testX;
			player.fY = testY;
		}
	}
	if (GetAsyncKeyState('S'))
	{
		float testX = player.fX - player.fSpeed * cosf(player.fAngle) * time;
		float testY = player.fY - player.fSpeed * sinf(player.fAngle) * time;
		if (!IsWall(testX, testY) && !isCollision(testX, testY))
		{
			player.fX = testX;
			player.fY = testY;
		}
	}
	if (GetAsyncKeyState(' '))
	{
		//	Player.tmAfterShoot += time;
		if (player.tmReload < player.tmAfterShoot)
		{
			sound->Play(sndShoot);
			float b_x = player.fX + ( player.fSize + 0.1f) * cosf(player.fAngle);
			float b_y = player.fY + (player.fSize + 0.1f) * sinf(player.fAngle);
			Bullet bullet(b_x, b_y, cosf(player.fAngle), sinf(player.fAngle));
			bullets.push_back(bullet);
			player.tmAfterShoot = 0;
		}
	}
	player.tmAfterShoot += time;
}

void LoadMyRecords()
{
	bool isCorrect = true;
	char temp;
	int size;
	EDS eds;
	Record tempRec;
	Record tempRecs[4];
	std::ifstream fin(RecordsFileName);
	if (fin.is_open())
	{
		for (int i = 0; i < 4; i++)
		{
			tempRec = records.GetRecord(0);
			if (fin >> tempRec.kills && fin >> tempRec.time)
				records.SetRecord(i, tempRec);
			else
				isCorrect = false;
		}
		if (!(fin >> temp) && isCorrect)
			isCorrect = false;
		if (temp == EDSSign && isCorrect)
		{
			if (isCorrect && !(fin >> Siganture))
				isCorrect = false;
			for (int i = 0; i < 4; i++)
			{
				tempRecs[i] = records.GetRecord(i);
			}
			if (isCorrect && eds.GetHash((char*)tempRecs, 4 * sizeof(Record)) != Siganture)
				isCorrect = false;
		}
		else
		{
			isCorrect = false;
		}
	}
	else
	{
		isCorrect = false;
	}
	fin.close();
	if (!isCorrect)
	{
		records.SetDefaultRecords();
	}
}

void SaveRecords()
{
	FILE* f;
	if (fopen_s(&f, RecordsFileName, "w"))
	{
		EDS eds;
		Record tempRecs[4];
		Record tempRec;
		for (int i = 0; i < 4; i++)
		{
			tempRecs[i] = records.GetRecord(i);
		}
		Siganture = eds.GetHash((char*)tempRecs, 4 * sizeof(Record));
		for (int i = 0; i < 4; i++)
		{
			tempRec = records.GetRecord(i);
			fprintf_s(f, "%d\n", tempRec.kills);
			fprintf_s(f, "%d\n", tempRec.time);
		}
		fprintf_s(f, "%c\n", EDSSign);
		fprintf_s(f, "%d\n", Siganture);
		fclose(f);
	}
}

void LoadSound()
{
	int size;
	ifstream f(SoundFileName);
	if (f.is_open())
	{
		f.seekg(0, ios_base::end);
		size = f.tellg();
		sndBuf = new char[size];
		f.seekg(0, ios_base::beg);
		f.read(sndBuf, size);
		f.close();
	}
}

void PrintLine(char* screen, int ScreenWidth, char *strBuf, int left, int height)
{
	int i, j = 0;
	for (i = ScreenWidth - left; i < ScreenWidth && strBuf[j] != '\0'; i++, j++)
		screen[i + height * ScreenWidth] = strBuf[j];
}

void DrawInterface(char* screen, float time)
{
	if (!time)
		time = 1;
	time = time / CLOCKS_PER_SEC;
	float FPS = 1000 / time;
	char strBuf[20] = "         ";
	sprintf_s(strBuf, "%f", FPS);
	int i = 0, j = 0;
	PrintLine(screen, ScreenWidth, strBuf, 15, 0);
	int angle = player.fAngle * 180 / PI;
	j = 0;
	_itoa_s(angle, strBuf, 10);
	PrintLine(screen, ScreenWidth, strBuf, 10, 1);
	sprintf_s(strBuf, "%f", player.fX);
	screen[ScreenWidth * 2 + ScreenWidth - 10] = 'X';
	PrintLine(screen, ScreenWidth, strBuf, 5, 2);
	screen[ScreenWidth * 3 + ScreenWidth - 10] = 'Y';
	sprintf_s(strBuf, "%f", player.fY);
	PrintLine(screen, ScreenWidth, strBuf, 5, 3);
	sprintf_s(strBuf, "%d", player.HP);
	PrintLine(screen, ScreenWidth, strBuf, 3, 4);
}

void DrawWalls(char* screen)
{
	int x;
	for (x = 0; x < ScreenWidth; x++)
	{
		float fRayAngle = (player.fAngle - fViewAngle / 2.0f) + ((float)x / ScreenWidth) * fViewAngle;

		float fWallDistance = 0;
		bool isWall = false;
		int TestX;
		int TestY;
		while (!isWall)
		{
			fWallDistance += 0.1f;
			TestX = (int)(player.fX + cosf(fRayAngle) * fWallDistance);
			TestY = (int)(player.fY + sinf(fRayAngle) * fWallDistance);

			if (TestX < 0 || TestX >= MapWidth || TestY < 0 || TestY >= MapHeigth)
			{
				isWall = true;
				fWallDistance = fDepth;
			}
			else
			{
				if (IsWall((float)TestX, (float)TestY))
					isWall = true;
			}
		}
		DepthBuf[x] = fWallDistance;
		int Ceiling = (float)(ScreenHeight / 2.0) - ScreenHeight / ((float)fWallDistance);
		int Floor = ScreenHeight - Ceiling;

		int y;
		char WallShade;
		if (fWallDistance <= 5)
			WallShade = 178;
		else
			if (fWallDistance <= 10)
				WallShade = 177;
			else
				WallShade = 176;
		if (map[(int)TestY * MapWidth + (int)TestX] == 'f')
			WallShade = 'F';
		for (y = 0; y < ScreenHeight; y++)
		{

			if (y <= Ceiling)
				screen[y * ScreenWidth + x] = ' ';
			else if (y > Ceiling && y <= Floor && fWallDistance <= fViewRad)
				screen[y * ScreenWidth + x] = WallShade;
			else
			{
				char FloorShade;
				if (y >= ScreenHeight - ScreenHeight / (2 * 3))
					FloorShade = '#';
				else
					if (y >= ScreenHeight - ScreenHeight / 4)
						FloorShade = 'X';
					else
						FloorShade = '.';
				screen[y * ScreenWidth + x] = FloorShade;
			}
		}
	}
}


//does not work correctly
void DrawEnemy(char* screen, Enemy* enemy)
{
	while (player.fAngle > 2 * PI)
		player.fAngle -= 2 * PI;
	while (player.fAngle < 0)
		player.fAngle += 2 * PI;
	float MainAlpha = atan2(enemy->getY() - player.fY, enemy->getX() - player.fX);
	if (MainAlpha < 0)
		MainAlpha += 2 * PI;
	bool isVisual = false;
	float fEnemyDistance = sqrt(pow(enemy->getX() - player.fX, 2) +
		pow(enemy->getY() - player.fY, 2));
	float j;
	float N = fEnemyDistance / (120.0f + 60.0f);//replace 60.0f
	for (j = -enemy->getSize(); j < enemy->getSize(); j += N)
	{
		float alpha = atan2(j, fEnemyDistance) + MainAlpha;
		float CheckLow = player.fAngle - fViewAngle / 2.0f;
		float CheckHigh = player.fAngle + fViewAngle / 2.0f;
		if (CheckLow < 0)
			if (alpha < PI / 2.0f && alpha < CheckHigh)
				isVisual = true;
			else
				if (alpha > CheckLow + 2 * PI)
					isVisual = true;
		if ((alpha > CheckLow && alpha < CheckHigh) || isVisual)//rename identificators 
		{
			screen[ScreenWidth / 2] = '0';
			//fEnemyDistance = sqrt(pow(enemy.x - Player.fX, 2) + pow(enemy.y - Player.fY, 2));
			int Ceiling = ScreenHeight / 2.0f - ScreenHeight / (fEnemyDistance);
			int Floor = ScreenHeight - Ceiling;
			int Height = Floor - Ceiling;
			int textureX = (j + enemy->getSize()) * enemy->getTextureWidth()
				/ (2 * enemy->getSize());
			int y;
			int x = (alpha - CheckLow) * ScreenWidth / (CheckHigh - CheckLow);
			for (y = Ceiling; y < Floor; y++)
			{
				if (!(y > ScreenHeight || y < 0))
				{
					int textureY = (y - Ceiling) * enemy->getTextureHeight() / Height;
					char Shade = enemy->texture[textureY][textureX];
					if (Shade != EMPTY && DepthBuf[x] > fEnemyDistance)
					{
						screen[y * ScreenWidth + x] = Shade;//ENEMY_SHADE;
					//	DepthBuf[x] = fEnemyDistance;
					}
				}
			}
		}
	}
}

void DrawEnemyes(char* screen)
{
	for (Enemy* pEnemy : enemyes)
	{
		DrawEnemy(screen, pEnemy);
	}
}


void SortEnemyes()
{
	Enemy* temp = new Enemy(0.0f, 0.0f, 0.0f);
	for (Enemy* pEnemyI : enemyes)
	{
		for (Enemy* pEnemyJ : enemyes)
			if (*pEnemyI > * pEnemyJ)
			{
				*temp = *pEnemyI;
				*pEnemyI = *pEnemyJ;
				*pEnemyJ = *temp;
			}
	}
}


void UpdateEnemyes()
{
	enemyes.sort([](const Enemy* en1, const Enemy* en2) {return *en1 < *en2; });
	float size;
	for (Enemy* pEnemy : enemyes)
	{
		for (Bullet &bullet : bullets)
		{
			float size = pEnemy->getSize();
			if (bullet.getX() > pEnemy->getX() - size
				&& bullet.getX() < pEnemy->getX() + size
				&& bullet.getY() < pEnemy->getY() + size
				&& bullet.getY() > pEnemy->getY() - size)
			{
				KillsCounter++;
				pEnemy->setDestroy(true);
				bullet.setDestroy(true);
			}
		}
	}
	enemyes.remove_if([](Enemy* en) {
		if (en->getDestroy())
		{
			delete(en);
			return true;
		}
		return false; });
	
}

void DrawCircle(int x, int y, int Rad, char shade, char* screen)
{
	int i, width;
	for (i = 0; i < Rad && i < (ScreenHeight / 2); i++)
	{
		width = (int)sqrt(Rad * Rad - i * i);
		int j;
		for (j = 0; j < width && j < (ScreenWidth / 2); j++)
		{
			if (j + x < ScreenWidth)
			{
				screen[(i + y) * ScreenWidth + j + x] = shade;
				screen[(-i + y) * ScreenWidth + j + x] = shade;
			}
			if (x - j > 0)
			{
				screen[(i + y) * ScreenWidth - j + x] = shade;
				screen[(-i + y) * ScreenWidth - j + x] = shade;
			}
		}
	}
}

void UpdateBullets()
{
	for (Bullet& bullet : bullets)
	{
		if (bullet.getX() > player.fX - player.fSize
			&& bullet.getX() < player.fX + player.fSize
			&& bullet.getY() > player.fY - player.fSize
			&& bullet.getY() < player.fY + player.fSize)
		{
			if (!bullet.getDestroy())
			{
				bullet.setDestroy(true);
				player.HP -= bullet.GetDamage();
			}
		}
	}
	bullets.remove_if([](Bullet &bullet){return bullet.getDestroy();});//explain
}

void DrawBullets(char* screen)
{
	float x, y;
	for (Bullet bullet: bullets)
	{
		x = bullet.getX();
		y = bullet.getY();
		while (player.fAngle > 2 * PI)
			player.fAngle -= 2 * PI;
		while (player.fAngle < 0)
			player.fAngle += 2 * PI;
		float alpha = atan2(y - player.fY, x - player.fX);
		if (alpha < 0)
			alpha += 2 * PI;
		bool isVisual = false;
		float fBulletDistance = sqrt(pow(x - player.fX, 2) + pow(y - player.fY, 2));
		float CheckLow = player.fAngle - fViewAngle / 2.0f;
		float CheckHigh = player.fAngle + fViewAngle / 2.0f;
		if (CheckLow < 0)
			if (alpha < PI / 2.0f && alpha < CheckHigh)
				isVisual = true;
			else
				if (alpha > CheckLow + 2 * PI)
					isVisual = true;
		if ((alpha > CheckLow && alpha < CheckHigh) || isVisual)
		{
			int Ceiling = ScreenHeight / 2.0f - ScreenHeight / (fBulletDistance );
			int Floor = ScreenHeight - Ceiling;
			int Radius = (Floor - Ceiling) / 2;
			int x = (alpha - CheckLow) * ScreenWidth / (CheckHigh - CheckLow);
			DrawCircle(x, ScreenHeight / 2, Radius, BULLET_SHADE, screen);
		}
	}
}

void MoveEnemyes(float time)
{
	for (Enemy* pEnemy : enemyes)
		pEnemy->Move(player.fX, player.fY, time, map);
}

void MoveBullets(float time)
{
	for (Bullet &bullet : bullets)
		bullet.Move(map, time, enemyes);
}

void DrawRect(char *screen, int ScreenWidth, int x, 
	int y, int Width, int Height, char Fill)
{
	int i, j;
	for (i = 0; i < Height; i++)
		for (j = 0; j < Width; j++)
			screen[(y + i) * ScreenWidth + j + x] = Fill;
}

void DrawMainMenu(char *MainMenuScreen, Menu &menu)
{
	SMALL_RECT rect = { 0, 0, MenuWidth, MenuHeight };
	SetConsoleWindowInfo(hConsole, true, &rect);
	DWORD dwBytes = 0;
	memset(MainMenuScreen, ' ', MenuWidth * MenuHeight);
	DrawRect(MainMenuScreen, MenuWidth, 2, 2 * (int)menu.GetIP(), 9, 3, '#');
	char strBuf[20] = "start";
	PrintLine(MainMenuScreen, MenuWidth, strBuf, 9, 1);
	sprintf_s(strBuf, "%s", "levels");
	PrintLine(MainMenuScreen, MenuWidth, strBuf, 9, 3);
	sprintf_s(strBuf, "%s", "records");
	PrintLine(MainMenuScreen, MenuWidth, strBuf, 9, 5);
	sprintf_s(strBuf, "%s", "Quit");
	PrintLine(MainMenuScreen, MenuWidth, strBuf, 9, 7);
	WriteConsoleOutputCharacter(hConsole, MainMenuScreen, MenuWidth * MenuHeight, { 0, 0 }, &dwBytes);
}

void DrawLevelsMenu(char* screen, Levels &levels)
{
	DWORD dwBytes = 0;
	memset(screen, ' ', LvlMenuWidth * LvlMenuHeight);
	DrawRect(screen, LvlMenuWidth, 2, 2 * levels.GetLevelNum(), 9, 3, '#');
	int i;
	char strBuf[20] = "Level 1";
	for (i = 0; i < levels.GetMaxLevel(); i++)
	{
		PrintLine(screen, LvlMenuWidth, strBuf, 9, i * 2 + 1);
		sprintf_s(strBuf, "Level %d", i + 2);
	}
	WriteConsoleOutputCharacter(hConsole, screen, 
		MenuWidth * MenuHeight, { 0, 0 }, &dwBytes);
}

void LoadLevel(Levels& levels)
{
	int LevelNum = levels.GetLevelNum();
	Level level = levels.GetLevel();
	int i;
	float x, y;
	for (i = 0; i < level.ObjectsNum; i++)
	{
		x = level.objects[i].x;
		y = level.objects[i].y;
		switch (level.objects[i].type)
		{
		case otEnemy:
			enemyes.push_back(new Enemy(x, y, 0.3f));
			break;
		case otEnemySoldier:
			enemyes.push_back(new EnemySoldier(x, y, 0.3f));
			break;
		}
	}
	map = level.map;
	MapWidth = level.MapWidth;
	MapHeigth = level.MapHeight;
	player.fX = level.PlayerX;
	player.fY = level.PlayerY;
	player.HP = 20;
	TotalTime = 0.0f;
	KillsCounter = 0;
	sound->Play(sndMain);
}

void DrawPauseMenu(char *screen,Pause &pause)
{
	SMALL_RECT rect = { 0, 0, PauseWidth, PauseHeight};
	SetConsoleWindowInfo(hConsole, true, &rect);
	DWORD dwBytes = 0;
	memset(screen, ' ', MenuWidth * MenuHeight);
	DrawRect(screen, MenuWidth, 2, 2 * (int)pause.GetIPP(), 10, 3, '#');
	char strBuf[20] = "Continue";
	PrintLine(screen, MenuWidth, strBuf, 9, 1);
	sprintf_s(strBuf, "%s", "Main menu");
	PrintLine(screen, MenuWidth, strBuf, 9, 3);
	sprintf_s(strBuf, "%s", "Quit");
	PrintLine(screen, MenuWidth, strBuf, 9, 5);
	WriteConsoleOutputCharacter(hConsole, screen, MenuWidth * MenuHeight, { 0, 0 }, &dwBytes);
}

void DrawGameOver(char* screen, GameOver& gameOver)
{
	SMALL_RECT rect = { 0, 0, GameOverWidth, GameOverHeight};
	SetConsoleWindowInfo(hConsole, true, &rect);
	DWORD dwBytes = 0;
	memset(screen, ' ', GameOverHeight * GameOverWidth);
	DrawRect(screen, GameOverWidth, 2, 2 * (int)gameOver.GetGOIP() + 6, 10, 3, '#');
	char strBuf[20] = "Game Over";
	PrintLine(screen, GameOverWidth, strBuf, 18, 1);
	sprintf_s(strBuf, "%s %d", "Enemyes killed:", KillsCounter);
	PrintLine(screen, GameOverWidth, strBuf, 18, 3);
	sprintf_s(strBuf, "Time: %d s", (int)TotalTime / 1000);
	PrintLine(screen, GameOverWidth, strBuf, 18, 5);
	sprintf_s(strBuf, "%s", "Main menu");
	PrintLine(screen, GameOverWidth, strBuf, 18, 7);
	sprintf_s(strBuf, "%s", "Quit");
	PrintLine(screen, GameOverWidth, strBuf, 18, 9);
	WriteConsoleOutputCharacter(hConsole, screen, GameOverWidth * GameOverHeight, { 0, 0 }, &dwBytes);
}


void DrawRecords(char* screen/*, Records& records*/)
{
	SMALL_RECT rect = { 0, 0, RecordsWidth, RecordsHeight };
	SetConsoleWindowInfo(hConsole, true, &rect);
	DWORD dwBytes = 0;
	char strBuf[20] = "Game Over";
	Record tempRec;
	memset(screen, ' ', RecordsWidth * RecordsHeight);
	for (int i = 0; i < 4; i++)
	{
		tempRec = records.GetRecord(i);
		if (i == 3)
			sprintf_s(strBuf, "Random lvl");
		else
			sprintf_s(strBuf, "Level num: %d", i);
		PrintLine(screen, RecordsWidth, strBuf, 18, 3 * i);
		sprintf_s(strBuf, "Enemyes killed: %d", tempRec.kills);
		PrintLine(screen, RecordsWidth, strBuf, 18, 3 * i + 1);
		sprintf_s(strBuf, "Time: %d", tempRec.time);
		PrintLine(screen, RecordsWidth, strBuf, 18, 3 * i + 2);
	}
	WriteConsoleOutputCharacter(hConsole, screen, GameOverWidth * GameOverHeight, { 0, 0 }, &dwBytes);
}

int main() {
	char Titles[BUF_SIZE];
	gameState = gsMainMenu;
	SetConsoleTitle("Shooter 3d");
	CreateMap();
	char* screen = new char[ScreenWidth * ScreenHeight];
	char* MainMenuScreen = new char[MenuWidth * MenuHeight];
	char* LvlMenuScreen = new char[LvlMenuWidth * LvlMenuHeight];
	char* PausMenuScreen = new char[PauseWidth * PauseHeight];
	char* GameOverScreen = new char[GameOverWidth * GameOverHeight];
	char* RecordsScreen = new char[RecordsWidth * RecordsHeight];
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD dwBytes = 0;
	SetConsoleActiveScreenBuffer(hConsole);
	GetConsoleTitle(Titles, BUF_SIZE);
	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	font.nFont = 0;
	font.FontFamily = 0;
	font.FontWeight = 400;
	font.dwFontSize.X = 32;
	font.dwFontSize.Y = 64;
	SetCurrentConsoleFontEx(hConsole, true, &font);
	EventHandler event;
	int tm1 = clock();
	int tm2 = clock();
	Menu menu(map);
	Levels levels;
	Pause pause;
	GameOver gameOver;
	sound = new Sound();
	LoadMyRecords();
	gameState = gsMainMenu;
	while (gameState != gsQuit)
	{
		font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		font.nFont = 0;
		font.FontFamily = 0;
		font.FontWeight = 400;
		font.dwFontSize.X = 32;
		font.dwFontSize.Y = 64;
		SetCurrentConsoleFontEx(hConsole, true, &font);
		while (gameState == gsMainMenu)
		{
			DrawMainMenu(MainMenuScreen, menu);
			event.Update();
			if (event.isKeyUp(' '))
				gameState = menu.Move(IN);
			if (event.isKeyUp('W'))
				gameState = menu.Move(DOWN);
			if (event.isKeyUp('S'))
				gameState = menu.Move(UP);
		}
		while (gameState == gsChooseLevel)
		{
			DrawLevelsMenu(LvlMenuScreen, levels);
			event.Update();
			if (event.isKeyUp(' '))
				gameState = levels.Move(IN);
			if (event.isKeyUp('W'))
				gameState = levels.Move(DOWN);
			if (event.isKeyUp('S'))
				gameState = levels.Move(UP);
			if (event.isKeyUp(VK_ESCAPE))
				gameState = levels.Move(BACK);
		}
		while (gameState == gsLoadLevel)
		{
			LoadLevel(levels);
			SMALL_RECT rect = { 0, 0, ScreenWidth, ScreenHeight };
			SetConsoleWindowInfo(hConsole, true, &rect);
			gameState = gsRun;
		}
		while (gameState == gsPause)
		{
			DrawPauseMenu(PausMenuScreen, pause);
			event.Update();
			if (event.isKeyUp('W'))
				gameState = pause.Move(DOWN);
			if (event.isKeyUp('S'))
				gameState = pause.Move(UP);
			if (event.isKeyUp(' '))
				gameState = pause.Move(IN);
		}
		font.FontFamily = 0;
		font.FontWeight = 400;
		font.dwFontSize.X = 16;
		font.dwFontSize.Y = 32;
		SetCurrentConsoleFontEx(hConsole, true, &font);
		while (gameState == gsShowRecords)
		{
			DrawRecords(RecordsScreen);
			event.Update();
			if (event.isKeyUp(VK_ESCAPE))
				gameState = records.Move(BACK);
		}
		while (gameState == gsGameOver)
		{
			DrawGameOver(GameOverScreen, gameOver);
			event.Update();
			if (event.isKeyUp('W') || event.isKeyUp('S'))
				gameState = gameOver.Move(UP);
			if (event.isKeyUp(' '))
				gameState = gameOver.Move(IN);
		}
		font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		font.nFont = 0;
		font.FontFamily = 0;
		font.FontWeight = 400;
		font.dwFontSize.X = 8;
		font.dwFontSize.Y = 8;
		SetCurrentConsoleFontEx(hConsole, true, &font);
		while (gameState == gsRun)
		{
			tm1 = clock();
			float time = (tm1 - tm2);
			TotalTime += time;
			tm2 = tm1;
			MovePlayer(time);
			MoveEnemyes(time);
			UpdateBullets();
			UpdateEnemyes();
			MoveBullets(time);
			DrawWalls(screen);
			DrawEnemyes(screen);
			DrawBullets(screen);
			DrawMap(screen, MapHeigth, MapWidth, ScreenWidth);
			DrawInterface(screen, time);
			WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0, 0 }, &dwBytes);
			if (player.HP <= 0)
			{
				sound->Play(sndGameOver);
				gameState = gsGameOver;
				bool isUpdate = false;
				Record tempRec = records.GetRecord(levels.GetLevelNum());
				if (KillsCounter > tempRec.kills || 
					(KillsCounter == tempRec.kills && TotalTime / 1000 < tempRec.time))
				{
					tempRec.kills = KillsCounter;
					tempRec.time = TotalTime / 1000;
					records.SetRecord(levels.GetLevelNum(), tempRec);
				}
				SaveRecords();
			}
		}
	}
	return 0;
} 
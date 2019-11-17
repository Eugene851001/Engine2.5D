#include "Engine.h"
#include "Enemy.h"
#include "EnemySoldier.h"
#include "Bullet.h"
#include "DrawLetter.h"
#include "EventHadler.h"
#include "Menu.h"
#define _CRT_SECURE_NO_WARNINGS

//TODO: add to Engine.h
#define SOUND_RATE 44100
#define SOUND_LENGTH 4

typedef struct _WAVEFile
{
	char chunkId[4];
	long chunkSize;
	char format[4];
	char subchunk1Id[4];
	long subchunk1Size;
	PCMWAVEFORMAT fmt;
	char subchunk2Id[4];
	long subchunk2Size;
	short data[SOUND_RATE * SOUND_LENGTH];
} WAVEFile, * PWAVEFile;

WAVEFile wf = { {'R', 'I', 'F', 'F'}, sizeof(WAVEFile) - 8, {'W', 'A', 'V', 'E'}, {'f', 'm', 't', ' '}, sizeof(PCMWAVEFORMAT),
				{WAVE_FORMAT_PCM, 1, SOUND_RATE, SOUND_RATE * 2, 2, 16},
				{'d', 'a', 't', 'a'}, sizeof(wf.data) };
CONSOLE_FONT_INFOEX font;
HANDLE hConsole;
char* sndBuf;
const char* SoundFileName = "gun-gunshot-01.wav";
GameState gameState;
//---------------------------

using namespace std;

player Player;

string map = "";
list<Enemy*> enemyes;
list<Bullet*> bullets;
HMIDIOUT hMdiOut;
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
	map += "#..............#";
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
	screen[(int)Player.fY * ScreenWidth - ScreenWidth + (int)Player.fX] = 'P';
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

void MovePlayer(float time)
{
	if (GetAsyncKeyState('A'))
		Player.fAngle -= Player.fAngleSpeed * time;
	if (GetAsyncKeyState('D'))
		Player.fAngle += Player.fAngleSpeed * time;
	if (GetAsyncKeyState('W'))
	{
		float testX = Player.fX + Player.fSpeed * cosf(Player.fAngle) * time;
		float testY = Player.fY + Player.fSpeed * sinf(Player.fAngle) * time;
		if (map[(int)testY * MapWidth + (int)testX] != '#' && !isCollision(testX, testY))
		{
			Player.fX = testX;
			Player.fY = testY;
		}
	}
	if (GetAsyncKeyState('S'))
	{
		float testX = Player.fX - Player.fSpeed * cosf(Player.fAngle) * time;
		float testY = Player.fY - Player.fSpeed * sinf(Player.fAngle) * time;
		if (map[(int)testY * MapWidth + (int)testX] != '#' && !isCollision(testX, testY))
		{
			Player.fX = testX;
			Player.fY = testY;
		}
	}
	if (GetAsyncKeyState(' '))
	{
		//	Player.tmAfterShoot += time;
		if (Player.tmReload < Player.tmAfterShoot)
		{
			sndPlaySound("gun-gunshot-01.wav", SND_ASYNC);
			/*note = rand() % 128;
			midiOutShortMsg(hMdiOut, 0x7F0090 | (note<<8));*/
			float x = Player.fX + (Player.fSize + 0.1f) * cosf(Player.fAngle);
			float y = Player.fY + (Player.fSize + 0.1f) * sinf(Player.fAngle);
			bullets.push_back(new Bullet(x, y,
				cosf(Player.fAngle), sinf(Player.fAngle)));
			Player.tmAfterShoot = 0;
		}
	}
	Player.tmAfterShoot += time;
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

void makeScreenShoot(char* screen)
{
/*	FILE* f;
	f = fopen(OUTPUT, "w");
	int i, j;
	for (i = 0; i < ScreenHeight; i++)
		for (j = 0; j < ScreenWidth; fputc(screen[i * ScreenWidth + j++], f));
	fclose(f);*/
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
	int angle = Player.fAngle * 180 / PI;
	//strFPS = "         ";
	j = 0;
	_itoa_s(angle, strBuf, 10);
	PrintLine(screen, ScreenWidth, strBuf, 10, 1);
	sprintf_s(strBuf, "%f", Player.fX);
	screen[ScreenWidth * 2 + ScreenWidth - 10] = 'X';
	PrintLine(screen, ScreenWidth, strBuf, 5, 2);
	screen[ScreenWidth * 3 + ScreenWidth - 10] = 'Y';
	sprintf_s(strBuf, "%f", Player.fY);
	PrintLine(screen, ScreenWidth, strBuf, 5, 3);
	sprintf_s(strBuf, "%d", Player.HP);
	//PrintLine(screen, , 5, 4);
	PrintLine(screen, ScreenWidth, strBuf, 3, 4);
	/*j = 0;
	for (i = ScreenWidth - 5; i < ScreenWidth && strBuf[j] != '\0'; i++, j++)
		screen[ScreenWidth * 2 + i] = strBuf[j];*/
}

void DrawWalls(char* screen)
{
	int x;
	for (x = 0; x < ScreenWidth; x++)
	{
		float fRayAngle = (Player.fAngle - fViewAngle / 2.0f) + ((float)x / ScreenWidth) * fViewAngle;

		float fWallDistance = 0;
		bool isWall = false;

		while (!isWall)
		{
			fWallDistance += 0.1f;
			int TestX = (int)(Player.fX + cosf(fRayAngle) * fWallDistance);
			int TestY = (int)(Player.fY + sinf(fRayAngle) * fWallDistance);

			if (TestX < 0 || TestX >= MapWidth || TestY < 0 || TestY >= MapHeigth)
			{
				isWall = true;
				fWallDistance = fDepth;
			}
			else
			{
				if (map[TestY * MapWidth + TestX] == '#')
					isWall = true;
			}
		}
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
	while (Player.fAngle > 2 * PI)
		Player.fAngle -= 2 * PI;
	while (Player.fAngle < 0)
		Player.fAngle += 2 * PI;
	float MainAlpha = atan2(enemy->getY() - Player.fY, enemy->getX() - Player.fX);
	if (MainAlpha < 0)
		MainAlpha += 2 * PI;
	bool isVisual = false;
	float fEnemyDistance = sqrt(pow(enemy->getX() - Player.fX, 2) +
		pow(enemy->getY() - Player.fY, 2));
	float j;
	float N = fEnemyDistance / (120.0f + 60.0f);//replace 60.0f
	for (j = -enemy->getSize(); j < enemy->getSize(); j += N)
	{
		float alpha = atan2(j, fEnemyDistance) + MainAlpha;
		float CheckLow = Player.fAngle - fViewAngle / 2.0f;
		float CheckHigh = Player.fAngle + fViewAngle / 2.0f;
		if (CheckLow < 0)
			if (alpha < PI / 2.0f && alpha < CheckHigh)
				isVisual = true;
			else
				if (alpha > CheckLow + 2 * PI)
					isVisual = true;
		//raytracing 
		float fRayEnemyDistance = 0, Accuracy = 0.2f;
		bool isWall = false;
		bool isPlayer = false;
		while (!isWall && !isPlayer && fRayEnemyDistance < fEnemyDistance)
		{
			fRayEnemyDistance += 0.1f;
			float testX = enemy->getX() + cosf(alpha - PI) * fRayEnemyDistance;
			float testY = enemy->getY() + sinf(alpha - PI) * fRayEnemyDistance;
			if (map[int(testY + 0.5f) * MapWidth + int(testX + 0.5f)] == '#')
				isWall = true;
			if (testX > Player.fX - Accuracy && testX < Player.fX + Accuracy
				&& testY > Player.fY - Accuracy && testY < Player.fY + Accuracy)
				isPlayer = true;
		}
		//------------------------------------------------------
		if (((alpha > CheckLow && alpha < CheckHigh) || isVisual) && isPlayer)//rename identificators 
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
					if (Shade != EMPTY)
						screen[y * ScreenWidth + x] = Shade;//ENEMY_SHADE;
				}
			}
		}
	}
}

void DrawEnemyes(char* screen)
{
	//list<Enemy*>::iterator p = enemyes.begin();
	for (Enemy* pEnemy : enemyes)
	{
		DrawEnemy(screen, pEnemy);
	}
}

//repair
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


//Add sort to enemyes and  maybe bullets
void UpdateEnemyes()
{
	enemyes.sort([](const Enemy* en1, const Enemy* en2) {return *en1 < *en2; });
	//SortEnemyes();
	float size;
	for (Enemy* pEnemy : enemyes)
	{
		for (Bullet* pBullet : bullets)
		{
			float size = pEnemy->getSize();
			if (pBullet->getX() > pEnemy->getX() - size
				&& pBullet->getX() < pEnemy->getX() + size
				&& pBullet->getY() < pEnemy->getY() + size
				&& pBullet->getY() > pEnemy->getY() - size)
			{
				pEnemy->setDestroy(true);
				pBullet->setDestroy(true);
			}
		}
	}
	list<Enemy*>::iterator pEnemy = enemyes.begin();
	pEnemy = enemyes.begin();
	list<Enemy*>::iterator pTemp = enemyes.begin();
	while ((pEnemy != enemyes.end()) && (enemyes.size() > 0))
	{
		if ((*pEnemy)->getDestroy())
		{
			//pTemp = pEnemy;
			//pTemp++;
			delete* pEnemy;
			enemyes.remove(*pEnemy);
			if (enemyes.size() > 0)
				//pEnemy = pTemp;
				pEnemy = enemyes.begin();
		}
		else
			pEnemy++;
	}
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
	for (Bullet *pBullet : bullets)
		if (pBullet->getX() > Player.fX - Player.fSize
			&& pBullet->getX() < Player.fX + Player.fSize
			&& pBullet->getY() > Player.fY - Player.fSize
			&& pBullet->getY() < Player.fY + Player.fSize)
		{
			if (!pBullet->getDestroy())
			{
				pBullet->setDestroy(true);
				Player.HP -= pBullet->GetDamage();
			}
		}
	list<Bullet*>::iterator p = bullets.begin();
	list<Bullet*>::iterator ptemp = p;
	while (p != bullets.end() && bullets.size() > 0)
	{
		if ((*p)->getDestroy())
		{
			ptemp = p;
			ptemp++;
			delete* p;
			bullets.remove(*p);
			p = ptemp;
		}
		else
			p++;
	}
}

void DrawBullets(char* screen)
{
	float x, y;
	list<Bullet*>::iterator p = bullets.begin();
	while (p != bullets.end())
	{
		x = (*p)->getX();
		y = (*p)->getY();
		while (Player.fAngle > 2 * PI)
			Player.fAngle -= 2 * PI;
		while (Player.fAngle < 0)
			Player.fAngle += 2 * PI;
		float alpha = atan2(y - Player.fY, x - Player.fX);
		if (alpha < 0)
			alpha += 2 * PI;
		bool isVisual = false;
		float fBulletDistance = sqrt(pow(x - Player.fX, 2) + pow(y - Player.fY, 2));
		float CheckLow = Player.fAngle - fViewAngle / 2.0f;
		float CheckHigh = Player.fAngle + fViewAngle / 2.0f;
		if (CheckLow < 0)
			if (alpha < PI / 2.0f && alpha < CheckHigh)
				isVisual = true;
			else
				if (alpha > CheckLow + 2 * PI)
					isVisual = true;
		if ((alpha > CheckLow && alpha < CheckHigh) || isVisual)
		{
			int Ceiling = ScreenHeight / 2.0f - ScreenHeight / (fBulletDistance);
			int Floor = ScreenHeight - Ceiling;
			int Radius = (Floor - Ceiling) / 2;
			int x = (alpha - CheckLow) * ScreenWidth / (CheckHigh - CheckLow);
			DrawCircle(x, ScreenHeight / 2, Radius, BULLET_SHADE, screen);
		}
		p++;
	}

}

void AddEnemyes()
{
	enemyes.push_back(new Enemy(10.0f, 3.0f, 0.3f));
	enemyes.push_back(new Enemy(3.0f, 3.0f, 0.3f));
	enemyes.push_back(new EnemySoldier(4.0f, 9.0f, 0.3f));
	//enemyes.push_back(new Enemy(2.0f, 2.0f, 0.3f));
	//	enemyes.push_back(new Enemy(8.0f, 11.0f, 0.3f));
}

void MoveEnemyes(float time)
{
	for (Enemy* pEnemy : enemyes)
		pEnemy->Move(Player.fX, Player.fY, time, map);
}

void MoveBullets(float time)
{
	for (Bullet* pBullet : bullets)
		pBullet->Move(map, time, enemyes);
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
	DWORD dwBytes = 0;
	memset(MainMenuScreen, ' ', MenuWidth * MenuHeight);
	DrawRect(MainMenuScreen, MenuWidth, 2, 2 * (int)menu.GetIP(), 9, 3, '#');
	char strBuf[20] = "start";
	PrintLine(MainMenuScreen, MenuWidth, strBuf, 9, 1);
	sprintf_s(strBuf, "%s", "levels");
	PrintLine(MainMenuScreen, MenuWidth, strBuf, 9, 3);
	sprintf_s(strBuf, "%s", "options");
	PrintLine(MainMenuScreen, MenuWidth, strBuf, 9, 5);
	sprintf_s(strBuf, "%s", "Quit");
	PrintLine(MainMenuScreen, MenuWidth, strBuf, 9, 7);
	WriteConsoleOutputCharacter(hConsole, MainMenuScreen, MenuWidth * MenuHeight, { 0, 0 }, &dwBytes);
}

int main() {
	char Titles[BUF_SIZE];
	gameState = gsMainMenu;
	SetConsoleTitle("wolfenstein 3d");
	midiOutOpen(&hMdiOut, MIDI_MAPPER, 0, 0, CALLBACK_NULL);
	CreateMap();
	char* screen = new char[ScreenWidth * ScreenHeight];
	char* MainMenuScreen = new char[MenuWidth * MenuHeight];
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD dwBytes = 0;
	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	SetConsoleActiveScreenBuffer(hConsole);
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD point;
	point.X = 0;
	point.Y = 0;
	DWORD l;
	//FillConsoleOutputAttribute(hConsole, 60, 120, point, &l);
	GetConsoleTitle(Titles, BUF_SIZE);
	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	font.nFont = 0;
	font.FontFamily = 0;
	font.FontWeight = 400;
	font.dwFontSize.X = 32;
	font.dwFontSize.Y = 64;
	SetCurrentConsoleFontEx(hConsole, true, &font);
	/*COORD dimensioin;
	dimensioin.X = 600;
	dimensioin.Y = 700;
	SetConsoleDisplayMode(hConsole, CONSOLE_WINDOWED_MODE, &dimensioin);*/
	AddEnemyes();
	EventHandler event;
	int tm1 = clock();
	int tm2 = clock();
	Menu menu(map);
	gameState = gsMainMenu;
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
	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	font.nFont = 0;
	font.FontFamily = 0;
	font.FontWeight = 400;
	font.dwFontSize.X = 8;
	font.dwFontSize.Y = 8;
	SetCurrentConsoleFontEx(hConsole, true, &font);
	LoadSound();
	while (gameState == gsRun)
	{
		tm1 = clock();
		float time = (tm1 - tm2);
		tm2 = tm1;
		list<Enemy*>::iterator pEnemy = enemyes.begin();
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
		screen[ScreenWidth * ScreenHeight - 1] = '\0';	
		WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0, 0 }, &dwBytes);
		if (Player.HP <= 0)
			gameState = gsPause;
	}
	//midiOutClose(hMdiOut);
	return 0;
}
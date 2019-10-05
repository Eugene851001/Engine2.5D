#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#define PI 3.14159265
#define BULLET_SHADE '@'
#define OUTPUT "Photo.txt"
#define max(a, b) ((a) > (b)) ? a : b

using namespace std;
void makeScreenShoot(char* screen);

#include <Windows.h>

const int ScreenWidth = 120;
const int ScreenHeight = 40;
const int MapWidth = 16;
const int MapHeigth = 16;

const float fViewAngle = PI / 4.0f;
const float fViewRad = 10.0f;
const float fDepth = 16.0f;

struct
{
	float fX = 8.0f;
	float fY = 8.0f;
	float fAngle = 0.0f;
	float fSpeed = 0.001f;
	float fAngleSpeed = 0.001f;
	float tmReload = 1000.0f;
	float tmAfterShoot = 1000.0f;
} Player;

string map = "";

class Enemy
{

public:
	int HP = 20;
	float x, y;
	float size;
	float fspeed = 0.0f;//0.0001f;
	int textureWidth = 4;
	int textureHeight = 4;
	bool isDestroy = false;
	char texture[4][4] = { {'$', '$', '$', '$'},
						   {' ', '$', '$', ' '},
						   {'$', '$', '$', '$'},
						   {'$', ' ', ' ', '$'} };
	Enemy(float x, float y, float size)
	{
		this->x = x;
		this->y = y;
		this->size = size;
	}
	void Move(int plX, int plY, float time)
	{
		if (!isDestroy)
		{
			float distance = sqrt(pow(x - plX, 2) + pow(y - plY, 2));
			if (distance > size + 0.2f)
			{
				float dx = (plX - x) / distance;
				float dy = (plY - y) / distance;
				x += dx * fspeed * time;
				y += dy * fspeed * time;
			}
		}
	}
};

list<Enemy*> enemyes;

class Bullet
{

public:
	int Damage;
	float x, y;
	float fspeed = 0.002f;
	float dx, dy;
	bool isDestroy;
	Bullet(float x, float y, float dx, float dy)
	{
		this->x = x;
		this->y = y;
		this->dx = dx;
		this->dy = dy;
		isDestroy = false;
	}
	void Move(const string& Map, float time)
	{
		if (!isDestroy)
		{
			x += dx * fspeed * time;
			y += dy * fspeed * time;
			if (map[int(y) * MapWidth + int(x)] == '#')
				isDestroy = true;
			if (enemyes.size() > 0)
			{
				list<Enemy*>::iterator pEnemy = enemyes.begin();
				int size = (*pEnemy)->size;
				while (pEnemy != enemyes.end())
				{
					if (x > (*pEnemy)->x - size && x < (*pEnemy)->x + size
						&& y >(*pEnemy)->y - size && y < (*pEnemy)->y + size)
						isDestroy = true;
					pEnemy++;
				}
			}
		}
	}
};

list<Bullet*> bullets;

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
	while ((p != enemyes.end()) && (!isCollision) && (bullets.size() > 0))
	{
		float size = (*p)->size;
		if (x > (*p)->x - size && x < (*p)->x + size
			&& y > (*p)->y - size && y < (*p)->y + size)
			isCollision = true;
		p++;
	}
	return isCollision;
}

void MovePlayer(float time, char* screen)
{
	if (GetAsyncKeyState('A'))
		Player.fAngle -= Player.fAngleSpeed * time;
	if (GetAsyncKeyState('D'))
		Player.fAngle += Player.fAngleSpeed * time;
	if (GetAsyncKeyState('P'))
		makeScreenShoot(screen);
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
			bullets.push_back(new Bullet(Player.fX, Player.fY,
				cosf(Player.fAngle), sinf(Player.fAngle)));
			Player.tmAfterShoot = 0;
		}
	}
	Player.tmAfterShoot += time;
}

void makeScreenShoot(char* screen)
{
	FILE* f;
	f = fopen(OUTPUT, "w");
	int i, j;
	for (i = 0; i < ScreenHeight; i++)
		for (j = 0; j < ScreenWidth; fputc(screen[i * ScreenWidth + j++], f));
	fclose(f);
}

void PrintLine(char* screen, char* strBuf, int left, int height)
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
	sprintf(strBuf, "%f", FPS);
	int i = 0, j = 0;
	PrintLine(screen, strBuf, 15, 0);
	int angle = Player.fAngle * 180 / PI;
	//strFPS = "         ";
	j = 0;
	itoa(angle, strBuf, 10);
	PrintLine(screen, strBuf, 10, 1);
	sprintf(strBuf, "%f", Player.fX);
	screen[ScreenWidth * 2 + ScreenWidth - 10] = 'X';
	PrintLine(screen, strBuf, 5, 2);
	screen[ScreenWidth * 3 + ScreenWidth - 10] = 'Y';
	sprintf(strBuf, "%f", Player.fY);
	PrintLine(screen, strBuf, 5, 3);
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
	float MainAlpha = atan2(enemy->y - Player.fY, enemy->x - Player.fX);
	if (MainAlpha < 0)
		MainAlpha += 2 * PI;
	bool isVisual = false;
	float fEnemyDistance = sqrt(pow(enemy->x - Player.fX, 2) + pow(enemy->y - Player.fY, 2));
	float j;
	float N = fEnemyDistance / (120.0f + 60.0f);//replace 60.0f
	for (j = -enemy->size; j < enemy->size; j += N)
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
			float testX = enemy->x + cosf(alpha - PI) * fRayEnemyDistance;
			float testY = enemy->y + sinf(alpha - PI) * fRayEnemyDistance;
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
			int textureX = (j + enemy->size) * enemy->textureWidth / (2 * enemy->size);
			int y;
			int x = (alpha - CheckLow) * ScreenWidth / (CheckHigh - CheckLow);
			for (y = Ceiling; y < Floor; y++)
			{
				if (!(y > ScreenHeight || y < 0))
				{
					int textureY = (y - Ceiling) * enemy->textureHeight / Height;
					screen[y * ScreenWidth + x] = enemy->texture[textureY][textureX];//ENEMY_SHADE;
				}
			}
		}
	}
}

void DrawEnemyes(char* screen)
{
	list<Enemy*>::iterator p = enemyes.begin();
	while (p != enemyes.end())
	{
		DrawEnemy(screen, *p);
		p++;
	}
}


//Add sort to enemyes and  maybe bullets
void UpdateEnemyes()
{
	list<Enemy*>::iterator pEnemy = enemyes.begin();
	list<Bullet*>::iterator pBullet = bullets.begin();
	float size;
	while (pEnemy != enemyes.end())
	{
		pBullet = bullets.begin();
		while (pBullet != bullets.end())
		{
			float size = (*pEnemy)->size;
			if ((*pBullet)->x > (*pEnemy)->x - size
				&& (*pBullet)->x < (*pEnemy)->x + size
				&& (*pBullet)->y < (*pEnemy)->y + size
				&& (*pBullet)->y >(*pEnemy)->y - size)
			{
				(*pEnemy)->isDestroy = true;
				(*pBullet)->isDestroy = true;
			}
			pBullet++;
		}
		pEnemy++;
	}
	pEnemy = enemyes.begin();
	list<Enemy*>::iterator pTemp = enemyes.begin();
	while ((pEnemy != enemyes.end()) && (enemyes.size() > 0))
	{
		if ((*pEnemy)->isDestroy)
		{
			pTemp = pEnemy;
			pTemp++;
			enemyes.remove(*pEnemy);
			pEnemy = pTemp;
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
			screen[(i + y) * ScreenWidth + j + x] = shade;
			screen[(i + y) * ScreenWidth - j + x] = shade;
			screen[(-i + y) * ScreenWidth + j + x] = shade;
			screen[(-i + y) * ScreenWidth - j + x] = shade;
		}
	}
}

void UpdateBullets()
{
	list<Bullet*>::iterator p = bullets.begin();
	list<Bullet*>::iterator ptemp = p;
	while (p != bullets.end() && bullets.size() > 0)
	{
		if ((*p)->isDestroy)
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
		x = (*p)->x;
		y = (*p)->y;
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
//	enemyes.push_back(new Enemy(8.0f, 11.0f, 0.3f));
}

int main() {
	CreateMap();
	char* screen = new char[ScreenWidth * ScreenHeight];
	screen[20 * 120 + 60] = 'L';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD dwBytes = 0;
	SetConsoleActiveScreenBuffer(hConsole);
	AddEnemyes();
	int tm1 = clock();
	int tm2 = clock();

	bool IsContinue = true;
	while (IsContinue)
	{
		tm1 = clock();
		float time = (tm1 - tm2);
		tm2 = tm1;
		list<Enemy*>::iterator pEnemy = enemyes.begin();
		MovePlayer(time, screen);
		UpdateBullets();
		UpdateEnemyes();
		//enemy.Move(Player.fX, Player.fY, time);
		int j;
		//move bullets
		list<Bullet*>::iterator p = bullets.begin();
		while (p != bullets.end())
		{
			(*p)->Move(map, time);
			p++;
		}
		//draw walls and floor
		DrawWalls(screen);
		DrawEnemyes(screen);
		DrawBullets(screen);
		DrawMap(screen, MapHeigth, MapWidth, ScreenWidth);
		DrawInterface(screen, time);
		//DrawCircle(10, 10, 4, '@', screen);		
		screen[ScreenWidth * ScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0, 0 }, &dwBytes);
	}

	return 0;
}

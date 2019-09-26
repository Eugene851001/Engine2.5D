#include <iostream>
#include <cmath>
#include <ctime>
#define PI 3.14159265
#define ENEMY_SHADE '$'
#define OUTPUT "Photo.txt"

using namespace std;
void makeScreenShoot(char *screen);

#include <Windows.h>

class Enemy
{
	
public:
	float x, y;
	float size;
	Enemy(float x, float y, float size)
	{
		this->x = x;
		this->y = y;
		this->size = size;
	}
		
};

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
	float fAngle = 0.0f;//óãîë ñ ïîëîæèòåëüíûì íàïðàâëåíèåì ox
	float fSpeed = 0.001f;
	float fAngleSpeed = 0.001f;
} Player;

string map = "";

void CreateMap()
{
	map +="################";
	map +="#...#..........#";
	map +="#...#..........#"; 
	map +="#...#..........#";
	map +="#..............#";
	map +="#..............#";
	map +="#.....###......#";
	map +="#..............#";
	map +="#..............#";
	map +="#.......#......#";
	map +="#..............#";
	map +="#.........#....#";
	map +="#.........#....#";
	map +="#.........######";
	map +="#..............#";
	map +="################";
}

void DrawMap(char *screen, int MapHeight, int MapWidth, int ScreenWidth)
{
	int y, x, i;
	i = 0;
	for (y = 0; y < MapHeight; y++)
	{
		for (x = 0; x  < MapWidth; x++)
		{
			screen[i] = map[y * MapWidth + x];
			i++;
		}
		i = y * ScreenWidth;	
	}
	screen[(int)Player.fY * ScreenWidth  - ScreenWidth + (int)Player.fX] = 'P';	
}

char isCollision(Enemy *enemy, float x, float y)
{
	char isCollision = 0;
	float size = enemy->size;
	if (x > enemy->x - size && x < enemy->x + size 
		&& y > enemy->y - size && y < enemy->y + size)
		isCollision = 1;
	return isCollision;	
}

void MovePlayer(float time, char *screen, Enemy *enemy)
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
		if (map[(int)testY * MapWidth + (int)testX] != '#' && !isCollision(enemy, testX, testY))
		{
			Player.fX = testX;
			Player.fY = testY;
		}
	}
	if (GetAsyncKeyState('S'))
	{
       	float testX = Player.fX - Player.fSpeed * cosf(Player.fAngle) * time; 
		float testY = Player.fY - Player.fSpeed * sinf(Player.fAngle) * time; 
		if (map[(int)testY * MapWidth + (int)testX] != '#' && !isCollision(enemy, testX, testY))
		{
			Player.fX = testX;
			Player.fY = testY;
		}
	} 	
}

void makeScreenShoot(char *screen)
{
	FILE *f;
	f = fopen(OUTPUT, "w");
	int i, j;
	for (i = 0; i < ScreenHeight; i++)
		for (j = 0; j < ScreenWidth; fputc(screen[i * ScreenWidth + j++], f));
	fclose(f);
}

void PrintLine(char *screen, char *strBuf, int left, int height)
{
	int i, j = 0;
	for (i = ScreenWidth - left; i < ScreenWidth && strBuf[j] != '\0'; i++, j++)
		screen[i + height * ScreenWidth] = strBuf[j];		
}

void DrawInterface(char *screen, float time)
{
	if (!time)
		time = 1;
	time = time / CLOCKS_PER_SEC;
	float FPS = 1000 / time ;
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

int main(){
	CreateMap();
	char *screen = new char[ScreenWidth * ScreenHeight];
	screen[20 * 120 + 60] = 'L';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ| GENERIC_WRITE, 
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD dwBytes = 0;	
	SetConsoleActiveScreenBuffer(hConsole);
	Enemy enemy(3, 3, 0.3);
	int tm1 = clock();
	int tm2 = clock();
	
	bool IsContinue = true;
	while (IsContinue)
	{
		tm1 = clock();
		float time = (tm1 - tm2);
		tm2 = tm1;
		MovePlayer(time, screen, &enemy);
		//draw walls and floor
		int x;
		for (x = 0; x < ScreenWidth; x++)
		{
			float fRayAngle = (Player.fAngle - fViewAngle / 2.0f) + ((float) x / ScreenWidth) * fViewAngle;
			
			float fWallDistance = 0;
			bool isWall = false;
			
			while (!isWall)
			{
				fWallDistance+= 0.1f;
				int TestX = (int)(Player.fX + cosf(fRayAngle) * fWallDistance);
				int TestY = (int)(Player.fY + sinf(fRayAngle) * fWallDistance);	
				
				if (TestX < 0 || TestX >= MapWidth || TestY < 0 || TestY >= MapHeigth){
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
				else if(y > Ceiling &&  y <= Floor && fWallDistance <= fViewRad)
					screen[y * ScreenWidth + x] = WallShade;
				else{
					char FloorShade;
					if (y >= ScreenHeight - ScreenHeight/(2 * 3))
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
		//----------------------------------------------------
		//Draw Enemy
		//does not work correctly
		while (Player.fAngle > 2 * PI)
			Player.fAngle -= 2 * PI;
		while (Player.fAngle < 0)
			Player.fAngle += 2 * PI;
		float alpha = atan2(enemy.y - Player.fY, enemy.x - Player.fX);
		if (alpha < 0)
			alpha += 2 * PI;
		bool isVisual = false;
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
			screen[ScreenWidth  / 2] = '0';
			float fEnemyDistance = sqrt(pow(enemy.x - Player.fX, 2) + pow(enemy.y - Player.fY, 2));
			int Ceiling = ScreenHeight  / 2.0f - ScreenHeight / (fEnemyDistance);
			int Floor = ScreenHeight - Ceiling;
			int y;
			x = (alpha - CheckLow) * ScreenWidth / (CheckHigh - CheckLow);
			for (y = Ceiling; y < Floor && y > 0 && y < ScreenHeight; y++)
			{
				screen[y * ScreenWidth + x] = ENEMY_SHADE;
			}	 
		}
		//----------------------------------------------------
		
		DrawMap(screen, MapHeigth, MapWidth, ScreenWidth);
		DrawInterface(screen, time);
				
		screen[ScreenWidth * ScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, {0, 0}, &dwBytes);
	}
	
	return 0;
}

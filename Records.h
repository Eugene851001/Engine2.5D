#pragma once

#include "Engine.h"

const int RecordsWidth = 20;
const int RecordsHeight = 12;

typedef struct _Record
{
	int kills;
	int time;
} Record;

class Records
{
	Record defaultRecord = { 0, 999 };
	Record values[4];
	const int MaxLevelNum = 4;
public:
	Records();
	void SetDefaultRecords();
	void SetRecord(int n, Record value);
	Record GetRecord(int n);
	GameState Move(int direction);
};


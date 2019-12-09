#include "Sound.h"
#include <Windows.h>
#include <iostream>
#include <fstream>

Sound::Sound()
{
	int size;
	for (int i = 0; i < sndAmount; i++)
	{
		std::ifstream f;
		switch (i)
		{
		case sndShoot:
			f.open(FileShoot, std::ios::binary);
			break;
		case sndGameOver:
			f.open(FileGameOver, std::ios::binary);
			break;
		case sndMain:
			f.open(FileMain, std::ios::binary);
			break;
		}
		if (f.is_open())
		{
			f.seekg(0, std::ios_base::end);
			size = f.tellg();
			sounds[i].size = size;
			sounds[i].sndBuf = new char[size];
			f.seekg(0, std::ios_base::beg);
			f.read(sounds[i].sndBuf, size);
			f.close();
			sounds[i].isLoaded = true;
		}
		else
			sounds[i].isLoaded = false;
	}
}

void Sound::Play(SoundName Name)
{
	if (sounds[Name].isLoaded)
		sndPlaySound((LPCSTR)sounds[Name].sndBuf, SND_MEMORY |SND_ASYNC);
}
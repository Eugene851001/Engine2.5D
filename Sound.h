#pragma once

enum SoundName {sndShoot, sndGameOver, sndMain, sndAmount};

typedef struct _SoundData
{
	char *sndBuf;//буфер, куда записываются данные аудифайла
	int size;//размер буфера
	bool isLoaded;//флаг, который показывает загружен ли аудифайл в динамичсекую память
} SoundData;

class Sound
{
	const char* FileShoot = "gun1.wav";
	const char* FileGameOver = "Game Over.wav";
	const char* FileMain = "Main Theme.wav";
	SoundName sndName;
	SoundData sounds[sndAmount];
public:
	Sound();
	void Play(SoundName Name);
	//void PlayLoop(SoundName Name);
	//void Stop(SoundName Name);
};


#ifndef __MODULESCENEMAIN_H__
#define __MODULESCENEMAIN_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleSceneMain : public Module
{
public:
	ModuleSceneMain(bool active = true);
	~ModuleSceneMain();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* logoTexture;
	SDL_Texture* background;
	SDL_Texture* letters;
	SDL_Texture* radioTexture;
	SDL_Texture* background2;
	Animation logo;
	Animation car;
	Animation palm;
	Animation insertCoins;
	Animation pressStart;
	Animation radioLeds;
	vector<SDL_Rect> radioNum;
	vector<SDL_Rect> radioHand;
	vector<SDL_Rect> radioMusic;

private:
	float iniTime, countdown;
	unsigned int credit, music;
	unsigned int audioCoin, audioWave;
	bool fxWavePlayed;
};

#endif // __MODULESCENEMAIN_H__
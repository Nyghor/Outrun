#include "Globals.h"
#include "Application.h"
#include "ModuleFontManager.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL.h"
#include "SDL_image.h"

using namespace std;

ModuleFontManager::ModuleFontManager(bool start_enabled) : Module(start_enabled)
{}

// Destructor
ModuleFontManager::~ModuleFontManager()
{}

bool ModuleFontManager::loadFont(const char* fontName, const char* path, const char* trad, const int w, const int h)
{
	//Loading success flag
	bool ret = true;
	Font f;
	f.font = App->textures->Load(path);

	if (f.font == nullptr)
	{
		LOG("Failed to create texture font");
		ret = false;
	}
	else {
		//Initialize index table for each character
		map<char, int> indexTable;
		for (int i = 0; trad[i] != '\0'; ++i) {
			indexTable[trad[i]] = i;
		}
		f.indexTable = indexTable; f.w = w; f.h = h;

		//Add new font to table
		fontTable[fontName] = f;
	}

	return ret;
}

void ModuleFontManager::print(const char* text, const int posx, const int posy, const char* fontName, fPoint pivot)
{
	Font f = fontTable[fontName];

	int x = posx;
	int y = posy;

	SDL_Rect rectFont;
	rectFont.y = 0;
	rectFont.w = f.w; rectFont.h = f.h;

	for (int i = 0; text[i] != '\0'; ++i) {
		int index = f.indexTable[text[i]];
		rectFont.x = (f.w + 1)*index + 1;	//Each symbol is separated by one pixel
		x = posx + f.w*i;
		App->renderer->Blit(f.font, x, y, &rectFont, 1.0f, 1.0f, pivot);
		//SDL_BlitSurface(f.font, &rectFont, App->window->screen_surface, &rectWindow);
	}
}

int ModuleFontManager::getFontHeight(const char* fontName)
{
	return fontTable[fontName].h;
}

int ModuleFontManager::getFontWidth(const char* fontName)
{
	return fontTable[fontName].w;
}

void ModuleFontManager::closeFont(const char* fontName)
{
	SDL_DestroyTexture(fontTable[fontName].font);
	fontTable[fontName].font = nullptr;
	fontTable[fontName].indexTable.clear();
	fontTable.erase(fontName);
}

bool ModuleFontManager::CleanUp()
{
	LOG("Freeing fonts surfaces, clearing Font Table");

	for (map<const char*, Font>::iterator it = fontTable.begin(); it != fontTable.end(); ++it) {
		SDL_DestroyTexture(it->second.font);
		it->second.font = nullptr;
		it->second.indexTable.clear();
	}
	fontTable.clear();
	return true;
}
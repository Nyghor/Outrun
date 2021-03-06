#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL.h"
#include "sdl2_gfxprimitives.h"

ModuleRender::ModuleRender()
{
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH * SCREEN_SIZE;
	camera.h = SCREEN_HEIGHT* SCREEN_SIZE;
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	bool ret = true;
	Uint32 flags = 0;

	if (VSYNC == true)
		flags |= SDL_RENDERER_PRESENTVSYNC;

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);

	if (renderer == nullptr)
		ret = false;

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	//Destroy window
	if (renderer != nullptr)
		SDL_DestroyRenderer(renderer);

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed, fPoint scale, fPoint pivot)
{
	//Pivot (0,0) to (1,1)
	//(0,0) -> up left, (1,1) -> down right

	bool ret = true;
	SDL_Rect rect;
	int w, h;

	if (section != nullptr)
	{
		w = section->w;
		h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
		w = rect.w;
		h = rect.h;
	}

	int xValue = (int)(w - (w * scale.x));
	int yValue = (int)(h - (h * scale.y));

	rect.x = (int)(camera.x * speed) + (int)(x - (w * pivot.x) + (int)(xValue * pivot.x)) * SCREEN_SIZE;
	rect.y = (int)(camera.y * speed) + (int)(y - (h * pivot.y) + (int)(yValue * pivot.y)) * SCREEN_SIZE;

	rect.w = (int)(w * scale.x) * SCREEN_SIZE;
	rect.h = (int)(h * scale.y) * SCREEN_SIZE;

	if (SDL_RenderCopy(renderer, texture, section, &rect) != 0)
		ret = false;

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * SCREEN_SIZE);
		rec.y = (int)(camera.y + rect.y * SCREEN_SIZE);
		rec.w *= SCREEN_SIZE;
		rec.h *= SCREEN_SIZE;
	}

	if (SDL_RenderFillRect(renderer, &rec) != 0)
		ret = false;

	return ret;
}

bool ModuleRender::DrawPoly4(short x1, short y1, short x2, short y2, short x3, short y3, short x4, short y4, const SDL_Color& c) {
	int ret = 0;
	short vx[4] = { x1 * SCREEN_SIZE, x2 * SCREEN_SIZE, x3 * SCREEN_SIZE, x4 * SCREEN_SIZE };
	short vy[4] = { (y1 + SCREEN_Y_OFFSET) * SCREEN_SIZE, (y2 + SCREEN_Y_OFFSET) * SCREEN_SIZE, (y3 + SCREEN_Y_OFFSET) * SCREEN_SIZE, (y4 + SCREEN_Y_OFFSET) * SCREEN_SIZE };

	if (renderer != nullptr)
		ret = filledPolygonRGBA(renderer, vx, vy, 4, c.r, c.g, c.b, c.a);

	return (ret == 0);
}
#ifndef xSDL_ALLEGRO_WRAPPER_H_
	#include "xSDL_helper.h"
	#include "cstdarg"
	#include "cstdio"
	#define xSDL_ALLEGRO_WRAPPER_H_
	void textprintf_ex(SDL_Renderer *target, TTF_Font *f, int x, int y, SDL_Color textColor, void* ignored, const char *fmt, ...);
	int text_length(TTF_Font *f, const char *text);
	int text_height(TTF_Font *f);
	SDL_Color makecol(int r, int g, int b);
	void clear_to_color(SDL_Renderer *target, SDL_Color col);
#endif

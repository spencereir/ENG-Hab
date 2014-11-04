#include "xSDL_allegro_wrapper.h"

TTF_Font *font = NULL;

void textprintf_ex(SDL_Renderer *target, TTF_Font *f, int x, int y, SDL_Color textColor, void* ignored, const char *fmt, ...){
	if (!f){
		if (!font){
			font = TTF_OpenFont("res/mainstreet2.ttf", 12);
			if (!font){
				xSDL::logError("TTF_OpenFon");
				throw "Could not load res/mainstreet2.ttf";
			}
		}
		f = font;
	}

	char buffer [1024];
	 va_list args;
	 va_start(args, fmt);
	 vsnprintf(buffer, 1024, fmt, args);
	 va_end(args);
  	
	SDL_Surface *surf = TTF_RenderText_Solid(f, buffer, textColor);
	if (surf == NULL){
		xSDL::logError("TTF_RenderText");
		throw "Could not render to surface";
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(target, surf);
	if (texture == NULL){
		xSDL::logError("CreateTexture");
		throw "Could not convert surface to texture";
	}
	
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(target, texture, NULL, &dst);

	SDL_FreeSurface(surf);
	SDL_DestroyTexture(texture);
	return;
}

//-std=gnu++0x

int text_length(TTF_Font *f, const char *text){
	if (!f){
		if (!font){
			font = TTF_OpenFont("res/mainstreet2.ttf", 12);
			if (!font){
				xSDL::logError("TTF_OpenFon");
				throw "Could not load res/mainstreet2.ttf";
			}
		}
		f = font;
	}
	int w = 0;
	TTF_SizeText(f,text,&w, NULL);
	
	return w;
}

int text_height(TTF_Font *f){
	if (!f){
		if (!font){
			font = TTF_OpenFont("res/mainstreet2.ttf", 12);
			if (!font){
				xSDL::logError("TTF_OpenFon");
				throw "Could not load res/mainstreet2.ttf";
			}
		}
		f = font;
	}
	return TTF_FontHeight(f);
}

SDL_Color makecol(int r, int g, int b){
	SDL_Color textColor = { r, g, b};
	return textColor;
}

void clear_to_color(SDL_Renderer *target, SDL_Color col){
	SDL_SetRenderDrawColor(target, col.r, col.g, col.b, 255);
    SDL_RenderFillRect(target, NULL);
}

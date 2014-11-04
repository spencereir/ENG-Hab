#ifndef xSDL_BITMAP_H_
	#include "xSDL_bitmap.h"
	
	BITMAP::BITMAP(){
		w = 0;
		h = 0;
		data = NULL;
	}
	

	void masked_blit(BITMAP *source, SDL_Renderer *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height){
		SDL_Rect src, dst;
		src.x = source_x;
		src.y = source_y;
		dst.x = dest_x;
		dst.y = dest_y;
		dst.w = width;
		dst.h = height;
		//Query the texture to get its width and height to use
		SDL_QueryTexture(source->data, NULL, NULL, &src.w, &src.h);
		SDL_RenderCopy(dest, source->data, &src, &dst);
	}

	BITMAP* load_bitmap(const char* filepath, SDL_Renderer* context){
		static int counter = 0;
		BITMAP* current = new BITMAP;
		SDL_Surface* loadedSurface = IMG_Load(filepath);
		if (!loadedSurface){
			xSDL::logError("IMG_Load");
			throw "Could not load image";
		}
		SDL_Texture* newTexture = SDL_CreateTextureFromSurface( context, loadedSurface );
        if (!newTexture ){
			xSDL::logError("SDL_CreateTextureFromSurface");
			throw "Could not convert texture";
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
        current->data = newTexture;
		SDL_QueryTexture(current->data, NULL, NULL, &current->w, &current->h);
        return current;
	}
	
	#define xSDL_BITMAP_H_
#endif

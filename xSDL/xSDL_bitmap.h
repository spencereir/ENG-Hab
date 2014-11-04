// Allegro-Style Bitmap -> sdl_texture wrapper
#ifndef xSDL_BITMAP_H_
	#include "xSDL_helper.h"

	#define xSDL_BITMAP_H_
	class BITMAP{
		public:
			SDL_Texture* data;
			BITMAP();
			int w;
			int h;
	};
	void masked_blit(BITMAP *source, SDL_Renderer *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);

	BITMAP* load_bitmap(const char* filepath, SDL_Renderer* context);
#endif

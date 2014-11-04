#include "xSDL/xSDL_helper.h"
#include <string>
char processEvents(SDL_Window* window){
	SDL_Event* current_event;
	while(SDL_PollEvent(current_event)){
		if (SDL_GetKeyboardFocus()==window){
			if(current_event->type == SDL_KEYDOWN){
				switch(current_event->key.keysym.scancode){
					case SDL_SCANCODE_TAB:
						return '!';
					case SDL_SCANCODE_PAGEUP:
						return '@';
					case SDL_SCANCODE_PAGEDOWN:
						return '#';
					case SDL_SCANCODE_LEFT:
						return '$';
					case SDL_SCANCODE_RIGHT:
						return '%';
					default:
						const char* keyname = SDL_GetScancodeName(current_event->key.keysym.scancode);
						return (keyname) ? keyname[0] : ' ';
				}
			}
		}
	}
	return ' ';
}

#include "xSDL/xSDL_helper.h"
#include <string>
// prototyped in main
char processEvents(SDL_Window* window){
	SDL_Event current_event;
	while(SDL_PollEvent(&current_event)){
		if (window==SDL_GetKeyboardFocus()){
			if(current_event.type == SDL_KEYDOWN){
				xSDL::logInfo("processEvent", "KEYDOWN!");
				xSDL::logInfo("processEvent", SDL_GetScancodeName(current_event.key.keysym.scancode));
				switch(current_event.key.keysym.scancode){
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
					case SDL_SCANCODE_Q:
					case SDL_SCANCODE_ESCAPE:
						throw 100;
					default:
						const char* keyname = SDL_GetScancodeName(current_event.key.keysym.scancode);
						return (keyname) ? keyname[0] : ' ';
				}
			}
		}
	}
	return ' ';
}

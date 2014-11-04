#include "xSDL_helper.h"

namespace xSDL{
	void logError(const char* context, const char* error){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s: %s", context, error);
	}
	void logInfo(const char* context, const char* info){
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s: %s", context, info);
	}
	void logValue(const char* context, int value){
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s: %d", context, value);
	}

	bool isFullScreen(SDL_Window *win){
		Uint32 flags = SDL_GetWindowFlags(win);
		if (flags & SDL_WINDOW_FULLSCREEN){
			return true; // return SDL_TRUE if fullscreen
		}
		return false; // Return SDL_FALSE if windowed
	}

	bool init(){
		//Start SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) == -1){
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, SDL_GetError());
			return false;
		}
		version::sdl_compiled = new SDL_version;
		version::sdl_linked = new SDL_version;
		SDL_VERSION(version::sdl_compiled);
		SDL_GetVersion(version::sdl_linked);
		
		version::log("SDL2", version::sdl_compiled, version::sdl_linked);

		#ifndef WITHOUT_AUDIO_H_
			if (Mix_Init(/*MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 |*/ MIX_INIT_OGG) == -1){
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, SDL_GetError());
				return false;
			}
			version::mixer_compiled = new SDL_version;
			SDL_MIXER_VERSION(version::mixer_compiled);
			version::mixer_linked=Mix_Linked_Version();
			
			version::log("SDL_mixer", version::mixer_compiled, version::mixer_linked);
		#endif
		#ifndef WITHOUT_TEXTURE_H_
			if (IMG_Init(IMG_INIT_PNG) == -1){
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, SDL_GetError());
				return false;
			}
			version::image_compiled = new SDL_version;
			SDL_IMAGE_VERSION(version::image_compiled);
			version::image_linked=IMG_Linked_Version();
			
			version::log("SDL_image", version::image_compiled, version::image_linked);
		#endif
		#ifndef WITHOUT_FONT_H_
			if (TTF_Init() == -1){
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, SDL_GetError());
				return false;
			}
			version::ttf_compiled = new SDL_version;
			SDL_TTF_VERSION(version::ttf_compiled);
			version::ttf_linked=TTF_Linked_Version();

			version::log("SDL_ttf", version::ttf_compiled, version::ttf_linked);
		#endif
		#ifndef WITHOUT_NET_H_
			if (SDLNet_Init()==-1) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, SDL_GetError());
				return false;
			}
			version::net_compiled = new SDL_version;
			SDL_NET_VERSION(version::net_compiled);
			version::net_linked=SDLNet_Linked_Version();
		
			version::log("SDL_net", version::net_compiled, version::net_linked);
		#endif
		return true;
	}
	
	void quit(){
		delete version::sdl_compiled;
		delete version::sdl_linked;
		delete version::mixer_compiled;
		delete version::image_compiled;
		delete version::ttf_compiled;
		delete version::net_compiled;
		
		//Quit SDL
		#ifndef WITHOUT_NET_H_
			SDLNet_Quit();
		#endif
		#ifndef WITHOUT_FONT_H_
			TTF_Quit();
		#endif
		#ifndef WITHOUT_TEXTURE_H_
			IMG_Quit();
		#endif
		#ifndef WITHOUT_AUDIO_H_
			Mix_Quit();
		#endif
		SDL_Quit();
	}

	namespace version{
		SDL_version *sdl_compiled;
		SDL_version *sdl_linked;

		SDL_version *mixer_compiled;
		const SDL_version *mixer_linked;

		SDL_version *image_compiled;
		const SDL_version *image_linked;

		SDL_version *ttf_compiled;
		const SDL_version *ttf_linked;

		SDL_version *net_compiled;
		const SDL_version *net_linked;

		void log (const char* name, const SDL_version *version_compiled, const SDL_version *version_linked){
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Compiled with %s: %d.%d.%d\n", name, version_compiled->major, version_compiled->minor, version_compiled->patch);
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Running  with %s: %d.%d.%d\n", name, version_linked->major, version_linked->minor, version_linked->patch);
			if (version_compiled->major != version_linked->major){
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Major version mismatch!");
			}else if (version_compiled->minor != version_linked->minor){
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Minor version mismatch!");
			}else if (version_compiled->patch != version_linked->patch){
				SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Patch version mismatch!");
			}
		}
	}

}
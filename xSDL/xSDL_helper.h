/*
#define WITHOUT_SDL_NET_H_
#define WITHOUT_SDL_TTF_H_
#define WITHOUT_SDL_IMAGE_H_
#define WITHOUT_SDL_MIXER_H_
*/
#ifndef xSDL_HELPER_H_
	#define xSDL_HELPER_H_
	
	#define xREPEAT(x)for(int REPEAT_i = 0; REPEAT_i < x; REPEAT_i++)

	#include <SDL.h>

	#ifndef WITHOUT_SDL_NET_H_
		#include <SDL_net.h>
	#endif
	#ifndef WITHOUT_SDL_TTF_H_
		#include <SDL_ttf.h>
	#endif
	#ifndef WITHOUT_SDL_IMAGE_H_
		#include <SDL_image.h>
	#endif
	#ifndef WITHOUT_SDL_MIXER_H_
		#include <SDL_mixer.h>
	#endif

	namespace xSDL{
		void logError(const char* context, const char* error = SDL_GetError());
		void logInfo(const char* context, const char* info);
		void logValue(const char* context, int value);
		bool isFullScreen(SDL_Window *window);
		bool init();
		void quit();

		namespace version{
			extern SDL_version *sdl_compiled;
			extern SDL_version *sdl_linked;

			extern SDL_version *mixer_compiled;
			extern const SDL_version *mixer_linked;

			extern SDL_version *image_compiled;
			extern const SDL_version *image_linked;

			extern SDL_version *ttf_compiled;
			extern const SDL_version *ttf_linked;

			extern SDL_version *net_compiled;
			extern const SDL_version *net_linked;
			
			void log(const char* name, const SDL_version *version_compiled, const SDL_version *version_linked);
		}
	}

#endif
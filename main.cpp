#define SDL_MAIN_USE_CALLBACKS 1
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;
static SDL_Texture *texture = nullptr;
static Mix_Chunk *applaude = nullptr;

static SDL_FPoint texture_pos = {0, 0};
static SDL_FPoint texture_dir = {1, 1};
static SDL_Color texture_col = {255, 255, 255, 255};

static Uint32 last_time = 0;

static int window_width = 1280;
static int window_height = 720;

#define TEXTURE_SIZE 128.0f /* px */
#define TEXTURE_SPEED 225 /* px per second */

#define PERFECT_CORNER_THRESHOLD 7.5f /* px */

static float texture_width = 0.0f;
static float texture_height = TEXTURE_SIZE;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
	SDL_Surface *surface = nullptr;

	SDL_SetAppMetadata("DVD Bounce", "1.0", "io.github.stupidrepo.dvdbounce");

	/* boilerplate START */
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	if (!SDL_CreateWindowAndRenderer("DVD Bounce", window_width, window_height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// init SDL Mixer
	SDL_AudioDeviceID audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	if (!audio_device) {
		SDL_Log("Couldn't open audio device: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	if (!Mix_OpenAudio(audio_device, nullptr)) {
		SDL_Log("Couldn't open audio: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	/* boilerplate END */

	last_time = SDL_GetTicks();

	/* img START */
	char *img_path = nullptr;
	SDL_asprintf(&img_path, "%sassets/dvd-logo.png", SDL_GetBasePath());

	surface = IMG_Load(img_path);
	if (!surface) {
		SDL_Log("Couldn't load image: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	SDL_free(img_path);

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		SDL_Log("Couldn't create static texture: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);

	// keep aspect ratio of the texture
	texture_width = static_cast<float>(texture->w) * (TEXTURE_SIZE / static_cast<float>(texture->h));
	/* img END */

	/* sfx START */
	char *sfx_path = nullptr;
	SDL_asprintf(&sfx_path, "%sassets/applaude.sfx", SDL_GetBasePath());

	applaude = Mix_LoadWAV(sfx_path);
	if (applaude == nullptr) {
		SDL_Log("Couldn't load applaude sfx: %s", SDL_GetError());
	}

	SDL_free(sfx_path);
	/* sfx END */

	SDL_DestroySurface(surface); // no longer needed
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	if (event->type == SDL_EVENT_KEY_UP) {
		if (event->key.key == SDLK_F) {
			const bool was_fullscreen = (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN);

			SDL_SetWindowFullscreen(window, was_fullscreen ? false : SDL_WINDOW_FULLSCREEN);

			const bool did_manage_to_toggle_vis = !was_fullscreen ? SDL_HideCursor() : SDL_ShowCursor();
			if (!did_manage_to_toggle_vis) {
				SDL_Log("Couldn't toggle cursor visibility: %s", SDL_GetError());
			}
		}
	}


	return SDL_APP_CONTINUE;
}

bool did_hit_corner_perfectly() {
	// top-left corner
	const bool top_left = (texture_pos.x <= PERFECT_CORNER_THRESHOLD &&
					texture_pos.y <= PERFECT_CORNER_THRESHOLD);

	// top-right corner
	const bool top_right = (texture_pos.x >= window_width - texture_width - PERFECT_CORNER_THRESHOLD &&
					 texture_pos.y <= PERFECT_CORNER_THRESHOLD);

	// bottom-left corner
	const bool bottom_left = (texture_pos.x <= PERFECT_CORNER_THRESHOLD &&
					   texture_pos.y >= window_height - TEXTURE_SIZE - PERFECT_CORNER_THRESHOLD);

	// bottom-right corner
	const bool bottom_right = (texture_pos.x >= window_width - texture_width - PERFECT_CORNER_THRESHOLD &&
						texture_pos.y >= window_height - TEXTURE_SIZE - PERFECT_CORNER_THRESHOLD);

	return top_left || top_right || bottom_left || bottom_right;
}

bool calculate_new_position() {
	const Uint64 now = SDL_GetTicks();
	const float elapsed = static_cast<float>(now - last_time) / 1000.0f;
	const float distance = elapsed * TEXTURE_SPEED;

	bool bounced = false;

	texture_pos.x += distance * texture_dir.x;
	texture_pos.y += distance * texture_dir.y;

	if (texture_pos.x < 0.0f) {
		texture_pos.x = 0.0f;
		// texture_dir.x = 1.0f + (static_cast<float>(random()) / RAND_MAX * 0.5f - 0.1f);
		texture_dir.x = 1.0f;

		bounced = true;
	} else if (texture_pos.x > window_width - texture_width) {
		texture_pos.x = window_width - texture_width - 1;
		// texture_dir.x = -1.0f + (static_cast<float>(random()) / RAND_MAX * 0.5f - 0.1f);
		texture_dir.x = -1.0f;

		bounced = true;
	}

	if (texture_pos.y < 0.0f) {
		texture_pos.y = 0.0f;
		// texture_dir.y = 1.0f + (static_cast<float>(random()) / RAND_MAX * 0.5f - 0.1f);
		texture_dir.y = 1.0f;

		bounced = true;
	} else if (texture_pos.y > window_height - TEXTURE_SIZE) {
		texture_pos.y = window_height - TEXTURE_SIZE - 1;
		// texture_dir.y = -1.0f + (static_cast<float>(random()) / RAND_MAX * 0.5f - 0.1f);
		texture_dir.y = -1.0f;

		bounced = true;
	}

	last_time = now;
	return bounced;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
	SDL_GetWindowSize(window, &window_width, &window_height);

	const SDL_FRect dst_rect = {
		texture_pos.x,
		texture_pos.y,
		texture_width,
		texture_height
	};

	if (bool bounced = calculate_new_position()) {
		SDL_Log("Bounced at (%f, %f)", texture_pos.x, texture_pos.y);

		texture_col.r = random() % 256;
		texture_col.g = random() % 256;
		texture_col.b = random() % 256;

		if (did_hit_corner_perfectly()) {
			SDL_Log("Hit a corner perfectly at (%f, %f)", texture_pos.x, texture_pos.y);

			// play sound
			if (!Mix_PlayChannel(1, applaude, 1)) {
				SDL_Log("Couldn't play applaude sfx: %s", SDL_GetError());
			}
		}
	}

	// clear screen
	SDL_SetRenderDrawColor(renderer, 32, 24, 24, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	// draw texture
	SDL_SetTextureColorMod(texture, texture_col.r, texture_col.g, texture_col.b);
	SDL_RenderTexture(renderer, texture, nullptr, &dst_rect);

	// draw screen
	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
	SDL_DestroyTexture(texture);
	Mix_FreeChunk(applaude);
}
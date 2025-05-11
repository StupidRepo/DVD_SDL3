#include <raylib.h>
#include <cstdlib>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define TEXTURE_SIZE 128.0f // size of texture in px
#define TEXTURE_SPEED 100.0f // speed of texture in px/s

#define PERFECT_CORNER_HIT_THRESHOLD 10.0f // threshold in px

struct DVD {
	Vector2 texture_size;
	Vector2 position;
	Vector2 direction;
	Color color;
};

bool UpdateDVDPosition(DVD &dvd) {
	dvd.position.x += dvd.direction.x * TEXTURE_SPEED * GetFrameTime();
	dvd.position.y += dvd.direction.y * TEXTURE_SPEED * GetFrameTime();

	const bool xBelowZero = (dvd.position.x < 0.0f);
	const bool yBelowZero = (dvd.position.y < 0.0f);

	if (xBelowZero || dvd.position.x + dvd.texture_size.x > WINDOW_WIDTH) {
		dvd.position.x = xBelowZero ? 0.0f : WINDOW_WIDTH - dvd.texture_size.x;
		dvd.direction.x *= -1;
		return true;
	}

	if (yBelowZero || dvd.position.y + dvd.texture_size.y > WINDOW_HEIGHT) {
		dvd.position.y = yBelowZero ? 0.0f : WINDOW_HEIGHT - dvd.texture_size.y;
		dvd.direction.y *= -1;
		return true;
	}

	return false;
}

bool DidHitCornerPerfectly(const DVD &dvd) {
	return (
		(dvd.position.x < PERFECT_CORNER_HIT_THRESHOLD && dvd.direction.x < 0.0f) ||
		(dvd.position.x + dvd.texture_size.x > WINDOW_WIDTH - PERFECT_CORNER_HIT_THRESHOLD && dvd.direction.x > 0.0f) ||
		(dvd.position.y < PERFECT_CORNER_HIT_THRESHOLD && dvd.direction.y < 0.0f) ||
		(dvd.position.y + dvd.texture_size.y > WINDOW_HEIGHT - PERFECT_CORNER_HIT_THRESHOLD && dvd.direction.y > 0.0f)
	);
}

int main() {
	DVD dvd = {
		.direction = {1, 1},
		.color = {255, 255, 255, 255},
	};

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "DVD Bounce");
	InitAudioDevice();

	/* setup logo */
	Texture2D dvdLogo = LoadTexture("assets/dvd-logo.png");
	GenTextureMipmaps(&dvdLogo);

	SetTextureFilter(dvdLogo, TEXTURE_FILTER_BILINEAR);

	dvdLogo.width *= TEXTURE_SIZE / dvdLogo.height;
	dvdLogo.height = TEXTURE_SIZE;
	dvd.texture_size = {static_cast<float>(dvdLogo.width), static_cast<float>(dvdLogo.height)};

	/* setup sfx */
	const Sound perfectCornerHitSound = LoadSound("assets/applaude.mp3");
	SetSoundVolume(perfectCornerHitSound, 1.0f);

	const Sound bounceSound = LoadSound("assets/bounce.mp3");
	SetSoundVolume(bounceSound, .5f);

	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground({28, 26, 26, 255});

			DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, LIGHTGRAY);
			if (UpdateDVDPosition(dvd)) { // if we bounced this frame
				dvd.color = {
					static_cast<unsigned char>(random() % 256),
					static_cast<unsigned char>(random() % 256),
					static_cast<unsigned char>(random() % 256),
					255
				};

				if (!DidHitCornerPerfectly(dvd)) {
					PlaySound(bounceSound);
				} else {
					PlaySound(perfectCornerHitSound);
				}
			}
			DrawTextureV(dvdLogo, dvd.position, dvd.color);
		EndDrawing();
	}

	UnloadTexture(dvdLogo);

	UnloadSound(perfectCornerHitSound);
	UnloadSound(bounceSound);
}

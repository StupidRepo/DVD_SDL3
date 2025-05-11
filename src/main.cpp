#include <raylib.h>
#include <cstdlib>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define TEXTURE_SIZE 128.0f // size of texture in px
#define TEXTURE_SPEED 100.0f // speed of texture in px/s

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

int main() {
	DVD dvd = {
		.direction = {1, 1},
		.color = {255, 255, 255, 255},
	};

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "DVD Bounce");
	InitAudioDevice();

	Texture2D dvdLogo = LoadTexture("assets/dvd-logo.png");
	GenTextureMipmaps(&dvdLogo);
	SetTextureFilter(dvdLogo, TEXTURE_FILTER_BILINEAR);

	dvdLogo.width *= TEXTURE_SIZE / dvdLogo.height;
	dvdLogo.height = TEXTURE_SIZE;
	dvd.texture_size = {static_cast<float>(dvdLogo.width), static_cast<float>(dvdLogo.height)};

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground({28, 26, 26, 255});

		if (bool didBounceThisFrame = UpdateDVDPosition(dvd)) {
			dvd.color = {
				static_cast<unsigned char>(random() % 256),
				static_cast<unsigned char>(random() % 256),
				static_cast<unsigned char>(random() % 256),
				255
			};
		}

		DrawTextureV(dvdLogo, dvd.position, dvd.color);

		EndDrawing();
	}
}

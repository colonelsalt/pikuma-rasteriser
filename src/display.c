#include "display.h"

#include <stdio.h>

SDL_Window* g_Window = NULL;
SDL_Renderer* g_Renderer = NULL;

uint32_t* g_ColorBuffer = NULL;
SDL_Texture* g_ColorBufferTexture = NULL;

int g_WindowWidth = 800;
int g_WindowHeight = 600;

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS |
				 SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) < 0)
	{
		const char* error = SDL_GetError();
		fprintf(stderr, "Error initialising SDL: %s.\n", error);
		return false;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	g_WindowWidth = display_mode.w;
	g_WindowHeight = display_mode.h;

	g_Window = SDL_CreateWindow(NULL,
								SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED,
								g_WindowWidth, g_WindowHeight,
								SDL_WINDOW_BORDERLESS);
	if (!g_Window)
	{
		const char* error = SDL_GetError();
		fprintf(stderr, "Error creating SDL window: %s\n", error);
		return false;
	}

	g_Renderer = SDL_CreateRenderer(g_Window, -1, 0);
	if (!g_Renderer)
	{
		const char* error = SDL_GetError();
		fprintf(stderr, "Error creating SDL renderer: %s\n", error);
		return false;
	}
	SDL_SetWindowFullscreen(g_Window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
	for (int i = y; i < y + height; i++)
	{
		for (int j = x; j < x + width; j++)
		{
			draw_pixel(j, i, color);
		}
	}
}

void draw_pixel(int x, int y, uint32_t color)
{
	if (x < g_WindowWidth && x >= 0 && y < g_WindowHeight && y >= 0)
		g_ColorBuffer[y * g_WindowWidth + x] = color;
}

void draw_grid(void)
{
	for (int y = 0; y < g_WindowHeight; y += 10)
	{
		for (int x = 0; x < g_WindowWidth; x += 10)
		{
			draw_pixel(x, y, 0xFF333333);
		}
	}
}

void render_color_buffer(void)
{
	SDL_UpdateTexture(g_ColorBufferTexture,
					  NULL,
					  g_ColorBuffer,
					  g_WindowWidth * sizeof(uint32_t));
	SDL_RenderCopy(g_Renderer, g_ColorBufferTexture, NULL, NULL);
}

void clear_color_buffer(uint32_t clearColor)
{
	for (int y = 0; y < g_WindowHeight; y++)
	{
		for (int x = 0; x < g_WindowWidth; x++)
		{
			g_ColorBuffer[(g_WindowWidth * y) + x] = clearColor;
		}
	}
}

void destroy_window(void)
{
	free(g_ColorBuffer);
	SDL_DestroyRenderer(g_Renderer);
	SDL_DestroyWindow(g_Window);
	SDL_Quit();
}
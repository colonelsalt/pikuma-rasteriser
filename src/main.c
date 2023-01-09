#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

bool g_IsRunning = false;

SDL_Window* g_Window = NULL;
SDL_Renderer* g_Renderer = NULL;

uint32_t* g_ColorBuffer = NULL;
SDL_Texture* g_ColorBufferTexture = NULL;

g_WindowWidth = 800;
g_WindowHeight = 600;

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS |
				 SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) < 0)
	{
		char* error = SDL_GetError();
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
		char* error = SDL_GetError();
		fprintf(stderr, "Error creating SDL window: %s\n", error);
		return false;
	}

	g_Renderer = SDL_CreateRenderer(g_Window, -1, 0);
	if (!g_Renderer)
	{
		char* error = SDL_GetError();
		fprintf(stderr, "Error creating SDL renderer: %s\n", error);
		return false;
	}
	SDL_SetWindowFullscreen(g_Window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void setup(void)
{
	g_ColorBuffer = (uint32_t*)malloc(g_WindowWidth * g_WindowHeight * sizeof(uint32_t));
	g_ColorBufferTexture = SDL_CreateTexture(g_Renderer,
											 SDL_PIXELFORMAT_ARGB8888,
											 SDL_TEXTUREACCESS_STREAMING,
											 g_WindowWidth, g_WindowHeight);

}

void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
			g_IsRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				g_IsRunning = false;
			break;
	}
}

void update(void)
{

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

void render(void)
{
	SDL_SetRenderDrawColor(g_Renderer, 255, 0, 0, 255);
	SDL_RenderClear(g_Renderer);

	render_color_buffer();
	clear_color_buffer(0xFFFFFF00);

	SDL_RenderPresent(g_Renderer);
}

void destroy_window(void)
{
	free(g_ColorBuffer);
	SDL_DestroyRenderer(g_Renderer);
	SDL_DestroyWindow(g_Window);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	g_IsRunning = initialize_window();
	
	setup();
	while (g_IsRunning)
	{
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}
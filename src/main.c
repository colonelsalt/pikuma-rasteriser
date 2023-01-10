#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

#include "display.h"

bool g_IsRunning = false;

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

void render(void)
{
	SDL_SetRenderDrawColor(g_Renderer, 255, 0, 0, 255);
	SDL_RenderClear(g_Renderer);

	draw_grid();
	draw_rect(200, 200, 800, 800, 0xFF00FF00);

	render_color_buffer();
	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(g_Renderer);
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
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

SDL_Window* g_Window = NULL;
SDL_Renderer* g_Renderer = NULL;
bool g_IsRunning = false;

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS |
				  SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) < 0)
	{
		char* error = SDL_GetError();
		fprintf(stderr, "Error initialising SDL: %s.\n", error);
		return false;
	}

	g_Window = SDL_CreateWindow(NULL,
								SDL_WINDOWPOS_CENTERED,
							    SDL_WINDOWPOS_CENTERED,
							    800, 600,
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
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	return true;
}

void setup(void)
{

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

	return 0;
}
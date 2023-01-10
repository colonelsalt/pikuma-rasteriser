#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

#include "display.h"
#include "vector.h"

#define NUM_POINTS 9 * 9 * 9
vec3_t g_CubePoints[NUM_POINTS];

vec2_t g_ProjectedPoints[NUM_POINTS];

bool g_IsRunning = false;

vec3_t g_CameraPos = { 0, 0, -5 };

const float FOV_FACTOR = 640.0f;

void setup(void)
{
	g_ColorBuffer = (uint32_t*)malloc(g_WindowWidth * g_WindowHeight * sizeof(uint32_t));
	g_ColorBufferTexture = SDL_CreateTexture(g_Renderer,
											 SDL_PIXELFORMAT_ARGB8888,
											 SDL_TEXTUREACCESS_STREAMING,
											 g_WindowWidth, g_WindowHeight);

	int point_count = 0;
	for (float x = -1; x <= 1; x += 0.25f)
	{
		for (float y = -1; y <= 1; y += 0.25f)
		{
			for (float z = -1; z <= 1; z += 0.25f)
			{
				vec3_t point = { .x = x, .y = y, .z = z };
				g_CubePoints[point_count++] = point;
			}
		}
	}

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

vec2_t project(vec3_t point)
{
	vec2_t projected_point = {
		.x = (FOV_FACTOR * point.x) / point.z,
		.y = (FOV_FACTOR * point.y) / point.z
	};
	return projected_point;
}

void update(void)
{
	for (int i = 0; i < NUM_POINTS; i++)
	{
		vec3_t point = g_CubePoints[i];
		point.z -= g_CameraPos.z;

		vec2_t projected_point = project(point);
		g_ProjectedPoints[i] = projected_point;
	}
}

void render(void)
{
	draw_grid();

	for (int i = 0; i < NUM_POINTS; i++)
	{
		vec2_t p = g_ProjectedPoints[i];
		draw_rect(p.x + (g_WindowWidth / 2),
				  p.y + (g_WindowHeight / 2),
				  4, 4,
				  0xFFFFFF00);
	}

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
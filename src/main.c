#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

#include "display.h"
#include "vector.h"
#include "mesh.h"

triangle_t g_TrianglesToRender[NUM_MESH_FACES];

bool g_IsRunning = false;

int g_PrevFrameTime = 0;

vec3_t g_CameraPos = { 0, 0, -5 };
vec3_t g_CubeRotation = { 0, 0, 0 };

const float FOV_FACTOR = 640.0f;

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
	int time_to_sleep = FRAME_TARGET_TIME - (SDL_GetTicks() - g_PrevFrameTime);
	if (time_to_sleep > 0 && time_to_sleep <= FRAME_TARGET_TIME)
	{
		SDL_Delay(time_to_sleep);
	}
	g_PrevFrameTime = SDL_GetTicks();

	g_CubeRotation.x += 0.01f;
	g_CubeRotation.y += 0.01f;
	g_CubeRotation.z += 0.01f;

	for (int i = 0; i < NUM_MESH_FACES; i++)
	{
		face_t mesh_face = g_MeshFaces[i];

		vec3_t face_vertices[3];

		face_vertices[0] = g_MeshVertices[mesh_face.a - 1];
		face_vertices[1] = g_MeshVertices[mesh_face.b - 1];
		face_vertices[2] = g_MeshVertices[mesh_face.c - 1];

		triangle_t projected_triangle;

		for (int j = 0; j < 3; j++)
		{
			vec3_t transformed_vertex = face_vertices[j];

			transformed_vertex = vec3_rotate_x(transformed_vertex, g_CubeRotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, g_CubeRotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, g_CubeRotation.z);

			// Shift point away from camera
			transformed_vertex.z -= g_CameraPos.z;

			vec2_t projected_point = project(transformed_vertex);

			// Shift point to middle of screen
			projected_point.x += (g_WindowWidth / 2);
			projected_point.y += (g_WindowHeight / 2);

			projected_triangle.points[j] = projected_point;
		}

		g_TrianglesToRender[i] = projected_triangle;
	}
}

void render(void)
{
	draw_grid();

	for (int i = 0; i < NUM_MESH_FACES; i++)
	{
		triangle_t triangle = g_TrianglesToRender[i];

		draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00);
		draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00);
		draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00);
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
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

extern SDL_Window* g_Window;
extern SDL_Renderer* g_Renderer;

extern uint32_t* g_ColorBuffer;
extern SDL_Texture* g_ColorBufferTexture;

extern int g_WindowWidth;
extern int g_WindowHeight;

bool initialize_window(void);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_grid(void);
void render_color_buffer(void);
void clear_color_buffer(uint32_t clearColor);
void destroy_window(void);

#endif
#ifndef __GUI_H__
#define __GUI_H__

#include <SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define GUI_WIN_NAME "Hawknest"

#define GUI_WIN_SIZEX 640
#define GUI_WIN_SIZEY 480

#define GUI_FONT_FILE "fonts/font.otf"
#define GUI_FONT_SIZE 14

#define GUI_CGA_WIDTH 80
#define GUI_CGA_HEIGHT 25

#define GUI_X_POS(x) ((GUI_WIN_SIZEX / GUI_CGA_WIDTH)*(x))
#define GUI_Y_POS(y) ((GUI_WIN_SIZEY / GUI_CGA_HEIGHT)*(y))


typedef void (*key_handler_t)(SDL_Keycode k, void * priv_data);

typedef struct gui_state {
	SDL_Window * win;
	SDL_Renderer * rend;
	TTF_Font * font;
	SDL_Color font_color;

	key_handler_t key_handler;
	void * key_handler_data;

} gui_state_t;

void gui_init (struct system * sys);
void gui_deinit (gui_state_t * gs);
void gui_loop (struct system * sys);
int gui_register_keyhandler(gui_state_t * gs, key_handler_t handler, void * priv_data);

void gui_draw_char (struct system * sys, char c, int x, int y);
void gui_clear_screen (gui_state_t * gs);
void gui_update_screen (gui_state_t * gs);

#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <base.h>
#include <sys.h>
#include <gui/gui.h>
#include <dev/controller.h>
#include <mos6502/cpu.h>

#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>



static SDL_Texture * 
gui_render_txt (gui_state_t * gs, const char * msg)
{
	SDL_Surface * surf = TTF_RenderText_Blended(gs->font, msg, gs->font_color);
	if (!surf) {
		ERROR_PRINT("COuld not create surface: %s\n", SDL_GetError());
		return NULL;
	}

	SDL_Texture * texture = SDL_CreateTextureFromSurface(gs->rend, surf);

	if (!texture) {
		ERROR_PRINT("Could not create texture: %s\n", SDL_GetError());
		return NULL;
	}

	SDL_FreeSurface(surf);

	return texture;
}



static void
render_texture (SDL_Texture * tex, SDL_Renderer * rend, int x, int y)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(rend, tex, NULL, &dst);
}


void
gui_draw_char (system_t * sys, char c, int x, int y)
{
	gui_state_t * gs = sys->gui;
	int iw, ih, xpos, ypos;
	SDL_Texture * img = NULL;

	char buf[2] = {c, '\0'};

	img = gui_render_txt(gs, buf);

	SDL_QueryTexture(img, NULL, NULL, &iw, &ih);

	//xpos = GUI_X_POS(x);
	xpos = x*iw;
	//ypos = GUI_Y_POS(y);
	ypos = y*ih;

	render_texture(img, gs->rend, xpos, ypos);
}


void
gui_init (system_t * sys)
{
	gui_state_t * gs = NULL;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		ERROR_PRINT("Could not init SDL: %s\n", SDL_GetError());
		return;
	}

	if (TTF_Init() != 0) {
		ERROR_PRINT("Could not init SDL TTL library: %s\n", SDL_GetError());
		return;	
	}


	gs = malloc(sizeof(gui_state_t));
	if (!gs) {
		ERROR_PRINT("Could not allocate GUI state\n");
		goto out_err;
	}

	memset(gs, 0, sizeof(gui_state_t));
	
	gs->win = SDL_CreateWindow(GUI_WIN_NAME,
				   SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED,
			           GUI_WIN_SIZEX, 
				   GUI_WIN_SIZEY,
				   0);

	if (!gs->win) {
		ERROR_PRINT("Could not create GUI window: %s\n", SDL_GetError());
		goto out_err1;
	}

	gs->rend = SDL_CreateRenderer(gs->win,
				      -1,
				      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!gs->rend) {
		ERROR_PRINT("Could not create GUI renderer: %s\n", SDL_GetError());
		goto out_err2;
	}

	gs->font = TTF_OpenFont(GUI_FONT_FILE, GUI_FONT_SIZE);

	if (!gs->font) {
		ERROR_PRINT("Could not open font file: %s\n", TTF_GetError());
		goto out_err3;
	}

	// white
	gs->font_color.r = 255;
	gs->font_color.g = 255;
	gs->font_color.b = 255;
	gs->font_color.a = 255;

	sys->gui = gs;

	return;

out_err3:
	SDL_DestroyRenderer(gs->rend);
out_err2:
	SDL_DestroyWindow(gs->win);
out_err1:
	free(gs);
out_err:
	SDL_Quit();
}


void
gui_deinit (gui_state_t * gs)
{
	TTF_CloseFont(gs->font);
	TTF_Quit();

	SDL_DestroyRenderer(gs->rend);
	SDL_DestroyWindow(gs->win);
	SDL_Quit();
	free(gs);
}

int
gui_register_keyhandler (gui_state_t * gs,
			 key_handler_t handler,
			 void * priv_data)
{
	gs->key_handler      = handler;	
	gs->key_handler_data = priv_data;

	INFO_PRINT("Registered keyboard event handler (%p)\n", (void*)handler);

	return 0;
}

static void
default_key_handler (SDL_Keycode k, system_t * sys)
{
	// do nothing
	return;
}

static void 
handle_key (system_t * sys, SDL_Keycode k)
{
	gui_state_t * gs = sys->gui;
	
	if (gs->key_handler) {
		gs->key_handler(k, gs->key_handler_data);
	} else {
		default_key_handler(k, sys);
	}
}


void
gui_clear_screen (gui_state_t * gs)
{
	SDL_RenderClear(gs->rend);
}

void
gui_update_screen (gui_state_t * gs)
{
	SDL_RenderPresent(gs->rend);
}

void 
gui_loop (system_t * sys)
{
	int quit = 0;
	SDL_Event e;

	while (!quit) {

		while (SDL_PollEvent(&e)) {

			switch (e.type) {
				case SDL_KEYDOWN:
					handle_key(sys, e.key.keysym.sym);
					break;
				case SDL_QUIT:
					quit = 1;
					break;
				default:
					break;
			}

		}

		mos6502_step(sys->cpu);
	}

	gui_deinit(sys->gui);
}

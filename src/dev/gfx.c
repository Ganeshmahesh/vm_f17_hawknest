#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <base.h>
#include <sys.h>

#include <dev/gfx.h>

#include <gui/gui.h>


uint8_t frame_buffer[GFX_FB_SIZE];

void
gfx_reset (gfx_card_t * gfx)
{
	memset(frame_buffer, 0, GFX_FB_SIZE);

}

gfx_card_t *
gfx_init (system_t * sys)
{
	gfx_card_t * gfx = NULL;

	
	return gfx;

out_err:
	free(gfx);
	return NULL;

}



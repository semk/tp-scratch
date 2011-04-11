/*
  scratch.c

  'Scratch' Magic Tools Plugin
  Tux Paint - A simple drawing program for children.

  Sreejith K <sreejithemk@gmail.com>

  Copyright (c) 2011-2012 by Sreejith K
  sreejithemk@gmail.com
  http://www.foobarnbaz.com

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  (See COPYING.txt)

  Last updated: Apr 8, 2011
*/


#include <stdio.h>
#include <string.h>  // For "strdup()"
#include <libintl.h>  // For "gettext()"

#include "tp_magic_api.h"  // Tux Paint "Magic" tool API header
#include "SDL_image.h"  // For IMG_Load(), to load our PNG icon
#include "SDL_mixer.h"  // For Mix_LoadWAV(), to load our sound effects


/* Sound effects: */
Mix_Chunk * snd_effect;

/* The current color (an "RGB" value) the user has selected in Tux Paint: */
Uint8 scratch_r, scratch_g, scratch_b;


/* Our local function prototypes: */
void scratch_drag(magic_api * api, int which, SDL_Surface * canvas,
        SDL_Surface * snapshot, int ox, int oy, int x, int y,
        SDL_Rect * update_rect);

void scratch_line_callback(void * ptr, int which,
        SDL_Surface * canvas, SDL_Surface * snapshot,
        int x, int y);


// API Version check
Uint32 scratch_api_version(void)
{
    return(TP_MAGIC_API_VERSION);
}


// Initialization
// Loads the sound from TP's data directory.
int scratch_init(magic_api * api)
{
    int i;
    char fname[1024];

    snprintf(fname, sizeof(fname),
            "%s/sounds/magic/%s",
            api->data_directory, "scratch.wav");

    // Try to load the file!
    snd_effect = Mix_LoadWAV(fname);

    return(snd_effect != NULL);
}


// Report our tool count
int scratch_get_tool_count(magic_api * api)
{
    return(1);
}


// Load icons
SDL_Surface * scratch_get_icon(magic_api * api, int which)
{
    char fname[1024];
    snprintf(fname, sizeof(fname), "%s/images/magic/%s",
            api->data_directory, "scratch.png");

    // Try to load the image, and return the results to Tux Paint:
    return(IMG_Load(fname));
}


// Report our "Magic" tool names
char * scratch_get_name(magic_api * api, int which)
{
    return(strdup("Scratch"));
}


// Report our "Magic" tool descriptions
char * scratch_get_description(magic_api * api, int which, int mode)
{
    return(strdup("Click here and drag to make a scratched effect"));
}


// Report whether we accept colors
int scratch_requires_colors(magic_api * api, int which)
{
    // We set this color for the background
    return 1;
}


// Report what modes we work in
int scratch_modes(magic_api * api, int which)
{
    // This plugin doesn't affect the full-screen),
    // so we're always returning 'MODE_PAINT'
    return MODE_PAINT;
}


// Shut down
void scratch_shutdown(magic_api * api)
{
    // Free (aka release, aka deallocate) the memory used to store the
    // sound effect that we loaded during init():
    Mix_FreeChunk(snd_effect);
}


/* Functions that respond to events in Tux Paint: */
/* ---------------------------------------------- */

// Affect the canvas on click:
void scratch_click(magic_api * api, int which, int mode,
        SDL_Surface * canvas, SDL_Surface * snapshot,
        int x, int y, SDL_Rect * update_rect)
{
    // In our case, a single click (which is also the start of a drag!)
    // is identical to what dragging does, but just at one point, rather
    // than across a line.
    // 
    // So we 'cheat' here, by calling our draw() function with
    // (x,y) for both the beginning and end points of a line.

    scratch_drag(api, which, canvas, snapshot, x, y, x, y, update_rect);
}


// Affect the canvas on drag:
void scratch_drag(magic_api * api, int which, SDL_Surface * canvas,
        SDL_Surface * snapshot, int ox, int oy, int x, int y,
        SDL_Rect * update_rect)
{
    // Call Tux Paint's "line()" function.
    api->line((void *) api, which, canvas, snapshot,
            ox, oy, x, y, 1, scratch_line_callback);


    // If we need to, swap the X and/or Y values, so that
    // (ox,oy) is always the top left, and (x,y) is always the bottom right,
    // so the values we put inside "update_rect" make sense:

    if (ox > x) { int tmp = ox; ox = x; x = tmp; }
    if (oy > y) { int tmp = oy; oy = y; y = tmp; }


    // Fill in the elements of the "update_rect" SDL_Rect structure
    // that Tux Paint is sharing with us.

    update_rect->x = ox - 4;
    update_rect->y = oy - 4;
    update_rect->w = (x + 4) - update_rect->x;
    update_rect->h = (y + 4) - update_rect->h;

    // Play the appropriate sound effect
    api->playsound(snd_effect,
            (x * 255) / canvas->w, // pan
            255); // distance
}


// Affect the canvas on release:
void scratch_release(magic_api * api, int which,
        SDL_Surface * canvas, SDL_Surface * snapshot,
        int x, int y, SDL_Rect * update_rect)
{
    // Neither of our effects do anything special when the mouse is released
    // from a click or click-and-drag, so there's no code here...
}


// Accept colors
void scratch_set_color(magic_api * api, Uint8 r, Uint8 g, Uint8 b)
{
    // We simply store the RGB values in the global variables we
    // declared at the top of this file.

    scratch_r = r;
    scratch_g = g;
    scratch_b = b;
}


/* The Magic Effect Routines! */
/* -------------------------- */

// Our "callback" function
void scratch_line_callback(void * ptr, int which,
        SDL_Surface * canvas, SDL_Surface * snapshot,
        int x, int y)
{
    // For technical reasons, we can't accept a pointer to the "magic_api"
    // struct, like the other functions do.
    magic_api * api = (magic_api *) ptr;
    int xx, yy;
    Uint8 r, g, b;
    Uint8 step = 1, colorbits = 255;

    // Fill the circle of radius 4 with centre (x, y) with pixels.
    for (xx = -4; xx <= 4; xx++)
    {
        for (yy = -4; yy<= 4; yy++)
        {
            // Check whether the point is in a circle of radius 4
            if (api->in_circle(xx, yy, 4))
            {
                // Convert the Uint32 pixel value to Uint8 value
                SDL_GetRGB(api->getpixel(canvas, x + xx, y +yy),
                        canvas->format, &r, &g, &b);
                if (r % colorbits == 0) r = 0;
                if (g % colorbits == 0) g = 0;
                if (b % colorbits == 0) b = 0;
                api->putpixel(canvas, x + xx, y + yy, 
                        SDL_MapRGB(canvas->format,
                        r + step, g + step, b + step));
            }
        }
    }
}

// Switch-In event
void scratch_switchin(magic_api * api, int which, int mode, SDL_Surface * canvas)
{
}

// Switch-Out event
void scratch_switchout(magic_api * api, int which, int mode, SDL_Surface * canvas)
{
}

/*
  scratch.c

  Scratch Magic Tools Plugin
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
#include <string.h>
#include "tp_magic_api.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

// Place to hold the sound effect
Mix_Chunk * snd_effect;
// Place to hold the current RGB color
Uint8 scratch_r, scratch_g, scratch_b;

// Local function declaration
void do_scratch(void * ptr, int which, SDL_Surface * canvas,
                SDL_Surface * snapshot, int x, int y);

// API version from which this plugin is built against
Uint32 scratch_api_version(void){
    return(TP_MAGIC_API_VERSION);
}


// Plugin initialization.
// Loads the sound from TP's data directory.
int scratch_init(magic_api * api){
    char fname[1024];
    sprintf(fname, "%s/sounds/magic/scratch.wav", 
            api->data_directory);
    snd_effect = Mix_LoadWAV(fname);
    return(snd_effect != NULL); // Success ?
}


// Tell TP that we have only one tool
int scratch_get_tool_count(magic_api * api){
    return(1);
}


// Load plugin Icon
SDL_Surface * scratch_get_icon(magic_api * api, int which){
    char fname[1024];
    sprintf(fname, "%s/images/magic/scratch.png", 
            api->data_directory);
    return(IMG_Load(fname)); // Return the icon
}


// Give Tux Paint our tool's name
char * ex_get_name(magic_api * api, int which){
    return(strdup("Scratch"));
}


// Give Tux Paint our tool's description
char * scratch_get_description(magic_api * api, int which){
    return(strdup("Click here and drag to make a scratched effect"));
}


// We are not using color palette
int scratch_requires_colors(magic_api * api, int which){
    return(0);
}


// Clean up after ourselves when TP quits.
void scratch_shutdown(magic_api * api){
    // Release RAM used by our sfx
    Mix_FreeChunk(snd_effect);
}


// Respond to clicks (mouse button down event)
void scratch_click(magic_api * api, int which,
        SDL_Surface * canvas,
        SDL_Surface * snapshot,
        int x, int y,
        SDL_Rect * update_rect){
    // In our case a click is same as drag. 
    // So send the same x, y to drag.
    scratch_drag(api, which, canvas, snapshot,
            x, y, x, y, update_rect);
}


// Respond to drags (mouse motion events while
// user is clicking)
void scratch_drag(magic_api * api, int which,
        SDL_Surface * canvas,
        SDL_Surface * snapshot,
        int ox, int oy, int x, int y,
        SDL_Rect * update_rect){
    // Tell Tux Paint to calculate a line between
    // the two points (ox,oy) and (x,y),
    // calling our callback function every step
    api->line((void *) api, which,
            canvas, snapshot,
            ox, oy, x, y, 1, // old, new coordinates and step
            do_scratch); // do the scratch effect
    // Tell TP what part of canvas was changed.
    // We'll take top-left and bottom-right.
    if (ox > x) { int tmp=ox; ox=x; x=tmp; }
    if (oy > y) { int tmp=oy; oy=y; y=tmp; }

    // Fill in the (x,y) and (w,h) elements of
    // the update rectangle for Tux Paint
    // (Our brush is 9x9, centered around (x,y))
    update_rect->x = ox - 4;
    update_rect->y = oy - 4;
    update_rect->w = (x + 4) - update_rect->x;
    update_rect->h = (y + 4) - update_rect->y;

    // Play the sound
    api->playsound(snd_effect,
            (x * 255) / canvas->w,
            255);
}


// Respond to releases (mouse button up event)
void scratch_release(magic_api * api, int which,
        SDL_Surface * canvas,
        SDL_Surface * snapshot,
        int x, int y,
        SDL_Rect * update_rect){
} // Nothing to do here


// Do the actual effect
void do_scratch(void * ptr, int which,
        SDL_Surface * canvas,
        SDL_Surface * snapshot,
        int x, int y){
    // Need to cast, since we get a void ptr
    magic_api * api = (magic_api *) ptr;
    
    // Quick-and-dirty 9x9 round brush
    for (int yy = -4; yy <= 4; yy++){
        for (int xx = -4; xx <= 4; xx++){
            if (api->in_circle(xx, yy, 4)){ // Round?
                // Ask TP to change the canvas...
                api->putpixel(canvas, x+xx, y+yy,
                        SDL_MapRGB(canvas->format,
                            scratch_r, scratch_g, scratch_b));
            }
        }
    }
}

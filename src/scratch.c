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

// API version from which this plugin is built against
Uint32 scratch_api_version(void){
    return (TP_MAGIC_API_VERSION);
}


// Plugin initialization.
// Loads the sound from TP's data directory.
int scratch_init(magic_api * api){
    char fname[1024];
    sprintf(fname, "%s/sounds/magic/scratch.wav", api->data_directory);
    snd_effect = Mix_LoadWAV(fname);
    return (snd_effect != NULL);
}

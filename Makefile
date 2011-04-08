# Makefile to compile Scatch Tool for Tuxpaint
# @author: Sreejith K <sreejithemk@gmail.com>
# Created on 8th Apr 2011

CC := gcc
SDL_LIB_PATH := /usr/include/SDL
TP_LIB_PATH := /usr/local/include/tuxpaint
TP_PLUGIN_DIR := /usr/local/lib/tuxpaint/plugins
TP_DATA_DIR := /usr/local/share/tuxpaint
SOURCE_PATH := ./src/scratch.c
ICON_PATH := ./resources/scratch.png
SOUND_PATH := ./resources/scratch.wav
PLUGIN_NAME := scratch.so
CFLAGS := -I$(SDL_LIB_PATH) -D_GNU_SOURCE=1 -D_REENTRANT -I$(TP_LIB_PATH) -std=c99
BUILD_DIR := ./build

build: clean
	@echo "Compiling plugin"
	mkdir $(BUILD_DIR)
	$(CC) --shared $(CFLAGS) $(SOURCE_PATH) -o $(BUILD_DIR)/$(PLUGIN_NAME)
	@echo "Done."

install:
	@echo "Installing plugin"
	sudo cp $(BUILD_DIR)/$(PLUGIN_NAME) $(TP_PLUGIN_DIR)
	sudo cp $(ICON_PATH) $(TP_DATA_DIR)/images/magic
	sudo cp $(SOUND_PATH) $(TP_DATA_DIR)/sounds/magic
	@echo "Done."

clean:
	@echo "Cleaning up build files"
	rm -rf $(BUILD_DIR)
	@echo "Done."

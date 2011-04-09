# Makefile to compile Scatch Tool for Tuxpaint
# @author: Sreejith K <sreejithemk@gmail.com>
# Created on 8th Apr 2011

CC := gcc
SDL_LIB_PATH := /usr/include/SDL
TP_LIB_PATH := /usr/local/include/tuxpaint
TP_PLUGIN_DIR := /usr/local/lib/tuxpaint/plugins
TP_DATA_DIR := /usr/local/share/tuxpaint
PLUGIN_NAME := scratch
SOURCE_PATH := src/$(PLUGIN_NAME).c
ICON_PATH := resources/$(PLUGIN_NAME).png
SOUND_PATH := resources/$(PLUGIN_NAME).wav
CFLAGS := -I$(SDL_LIB_PATH) -D_GNU_SOURCE=1 -D_REENTRANT -I$(TP_LIB_PATH) -std=c99
BUILD_DIR := build

build: clean
	@echo "Compiling plugin"
	mkdir $(BUILD_DIR)
	$(CC) --shared $(CFLAGS) $(SOURCE_PATH) -o $(BUILD_DIR)/$(PLUGIN_NAME).so
	@echo "Done."

install:
	@echo "Installing plugin"
	cp $(BUILD_DIR)/$(PLUGIN_NAME).so $(TP_PLUGIN_DIR)
	cp $(ICON_PATH) $(TP_DATA_DIR)/images/magic
	cp $(SOUND_PATH) $(TP_DATA_DIR)/sounds/magic
	chmod 644 $(TP_PLUGIN_DIR)/$(PLUGIN_NAME).so
	chmod 644 $(TP_DATA_DIR)/images/magic/$(PLUGIN_NAME).png
	chmod 644 $(TP_DATA_DIR)/sounds/magic/$(PLUGIN_NAME).wav
	@echo "Done."

clean:
	@echo "Cleaning up build files"
	rm -rf $(BUILD_DIR)
	@echo "Done."

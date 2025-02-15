#ifndef MODEL_NAME
	#include "../model/xf1_101.h"
#endif

// for xf1 only at the moment

#include <string.h>
#include <stdint.h>
#include "fujihack.h"
#include "fujifilm.h"
#include "sqlite.h"
#include "hijack.h"

void fujihack();

#define FLASH_TASK_PATCH 0x0064632c

// key codes have different offsets in different menus
enum FujiKey {
	KEY_OK = 0x1,
	KEY_UP = 0x2,
	KEY_DOWN = 0x3,
	KEY_LEFT = 0x4,
	KEY_RIGHT = 0x5,
	KEY_SHUTTER = 0x8,
	KEY_DISPBACK = 0x9,
	KEY_PREVIEW = 0xa2a, // ???
	KEY_FLASH = 0x1c, // when flash is opened/closed
	KEY_WHEEL_CLOCKWISE = 0x1f,
	KEY_WHEEL_COUNTERCLOCKWISE = 0x20,
	KEY_RECORD = 0x2e,
	KEY_FN = 0x2f,
	KEY_EFN = 0x36,
	KEY_SCROLL_DOWN = 0x3a,
	KEY_SCROLL_LEFT = 0x3b,
	KEY_SCROLL_RIGHT = 0x3c,
	KEY_SCROLL_PRESS = 0x3a,
};

static struct Menu {
	char curr_screen;
	uint8_t screens;
	char cursor;
	uint8_t i;
}menu;

void fujihack_init(uintptr_t base) {
	menu.curr_screen = 0;
	menu.screens = 1;
	menu.cursor = 0;

	generate_call((uintptr_t)FLASH_TASK_PATCH, (uintptr_t)(base + fujihack), (uint8_t*)FLASH_TASK_PATCH);
}

void menuPrint(struct Menu *menu, char string[]) {
	if (menu->i == menu->cursor) {
		fuji_screen_write(string, 1, 2 + menu->i, 3, 7);
	} else {
		fuji_screen_write(string, 1, 2 + menu->i, 0, 7);
	}

	menu->i++;
}

void fujihack() {
	struct FujiInputMap *k = (struct FujiInputMap*)MEM_INPUT_MAP;
	char buffer[32];

	uint8_t count = 0;
	while (1) {
		if (k->key_status != 128) {
			switch ((uint8_t)k->key_code) {
			case KEY_DOWN:
				menu.cursor++;
				break;
			case KEY_UP:
				menu.cursor--;
				break;
#if 0 // no support right now
			case KEY_RIGHT:
				if (menu.curr_screen + 1 >= menu.screens) {
					menu.curr_screen++;
				}
				break;
			case KEY_LEFT:
				if (menu.curr_screen - 1 <= menu.screens) {
					menu.curr_screen++;
				}
				break;
#endif
			case KEY_EFN:
				generate_nop((uint8_t*)FLASH_TASK_PATCH);
				fuji_discard_text_buffer();
				return;
			}
		}

		menu.i = 0;
		switch (menu.curr_screen) {
		case 0:
			fuji_screen_write("FujiHack Menu", 1, 1, 10, 7);

			menuPrint(&menu, "FujiHack By Daniel C");

			sqlite_snprintf(sizeof(buffer), buffer, "Key Code: %x", k->key_code);
			menuPrint(&menu, buffer);

			sqlite_snprintf(sizeof(buffer), buffer, "Key Down: %x", k->key_status);
			menuPrint(&menu, buffer);

			sqlite_snprintf(sizeof(buffer), buffer, "Gryroscope: %x", k->gyro);
			menuPrint(&menu, buffer);

			sqlite_snprintf(sizeof(buffer), buffer, "Accelerometer: %x", k->accel);
			menuPrint(&menu, buffer);

			sqlite_snprintf(sizeof(buffer), buffer, "Renders: %u", count);
			menuPrint(&menu, buffer);
		}

		count++;
	}
}

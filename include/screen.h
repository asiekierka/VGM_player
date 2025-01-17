#ifndef __SCREEN_H_INCLIDE__
#define __SCREEN_H_INCLIDE__

#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "vwf.h"
#include "misc_assets.h"

#if defined(NINTENDO)
#define TILE_BANK_0 _VRAM8800
#define TILE_BANK_1 _VRAM8000
#elif defined(SEGA)
static uint8_t AT(0x4000) TILE_BANK_0[];
static uint8_t AT(0x6000) TILE_BANK_1[];
#endif

#define TO_TILE_ADDRESS(BASE, NO) ((BASE) + ((NO) << DEVICE_TILE_SIZE_BITS))

BANKREF_EXTERN(module_screen)

extern const uint8_t * const screen_tile_addresses[DEVICE_SCREEN_HEIGHT];
extern const uint8_t screen_tile_map[DEVICE_SCREEN_HEIGHT * DEVICE_SCREEN_WIDTH];
#if defined(SEGA)
extern const uint8_t screen_tile_attr[DEVICE_SCREEN_HEIGHT * DEVICE_SCREEN_WIDTH];
#endif

inline uint8_t screen_clear_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
    return (w) ? fill_rect(x, y, w, h, SLD_WHITE + BG_COLOR(color)), w : w;
}

inline uint8_t screen_restore_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
#if defined(NINTENDO)
    return (w) ? set_bkg_submap(x, y, w, h, screen_tile_map, DEVICE_SCREEN_WIDTH), w : w;
#elif defined(SEGA)
    if (w) {
        set_bkg_submap(x, y, w, h, screen_tile_map, DEVICE_SCREEN_WIDTH);
        set_bkg_submap_attributes(x, y, w, h, screen_tile_attr, DEVICE_SCREEN_WIDTH);
    }
    return w;
#endif
}

inline uint8_t screen_text_render(uint8_t x, uint8_t y, const uint8_t * text) {
    return vwf_draw_text(screen_tile_addresses[y] + (x << DEVICE_TILE_SIZE_BITS), text, 0);
}

inline uint8_t screen_text_out(uint8_t x, uint8_t y, const uint8_t * text) {
    return screen_restore_rect(x, y, vwf_draw_text(screen_tile_addresses[y] + (x << DEVICE_TILE_SIZE_BITS), text, 0), 1);
}

uint8_t INIT_module_screen(void) BANKED;

#endif
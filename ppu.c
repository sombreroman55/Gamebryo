/* ppu.c
 *
 * PPU implementation
 */

#include "ppu.h"
#include <stdlib.h>

PPU* ppu_init() {
    PPU* ppu = (PPU*)malloc(sizeof(PPU));
    return ppu;
}

void ppu_tick(PPU* ppu) {
    // TODO: Tick the PPU
    // TBD: What does that mean? Draw a line? Draw a pixel? How granular?
}

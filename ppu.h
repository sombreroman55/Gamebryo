/* ppu.h
 *
 * PPU interface
 *
 */

#include <stdint.h>

typedef struct {
    uint8_t y_pos;
    uint8_t x_pos;
    uint8_t byte_num;
    union {
        uint8_t flags;
        struct {
            uint8_t priority : 1;
            uint8_t y_flip : 1;
            uint8_t x_flip : 1;
            uint8_t pallete : 1;
            uint8_t padding : 4;
        };
    };
} OamEntry;

typedef struct {
    uint8_t scx; // Scroll X
    uint8_t scy; // Scroll Y
} PPU;

PPU* ppu_init(void);
void ppu_tick(PPU* ppu);

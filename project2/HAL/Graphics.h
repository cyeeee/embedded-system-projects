/*
 * Graphics.h
 *
 *  Created on: Dec 30, 2019
 *      Author: Matthew Zhong
 */

#ifndef HAL_GRAPHICS_H_
#define HAL_GRAPHICS_H_

#include <HAL/LcdDriver/Crystalfontz128x128_ST7735.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

// default background and foreground color
#define FG_COLOR GRAPHICS_COLOR_WHITE
#define BG_COLOR GRAPHICS_COLOR_BLACK
// highlight color
#define HIGHLIGHT  GRAPHICS_COLOR_GREEN
// border values
#define X_MIN   0
#define Y_MIN   13
#define X_MAX   127
#define Y_MAX   93
// dividing lines
#define Y_1     53
#define X_1     42
#define X_2     84
// selection indicator
#define RADIUS  2
#define ROW_1   49
#define ROW_2   89
#define COLUMN_1_1  11
#define COLUMN_1_2  21
#define COLUMN_1_3  31
#define COLUMN_2_1  53
#define COLUMN_2_2  63
#define COLUMN_2_3  73
#define COLUMN_3_1  96
#define COLUMN_3_2  106
#define COLUMN_3_3  116


struct _GFX
{
    Graphics_Context context;
    uint32_t foreground;
    uint32_t background;
    uint32_t defaultForeground;
    uint32_t defaultBackground;
    Graphics_Rectangle border;      // set border
};
typedef struct _GFX GFX;

GFX GFX_construct(uint32_t defaultForeground, uint32_t defaultBackground);

void GFX_resetColors(GFX* gfx_p);
void GFX_clear(GFX* gfx_p);

void GFX_print(GFX* gfx_p, char* string, int row, int col);
void GFX_setForeground(GFX* gfx_p, uint32_t foreground);
void GFX_setBackground(GFX* gfx_p, uint32_t background);

void GFX_drawSolidCircle(GFX* gfx_p, int x, int y, int radius);
void GFX_drawHollowCircle(GFX* gfx_p, int x, int y, int radius);

void GFX_removeSolidCircle(GFX* gfx_p, int x, int y, int radius);
void GFX_removeHollowCircle(GFX* gfx_p, int x, int y, int radius);

void GFX_drawRectangle(GFX* gfx_p, int x_min, int y_min, int x_max, int y_max);
void GFX_drawLineH(GFX* gfx_p, int x1, int x2, int y);
void GFX_drawLineV(GFX* gfx_p, int x, int y1, int y2);

void GFX_hightlightRectangle(GFX* gfx_p, int x_min, int y_min, int x_max, int y_max);

#endif /* HAL_GRAPHICS_H_ */

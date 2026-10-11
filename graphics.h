#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdlib.h>
#include "types.h"
#include "constants.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

//extern uint8_t framebuffer[HEIGHT][WIDTH];

void drawPixel(int x, int y, uint8_t color, uint32_t *framebuffer);
void drawLine(int x0, int y0, int x1, int y1, uint8_t color, uint32_t *framebuffer);
void drawModel(const model *m, camera *cam, point3D pos, uint8_t color, uint32_t *framebuffer);
void flushFrameBuffer();
void drawGround(camera *cam, uint32_t *framebuffer);
smallPoint2D projectPoint(point3D toProject, camera *cam);

#endif

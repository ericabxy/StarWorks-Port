#ifndef BULLET_H
#define BULLET_H

#include "types.h"
#include "constants.h"
#include <stdint.h>

void initBullets();
void updateBullets(camera *cam);
void addBullet(point3D pos, smallPoint3D dir, bool owner);
void drawBullets(camera *cam, uint32_t *framebuffer);

#endif

#include "bullet.h"
#include "constants.h"
#include "graphics.h"
#include "types.h"

static bullet bullets[MAX_BULLETS];

void initBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

void updateBullets(camera *cam) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            continue;
        }
        bullets[i].position.x += bullets[i].direction.x;
        bullets[i].position.y += bullets[i].direction.y;
        bullets[i].position.z += bullets[i].direction.z;

        int16_t distance = bullets[i].position.z - cam->position.z;

        if (distance > 500 || distance < 0) {
            bullets[i].active = false;
        }
    }
}

void addBullet(point3D pos, smallPoint3D dir, bool owner) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].position = pos;
            bullets[i].active = true;
            bullets[i].owner = owner;
            bullets[i].direction = dir;
            return;
        }
    }
}

void drawBullets(camera *cam, uint32_t *framebuffer) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            continue;
        }
        smallPoint2D topScreen = projectPoint(bullets[i].position, cam);

        if (topScreen.x == -1 && topScreen.y == -1) {
            continue;
        }
        point3D bottom = {
            bullets[i].position.x - bullets[i].direction.x * 5,
            bullets[i].position.y - bullets[i].direction.y * 5,
            bullets[i].position.z - bullets[i].direction.z * 5
        };
        smallPoint2D bottomScreen = projectPoint(bottom, cam);
        
        if (bottomScreen.x == -1 && bottomScreen.y == -1) {
            drawPixel(topScreen.x, topScreen.y, 4, framebuffer);
        } else {
            drawLine(topScreen.x, topScreen.y, bottomScreen.x, bottomScreen.y, 4, framebuffer);
        }
    }
}

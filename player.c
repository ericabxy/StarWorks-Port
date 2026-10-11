#include "player.h"
#include "bullet.h"
#include "types.h"

/* Modified: Player keys now detected in (libretro.c). */
static int shootCooldown = 0;

void updatePlayer(ship *player, camera *cam/*, eadk_keyboard_state_t keyboardState*/) {
    // it looks good now :D
/*
    if (shootCooldown > 0) {
        shootCooldown--;
    }

    if (eadk_keyboard_key_down(keyboardState, eadk_key_up)) {
        player->position.y += 1;
    }

    if (eadk_keyboard_key_down(keyboardState, eadk_key_down)) {
        player->position.y -= 1;
    }

    if (eadk_keyboard_key_down(keyboardState, eadk_key_left)) {
        player->position.x -= 1;
    }

    if (eadk_keyboard_key_down(keyboardState, eadk_key_right)) {
        player->position.x += 1;
    }

    if (eadk_keyboard_key_down(keyboardState, eadk_key_ok) && shootCooldown == 0) {
        addBullet(
            (point3D) {player->position.x, player->position.y, player->position.z + 100},
            (smallPoint3D) {0, 0, 10},
            0 // player
        );
        shootCooldown = 12;
    }
*/
    cam->position.z += CAMERA_SPEED;
    player->position.z = cam->position.z + 80;

    if (player->position.x > cam->position.x + 50) { player->position.x = cam->position.x + 50;} 
    if (player->position.x < cam->position.x - 50) { player->position.x = cam->position.x - 50;} 
    if (player->position.y > cam->position.y + 50) { player->position.y = cam->position.y + 50;} 
    if (player->position.y < cam->position.y - 50) { player->position.y = cam->position.y - 50;} 
}

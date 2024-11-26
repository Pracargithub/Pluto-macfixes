// sound_birds.inc.c
#include "src/saturn/saturn.h"

void bhv_birds_sound_loop(void) {
    if (gCamera && gCamera->mode == CAMERA_MODE_BEHIND_MARIO || freeze_camera == true)
        return;

    switch (o->oBehParams2ndByte) {
        case 0:
            cur_obj_play_sound_1(SOUND_OBJ2_BIRD_CHIRP1);
            break;

        case 1:
            cur_obj_play_sound_1(SOUND_GENERAL2_BIRD_CHIRP2);
            break;

        case 2:
            cur_obj_play_sound_1(SOUND_OBJ_BIRD_CHIRP3);
            break;
    }
}

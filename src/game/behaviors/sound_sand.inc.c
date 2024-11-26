// sound_sand.inc.c
#include "src/saturn/saturn.h"

void bhv_sand_sound_loop(void) {
    if (gCamera->mode == CAMERA_MODE_BEHIND_MARIO || freeze_camera == true)
        return;

    cur_obj_play_sound_1(SOUND_ENV_MOVINGSAND);
}

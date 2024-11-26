// sound_waterfall.inc.c
#include "src/saturn/saturn.h"

void bhv_waterfall_sound_loop(void) {
    if (freeze_camera == true) return;
    
    cur_obj_play_sound_1(SOUND_ENV_WATERFALL2);
}

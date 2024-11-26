// sound_ambient.inc.c
#include "src/saturn/saturn.h"

void bhv_ambient_sounds_init(void) {
    if (gCamera->mode == CAMERA_MODE_BEHIND_MARIO || freeze_camera == true)
        return;

    play_sound(SOUND_AIR_CASTLE_OUTDOORS_AMBIENT, gGlobalSoundSource);
}

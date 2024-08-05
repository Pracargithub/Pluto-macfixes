#ifndef SaturnImGuiAnimations
#define SaturnImGuiAnimations

#include <SDL2/SDL.h>
#include <PR/ultratypes.h>

#ifdef __cplusplus

#include <string>

extern "C" {
#endif
extern bool update_bone_anim(struct MarioState* m);
extern void OpenAnimationsMenu();
#ifdef __cplusplus
}
#endif

#endif
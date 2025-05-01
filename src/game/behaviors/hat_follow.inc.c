#include "game/mario.h"
#include "game/mario_misc.h"
#include "game/obj_behaviors.h"
#include "game/object_helpers.h"
#include "engine/math_util.h"
#include "behavior_data.h"
#include "saturn/saturn_models.h"

void bhv_hat_follow_init(void) {
    // Optionally set model or animation here
}

void bhv_hat_follow_loop(void) {
    if (gMarioStates[0].marioObj == NULL) return;
    if (active_accessory_index == -1) return;

    struct MarioState *m = gMarioState;

    f32 sinYaw = sins(m->faceAngle[1]);
    f32 cosYaw = coss(m->faceAngle[1]);

    f32 offsetX = hat_pos[0] * cosYaw - hat_pos[2] * sinYaw;
    f32 offsetZ = hat_pos[0] * sinYaw + hat_pos[2] * cosYaw;

    o->oPosX = m->pos[0] + offsetX;
    o->oPosY = m->pos[1] + hat_pos[1];
    o->oPosZ = m->pos[2] + offsetZ;

    o->oFaceAnglePitch = m->faceAngle[0] + (s16)(hat_rot[0] * 182.04f);
    o->oFaceAngleYaw   = m->faceAngle[1] + (s16)(hat_rot[1] * 182.04f);
    o->oFaceAngleRoll  = m->faceAngle[2] + (s16)(hat_rot[2] * 182.04f);

    o->header.gfx.pos[0] = m->marioObj->header.gfx.pos[0];
    o->header.gfx.pos[1] = m->marioObj->header.gfx.pos[1];
    o->header.gfx.pos[2] = m->marioObj->header.gfx.pos[2];
    o->header.gfx.angle[0] = m->marioObj->header.gfx.angle[0];
    o->header.gfx.angle[1] = m->marioObj->header.gfx.angle[1];
    o->header.gfx.angle[2] = m->marioObj->header.gfx.angle[2];
    o->header.gfx.scale[0] = m->marioObj->header.gfx.scale[0] * hat_scale[0];
    o->header.gfx.scale[1] = m->marioObj->header.gfx.scale[1] * hat_scale[1];
    o->header.gfx.scale[2] = m->marioObj->header.gfx.scale[2] * hat_scale[2];

    // Copy animation frame and state
    o->header.gfx.animInfo.animID = m->marioObj->header.gfx.animInfo.animID;
    o->header.gfx.animInfo.animFrame = m->marioObj->header.gfx.animInfo.animFrame;
    o->header.gfx.animInfo.animFrameAccelAssist = m->marioObj->header.gfx.animInfo.animFrameAccelAssist;
    o->header.gfx.animInfo.curAnim = m->marioObj->header.gfx.animInfo.curAnim;
    o->header.gfx.animInfo.animAccel = m->marioObj->header.gfx.animInfo.animAccel;
    o->header.gfx.animInfo.animYTrans = m->marioObj->header.gfx.animInfo.animYTrans;
}
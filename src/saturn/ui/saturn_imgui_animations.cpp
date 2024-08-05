#include "saturn_imgui_animations.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "saturn/saturn.h"
#include "saturn/saturn_colors.h"
#include "saturn/saturn_models.h"
#include "saturn/saturn_textures.h"
#include "saturn/saturn_animations.h"
#include "saturn/libs/imgui/imgui.h"
#include "saturn/libs/imgui/imgui_internal.h"
#include "saturn/libs/imgui/imgui_impl_sdl.h"
#include "saturn/libs/imgui/imgui_impl_opengl3.h"
#include "saturn/libs/imgui/imgui_impl_opengl3_loader.h"
extern "C" {
    #include "pc/pc_main.h"
    #include "pc/gfx/gfx_pc.h"
    #include "pc/djui/djui.h"
    #include "pc/djui/djui_chat_box.h"
    #include "pc/djui/djui_console.h"
    #include "pc/djui/djui_interactable.h"
    #include "pc/network/network_player.h"
    #include "game/object_collision.h"
    #include "game/camera.h"
    #include "game/mario.h"
    #include "engine/math_util.h"
    #include "engine/behavior_script.h"
}

#include <SDL2/SDL.h>

static char animSearchTerm[128];
PlutoAnim current_panim;

s16 bone_anim_values[61];
u16 bone_anim_indices[126] = {
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,
    0x0001, 0x0001, 0x0001, 0x0002, 0x0001, 0x0003,
    0x0001, 0x0004, 0x0001, 0x0005, 0x0001, 0x0006,
    0x0001, 0x0007, 0x0001, 0x0008, 0x0001, 0x0009,
    0x0001, 0x000A, 0x0001, 0x000B, 0x0001, 0x000C,
    0x0001, 0x000F, 0x0001, 0x000E, 0x0001, 0x000F,
    0x0001, 0x0010, 0x0001, 0x0011, 0x0001, 0x0012,
    0x0001, 0x0013, 0x0001, 0x0014, 0x0001, 0x0015,
    0x0001, 0x0016, 0x0001, 0x0017, 0x0001, 0x0018,
    0x0001, 0x0019, 0x0001, 0x001A, 0x0001, 0x001B,
    0x0001, 0x001C, 0x0001, 0x001D, 0x0001, 0x001E,
    0x0001, 0x001F, 0x0001, 0x0020, 0x0001, 0x0021,
    0x0001, 0x0022, 0x0001, 0x0023, 0x0001, 0x0024,
    0x0001, 0x0025, 0x0001, 0x0026, 0x0001, 0x0027,
    0x0001, 0x0028, 0x0001, 0x0029, 0x0001, 0x002A,
    0x0001, 0x002B, 0x0001, 0x002C, 0x0001, 0x002D,
    0x0001, 0x002E, 0x0001, 0x002F, 0x0001, 0x0030,
    0x0001, 0x0031, 0x0001, 0x0032, 0x0001, 0x0033,
    0x0001, 0x0034, 0x0001, 0x0035, 0x0001, 0x0036,
    0x0001, 0x0037, 0x0001, 0x0038, 0x0001, 0x0039,
    0x0001, 0x003A, 0x0001, 0x003B, 0x0001, 0x003C,
};

struct Animation bone_anim = {
    ANIM_FLAG_2,
    1,
    0,
    0,
    0,
    20,
    bone_anim_values,
    bone_anim_indices,
    1,
    61,
    126
};

bool update_bone_anim(struct MarioState* m) {
    struct NetworkPlayer* np = &gNetworkPlayers[m->playerIndex];
    if (!np->bone_editor_active) return false;
    struct AnimInfo* info = &m->marioObj->header.gfx.animInfo;
    info->animAccel = 0;
    info->animFrame = 0;
    info->animID = -1;
    info->animTimer = 0;
    info->animYTrans = m->unkB0;
    info->curAnim = &bone_anim;
    bone_anim_values[0] = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 3; j++) {
            bone_anim_values[1 + i * 3 + j] = np->bone_rotations[i][j] * 65536 / 360;
        }
    }
    return true;
}

void turn_off_bone_editor() {
    if (gNetworkPlayers[0].bone_editor_active)
        set_mario_animation(&gMarioStates[0], MARIO_ANIM_FIRST_PERSON); // reset anim
    gNetworkPlayers[0].bone_editor_active = false;
    pause_anim = false;
}

void OpenAnimationsMenu() {
    bool show_controls = true;
    if (ImGui::BeginTabBar("###animation_tab_bar")) {
        if (ImGui::BeginTabItem("Vanilla")) {
            turn_off_bone_editor();
            enable_custom_anim = false;
            ImGui::SetNextItemWidth(200);
            if (ImGui::BeginCombo("###v_anim_combo", saturn_animations[selected_anim_index], ImGuiComboFlags_None)) {
                ImGui::InputTextWithHint("###anim_search", "Search...", animSearchTerm, IM_ARRAYSIZE(animSearchTerm), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsUppercase);
                ImGui::Separator();
                for (int n = 0; n < 209; n++) {
                    const bool is_selected = (selected_anim_index == n);

                    if (std::string(saturn_animations[n]).find(animSearchTerm) == std::string::npos &&
                        std::string(animSearchTerm) != "")
                        continue;

                    if (ImGui::Selectable(saturn_animations[n], is_selected))
                        selected_anim_index = n;

                    if (is_selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::EndTabItem();
        }
        if (pluto_animations_list.size() > 0) {
            if (ImGui::BeginTabItem("PAnim")) {
                turn_off_bone_editor();
                enable_custom_anim = true;
                if (current_panim.Length == -1) {
                    current_panim = LoadPAnim("dynos/anims/" + pluto_animations_list[0]);
                    loop_anim = current_panim.Looping;
                }

                ImGui::BeginChild("###p_anim_select", ImVec2(200, 125), ImGuiChildFlags_Border);
                ImGui::SetNextItemWidth(200);
                ImGui::InputTextWithHint("###anim_search", "Search...", animSearchTerm, IM_ARRAYSIZE(animSearchTerm), ImGuiInputTextFlags_AutoSelectAll);
                ImGui::Separator();
                for (int n = 0; n < pluto_animations_list.size(); n++) {
                    const bool is_selected = (selected_panim_index == n);

                    if (pluto_animations_list[n].find(animSearchTerm) == std::string::npos &&
                        std::string(animSearchTerm) != "")
                        continue;

                    if (ImGui::Selectable(pluto_animations_list[n].c_str(), is_selected)) {
                        selected_panim_index = n;
                        current_panim = LoadPAnim("dynos/anims/" + pluto_animations_list[n]);
                        loop_anim = current_panim.Looping;
                    }

                    if (is_selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndChild();

                ImGui::BeginChild("###p_metadata", ImVec2(200, 48), ImGuiChildFlags_Border);
                ImGui::Text("%s", current_panim.Name.c_str());
                ImGui::TextDisabled("@ %s", current_panim.Author.c_str());
                ImGui::EndChild();

                ImGui::EndTabItem();
            }
            if (ImGui::IsItemClicked())
                pluto_animations_list = GetPAnimList("dynos/anims");
        }
        if (ImGui::BeginTabItem("Bone")) {
            if (!gNetworkPlayers[0].bone_editor_active) { // Just switched to bone editor, so we copy the anim state
                struct AnimInfo* anim_info = &gMarioStates[0].marioObj->header.gfx.animInfo;
                const u16* index = anim_info->curAnim->index;
                index += 6;
                for (int i = 0; i < 20; i++) {
                    for (int j = 0; j < 3; j++) {
                        int frame = anim_info->animFrame;
                        int offset = 0;
                        if (frame < index[0]) offset = index[1] + frame;
                        else offset = index[1] + index[0] - 1;
                        index += 2;
                        gNetworkPlayers[0].bone_rotations[i][j] = (float)(anim_info->curAnim->values[offset]) * 360.f / 65536.f;
                    }
                }
            }
            show_controls = false;
            gNetworkPlayers[0].bone_editor_active = true;
            int currbone = 0;
#define BONE_ENTRY(name) ImGui::DragFloat3(name, gNetworkPlayers[0].bone_rotations[currbone++]);
            BONE_ENTRY("Root"           );
            BONE_ENTRY("Body"           );
            BONE_ENTRY("Torso"          );
            BONE_ENTRY("Head"           );
            BONE_ENTRY("Left Arm"       );
            BONE_ENTRY("Upper Left Arm" );
            BONE_ENTRY("Lower Left Arm" );
            BONE_ENTRY("Left Hand"      );
            BONE_ENTRY("Right Arm"      );
            BONE_ENTRY("Upper Right Arm");
            BONE_ENTRY("Lower Right Arm");
            BONE_ENTRY("Right Hand"     );
            BONE_ENTRY("Left Leg"       );
            BONE_ENTRY("Upper Left Leg" );
            BONE_ENTRY("Lower Left Leg" );
            BONE_ENTRY("Left Foot"      );
            BONE_ENTRY("Right Leg"      );
            BONE_ENTRY("Upper Right Leg");
            BONE_ENTRY("Lower Right Leg");
            BONE_ENTRY("Right Foot"     );
#undef BONE_ENTRY
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    if (show_controls) {
        if (ImGui::Checkbox("Override Animation", &override_anim))
            if (!override_anim) set_character_animation(&gMarioStates[0], CHAR_ANIM_IDLE_HEAD_LEFT);
        ImGui::Separator();

        ImGui::BeginDisabled(!override_anim);
        if (enable_custom_anim && override_anim) ImGui::Text("Now Playing: %s", current_panim.Name.c_str());
        else ImGui::Text("Now Playing: %s", saturn_animations[gMarioStates[0].marioObj->header.gfx.animInfo.animID]);
        ImGui::BeginDisabled(!pause_anim);
        ImGui::SliderInt("###animation_frame", &paused_frame, gMarioStates[0].marioObj->header.gfx.animInfo.curAnim->loopStart, gMarioStates[0].marioObj->header.gfx.animInfo.curAnim->loopEnd-1, "frame %d", ImGuiSliderFlags_AlwaysClamp);
        ImGui::EndDisabled();
        ImGui::Checkbox("Paused", &pause_anim);
        ImGui::EndDisabled();
        ImGui::SameLine(); ImGui::Checkbox("Hang", &hang_anim);
        ImGui::BeginDisabled(hang_anim);
        ImGui::SameLine(); ImGui::Checkbox("Loop", &loop_anim);
        ImGui::EndDisabled();
    }
}
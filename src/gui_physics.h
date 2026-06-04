/*******************************************************************************************
*
*   Physics v1.0.0 - Physics Engine
*
*   MODULE USAGE:
*       #define GUI_PHYSICS_IMPLEMENTATION
*       #include "gui_physics.h"
*
*       INIT: GuiPhysicsState state = InitGuiPhysics();
*       DRAW: GuiPhysics(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 Kyler Cradic. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_PHYSICS_H
#define GUI_PHYSICS_H
#define GUI_TEXT(data) TextFormat("%0.2f", data)

typedef struct {
    Vector2 anchor01;
    Vector2 anchor02;
    
    bool PhysicsPanelActive;
    float BodyMassValue;
    float BodySizeValue;
    float BodyGravityValue;
    float BodyRestitutionValue;
    float BodyDampingValue;
    bool BodyTypeEditMode;
    int BodyTypeActive;
    float SpringStiffnessValue;
    float SpringDampingValue;
    bool SpringAutoLengthChecked;
    float SpringLengthValue;
    bool EffectorTypeEditMode;
    int EffectorTypeActive;
    float EffectorSizeValue;
    float EffectorForceValue;
    float EffectorAngleValue;
    bool SimulateActive;
    float GravityValue;
    float FPSValue;
    bool ShowDebugChecked;
    bool WorldBoxActive;
    float BodyVelocityValue;

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiPhysicsState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiPhysicsState InitGuiPhysics(void);
void GuiPhysics(GuiPhysicsState *state);

#ifdef __cplusplus
}
#endif

#endif // GUI_PHYSICS_H

/***********************************************************************************
*
*   GUI_PHYSICS IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_PHYSICS_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiPhysicsState InitGuiPhysics(void)
{
    GuiPhysicsState state = { 5 };

    state.anchor01 = (Vector2){ 936, 544 };
    state.anchor02 = (Vector2){ 24, 40 };
    
    state.PhysicsPanelActive = true;
    state.BodyMassValue = 1.0f;
    state.BodySizeValue = 5.0f;
    state.BodyGravityValue = 1.0f;
    state.BodyRestitutionValue = 0.5f;
    state.BodyDampingValue = 0.1f;
    state.BodyTypeEditMode = false;
    state.BodyTypeActive = 0;
    state.SpringStiffnessValue = 1.0f;
    state.SpringDampingValue = 1.0f;
    state.SpringAutoLengthChecked = false;
    state.SpringLengthValue = 1.0f;
    state.EffectorTypeEditMode = false;
    state.EffectorTypeActive = 0;
    state.EffectorSizeValue = 0.0f;
    state.EffectorForceValue = 0.0f;
    state.EffectorAngleValue = 0.0f;
    state.SimulateActive = true;
    state.GravityValue = 9.81f;
    state.FPSValue = 60.0f;
    state.ShowDebugChecked = false;
    state.WorldBoxActive = true;
    state.BodyVelocityValue = 40.0f;

    // Custom variables initialization

    return state;
}

void GuiPhysics(GuiPhysicsState *state)
{
    if (state->BodyTypeEditMode || state->EffectorTypeEditMode) GuiLock();

    if (state->PhysicsPanelActive)
    {
        state->PhysicsPanelActive = !GuiWindowBox((Rectangle){ state->anchor02.x + 0, state->anchor02.y + 0, 312, 744 }, "PHYSICS CONTROLS");
        GuiSliderBar((Rectangle){ state->anchor02.x + 80, state->anchor02.y + 112, 184, 16 }, "MASS", NULL, &state->BodyMassValue, 1, 20);
        GuiSliderBar((Rectangle){ state->anchor02.x + 80, state->anchor02.y + 88, 184, 16 }, "SIZE", NULL, &state->BodySizeValue, 5, 50);
        GuiSliderBar((Rectangle){ state->anchor02.x + 80, state->anchor02.y + 136, 184, 16 }, "GRAVITY", NULL, &state->BodyGravityValue, 0, 100);
        GuiSliderBar((Rectangle){ state->anchor02.x + 80, state->anchor02.y + 160, 184, 16 }, "BOUNCE", NULL, &state->BodyRestitutionValue, 0, 1);
        GuiSliderBar((Rectangle){ state->anchor02.x + 80, state->anchor02.y + 184, 184, 16 }, "DAMPING", NULL, &state->BodyDampingValue, 0, 100);
        GuiGroupBox((Rectangle){ state->anchor02.x + 8, state->anchor02.y + 256, 296, 120 }, "SPRING");
        GuiSliderBar((Rectangle){ state->anchor02.x + 80, state->anchor02.y + 272, 184, 16 }, "STIFFNESS", NULL, &state->SpringStiffnessValue, 1, 5);
        GuiSliderBar((Rectangle){ state->anchor02.x + 80, state->anchor02.y + 296, 184, 16 }, "DAMPING", NULL, &state->SpringDampingValue, 0, 1);
        GuiCheckBox((Rectangle){ state->anchor02.x + 104, state->anchor02.y + 344, 16, 16 }, "AUTO LENGTH", &state->SpringAutoLengthChecked);
        GuiSliderBar((Rectangle){ state->anchor02.x + 80, state->anchor02.y + 320, 184, 16 }, "LENGTH", NULL, &state->SpringLengthValue, 0, 100);
        GuiGroupBox((Rectangle){ state->anchor02.x + 8, state->anchor02.y + 392, 296, 136 }, "EFFECTOR");
        GuiSliderBar((Rectangle){ state->anchor02.x + 80, state->anchor02.y + 448, 184, 16 }, "SIZE", NULL, &state->EffectorSizeValue, 20, 200);
        GuiSlider((Rectangle){ state->anchor02.x + 80, state->anchor02.y + 472, 184, 16 }, "FORCE", NULL, &state->EffectorForceValue, 10000, 100000);
        GuiSliderBar((Rectangle){ state->anchor02.x + 80, state->anchor02.y + 496, 184, 16 }, "ANGLE", NULL, &state->EffectorAngleValue, 0, 360);
        GuiSliderBar((Rectangle){ state->anchor02.x + 80, state->anchor02.y + 208, 184, 16 }, "VELOCITY", NULL, &state->BodyVelocityValue, 0, 100);
        GuiGroupBox((Rectangle){ state->anchor02.x + 8, state->anchor02.y + 32, 296, 208 }, "BODY");
        if (GuiDropdownBox((Rectangle){ state->anchor02.x + 32, state->anchor02.y + 48, 240, 24 }, "DYNAMIC;KINEMATIC;STATIC", &state->BodyTypeActive, state->BodyTypeEditMode)) state->BodyTypeEditMode = !state->BodyTypeEditMode;
        if (GuiDropdownBox((Rectangle){ state->anchor02.x + 32, state->anchor02.y + 408, 240, 24 }, "GRAVITATION;POINT;AREA;DRAG", &state->EffectorTypeActive, state->EffectorTypeEditMode)) state->EffectorTypeEditMode = !state->EffectorTypeEditMode;
        GuiGroupBox((Rectangle){ state->anchor02.x + 8, state->anchor02.y + 540, 296, 158 }, "Other");
        GuiToggle((Rectangle){ state->anchor02.x + 25, state->anchor01.y + 130, 264, 24 }, "SIMULATE", &state->SimulateActive);
        GuiSlider((Rectangle){ state->anchor02.x + 80, state->anchor01.y + 75, 184, 16 }, "GRAVITY", NULL, &state->GravityValue, -9.81, 30);
        GuiSliderBar((Rectangle){ state->anchor02.x + 80, state->anchor01.y + 50, 184, 16 }, "FPS", NULL, &state->FPSValue, 0, 100);
        GuiCheckBox((Rectangle){ state->anchor02.x + 80, state->anchor01.y + 95, 16, 16 }, "SHOW DEBUG", &state->ShowDebugChecked);
    }
    
    GuiUnlock();
}

#endif // GUI_PHYSICS_IMPLEMENTATION

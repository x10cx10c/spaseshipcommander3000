/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "../../inc/raylib.h"
#include "../inc/screens.h"  
#include "gameply-space/gp_space.h"
#include "gameply-space/drawingEngine.h"
#include "gameply-space/meteor.h"
#include "gameply-space/guns.h"
#include "gameply-space/ship.h"
#include "gameply-space/interface.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
extern t_interface interface;
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    ShipPositionInit();
    InitTestFigure();
    GUNS_Init();
    HEALTH_Init();
    SHIP_hull_init();
    meteo_init();
}

//1
extern float x_bullet_speed;
//2
extern float x_MAIN_ENGINE_POWER;
extern float x_axel_rotationMAX;
extern int x_MAX_BULLETS_PER_SHOT;
//7
extern float x_FRICTION_FORCE;
//8

extern t_gun_v1 support_gun_1;
extern t_gun_v1 support_gun_2;
// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    if (!interface.level_up){
        // TODO: Update GAMEPLAY screen variables here!

        // Press enter or tap to change to ENDING screen
        ShipControl();
        ShipMove();
        BULLETS_and_METEORS_interceptions_check();
        meteorField_Update();
        GUNS_Update();
        GUN_BULLET_Update();
        SHIP_DETAILS_Update();
        if (interface.gameover) finishScreen = 1;
        // if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
        // {
        //     finishScreen = 1;
        //     PlaySound(fxCoin);
        // }
        HEALTH_Update();
    } else {
        //ship upgrade
        if (IsKeyPressed(KEY_ONE) ){
            x_bullet_speed*= 1.5;
            interface.level_up = false;
        }
        if (IsKeyPressed(KEY_TWO) ){
            x_axel_rotationMAX*= 1.5;
            x_MAIN_ENGINE_POWER*= 1.2;
            interface.level_up = false;
        }
        if (IsKeyPressed(KEY_THREE) ){
            x_MAX_BULLETS_PER_SHOT++;
            interface.level_up = false;
        }
        if (IsKeyPressed(KEY_FOUR) ){
            interface.xp_mult += 0.25f;
            interface.level_up = false;
        }
        if (IsKeyPressed(KEY_FIVE) ){
            interface.healthMAX++;
            interface.level_up = false;
        }
        if (IsKeyPressed(KEY_SIX) ){
            interface.health_regen_delta += 0.2f;
            interface.level_up = false;
        }
        if (IsKeyPressed(KEY_SEVEN) ){
            x_FRICTION_FORCE *= 1.2f;
            interface.level_up = false;
        }
        if (IsKeyPressed(KEY_EIGHT) ){
            support_gun_1.rotationSpeed *= 1.2f;
            support_gun_2.rotationSpeed *= 1.2f;
            interface.level_up = false;
        }


        if (!interface.level_up ) interface.health = interface.healthMAX;
    }
}

extern bool create;
// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    Color col = (Color){ 0, 121/4, 241/4, 255 };    // Blue
    // TODO: Draw GAMEPLAY screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), col);
    DrawTextEx(font, TextFormat("HP %3i", interface.health), (Vector2){ 20, 10 }, font.baseSize*2, 4, RAYWHITE);
    DrawTextEx(font, TextFormat("Fixing %3i/100", (int)interface.health_regen_cur), (Vector2){ 20, 50 }, font.baseSize*2, 4, RAYWHITE);
    DrawTextEx(font, TextFormat("Distance %10i", (int)interface.distance), (Vector2){ 20, 90 }, font.baseSize*2, 4, RAYWHITE);
    DrawTextEx(font, TextFormat("XP %10i/%10i", interface.exp, interface.exp_req), (Vector2){ 20, (GetScreenHeight()-50) }, font.baseSize*2, 4, RAYWHITE);
    // DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

    DrawTestFigure();
    meteorDraw_v2();
    SHIP_draw();
    DrawSpaceShip();
    GUN_BULLET_Draw();
    GUNS_Draw();
    // if (create) create_dbg();
    col = (Color){ 0, 0, 0, 40 }; 
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), col);

    if (interface.level_up) {
        col = (Color){ 0, 0, 0, 180 }; 
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), col);
        DrawTextEx(font, "LEVEL UP!", (Vector2){ 20, 80 }, font.baseSize*3, 4, WHITE);
        DrawTextEx(font, "Press 1: Increse bullet speed", (Vector2){ 20, 140 }, font.baseSize*2, 4, RAYWHITE);
        DrawTextEx(font, "Press 2: Increse spaceship speed", (Vector2){ 20, 170 }, font.baseSize*2, 4, RAYWHITE);
        DrawTextEx(font, "Press 3: Increse bullet amount per shot", (Vector2){ 20, 200 }, font.baseSize*2, 4, RAYWHITE);
        DrawTextEx(font, "Press 4: Increse XP multiplier", (Vector2){ 20, 230 }, font.baseSize*2, 4, RAYWHITE);
        DrawTextEx(font, "Press 5: Increse HP", (Vector2){ 20, 260 }, font.baseSize*2, 4, RAYWHITE);
        DrawTextEx(font, "Press 6: Increse repair speed", (Vector2){ 20, 290 }, font.baseSize*2, 4, RAYWHITE);
        DrawTextEx(font, "Press 7: Increse spaceship stabilization", (Vector2){ 20, 320 }, font.baseSize*2, 4, RAYWHITE);
        DrawTextEx(font, "Press 8: Increse gun rotation speed", (Vector2){ 20, 350 }, font.baseSize*2, 4, RAYWHITE);
    }
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}
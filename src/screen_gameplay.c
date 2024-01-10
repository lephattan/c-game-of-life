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

#include "raylib.h"
#include "screens.h"
#include <stdio.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static int isPlaying = 1;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        // finishScreen = 1;
        PlaySound(fxCoin);
    }
    if (IsKeyPressed(KEY_P))
    {
        if (isPlaying == 1)
        {
            isPlaying = 0;
        }
        else
        {
            isPlaying = 1;
        }
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    DrawRectangle(0, 0, w, h, BLACK);
    Vector2 pos = {20, 10};
    DrawTextEx(font, "GAMEPLAY SCREEN", pos, font.baseSize * 3.0f, 4, MAROON);
    // DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

    char isPlayingStr[] = "ISPLAYING: 1";
    sprintf(isPlayingStr, "ISPLAYING: %d", isPlaying);
    DrawText(isPlayingStr, w - 400, 30, 20, MAROON);
    DrawGameGrid();
}

void DrawGameGrid(void)
{
    int rows = 8;
    int cols = 5;
    int cellWidth;
    int cellHeight;
    int gap = 5;
    int borderThickness = 0;
    int paddingTop = 100;
    int paddingBottom = 50;
    int paddingLeft = 50;
    int paddingRight = 50;

    int w = GetScreenWidth();
    int h = GetScreenHeight();

    int displayWidth = w - paddingLeft - paddingRight;
    int displayHeight = h - paddingTop - paddingBottom;
    int displayPosX = paddingLeft;
    int displayPosY = paddingTop;
    DrawRectangle(displayPosX, displayPosY, displayWidth, displayHeight, GRAY);

    // GRID
    int centerX = displayPosX + displayWidth / 2;
    int centerY = displayPosY + displayHeight / 2;
    // Calculate cell size based the smaller side of display box
    int smallSide;
    if (displayWidth > displayHeight)
    {
        smallSide = displayHeight;
    }
    else
    {
        smallSide = displayWidth;
    }
    int gapSpace = (rows + 1) * gap;
    cellWidth = (smallSide - gapSpace) / rows;
    cellHeight = cellWidth;
    int gridPosX = centerX - (rows * cellWidth + gapSpace) / 2 + gap;
    int gridPosY = centerY - (cols * cellHeight + gapSpace) / 2 + gap;

    int cellInnerWidth = cellWidth - borderThickness * 2;
    int cellInnerHeight = cellHeight - borderThickness * 2;

    Color borderColor = RED;
    Color cellFill = WHITE;

    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            // Draw outer rectangle
            int posX = gridPosX + i * cellHeight + i * gap;
            int posY = gridPosY + j * cellWidth + j * gap;
            DrawRectangle(posX, posY, cellWidth, cellHeight, borderColor);

            // Draw inner rectangle
            int innerPosX = posX + borderThickness;
            int innerPosY = posY + borderThickness;
            DrawRectangle(innerPosX, innerPosY, cellInnerWidth, cellInnerHeight, cellFill);
        }
    }
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!

    // Pause the game when screen is unloaded
    isPlaying = 0;
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}

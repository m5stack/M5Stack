/******************************************************************************
 * M5Snake                                                                    *
 * -------                                                                    *
 * Snake game for M5Stack-Core with Gameboy Keyboard faces                    *
 * Author: Olivier Staquet                                                    *
 * Last version available on https://github.com/ostaquet/M5Snake              *
 ******************************************************************************
 * MIT License
 *
 * Copyright (c) 2020 Olivier Staquet
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************/
#include <M5Stack.h>

#include "GameBoard.h"
#include "GameboyInput.h"
#include "Power.h"

#define GAME_STATUS_INIT       0x00
#define GAME_STATUS_MENU       0x10
#define GAME_STATUS_START_GAME 0x20
#define GAME_STATUS_GAME       0x21
#define GAME_STATUS_GAMEOVER   0x30

#define GAME_CYCLES 3  // Define the number of cycle before moving the snake

/******************************************************************************
 * Global variables
 *******************************************************************************/
// Global game status
uint8_t game_status = GAME_STATUS_INIT;
void gameLoop();
void fromInitToMenu();
void fromMenuToGame();
void fromGameToGameOver();

/******************************************************************************
 * Initialize components
 *******************************************************************************/
void setup() {
    // Set game status
    game_status = GAME_STATUS_INIT;

    // Initialize M5Stack (LCD = true, SD = false, Serial = true, I2C = true)
    M5.begin(true, false, true, true);

    // Show that we are alive
    M5.Lcd.setBrightness(100);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println(F("M5Snake by Olivier Staquet"));

    // Disable white noise on speaker
    // (according to https://community.m5stack.com/topic/61/noise-on-speaker/15)
    dacWrite(25, 0);

    // Start the Gameboy faces controller
    GameboyInput.begin();

    // Show battery management
    Power.begin();
    Power.adaptChargeMode();

    M5.Lcd.print(F("Battery level "));
    int8_t batt = Power.getBatteryLevel();
    if (batt >= 0) {
        M5.Lcd.print(batt);
        M5.Lcd.println(F("%"));
    } else {
        M5.Lcd.println(F("unavailable"));
    }

    // Wait some time
    delay(1000);
}

/******************************************************************************
 * Main loop
 *******************************************************************************/
void loop() {
    // Depending on status, run the routine
    switch (game_status) {
        // Init phase (over)
        case GAME_STATUS_INIT:
            fromInitToMenu();
            break;

        // Start menu
        case GAME_STATUS_MENU:
            if (GameboyInput.getActivity() == GAMEBOY_KEY_START) {
                fromMenuToGame();
            }
            break;

        // Game in progress
        case GAME_STATUS_GAME:
            gameLoop();
            break;

        // Game over
        case GAME_STATUS_GAMEOVER:
            delay(3000);
            fromInitToMenu();
            break;
    }

    // By default, always adapt the charge mode for each cycle
    Power.adaptChargeMode();

    // Cycle delay
    delay(25);
}

/******************************************************************************
 * Routine for the game loop (Snake moving and eating cherry depending on input
 *******************************************************************************/
void gameLoop() {
    // Check the activity from the input
    switch (GameboyInput.getActivity()) {
        case GAMEBOY_KEY_UP:
            GameBoard.setDirection(DIRECTION_UP);
            break;
        case GAMEBOY_KEY_RIGHT:
            GameBoard.setDirection(DIRECTION_RIGHT);
            break;
        case GAMEBOY_KEY_DOWN:
            GameBoard.setDirection(DIRECTION_DOWN);
            break;
        case GAMEBOY_KEY_LEFT:
            GameBoard.setDirection(DIRECTION_LEFT);
            break;
    }

    // Move the snake
    if (!GameBoard.moveSnake()) {
        // If cannot move the snake -> game over...
        fromGameToGameOver();
    } else {
        // Add a cherry some time...
        if (random(0, 15 * GAME_CYCLES) == 0) {
            GameBoard.addCherry();
        }

        GameBoard.refresh();
    }
}

/******************************************************************************
 * Routine from GAME_STATUS_INIT to GAME_STATUS_MENU
 *******************************************************************************/
void fromInitToMenu() {
    // Clear the screen
    M5.Lcd.setBrightness(100);
    M5.Lcd.fillScreen(BLACK);

    // Show the title
    M5.Lcd.setTextSize(5);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.drawString(F("M5Snake"),
                      (M5.Lcd.width() - M5.Lcd.textWidth(F("M5Snake"))) / 2,
                      M5.Lcd.height() / 4);

    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.drawString(
        F("by Olivier Staquet"),
        (M5.Lcd.width() - M5.Lcd.textWidth(F("by Olivier Staquet"))) / 2,
        M5.Lcd.height() / 2);

    // Show "Press start"
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.drawString(
        F("Press START to continue"),
        (M5.Lcd.width() - M5.Lcd.textWidth(F("Press START to continue"))) / 2,
        (M5.Lcd.height() / 4) * 3);

    game_status = GAME_STATUS_MENU;
}

/******************************************************************************
 * Routine from GAME_STATUS_MENU to GAME_STATUS_GAME
 *******************************************************************************/
void fromMenuToGame() {
    // Init the board game
    GameBoard.begin(GAME_CYCLES);

    // Define the head of the snake
    GameBoard.startSnake();
    GameBoard.refresh();

    game_status = GAME_STATUS_GAME;
}

/******************************************************************************
 * Routine from GAME_STATUS_GAME to GAME_STATUS_GAMEOVER
 *******************************************************************************/
void fromGameToGameOver() {
    // Clear the screen
    M5.Lcd.setBrightness(100);
    M5.Lcd.fillScreen(RED);

    M5.Lcd.setTextSize(5);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.drawString(F("GAME OVER"),
                      (M5.Lcd.width() - M5.Lcd.textWidth(F("GAME OVER"))) / 2,
                      M5.Lcd.height() / 4);

    // Max score...
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.drawString(F("Score"),
                      (M5.Lcd.width() - M5.Lcd.textWidth(F("Score"))) / 2,
                      M5.Lcd.height() / 2);

    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.drawString(
        String(GameBoard.getMaxScore()),
        (M5.Lcd.width() - M5.Lcd.textWidth(String(GameBoard.getMaxScore()))) /
            2,
        (M5.Lcd.height() / 4) * 3);

    game_status = GAME_STATUS_GAMEOVER;
}

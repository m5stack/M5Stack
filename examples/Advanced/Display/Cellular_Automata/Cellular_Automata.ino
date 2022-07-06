// The Game of Life, also known simply as Life, is a cellular automaton
// devised by the British mathematician John Horton Conway in 1970.
// https://en.wikipedia.org/wiki/Conway's_Game_of_Life

#include <M5Stack.h>

//#define GRIDX 80
//#define GRIDY 60
//#define CELLXY 4

#define GRIDX  160
#define GRIDY  120
#define CELLXY 2

#define GEN_DELAY 0

// Current grid
uint8_t grid[GRIDX][GRIDY];

// The new grid for the next generation
uint8_t newgrid[GRIDX][GRIDY];

// Number of generations
#define NUMGEN 600

uint16_t genCount = 0;

void setup() {
    // Set up the display
    M5.begin();
    M5.Power.begin();
    // M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setCursor(0, 0);
}

void loop() {
    // Display a simple splash screen
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setCursor(40, 5);
    M5.Lcd.println(F("Arduino"));
    M5.Lcd.setCursor(35, 25);
    M5.Lcd.println(F("Cellular"));
    M5.Lcd.setCursor(35, 45);
    M5.Lcd.println(F("Automata"));

    delay(1000);

    M5.Lcd.fillScreen(TFT_BLACK);

    initGrid();

    genCount = NUMGEN;

    drawGrid();

    // Compute generations
    for (int gen = 0; gen < genCount; gen++) {
        computeCA();
        drawGrid();
        delay(GEN_DELAY);
        for (int16_t x = 1; x < GRIDX - 1; x++) {
            for (int16_t y = 1; y < GRIDY - 1; y++) {
                grid[x][y] = newgrid[x][y];
            }
        }
    }
}

// Draws the grid on the display
void drawGrid(void) {
    uint16_t color = TFT_WHITE;
    for (int16_t x = 1; x < GRIDX - 1; x++) {
        for (int16_t y = 1; y < GRIDY - 1; y++) {
            if ((grid[x][y]) != (newgrid[x][y])) {
                if (newgrid[x][y] == 1)
                    color = 0xFFFF;  // random(0xFFFF);
                else
                    color = 0;
                M5.Lcd.fillRect(CELLXY * x, CELLXY * y, CELLXY, CELLXY, color);
            }
        }
    }
}

// Initialise Grid
void initGrid(void) {
    for (int16_t x = 0; x < GRIDX; x++) {
        for (int16_t y = 0; y < GRIDY; y++) {
            newgrid[x][y] = 0;

            if (x == 0 || x == GRIDX - 1 || y == 0 || y == GRIDY - 1) {
                grid[x][y] = 0;
            } else {
                if (random(3) == 1)
                    grid[x][y] = 1;
                else
                    grid[x][y] = 0;
            }
        }
    }
}

// Compute the CA. Basically everything related to CA starts here
void computeCA() {
    for (int16_t x = 1; x < GRIDX; x++) {
        for (int16_t y = 1; y < GRIDY; y++) {
            int neighbors = getNumberOfNeighbors(x, y);
            if (grid[x][y] == 1 && (neighbors == 2 || neighbors == 3)) {
                newgrid[x][y] = 1;
            } else if (grid[x][y] == 1)
                newgrid[x][y] = 0;
            if (grid[x][y] == 0 && (neighbors == 3)) {
                newgrid[x][y] = 1;
            } else if (grid[x][y] == 0)
                newgrid[x][y] = 0;
        }
    }
}

// Check the Moore neighborhood
int getNumberOfNeighbors(int x, int y) {
    return grid[x - 1][y] + grid[x - 1][y - 1] + grid[x][y - 1] +
           grid[x + 1][y - 1] + grid[x + 1][y] + grid[x + 1][y + 1] +
           grid[x][y + 1] + grid[x - 1][y + 1];
}

/*
   The MIT License (MIT)

   Copyright (c) 2016 RuntimeProjects.com

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

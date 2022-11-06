/******************************************************************************
 * M5Snake : Game board management                                            *
 * -------------------------------                                            *
 * Manage the game board (storage in memory and display                       *
 * Author: Olivier Staquet                                                    *
 * Last version available on https://github.com/ostaquet/M5Snake              *
 *****************************************************************************/
#ifndef _GAMEBOARD_H_
#define _GAMEBOARD_H_

#include <Arduino.h>
#include <M5Stack.h>

#define LCD_WIDTH  320
#define LCD_HEIGHT 240

#define BLOCK_SIZE 16

#define BLOCK_STATUS_EMPTY 0x00  // Empty block
#define BLOCK_STATUS_HEAD  0x01  // Head of the snake
// All values between 2 and 511                   // Body of the snake, greatest
// value = tail
#define BLOCK_STATUS_CHERRY 0x200  // Cherry to increase the size of the snake

#define DIRECTION_UP    0x01
#define DIRECTION_RIGHT 0x02
#define DIRECTION_DOWN  0x03
#define DIRECTION_LEFT  0x04

class GameBoardClass {
   public:
    // Initialize
    void begin(uint8_t game_cycles = 4);

    // Refresh display
    void refresh();

    // Start the snake
    void startSnake();

    // Make the snake move on the board
    // Return boolean true if OK, false if game over
    bool moveSnake();

    // Set direction
    void setDirection(uint8_t direction);

    // Add a ramdom cherry on the board
    void addCherry();

    // Get the max score
    uint16_t getMaxScore();

   private:
    // Variables

    // Current direction
    uint8_t current_direction = 0x00;
    uint8_t current_head_x    = 0x00;
    uint8_t current_head_y    = 0x00;

    // Cycles
    uint8_t max_game_cycles    = 4;
    uint8_t current_game_cycle = 0;

    // Keep track of the size of the board
    uint8_t board_width  = LCD_WIDTH / BLOCK_SIZE;
    uint8_t board_height = LCD_HEIGHT / BLOCK_SIZE;

    // Game board in memory
    uint16_t board_data[LCD_WIDTH / BLOCK_SIZE][LCD_HEIGHT / BLOCK_SIZE];

    // Track change of the game board to optimize refresh
    uint8_t board_changes[LCD_WIDTH / BLOCK_SIZE][LCD_HEIGHT / BLOCK_SIZE];

    // Internal functions
    // Set a value in a cell
    void setCell(uint8_t x, uint8_t y, uint16_t status);

    // Draw a cell with the change indicated
    void drawChange(uint8_t x, uint8_t y);

    // Remove the tail of the snake (put the biggest cell at blank)
    void removeTail();
};

extern GameBoardClass GameBoard;

#endif  // _GAMEBOARD_H_

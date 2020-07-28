/******************************************************************************
 * M5Snake : Game board management                                            *
 * -------------------------------                                            *
 * Manage the game board (storage in memory and display                       *
 * Author: Olivier Staquet                                                    *
 * Last version available on https://github.com/ostaquet/M5Snake              *
 *****************************************************************************/
#include "GameBoard.h"

/**
 * Initialize
 */
void GameBoardClass::begin(uint8_t _max_game_cycles) {
  // Keep the number of game cycles
  max_game_cycles = _max_game_cycles;
  current_game_cycle = 0;
  
  // Init the board at blank
  for(uint8_t x = 0; x < board_width; x++) {
    for(uint8_t y = 0; y < board_height; y++) {
      board_data[x][y] = BLOCK_STATUS_EMPTY;
      board_changes[x][y] = 0;
    }
  }

  // Set screen to blank
  M5.Lcd.fillScreen(BLACK);
}

/**
 * Refresh display
 */
void GameBoardClass::refresh() {
  // Check where there are some changes
  for(uint8_t x = 0; x < board_width; x++) {
    for(uint8_t y = 0; y < board_height; y++) {
      // Check the cell
      if(board_changes[x][y] > 0) {
        // There is a change...
        drawChange(x, y);
        // Reset the change tracking
        board_changes[x][y] = 0;
      }
    }
  }
}

/**
 * Put the head of the snake on the board and go on right
 */
void GameBoardClass::startSnake() {
  // Define the middle of the screen
  setCell(board_width / 2, board_height / 2, BLOCK_STATUS_HEAD);

  // Define the direction
  setDirection(DIRECTION_RIGHT);
}

/**
 * Make the snake move on the board
 * Return boolean true if OK, false if game over
 */
bool GameBoardClass::moveSnake() {
  // Check if it is a cycle to move
  if(current_game_cycle < max_game_cycles) {
    // Wait for the next cycle
    current_game_cycle++;
    return true;
  } else {
    // Reset the game cycle
    current_game_cycle = 0; 
  }
  
  // Add 1 to all current block with between 1 and 512
  // to keep count of the movement of the snake (1 = head, 2 = 2nd block after head...)
  for(uint8_t x = 0; x < board_width; x++) {
    for(uint8_t y = 0; y < board_height; y++) {
      if(board_data[x][y] < BLOCK_STATUS_CHERRY && board_data[x][y] != BLOCK_STATUS_EMPTY) {
        board_data[x][y] = board_data[x][y] + 1;
      }
    }
  }
  
  // Next move to be defined
  int8_t next_block_x = current_head_x;
  int8_t next_block_y = current_head_y;
  
  // Define the next move of the head
  switch(current_direction) {
    case DIRECTION_UP :
      next_block_y = current_head_y - 1;
      break;
    case DIRECTION_RIGHT :
      next_block_x = current_head_x + 1;
      break;
    case DIRECTION_DOWN :
      next_block_y = current_head_y + 1;
      break;
    case DIRECTION_LEFT :
      next_block_x = current_head_x - 1;
      break;
  }

  // Check if the move is valid...
  // Check the limit of the board for X
  if(next_block_x < 0 || next_block_x >= board_width) {
    return false;
  }
  // Check the limit of the board for Y
  if(next_block_y < 0 || next_block_y >= board_height) {
    return false;
  }

  // Check if there is a cherry on the cell (if not a cherry, remove the last block of the tail
  if(board_data[next_block_x][next_block_y] != BLOCK_STATUS_CHERRY) {
    removeTail();
  }
  
  // Check if there is another part of the snake
  if(board_data[next_block_x][next_block_y] > BLOCK_STATUS_EMPTY && board_data[next_block_x][next_block_y] < BLOCK_STATUS_CHERRY) {
    return false;
  }

  // OK, move the head of the snake
  setCell(next_block_x, next_block_y, BLOCK_STATUS_HEAD);

  return true;
}

/**
 * Identify and remove the tail (last block of the snake)
 */
void GameBoardClass::removeTail() {
  uint16_t greatest_value = 0;
  uint8_t tail_x = 0;
  uint8_t tail_y = 0;

  // Find the cell with the biggest value (it is the tail)
  for(uint8_t x = 0; x < board_width; x++) {
    for(uint8_t y = 0; y < board_height; y++) {
      if(board_data[x][y] < BLOCK_STATUS_CHERRY) {
        if(board_data[x][y] > greatest_value) {
          tail_x = x;
          tail_y = y;
          greatest_value = board_data[x][y];
        }
      }
    }
  }

  // Remove the tail
  setCell(tail_x, tail_y, BLOCK_STATUS_EMPTY);
}

/**
 * Get the max score
 */
uint16_t GameBoardClass::getMaxScore() {
  uint16_t greatest_value = 0;

  // Find the cell with the biggest value (it is the tail)
  for(uint8_t x = 0; x < board_width; x++) {
    for(uint8_t y = 0; y < board_height; y++) {
      if(board_data[x][y] < BLOCK_STATUS_CHERRY) {
        if(board_data[x][y] > greatest_value) {
          greatest_value = board_data[x][y];
        }
      }
    }
  }

  return greatest_value - 1;
}

/**
 * Draw the change of one cell
 */
void GameBoardClass::drawChange(uint8_t x, uint8_t y) {
  // Compute box position
  uint16_t pos_x = x * BLOCK_SIZE;
  uint16_t pos_y = y * BLOCK_SIZE;
  
  // Depending on the content of the cell, draw the box
  switch(board_data[x][y]) {
    case BLOCK_STATUS_EMPTY :
      M5.Lcd.fillRect(pos_x, pos_y, BLOCK_SIZE, BLOCK_SIZE, BLACK);
      break;

    case BLOCK_STATUS_CHERRY :
      M5.Lcd.fillRect(pos_x, pos_y, BLOCK_SIZE, BLOCK_SIZE, BLACK);
      M5.Lcd.fillCircle(pos_x + BLOCK_SIZE / 2, pos_y + BLOCK_SIZE / 2, BLOCK_SIZE / 2 - 1, RED);
      break;

    default :
      M5.Lcd.drawRect(pos_x, pos_y, BLOCK_SIZE, BLOCK_SIZE, BLACK);
      M5.Lcd.fillRect(pos_x + 1, pos_y + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, WHITE);
      break;
  }
}

/**
 * Add a ramdom cherry on the board
 */
void GameBoardClass::addCherry() {
  uint8_t pos_x = random(0, board_width);
  uint8_t pos_y = random(0, board_height);

  while(board_data[pos_x][pos_y] != BLOCK_STATUS_EMPTY) {
    pos_x = random(0, board_width);
    pos_y = random(0, board_height);
  }

  setCell(pos_x, pos_y, BLOCK_STATUS_CHERRY);
}

/**
 * Set direction
 */
void GameBoardClass::setDirection(uint8_t direction) {
  current_direction = direction;
}

/**
 * Set a value in a cell
 */
void GameBoardClass::setCell(uint8_t x, uint8_t y, uint16_t status) {
  board_data[x][y] = status;
  board_changes[x][y] = 1;

  if(status == BLOCK_STATUS_HEAD) {
    current_head_x = x;
    current_head_y = y;
  }
}

GameBoardClass GameBoard;

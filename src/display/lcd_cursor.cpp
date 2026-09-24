#include "lcd_cursor.h"

/**
 * @brief By defaults sets rows = 4 and cols = 20
 */
LCD_cursor::LCD_cursor(LiquidCrystal_PCF8574& lcd, uint8_t rows, uint8_t cols) 
: _lcd(lcd), _rows(rows), _cols(cols) {};

void LCD_cursor::print(const char* text) {
  /* check if the pointer is null */
  if(!text) return; 

  /* print the text and automatically skips to the next row */
  for (int i=0; text[i] != '\0'; i++) {
    char c = text[i];

    if (_col >= _cols) {
      _col = 0;
      _row = (_row + 1) % _rows; // returns to zero if it reaches the last row
    }

    _lcd.setCursor(_col,_row);
    _lcd.write(c);
    _col++;
  }
};

/**
 * @brief Print at a specific location, by default it prints in (0,0)
 */
void LCD_cursor::printAt(const char* text, uint8_t row, uint8_t col) {
  _row = row;
  _col = col;
  print(text);
}

/**
 * @brief the same as printAt but it fills the remaining space on the row with blanck space
 */
void LCD_cursor::printPadded(const char* text, uint8_t row, uint8_t col) {
  uint8_t space = _cols - col;
  char buffer[space + 1];
  size_t len = strlen(text);
  if (len > space) len =space;

  memset(buffer,' ',space);
  buffer[space] = '\0';

  memcpy(buffer, text, len);

  printAt(buffer, row, col);
}

/**
 * @brief Print the text with a scrolling effect: it needs to be recalled periodically
 */
void LCD_cursor::printAutoScroll(const char* text, uint8_t row, uint8_t col) {
  size_t len = strlen(text);
  uint8_t space = _cols - col;

  /* If the text is short, it prints it normally */
  if (len <= space) {
    printPadded(text,row,col);
    return;
  }

  /* Prints only the characters that enter in the space */
  char buffer[21];
  strncpy(buffer, text + _offset, space);
  buffer[space] = '\0';       // add the null terminator
  printAt(buffer, row, col);

  _offset = (_offset + 1) % (len - space + 1);  // update the offset

}

void LCD_cursor::clear() {
  _lcd.clear();
  _row = 0;
  _col = 0;
}

void LCD_cursor::newLine() {
  _row = (_row + 1) % _rows;
  _col = 0;
}
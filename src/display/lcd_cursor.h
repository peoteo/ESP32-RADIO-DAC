#ifndef LCD_CURSOR_H
#define LCD_CURSOR_H

#include <LiquidCrystal_PCF8574.h>

/**
 * @class LCD cursor
 * @brief LCD high level render class
 */
class LCD_cursor {
public:
  LCD_cursor(LiquidCrystal_PCF8574& lcd, uint8_t rows = 4, uint8_t cols = 20);

	void print(const char* text);
	void printAt(const char* text, uint8_t row = 0, uint8_t col = 0);
	void printPadded(const char* text, uint8_t row, uint8_t col);
	void printAutoScroll(const char* text, uint8_t row, uint8_t col);
	void clear();
	void newLine();
	
private:
	LiquidCrystal_PCF8574& _lcd;
	uint8_t _rows;
	uint8_t _cols;

	/* cursor position */
	uint8_t _row;
	uint8_t _col;

	/* text offset (autoscroll effect) */
	uint8_t _offset = 0;
 };
#endif
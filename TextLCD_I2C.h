#include "Stream.h"
#include "mbed.h"

/**  A TextLCD interface for driving 4-bit HD44780-based LCDs
 * with  I2C PCF8574 or PCF8574A portexpander
 *
 * Currently supports 16x2, 20x2 and 20x4 panels
 *
 * @code
 * #include "mbed.h"
 * #include "TextLCD_I2C.h"
 *
 * I2C i2c_lcd(I2C_SDA, I2C_SCL);
 * char adr = 0x27<<1; // 0x4E
 * TextLCD_I2C lcd(&i2c_lcd,adr, TextLCD_I2C::LCD16x2); 
 *
 * int main() {
 *     lcd.setBackLight(true);    
 *     lcd.printf("Hello World!\n");
 * }
 * @endcode
 */

// I2C bus expander PCF8574 interface
#define LCD_BUS_I2C_RS (1 << 0)
#define LCD_BUS_I2C_RW (1 << 1)
#define LCD_BUS_I2C_E  (1 << 2)
#define LCD_BUS_I2C_BL (1 << 3)
#define LCD_BUS_I2C_D4 (1 << 4)
#define LCD_BUS_I2C_D5 (1 << 5)
#define LCD_BUS_I2C_D6 (1 << 6)
#define LCD_BUS_I2C_D7 (1 << 7)
#define LCD_BUS_I2C_MSK (LCD_BUS_I2C_D4 | LCD_BUS_I2C_D5 | LCD_BUS_I2C_D6 | LCD_BUS_I2C_D7)


class TextLCD_I2C : public Stream {
public:
  /** LCD panel format */
  enum LCDType {
    LCD16x2 /**< 16x2 LCD panel (default) */
    ,
    LCD16x2B /**< 16x2 LCD panel alternate addressing */
    ,
    LCD20x2 /**< 20x2 LCD panel */
    ,
    LCD20x4 /**< 20x4 LCD panel */
  };

  /** Create a TextLCD interface
   *
   * @param rs    Instruction/data control line
   * @param e     Enable line (clock)
   * @param d4-d7 Data lines for using as a 4-bit interface
   * @param type  Sets the panel size/addressing mode (default = LCD16x2)
   */
  TextLCD_I2C(I2C *i2c, char adr, LCDType type = LCD16x2);

#if DOXYGEN_ONLY
  /** Write a character to the LCD
   *
   * @param c The character to write to the display
   */
  int putc(int c);

  /** Write a formated string to the LCD
   *
   * @param format A printf-style format string, followed by the
   *               variables to use in formating the string.
   */
  int printf(const char *format, ...);
#endif

  /** Locate to a screen column and row
   *
   * @param column  The horizontal position from the left, indexed from 0
   * @param row     The vertical position from the top, indexed from 0
   */
  void locate(int column, int row);

  /** Clear the screen and locate to 0,0 */
  void cls();

  int rows();
  int columns();
  void setBackLight(bool value);


protected:
  // Stream implementation functions
  virtual int _putc(int value);
  virtual int _getc();

  int address(int column, int row);
  void character(int column, int row, int c);
  void writeByte(int value);
  void writeCommand(int command);
  void writeData(int data);
  

  //    DigitalOut _rs, _e;
  //    BusOut _d;

  // I2C bus
  I2C *_i2c;
  char _slaveAddress;

  // Internal bus shadow value for serial bus only
  char _lcd_bus;

  LCDType _type;
  int _column;
  int _row;

private:
  /** Implementation of pure Virtual Low level writes to LCD Bus (serial native)
   * Set the Enable pin.
   */
  virtual void _setEnable(bool value);

  /** Implementation of pure Virtual Low level writes to LCD Bus (serial native)
   * Set the RS pin ( 0 = Command, 1 = Data).
   */
  virtual void _setRS(bool value);

  /** Implementation of pure Virtual Low level writes to LCD Bus (serial native)
   * Set the databus value (4 bit).
   */
  virtual void _setDataBits(int value);

  
};

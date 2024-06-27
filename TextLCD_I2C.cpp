#include "TextLCD_I2C.h"
#include "Stream.h"
#include "mbed.h"

TextLCD_I2C::TextLCD_I2C(I2C *i2c, char deviceAddress, LCDType type)
    : _i2c(i2c), _type(type) {
  _slaveAddress = deviceAddress & 0xFE;
  _i2c->frequency(100000);

  //   _e  = 1;
  _setEnable(true);
  //   _rs = 0;            // command mode
  _setRS(false);
  wait_us(15000); // Wait 15ms to ensure powered up

  // send "Display Settings" 3 times (Only top nibble of 0x30 as we've got 4-bit
  // bus)
  for (int i = 0; i < 3; i++) {
    writeByte(0x3);
    wait_us(1640); // this command takes 1.64ms, so wait for it
  }
  writeByte(0x2); // 4-bit mode
//  wait_us(40);    // most instructions take 40us

  writeCommand(0x28); // Function set 001 BW N F - -
  writeCommand(0x0C);
  writeCommand(0x6); // Cursor Direction and Display Shift : 0000 01 CD S (CD
                     // 0-left, 1-right S(hift) 0-no, 1-yes
  cls();
}

void TextLCD_I2C::character(int column, int row, int c) {
  int a = address(column, row);
  writeCommand(a);
  writeData(c);
}

void TextLCD_I2C::cls() {
  writeCommand(0x01); // cls, and set cursor to 0
  wait_us(1640);      // This command takes 1.64 ms
  locate(0, 0);
}

void TextLCD_I2C::locate(int column, int row) {
  _column = column;
  _row = row;
}

int TextLCD_I2C::_putc(int value) {
  if (value == '\n') {
    _column = 0;
    _row++;
    if (_row >= rows()) {
      _row = 0;
    }
  } else {
    character(_column, _row, value);
    _column++;
    if (_column >= columns()) {
      _column = 0;
      _row++;
      if (_row >= rows()) {
        _row = 0;
      }
    }
  }
  return value;
}

int TextLCD_I2C::_getc() { return -1; }

void TextLCD_I2C::writeByte(int value) {
  //   _d = value >> 4;
  _setDataBits(value>>4);
  // write the new data to the I2C PCF8574 or PCF8574A portexpander
  _i2c->write(_slaveAddress, &_lcd_bus, 1);    

//  wait_us(40); // most instructions take 40us
  //   _e = 0;
  _setEnable(false);
  _i2c->write(_slaveAddress, &_lcd_bus, 1);   
//  wait_us(40);
  //   _e = 1;
  _setEnable(true);
  _i2c->write(_slaveAddress, &_lcd_bus, 1);   

  //   _d = value >> 0;
  _setDataBits(value>>0);
  _i2c->write(_slaveAddress, &_lcd_bus, 1);   
//  wait_us(40);
  //   _e = 0;
  _setEnable(false);
  _i2c->write(_slaveAddress, &_lcd_bus, 1);   
//  wait_us(40); // most instructions take 40us
  //   _e = 1;
  _setEnable(true);
  _i2c->write(_slaveAddress, &_lcd_bus, 1);   
}

void TextLCD_I2C::writeCommand(int command) {
  //  _rs = 0;
  _setRS(false);
  writeByte(command);
}

void TextLCD_I2C::writeData(int data) {
  //  _rs = 1;
  _setRS(true);
  writeByte(data);
}

int TextLCD_I2C::address(int column, int row) {
  switch (_type) {
  case LCD20x4:
    switch (row) {
    case 0:
      return 0x80 + column;
    case 1:
      return 0xc0 + column;
    case 2:
      return 0x94 + column;
    case 3:
      return 0xd4 + column;
    }
  case LCD16x2B:
    return 0x80 + (row * 40) + column;
  case LCD16x2:
  case LCD20x2:
  default:
    return 0x80 + (row * 0x40) + column;
  }
}

int TextLCD_I2C::columns() {
  switch (_type) {
  case LCD20x4:
  case LCD20x2:
    return 20;
  case LCD16x2:
  case LCD16x2B:
  default:
    return 16;
  }
}

int TextLCD_I2C::rows() {
  switch (_type) {
  case LCD20x4:
    return 4;
  case LCD16x2:
  case LCD16x2B:
  case LCD20x2:
  default:
    return 2;
  }
}

// Set BL pin
// Used for mbed pins, I2C bus expander
void TextLCD_I2C::setBackLight(bool value) {
 
  if (value) {
    _lcd_bus |= LCD_BUS_I2C_BL;    // Set BL bit 
  }  
  else {                    
    _lcd_bus &= ~LCD_BUS_I2C_BL;   // Reset BL bit                     
  }
}

// Set RS pin in the databus shadowvalue
// Used for mbed I2C bus expander
void TextLCD_I2C::_setRS(bool value) {

  if (value) {
    _lcd_bus |= LCD_BUS_I2C_RS; // Set RS bit
  } else {
    _lcd_bus &= ~LCD_BUS_I2C_RS; // Reset RS bit
  }
}

// Set E bit in the databus shadowvalue
// Used for mbed I2C bus expander
void TextLCD_I2C::_setEnable(bool value) {
  // Support only one controller
  if (value) {
    _lcd_bus |= LCD_BUS_I2C_E; // Set E bit
  } else {
    _lcd_bus &= ~LCD_BUS_I2C_E; // Reset E bit
  }
}

// Place the 4bit data in the databus shadowvalue
// Used for mbed I2C bus expander
void TextLCD_I2C::_setDataBits(int value) {
 
  //Clear all databits
  _lcd_bus &= ~LCD_BUS_I2C_MSK;
 
  // Set bit by bit to support any mapping of expander portpins to LCD pins 
  if (value & 0x01){
    _lcd_bus |= LCD_BUS_I2C_D4;   // Set Databit 
  }  
 
  if (value & 0x02){
    _lcd_bus |= LCD_BUS_I2C_D5;   // Set Databit 
  }  
 
  if (value & 0x04) {
    _lcd_bus |= LCD_BUS_I2C_D6;   // Set Databit 
  }  
 
  if (value & 0x08) {
    _lcd_bus |= LCD_BUS_I2C_D7;   // Set Databit 
  }  
}    

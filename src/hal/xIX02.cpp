#include <Wire.h>
#include "xIX02.h"
#include <Arduino.h>

xIX02::xIX02(void)
{
  i2c_addr = 0x2F;
}
xIX02::xIX02(uint8_t addr)
{
  i2c_addr = addr >> 1;
}

bool xIX02::begin(void)
{
  reset();
  return true;
}

bool xIX02::begin(uint8_t pin)
{
  ss_pin = pin;
  reset();
  pinFunction(ss_pin, 0);
  setClockDivider(SC18IS602_CLOCK_1843K);
  setDataMode(SC18IS602_SPI_MODE0);
  setBitOrder(LSBFIRST); // actually MSBFIRST. 
  return true;
}
void xIX02::pinMode(uint8_t pin, uint8_t mode)
{
  pinFunction(pin, 1);  // use pin as GPIO
  uint8_t pos;
  if (mode == OUTPUT) pos = SC18IS601_GPIO_MODE_PUSH_PULL << (pin * 2);         // 01 01 01 10
  else pos = SC18IS601_GPIO_MODE_INPUT_ONLY << (pin * 2);
  gpio_config &= ~(1 << pin * 2);
  gpio_config = gpio_config | pos;

  writeRegister(SC18IS601_GPIO_CONFIG, gpio_config);
}

void xIX02::digitalWrite(uint8_t pin, uint8_t state)
{
  if (state == LOW) gpio_write &= ~(1 << pin);
  else gpio_write |= 1 << pin;
  writeRegister(SC18IS601_GPIO_WRITE, gpio_write);
}

bool xIX02::digitalRead(uint8_t pin)
{
  writeRegister(SC18IS601_GPIO_READ, NULL);
  uint8_t val = readByte() & 0x0F;
  bool status = val & (1 << pin);
  return status;
}

uint8_t xIX02::transfer(uint8_t value)
{
  uint8_t result = 0;
  writeBytes(&value, 1);
  readBytes(&result, 1);
  return result;
}

uint8_t xIX02::transfer(uint8_t* buffer, uint8_t length)
{
  writeBytes(buffer, length);
  return (readBytes(buffer, length));
}
void xIX02::writeRegister(uint8_t cmd, uint8_t data)
{
  do {
    Wire.beginTransmission(i2c_addr);
    Wire.write(cmd);
    Wire.write(data);
  } while ( Wire.endTransmission(1) == 2) ;
  delay(1);
  return ;
}

void xIX02::writeBytes(uint8_t* buffer, uint8_t length)
{
  uint8_t i;
  do {
    Wire.beginTransmission(i2c_addr);
    Wire.write(0x01 << ss_pin);
    for (i = 0; i < length; i++) {
      Wire.write(buffer[i]);
    }
  } while ( Wire.endTransmission(1) == 2);
  delay(1);
  return;
}

uint8_t xIX02::readBytes(uint8_t* buffer, uint8_t length)
{
  uint8_t i;
  uint8_t count;
  while (Wire.requestFrom(i2c_addr, length) == 0);
  count = Wire.available();
  for (i = 0; i < count; i++) {
    buffer[i] = Wire.read();
  }
  return count;
}

uint8_t xIX02::readByte(void)
{
  while (Wire.requestFrom(i2c_addr, (uint8_t)1) == 0);
  uint8_t val = Wire.read();
  return val;
}

void xIX02::reset(void)
{
  for (uint8_t i = 0; i < 4; i++) {
    pinFunction(i, 1);
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void xIX02::pinFunction(uint8_t pin, uint8_t function)
{
  if (function) gpio_enable |= 1 << pin; // set pin for GPIO
  else gpio_enable &= ~(1 << pin);        // set pin as slave select
  SerialUSB.print("gpio_enable: "); SerialUSB.println(gpio_enable, BIN);

  writeRegister(SC18IS601_GPIO_ENABLE, gpio_enable);
}

void xIX02::setBitOrder(uint8_t order)
{
  spi_interface &= 0x0F; // clear order bit
  order = (order << 6) & 0x0F;
  spi_interface |= order;
  writeRegister(SC18IS601_CONFIG_SPI, spi_interface);
}

void xIX02::setDataMode(uint8_t mode)
{
  spi_interface &= 0xF3; // clear mode bits
  mode = (mode << 2) & 0xF3;
  spi_interface |= mode;
  writeRegister(SC18IS601_CONFIG_SPI, spi_interface);
}

void xIX02::setClockDivider(uint8_t divider)
{
  spi_interface &= 0xFC; // clear clock bits
  spi_interface |= divider;
  writeRegister(SC18IS601_CONFIG_SPI, spi_interface);
}

void xIX02::end(void)
{
  return;
}

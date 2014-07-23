#ifndef __RF24_SERIAL_DEBUG_H
#define __RF24_SERIAL_DEBUG_H

class SerialDebug: public RF24Debug
{
protected:
  /**
   * Decode and print the given status to stdout
   *
   * @param status Status value to print
   *
   * @warning Does nothing if stdout is not defined.  See fdevopen in stdio.h
   */
  void print_status(uint8_t status);

  /**
   * Print the name and value of an 8-bit register to stdout
   *
   * Optionally it can print some quantity of successive
   * registers on the same line.  This is useful for printing a group
   * of related registers on one line.
   *
   * @param name Name of the register
   * @param reg Which register. Use constants from nRF24L01.h
   * @param qty How many successive registers to print
   */
  void print_byte_register(const char* name, uint8_t reg, uint8_t qty = 1);

  /**
   * Print the name and value of a 40-bit address register to stdout
   *
   * Optionally it can print some quantity of successive
   * registers on the same line.  This is useful for printing a group
   * of related registers on one line.
   *
   * @param name Name of the register
   * @param reg Which register. Use constants from nRF24L01.h
   * @param qty How many successive registers to print
   */
  void print_address_register(const char* name, uint8_t reg, uint8_t qty = 1);

  RF24 &r;

public:
  /**
   * Print a giant block of debugging information to stdout
   *
   * @warning Does nothing if stdout is not defined.  See fdevopen in stdio.h
   */
  void printDetails(void);

  SerialDebug(RF24 &radio);

  /**
   * Callbacks from RF24
   */
  void on_write_register(uint8_t reg, uint8_t value);
  void observe_tx(uint8_t tx);
  void on_ack(uint8_t ack_len);
  void on_status(uint8_t status);
  void on_write_payload(uint8_t data_len, uint8_t blank_len);
  void on_read_payload(uint8_t data_len, uint8_t blank_len);
};

#endif

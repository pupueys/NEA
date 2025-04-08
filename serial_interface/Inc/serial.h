#ifndef SERIAL_PORT_HEADER
#define SERIAL_PORT_HEADER


#include <stdint.h>

// Defining the serial port struct, the definition is hidden in the
// c file as no one really needs to know this.
struct _SerialPort;
typedef struct _SerialPort SerialPort;


// make any number of instances of the serial port (they are extern because
//   they are fixed, unique values)
extern SerialPort USART1_PORT;


// The user might want to select the baud rate
enum {
  BAUD_9600,
  BAUD_19200,
  BAUD_38400,
  BAUD_57600,
  BAUD_115200
};


// SerialInitialise - initialise the serial port
// Input: baud rate as defined in the enum
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*transmit_complete_function)(uint32_t),  void (*receive_complete_function)(uint8_t, uint32_t));
void SerialOutputChar(uint8_t data, SerialPort *serial_port);

void SerialReceiveChar(uint8_t *pt, SerialPort *serial_port);

void SerialReceiveString(uint8_t *buffer, SerialPort *serial_port, uint8_t buffer_size, uint8_t terminator);

#endif

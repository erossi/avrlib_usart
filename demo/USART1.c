// AVR306: Using the AVR UART in C
// Routines for polled USART

/* Includes */
#include <inttypes.h>
#include <avr/io.h>

/* Initialize UART */
void
USART_Init (uint16_t baudrate)
{
  /* Set the baud rate */
  UBRRH = (uint8_t) (baudrate >> 8);
  UBRRL = (uint8_t) baudrate;

  /* Enable UART receiver and transmitter */
  UCSRB = _BV(RXEN) | _BV(TXEN);

  // 8N2
  UCSRC = _BV(URSEL) | _BV(USBS) | _BV(UCSZ0) | _BV(UCSZ1);
}


/* Read and write functions */
uint8_t
USART_Receive (void)
{
  /* Wait for incomming data */
  while ( !(UCSRA & _BV(RXC)) )
    ;
  /* Return the data */
  return UDR;
}

void
USART_Transmit (uint8_t data)
{
  /* Wait for empty transmit buffer */
  while ( !(UCSRA & _BV(UDRE)) )
    ;
  /* Start transmittion */
  UDR = data;
}

/* Main - a simple test program*/
int
main (void)
{
  /* Set the baudrate to 9,600 bps using a 4MHz crystal */
  USART_Init (25);

  for (;;)    	/* Forever */
    {
      USART_Transmit (USART_Receive ());	/* Echo the received character */
    }
}

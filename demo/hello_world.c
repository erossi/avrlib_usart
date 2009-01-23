// AVR306: Using the AVR UART in C
// Routines for polled USART

/* Includes */
#include <inttypes.h>
#include <stdio.h>
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

int
uart_putchar(char c)
{
  if (c == '\n')
    uart_putchar('\r');
  loop_until_bit_is_set(UCSRA, UDRE);
  UDR = c;
  return 0;
}

/* Main - a simple test program*/
int
main (void)
{
  /* Set the baudrate to 9,600 bps using a 4MHz crystal */
  USART_Init (25);

  fdevopen (uart_putchar, NULL, 0);
  
  for (;;)    	/* Forever */
    {
      printf ("Hello world! \n");
    }
}

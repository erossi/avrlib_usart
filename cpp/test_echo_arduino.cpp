#include <stdint.h>
#include "usart_base.h"

Usart0_Base usart0;

int main() {
	uint8_t c;

	usart0.resume();

	while(true)
		if (usart0.get(&c, true))
			usart0.put(c);

	return 0;
}

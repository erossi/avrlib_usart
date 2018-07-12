#include <stdint.h>
#include "usart_base.h"

Usart0_Base usart0;
Usart1_Base usart1;

int main() {
	uint8_t c;
	uint8_t hello[] {'h', 'e', 'l', 'l', 'o'};

	usart0.resume();
	usart1.resume();

	for (int i=0; i<5; i++) {
		usart0.put(hello[i]);
		usart1.put(hello[i]);
	}

	while(true) {
		if (usart0.get(&c, true))
			usart0.put(c);

		if (usart1.get(&c, true))
			usart1.put(c);
	}

	return 0;
}

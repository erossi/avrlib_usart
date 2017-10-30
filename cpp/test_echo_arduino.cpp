#include <stdint.h>
#include "usart_base.h"

Usart0_Base usart0;

int main() {
	uint8_t c;

	Usart0_Base::resume();

	while(true)
		if (Usart0_Base::get(&c, true))
			Usart0_Base::put(c);

	return 0;
}

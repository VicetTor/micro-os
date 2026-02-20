#include <stdint.h>
#include <stddef.h>

void kernel_main(void){
	uint16_t* terminal_buffer = (uint16_t*) 0xB8000;
	const char* str = "Bonjour Micro-Noyau !";
	size_t index = 0;

	for(size_t i = 0; str[i] != '\0'; i++){
		uint16_t c = (uint16_t) str[i];
		uint16_t color = 0x0F;
		terminal_buffer[index] = c | (color << 8);
		index++;
	}
}

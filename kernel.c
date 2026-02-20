#include <stdint.h>
#include <stddef.h>
#include "idt.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = 0x0F;
    terminal_buffer = (uint16_t*) 0xB8000;

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = (uint16_t) ' ' | (uint16_t) terminal_color << 8;
        }
    }
}
void terminal_scroll(void){
    for(size_t y = 1; y < VGA_HEIGHT; y++){
        for(size_t x = 0; x < VGA_WIDTH; x++){
            size_t source_index = (y * VGA_WIDTH) + x;
            size_t destination_index = ((y - 1) * VGA_WIDTH) + x;
            terminal_buffer[destination_index] = terminal_buffer[source_index];
        }
    }
    for(size_t x = 0; x < VGA_WIDTH; x++){
	size_t last_index = ((VGA_HEIGHT - 1) * VGA_WIDTH) + x;
	terminal_buffer[last_index] = (uint16_t) ' ' | (uint16_t) 0x0F << 8;
    }
}

void terminal_putchar(char c) {
    if (c == '\n') {
	terminal_row++;
	terminal_column = 0;
	if(terminal_row == VGA_HEIGHT){
	    terminal_scroll();
            terminal_row = 24;
	}
    } else {
        const size_t index = terminal_row * VGA_WIDTH + terminal_column;
        terminal_buffer[index] = (uint16_t) c | (uint16_t) terminal_color << 8;
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {
                terminal_scroll();
		terminal_row = 24;
            }
        }
    }
}

void terminal_write(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++)
        terminal_putchar(data[i]);
}

void delay(void){
    for(size_t i = 0; i < 50000000; i++){
    	volatile uint32_t j = 0;
    }
}

void kernel_main(void) {
    terminal_initialize();
    idt_install();
    __asm__ volatile("sti");
    for(size_t i = 0; i < 35; i++){
	terminal_write("Test de defilement ligne par ligne\n");
        delay();
    }
}

#ifndef OLED_H
#define OLED_H
#include "define.h"

uint8_t write_c(unsigned char out_com);
uint8_t write_d(unsigned char out_com);

void oled_init();
void init_program();
void oled_set_page();
void oled_set_column();
void clear_oled();
void OLED_print_arrow();

#endif
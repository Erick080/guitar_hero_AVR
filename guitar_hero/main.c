/* Nokia 5110 LCD AVR Library example
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 * Custom char code by Marcelo Cohen - 2021
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "nokia5110.h"
#define PD0 red_led
#define PD1 yellow_led
#define PD2 green_led

uint8_t glyph[] = {0b00010000, 0b00100100, 0b11100000, 0b00100100, 0b00010000};

int main(void)
{
    DDRB &= ~((1 << PB1) | (1 << PB2) | (1 << PB3)); //seta pinos de entrada PB1,PB2 e PB3
    DDRD |= (1<<red_led) | (1<<yellow_led) | (1<<green_led); //seta pinos de saida (leds)


    nokia_lcd_init();
    nokia_lcd_clear();
    nokia_lcd_custom(1, glyph);
    //nokia_lcd_write_string("IT'S WORKING!",1);
    //nokia_lcd_set_cursor(0, 12);
    //nokia_lcd_write_string("Nice!\001", 2);
    nokia_lcd_drawcircle(5,5,5);
    nokia_lcd_render();
     while(1);
   
   /* while (1)
    {
        nokia_lcd_clear();
        for (int i = 0; i < 20; i++)
        {
            uint8_t x1, y1, x2, y2;
            x1 = rand() % 84;
            y1 = rand() % 48;
            x2 = rand() % 84;
            y2 = rand() % 48;
            // uint8_t r = rand() % 10;
            nokia_lcd_drawline(x1, y1, x2, y2);
            // nokia_lcd_drawrect(x1, y1, x2, y2);
            // nokia_lcd_drawcircle(x1, y1, r);
        }
        nokia_lcd_render();
        // _delay_ms(100);
    }*/
}

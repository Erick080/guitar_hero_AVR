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
#include <avr/interrupt.h>
#include "nokia5110.h"
#define TIMER_CLK		F_CPU / 1024
#define FREQ 10 // frequencias para setar timer q estora 10x por segundo
int interrupt;
int timer;

uint8_t glyph[] = {0b00010000, 0b00100100, 0b11100000, 0b00100100, 0b00010000};

ISR(TIMER1_COMPA_vect){ //a cada segundo incrementa timer
    interrupt++;if(interrupt >= 10){interrupt = 0;timer++;}
}

void interface(){
 //implementar os botoes e score
}

void game_over(){
    nokia_lcd_clear();
    nokia_lcd_set_cursor(0,12);
    nokia_lcd_write_string("Game Over",2);
    nokia_lcd_render();
}
int main(void)
{
    cli(); // desabilita interrupções
    DDRB &= ~((1 << PB1) | (1 << PB2) | (1 << PB3)); //seta pinos de entrada PB1,PB2 e PB3
    PORTB |= (1 << PB0) | (1 << PB1)| (1 << PB2); //habilita pull-up
    DDRD |= (1<<PD0) | (1<<PD1) | (1<<PD2); //seta pinos de saida (leds)

    //-----configuracoes para habilitar interrupcoes-------
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
	OCR1A = (TIMER_CLK / FREQ) - 1; // seta o registrador output compare
	TCCR1B |= (1 << WGM12); // liga modo CTC
	TCCR1B |= (1 << CS12) | (1 << CS10);  // seta CS10 e CS12 para prescaler 1024
	TIMSK1 |= (1 << OCIE1A); // habilita máscara do timer1
    
    //caso seja necessario adicionar um botao p/ interrupcao
    /*EICRA = (1 << ISC01) | (1 << ISC00);  // interrupt sense control, borda de subida (rising edge) para INT0
    EICRA |= (1 << ISC11) | (0 << ISC10); // interrupt sense control, borda de descida (falling edge) para INT1
    EIMSK |= (1 << INT1) | (1 << INT0); */  // enable INT1 and INT0
    //-------------------------------------

    nokia_lcd_init();
    nokia_lcd_clear();
    nokia_lcd_custom(1, glyph);
    //nokia_lcd_write_string("IT'S WORKING!",1);
    //nokia_lcd_set_cursor(0, 12);
    //nokia_lcd_write_string("Nice!\001", 2);
    
    
    //nokia_lcd_drawcircle(66,5,5);
    //nokia_lcd_render();
    
    //p/ notas descerem na tela
    for(int x = 22;x <= 66;x+=22){
        for(int y = 5;y < 30;y++){
            nokia_lcd_clear();
            nokia_lcd_drawcircle(x,y,5);
            nokia_lcd_render();
            _delay_ms(100);
        }
    }
    game_over();
    PORTD  = 0b111;
     while(1){
        //sei();
        if(!(PINB & (1 << PB1))){
            nokia_lcd_clear();
            nokia_lcd_write_string("Botao apertado",1);
            nokia_lcd_render();
        } while(!(PINB & (1 << PB1))) _delay_ms(1);
        /*if(timer >=10){
            nokia_lcd_clear();
            nokia_lcd_write_string("Acabou o tempo",1);
            nokia_lcd_render();
        }*/
        
     };
   



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

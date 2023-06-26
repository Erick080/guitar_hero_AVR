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
//char count;
float timer; 
char segundos [4];//timer em string
float hit;
float miss;
float ratio;
char win_condition;



ISR(TIMER1_COMPA_vect){ //a cada segundo incrementa timer
    interrupt++;
    //count+=1;
    if(interrupt >= 10){interrupt = 0;timer++;}
}

void menu(){
    nokia_lcd_write_string("---GUITAR HERO",1);
    nokia_lcd_set_cursor(0,20);
    nokia_lcd_write_string("Aperte A p/ ",1);
    nokia_lcd_set_cursor(0,40);
    nokia_lcd_write_string("iniciar",1);
    nokia_lcd_render();
}

void interface(){
    //linhas verticais
 nokia_lcd_drawline(55,5,55,42);
 nokia_lcd_drawline(30,5,30,42);
    //botoes de hitbox
 nokia_lcd_drawcircle(22,41,3);
 nokia_lcd_drawcircle(42,41,3);
 nokia_lcd_drawcircle(64,41,3);
 nokia_lcd_render();
}

void game_over(){
    nokia_lcd_clear();
    nokia_lcd_set_cursor(0,12);
    nokia_lcd_write_string("Game Over",1);
    nokia_lcd_render();

}

void game_win(){
    nokia_lcd_clear();
    nokia_lcd_set_cursor(0,12);
    nokia_lcd_write_string("You Win!",1);
    nokia_lcd_render();
}

int verifica_acerto(int coluna){
    int s = timer +0.6;
    coluna = 4 - coluna;
    while(s >= timer){
        dtostrf(timer,4,2,segundos);
        if(!(PINB & (1 << coluna))){
            hit++;
            while(!(PINB & (1 << coluna))){_delay_ms(1);}
            return 1;
        }
        
    }
    miss++;
    ratio = hit/(hit + miss);
    if(ratio <= 0.25f && timer > 5) return 0; else return 1; //detecta se porcentagem for menor q 25%
}

void led_config(float r){
    if(r >= 0.70){
        PORTD = 0b100;
    }else if(r >= 0.50 && r < 0.70){
        PORTD = 0b010;
    }else if (r > 0.25 && r < 0.50){
        PORTD = 0b001;
    } else if(r <= 0.25){
        PORTD = 0b111;
    }
}

int main(void)
{
    cli(); // desabilita interrupções
    DDRB &= ~((1 << PB1) | (1 << PB2) | (1 << PB3)); //seta pinos de entrada PB1,PB2 e PB3
    PORTB |= (1 << PB1) | (1 << PB2)| (1 << PB3); //habilita pull-up
    DDRD |= (1<<PD0) | (1<<PD1) | (1<<PD2); //seta pinos de saida (leds)

    //-----configuracoes para habilitar interrupcoes-------
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
	OCR1A = (TIMER_CLK / FREQ) - 1; // seta o registrador output compare
	TCCR1B |= (1 << WGM12); // liga modo CTC
	TCCR1B |= (1 << CS12) | (1 << CS10);  // seta CS10 e CS12 para prescaler 1024
	TIMSK1 |= (1 << OCIE1A); // habilita máscara do timer1
    

    nokia_lcd_init();

    //menu inicial
    menu();
    while((PINB & (1 << PB3))){
        _delay_ms(5);
    }


    sei();
    interrupt = 0;
    timer = 0;
    miss = 0;
    hit = 0;
    win_condition = 1;
    
    while(timer < 60){
            int random = (rand() % 3) + 1; //gera numero de 1 a 3
            int x = 22 * random;
            for(int y = 5;y < 40;y+=5){
                nokia_lcd_clear();
                interface();
                nokia_lcd_drawcircle(x,y,5);
                dtostrf(60 - timer,4,0,segundos);
                nokia_lcd_write_string(segundos,1);
                nokia_lcd_render();
                if(timer > 60){break;}
               _delay_ms(500);

            }
            //verifica se acertou a nota
            if(verifica_acerto(random) == 0){win_condition = 0;break;}; //se porcentagem de acertos < 25% acaba o jogo    
            ratio = hit / (hit + miss);
            led_config(ratio); //acende led de acordo com quantidade de acertos 
        }

    //resultado do fim de jogo
    if(win_condition == 0){game_over();}
        else{game_win();}
}

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

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "nokia5110.h"

uint8_t cont1 = 0, cont2 = 0, cronometro = 0, creditos = 0, piscapisca = 0, contpisca = 0;
uint8_t gravidade = 0, cont = 0, taSubindo = 0, pula3vezes = 0, gameover = 0;

uint8_t cano = 0, posicaoCano = 80;

ISR(TIMER2_OVF_vect) {
    
    //passarinho pra cima e pra baixo
    if (taSubindo == 0) {
        if (cont == 10) {
            gravidade++;
            if (gravidade == 40) {
                gameover = 1;
                return;
            }
            cont = 0;
        }
        cont++;
    } else {
        if (cont == 10 && pula3vezes < 4) {
            gravidade--;
            if (gravidade == 0) {
                gameover = 1;
                return;
            }

            pula3vezes++;
            cont = 0;
        } else if (cont == 10 && pula3vezes == 4) {
            taSubindo = 0;
            pula3vezes = 0;
            cont = 0;
        }
        cont++;
    }


    //canos na horizontal
    if (cano == 10) {
        posicaoCano--;
        if (posicaoCano == 0)
            posicaoCano = 80;
        cano = 0;
    }
    cano++;

}

int main(void) {
    cli();
    TCCR2A = 0x00;
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
    TIMSK2 = (1 << TOIE2);

    DDRD &= ~(1 << PD7);//botao de pular
    sei();

    PORTC |= (1 << PC3);

    nokia_lcd_init();
   
    char msg[30];
    char cred[30];
    char dcano1[30];
    while (1) {
        if (PIND & (1 << PD7)) {
            taSubindo = 1;
            cont = 0;
            gravidade--;

            while (PIND & (1 << PD7))
                _delay_ms(1); // debounce
        }

        nokia_lcd_clear();
            
        sprintf(cred, ".");
        nokia_lcd_set_cursor(10, gravidade);
        nokia_lcd_write_string(cred, 1);
        nokia_lcd_set_cursor(0, 12);
        sprintf(msg, "%d", gravidade);
        nokia_lcd_write_string(msg, 1);


        sprintf(dcano1, "|");
        nokia_lcd_set_cursor(posicaoCano,0);
        nokia_lcd_write_string(dcano1, 1);
        nokia_lcd_set_cursor(posicaoCano,7);
        nokia_lcd_write_string(dcano1, 1);

        nokia_lcd_set_cursor(posicaoCano,27);
        nokia_lcd_write_string(dcano1, 1);
        nokia_lcd_set_cursor(posicaoCano,34);
        nokia_lcd_write_string(dcano1, 1);
        nokia_lcd_set_cursor(posicaoCano,40);
        nokia_lcd_write_string(dcano1, 1);
        nokia_lcd_render();

        if (gameover == 1) {
            nokia_lcd_clear();
            nokia_lcd_set_cursor(0, 0);
            nokia_lcd_write_string("GAME OVER", 1);
            nokia_lcd_render();
            break;
        }
    }
}

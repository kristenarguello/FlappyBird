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
    // if (cronometro) { 
    //     cont2++;
    //     if (cont2 == 61) {
    //         cont2 = 0;
    //         cont1--;
    //         if (cont1 == 0) {
    //             cronometro = 0;
    //             PORTC ^= (1 << PC4);
    //             piscapisca = 1;
    //             creditos = 0;
    //         }
    //     }  
    // }
    // if (piscapisca) {
    //     if (contpisca == 15) {
    //         PORTC ^= (1 << PC5);
    //         contpisca = 0;
    //     }
    //     contpisca++;
    // }

    //passarinho pra cima e pra baixo
    if (taSubindo == 0) {
        if (cont == 10) {
            gravidade++;
            if (gravidade == 40) {
                gameover = 1;
                PORTC |= (1 << PC5);
                return;
            }
            cont = 0;
        }
        cont++;
    } else {
        if (cont == 10 && pula3vezes < 5) {
            gravidade--;
            if (gravidade == 0) {
                gameover = 1;
                PORTC |= (1 << PC5);
                return;
            }

            pula3vezes++;
            cont = 0;
        } else if (cont == 10 && pula3vezes == 5) {
            taSubindo = 0;
            pula3vezes = 0;
            cont = 0;
        }
        cont++;
    }


    //canos na horizontal
    if (cano == 10) {
        posicaoCano--;
        cano = 0;
    }
    cano++;

}

int main(void) {
    cli();
    TCCR2A = 0x00;
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
    TIMSK2 = (1 << TOIE2);

    DDRC |= (1 << PC5) | (1 << PC4) | (1 << PC3);

    DDRC &= ~(1 << PC6);
    DDRD &= ~(1 << PD4) | ~(1 << PD7);
    DDRB &= ~(1 << PB7);
    sei();

    PORTC |= (1 << PC3);

    nokia_lcd_init();
   
    char msg[30];
    char cred[30];
    while (1) {
        if (PIND & (1 << PD7)) {

            // PORTC &= ~(1 << PC5);
            // piscapisca = 0; 
            // if (creditos < 150) {
            //     if (creditos + 25 > 150)
            //         creditos = 150;
            //     else 
            //         creditos += 25;
            // }
            
            // gravidade--;
            taSubindo = 1;
            cont = 0;

            while (PIND & (1 << PD7))
                _delay_ms(1); // debounce

        }


        
        // if (PINB & (1 << PB7)) {
        //     PORTC &= ~(1 << PC5);
        //     piscapisca = 0; 
        //     if (creditos < 150) {
        //         if (creditos + 50 > 150) 
        //             creditos = 150;
        //         else 
        //             creditos += 50;
        //     }
        //     while (PINB & (1 << PB7))
        //         _delay_ms(1); // debounce
        // }

        // if (PIND & (1 << PD4)) {
        //     piscapisca = 0; 
        //     PORTC &= ~(1 << PC5);
        //     if (creditos < 150) {
        //         if (creditos + 100 > 150) 
        //             creditos = 150;
        //         else 
        //             creditos += 100;
        //     }
        //     while (PIND & (1 << PD4))
        //         _delay_ms(1); // debounce
        // }

        // if (PINC & (1 << PC6)) {
        //     if (creditos == 150) {
        //         cronometro = 1;
        //         cont1 = 10;
        //         PORTC |= (1 << PC4);
        //         PORTC &= ~(1 << PC3);
        //     } 
        //     while (PINC & (1 << PC6))
        //         _delay_ms(1); // debounce
        // }

        // if (creditos > 0) 
        //     PORTC |= (1 << PC3);
        // if (creditos == 150)
        //     PORTC &= ~(1 << PC3);

        nokia_lcd_clear();
        // uint8_t inteiro = creditos/100, decimal = creditos%100;

        // if (piscapisca) {
        //     nokia_lcd_set_cursor(5, 40);
        //     nokia_lcd_write_string("Sem creditos!", 1);
        // }
            
        // sprintf(msg, "Tempo: %ds", cont1);
        // sprintf(cred, "Creditos:$%d,%d", inteiro, decimal);
        sprintf(cred, ".");
        nokia_lcd_set_cursor(0, gravidade);
        // nokia_lcd_write_string(msg, 1);
        nokia_lcd_write_string(cred, 1);
        nokia_lcd_set_cursor(0, 12);
        sprintf(msg, "%d", gravidade);
        nokia_lcd_write_string(msg, 1);
        nokia_lcd_set_cursor(posicaoCano,0);
        nokia_lcd_write_string("!", 1);


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

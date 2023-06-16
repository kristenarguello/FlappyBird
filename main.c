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
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "nokia5110.h"

uint8_t linha[] = {
    0b01111100,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

uint8_t cont1 = 0, cont2 = 0, cronometro = 0, creditos = 0, piscapisca = 0, contpisca = 0;
uint8_t gravidade = 0, cont = 0, taSubindo = 0, pula3vezes = 0, gameover = 0, cano1Aleatorio = 1, cano2Aleatorio = 1, ponto = 0;

uint8_t cano = 0, posicaoCano = 40, posicaoCano2 = 80, cano1naMetade = 0, jaPasso = 0, velocidade = 10, dificuldade = 0;//limpar essas variaevis aqui em cima

ISR(TIMER2_OVF_vect)
{
    // passarinho pra cima e pra baixo
    if (taSubindo == 0)
    {
        if (cont == 10)//comparacao de tempo pode ser variada para fazer a dificuldade
        {
            gravidade++;
            if (gravidade == 40)
            {
                gameover = 1;
                return;
            }
            cont = 0;
        }
        cont++;
    }
    else
    {
        if (cont == 10 && pula3vezes < 4)
        {
            gravidade--;
            if (gravidade == 0)
            {
                gameover = 1;
                return;
            }

            pula3vezes++;
            cont = 0;
        }
        else if (cont == 10 && pula3vezes == 4)
        {
            taSubindo = 0;
            pula3vezes = 0;
            cont = 0;
        }
        cont++;
    }

    // canos na horizontal
    if (cano == 10)
    {
        if (posicaoCano == 40)
        {
            cano1naMetade = 1;
        }

        posicaoCano--;
        if (posicaoCano == 0)
        {
            posicaoCano = 80;
            // cano1Aleatorio = rand() % (5 - 0 + 1) + 0;
        }

        if (cano1naMetade == 1)
        {
            posicaoCano2--;
            if (posicaoCano2 == 0)
            {
                posicaoCano2 = 80;
                // cano2Aleatorio = rand() % (5 - 0 + 1) + 0;
            }
        }

        cano = 0;
    }
    cano++;

    if (dificuldade == 100) {
        if (velocidade > 5)
            velocidade--;
        
        dificuldade = 0; //trocar os 10 por velocidade
    }
    dificuldade++;
}

int desenhaCano(uint8_t qualCano, uint8_t posicao)
{
    nokia_lcd_set_cursor(posicao, 0);
    nokia_lcd_write_string("|", 1);

    if (qualCano == 1)
    {
        // opcao 1
        nokia_lcd_set_cursor(posicao, 18);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 22);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 29);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 34);
        nokia_lcd_write_string("|", 1);
    }
    else if (qualCano == 2)
    {
        nokia_lcd_set_cursor(posicao, 7);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 14);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 17);
        nokia_lcd_write_string("|", 1);
    }
    else if (qualCano == 3)
    {
        nokia_lcd_set_cursor(posicao, 7);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 8);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 26);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 33);
        nokia_lcd_write_string("|", 1);
    }
    else if (qualCano == 4)
    {
        nokia_lcd_set_cursor(posicao, 4);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 22);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 29);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 33);
        nokia_lcd_write_string("|", 1);
    }
    else
    {
        nokia_lcd_set_cursor(posicao, 7);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 13);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 20);
        nokia_lcd_write_string("|", 1);
    }
    nokia_lcd_set_cursor(posicao, 40);
    nokia_lcd_write_string("|", 1);

    return 0;
}

int aumentaPonto(uint8_t posicao, uint8_t cano) {
    if (posicao == 10 && jaPasso == 0)
        {
            switch (cano) {
            case 1:
                if (gravidade < 5 || gravidade > 10) {
                    gameover = 1;
                } else {
                    ponto++;
                    jaPasso = 1;
                    PORTC ^= (1 << PC3);
                }
                break;

            case 2:

            case 3:
            case 4:
            case 5:
            default:
                break;
            }//falta o restante dos canos
        }

        if (posicao == 11)
            jaPasso = 0;

    return 0;

}

int main(void)
{
    cli();
    TCCR2A = 0x00;
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
    TIMSK2 = (1 << TOIE2);

    DDRD &= ~(1 << PD7); // botao de pular
    sei();

    DDRC |= (1 << PC3);

    nokia_lcd_init();

    // cano1Aleatorio = rand() % (5 - 0 + 1) + 0;
    // cano2Aleatorio = rand() % (5 - 0 + 1) + 0;
    
    
    



    char msg[30];
    // char cred[30];

    while (gameover == 0)
    {
        if (PIND & (1 << PD7))
        {
            taSubindo = 1;
            cont = 0;
            gravidade--;

            while (PIND & (1 << PD7))
                _delay_ms(1); // debounce
        }

        aumentaPonto(posicaoCano, cano1Aleatorio);
        aumentaPonto(posicaoCano2, cano2Aleatorio);
        
        nokia_lcd_clear();

        for (int i=0; i<80; i++) {
            nokia_lcd_set_cursor(i, 0);
            // nokia_lcd_write_string(".", 1);
            nokia_lcd_custom(1, linha);
            nokia_lcd_set_cursor(i, 40);
            nokia_lcd_custom(1, linha);//como escrever na tela customizados???
        }

        // sprintf(cred, ".");
        nokia_lcd_set_cursor(10, gravidade);
        nokia_lcd_write_string(".", 1);
        nokia_lcd_set_cursor(0, 12);
        sprintf(msg, "%d", velocidade);
        nokia_lcd_write_string(msg, 1);

        desenhaCano(cano1Aleatorio, posicaoCano);
        desenhaCano(cano2Aleatorio, posicaoCano2);//adicionar mais canos?

        nokia_lcd_render();
    }

    nokia_lcd_clear();
    nokia_lcd_set_cursor(0, 0);
    nokia_lcd_write_string("GAME OVER", 1);
    nokia_lcd_set_cursor(0, 12);
    sprintf(msg, "%d", ponto);
    nokia_lcd_write_string(msg, 1);
    nokia_lcd_render();
}

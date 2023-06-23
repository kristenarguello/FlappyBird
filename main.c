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

uint8_t gravidade = 15,
        tempo = 0,
        taSubindo = 0,
        pula5vezes = 0,
        gameover = 0,
        pontos = 0,

        cano1Aleatorio = 6,
        cano2Aleatorio = 6,
        posicaoCano1 = 40,
        posicaoCano2 = 80,

        movimentoCano = 0,
        jaPasso = 0,
        jaAumentou = 0,
        velocidade = 9,
        start = 0;

uint8_t glyph[] = {
    0b00100000,
    0b00010010,
    0b00010000,
    0b00010010,
    0b00100000};

uint8_t glyph2[] = {
    0b00000000,
    0b00111100,
    0b00111100,
    0b00111100,
    0b00111100};

ISR(TIMER2_OVF_vect)
{
    if (start == 0)
        return;

    // passarinho pra cima e pra baixo
    if (taSubindo == 0)
    {
        if (tempo == velocidade)
        {
            gravidade++;
            if (gravidade == 40)
            {
                gameover = 1;
                return;
            }
            tempo = 0;
        }
        tempo++;
    }
    else
    {
        if (tempo == velocidade && pula5vezes < 4)
        {
            gravidade--;
            if (gravidade <= 0)
            {
                gameover = 1;
                return;
            }

            pula5vezes++;
            tempo = 0;
        }
        else if (tempo == velocidade && pula5vezes == 4)
        {
            if (gravidade <= 0)
            {
                gameover = 1;
                return;
            }
            taSubindo = 0;
            pula5vezes = 0;
            tempo = 0;
        }
        tempo++;
    }

    // canos na horizontal
    if (movimentoCano == velocidade)
    {
        posicaoCano1--;
        if (posicaoCano1 == 0)
        {
            posicaoCano1 = 80;
            cano1Aleatorio = rand() % (7 - 1 + 1) + 1;
        }

        posicaoCano2--;
        if (posicaoCano2 == 0)
        {
            posicaoCano2 = 80;
            cano2Aleatorio = rand() % (7 - 1 + 1) + 1;
        }
        movimentoCano = 0;
    }
    movimentoCano++;

    if (pontos % 2 == 0 && pontos != 0 && jaAumentou == 0)
    {
        if (velocidade > 5)
        {
            velocidade--;
            tempo = 0;
            movimentoCano = 0;
        }
        jaAumentou = 1;
    }
    if (pontos % 3 == 0 && pontos != 0)
        jaAumentou = 0;
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
        nokia_lcd_set_cursor(posicao, 33);
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
    else if (qualCano == 5)
    {
        nokia_lcd_set_cursor(posicao, 7);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 26);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 32);
        nokia_lcd_write_string("|", 1);
        nokia_lcd_set_cursor(posicao, 39);
        nokia_lcd_write_string("|", 1);
    }
    else if (qualCano == 6)
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
        nokia_lcd_set_cursor(posicao, 37);
        nokia_lcd_write_string("|", 1);
    }
    nokia_lcd_set_cursor(posicao, 40);
    nokia_lcd_write_string("|", 1);

    return 0;
}

int aumentaPonto(uint8_t posicao, uint8_t cano)
{
    uint8_t max, min;
    if (posicao == 10 && jaPasso == 0)
    {
        switch (cano)
        {
        case 1:
            max = 10;
            min = 5;
            break;
        case 2:
            max = 28;
            min = 20;
            break;
        case 3:
            max = 16;
            min = 11;
            break;
        case 4:
            max = 13;
            min = 7;
            break;
        case 5:
            max = 16;
            min = 9;
            break;
        case 6:
            max = 13;
            min = 6;
            break;
        case 7:
            max = 29;
            min = 24;
            break;
        default:
            max = 80;
            min = 0;
            break;
        }

        if (gravidade < min || gravidade > max)
        {
            return 1;
        }
        else
        {
            pontos++;
            jaPasso = 1;
        }
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

    DDRD &= ~(1 << PD7) | ~(1 << PD0); // botao de pular e start
    sei();

    nokia_lcd_init();

    cano1Aleatorio = rand() % (7 - 0 + 1) + 0;
    cano2Aleatorio = rand() % (7 - 0 + 1) + 0;

    while (1)
    {
        nokia_lcd_clear();
        nokia_lcd_set_cursor(10, 15);
        nokia_lcd_custom(3, glyph2);
        nokia_lcd_write_string("FlappyDot \003", 1);
        nokia_lcd_set_cursor(12, 25);
        nokia_lcd_write_string("S to start", 1);
        nokia_lcd_render();

        if (PIND & (1 << PD0))
        {
            while (PIND & (1 << PD0))
                _delay_ms(1); // debounce

            start = 1;
            break;
        }
    }

    char msg[30];
    char limite[80];
    while (start == 1)
    {
        while (gameover == 0)
        {
            if (PIND & (1 << PD7))
            {
                taSubindo = 1;
                tempo = 0;
                gravidade--;

                if (gravidade <= 0)
                {
                    gameover = 1;
                    break;
                }

                // while (PIND & (1 << PD7)) {
                //     if (!(posicaoCano1 == 10 || posicaoCano2 == 10))
                //         _delay_ms(1); // debounce
                // }
            }

            nokia_lcd_clear();
            for (int i = 0; i < 14; i++)
            {
                limite[i] = '-';
            }
            nokia_lcd_set_cursor(0, 0);
            nokia_lcd_write_string(limite, 1);
            nokia_lcd_set_cursor(0, 40);
            nokia_lcd_write_string(limite, 1);

            nokia_lcd_set_cursor(10, gravidade);
            nokia_lcd_write_string(".", 1);
            nokia_lcd_set_cursor(70, 6);
            sprintf(msg, "%d", pontos);
            nokia_lcd_write_string(msg, 1);
            nokia_lcd_set_cursor(0, 0);
            sprintf(msg, "%d", gravidade);
            nokia_lcd_write_string(msg, 1);

            gameover = aumentaPonto(posicaoCano1, cano1Aleatorio);
            if (gameover)
                break;
            gameover = aumentaPonto(posicaoCano2, cano2Aleatorio);
            if (gameover)
                break;

            desenhaCano(cano1Aleatorio, posicaoCano1);
            desenhaCano(cano2Aleatorio, posicaoCano2);
            nokia_lcd_render();
        }

        while (gameover == 1)
        {
            start = 0;
            nokia_lcd_clear();
            nokia_lcd_set_cursor(5, 1);
            nokia_lcd_custom(2, glyph);
            nokia_lcd_write_string("GAME OVER \002", 1);
            nokia_lcd_set_cursor(15, 13);
            sprintf(msg, "Score: %d", pontos);
            nokia_lcd_write_string(msg, 1);
            nokia_lcd_set_cursor(6, 35);
            nokia_lcd_write_string("S to restart", 1);
            nokia_lcd_render();

            if (PIND & (1 << PD0))
            {
                gravidade = 15;
                tempo = 0;
                taSubindo = 0;
                pula5vezes = 0;
                gameover = 0;
                pontos = 0;
                movimentoCano = 0;
                jaPasso = 0;
                velocidade = 10;
                start = 1;

                cano1Aleatorio = rand() % (7 - 1 + 1) + 1;
                cano2Aleatorio = rand() % (7 - 1 + 1) + 1;
                posicaoCano1 = 40;
                posicaoCano2 = 80;

                while (PIND & (1 << PD0))
                    _delay_ms(1); // debounce
            }
        }
    }
}

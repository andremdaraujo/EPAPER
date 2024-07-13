/*
 * app.c
 *
 *  Created on: Feb 23, 2024
 *      Author: Andre
 */

#include "app.h"

void APP_Init(void)
{

    uint8_t image_bw[(EPD_W_BUFF_SIZE) * EPD_H];

    GPIO_InitTypeDef    GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // /Shutdown - C0
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;

    GPIO_Init(GPIOC, &GPIO_InitStructure); // Shuts down the board right after reset

    Delay_Ms(200);                          // User must hold ON button for this time...

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_0);        // ...The the MCU keeps the power circuit on

    // User Button - C3
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // LED Pin - C4
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_0);

    GPIO_SetBits(GPIOC, GPIO_Pin_4);        // LED D1 blinks to indicate the board is properly turned on
    Delay_Ms(500);
    GPIO_ResetBits(GPIOC, GPIO_Pin_4);

    printf("EPD IO Initialization... \n");
    epd_io_init();

    //epd_cs_set();

    printf("EPD Initialization... \n");
    epd_init();

//    printf("EPD Clear... \n");
//    epd_paint_clear(EPD_COLOR_WHITE);
//
//    printf("EPD Print... \n");
//    epd_paint_showString(10, 0, (uint8_t *)&"Franzininho", EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);

    epd_paint_newimage(image_bw, EPD_W, EPD_H, EPD_ROTATE_270, EPD_COLOR_WHITE);
    epd_paint_selectimage(image_bw);
    epd_paint_clear(EPD_COLOR_WHITE);
    epd_displayBW(image_bw);
    epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);

    Delay_Ms(2000);

    epd_init_partial();

    epd_paint_selectimage(image_bw);
    epd_paint_clear(EPD_COLOR_WHITE);

    epd_paint_showString(0,  0, (uint8_t *)&"   Hello   ", EPD_FONT_SIZE8x6, EPD_COLOR_BLACK);
    epd_paint_showString(0,  9, (uint8_t *)&"  E-Paper  ", EPD_FONT_SIZE8x6, EPD_COLOR_BLACK);
    epd_paint_showString(0, 18, (uint8_t *)&"   World   ", EPD_FONT_SIZE8x6, EPD_COLOR_BLACK);
    epd_paint_showString(0, 36, (uint8_t *)&"  CH32V003 ", EPD_FONT_SIZE8x6, EPD_COLOR_BLACK);
    epd_paint_showString(0, 54, (uint8_t *)&"Mechatronix", EPD_FONT_SIZE8x6, EPD_COLOR_BLACK);
    epd_paint_showString(0, 63, (uint8_t *)&"    Lab    ", EPD_FONT_SIZE8x6, EPD_COLOR_BLACK);

    epd_displayBW_partial(image_bw);

    //epd_update();

    printf("APP_Run() Start... \n");
    APP_Run();
}

void APP_Run(void)
{
    while (1)
    {
        if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3))
        {
            GPIO_ResetBits(GPIOC, GPIO_Pin_4);
            Delay_Ms(100);
        }
        else
        {
            GPIO_SetBits(GPIOC, GPIO_Pin_4);
            Delay_Ms(100);
        }
    }
}

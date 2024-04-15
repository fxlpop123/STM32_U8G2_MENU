#include "bsp.h" /* 底层硬件驱动 */

u8g2_t u8g2;
uint8_t ucKeyCode = 0;


uint8_t u8g2_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                            U8X8_UNUSED void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        OLED_ConfigGPIO();
        break;
    case U8X8_MSG_DELAY_MILLI:
        bsp_DelayMS(arg_int);
        break;
    case U8X8_MSG_DELAY_10MICRO:
        bsp_DelayUS(arg_int);
        break;
    case U8X8_MSG_DELAY_100NANO:
        __NOP();
        ;
        break;
    case U8X8_MSG_GPIO_SPI_CLOCK:
        if (arg_int)
            SSD_SCK_1();
        else
            SSD_SCK_0();
        break;
    case U8X8_MSG_GPIO_SPI_DATA:
        if (arg_int)
            SSD_SDIN_1();
        else
            SSD_SDIN_0();
        break;
    case U8X8_MSG_GPIO_CS:
        if (arg_int)
            SSD_CS_1();
        else
            SSD_CS_0();
        break;
    case U8X8_MSG_GPIO_DC:
        if (arg_int)
            SSD_RS_1();
        else
            SSD_RS_0();
        break;
    case U8X8_MSG_GPIO_RESET:
        if (arg_int)
            SSD_RST_1();
        else
            SSD_RST_0();
        break;
    default:
        return 0;
    }
    return 1; 
}



#define SPEED 4 // 16的因数
#define ICON_SPEED 12
#define ICON_SPACE 48 // 图标间隔，speed倍数


const uint8_t icon_pic[][200] = {
    {0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x7E,
        0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00,
        0x80, 0x7F, 0x00, 0x00, 0x00, 0x80, 0x7F, 0x00, 0x00, 0x00, 0xC0, 0x7F, 0x00, 0x00, 0x00, 0xE0, 0x7F, 0x00,
        0x00, 0x00, 0xF8, 0x7F, 0x00, 0x00, 0xF0, 0xF8, 0xFF, 0xFF, 0x01, 0xFC, 0xF8, 0xFF, 0xFF, 0x07, 0xFC, 0xF8,
        0xFF, 0xFF, 0x07, 0xFE, 0xF8, 0xFF, 0xFF, 0x07, 0xFE, 0xF8, 0xFF, 0xFF, 0x07, 0xFE, 0xF8, 0xFF, 0xFF, 0x07,
        0xFE, 0xF8, 0xFF, 0xFF, 0x07, 0xFE, 0xF8, 0xFF, 0xFF, 0x07, 0xFE, 0xF8, 0xFF, 0xFF, 0x03, 0xFE, 0xF8, 0xFF,
        0xFF, 0x03, 0xFE, 0xF8, 0xFF, 0xFF, 0x03, 0xFE, 0xF8, 0xFF, 0xFF, 0x03, 0xFE, 0xF8, 0xFF, 0xFF, 0x01, 0xFE,
        0xF8, 0xFF, 0xFF, 0x01, 0xFE, 0xF8, 0xFF, 0xFF, 0x01, 0xFE, 0xF8, 0xFF, 0xFF, 0x01, 0xFE, 0xF8, 0xFF, 0xFF,
        0x00, 0xFE, 0xF8, 0xFF, 0xFF, 0x00, 0xFC, 0xF8, 0xFF, 0x7F, 0x00, 0xFC, 0xF8, 0xFF, 0x3F, 0x00, 0xF8, 0xF8,
        0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00,
        0x1F, 0x00, 0x00, 0x00, 0x80, 0x1F, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0xC0, 0x3F, 0x00,
        0x00, 0x00, 0xC0, 0x3F, 0x00, 0x00, 0x00, 0xC0, 0x7F, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x00, 0x00, 0x00,
        0xF0, 0xFF, 0x01, 0x00, 0x00, 0xFC, 0xFF, 0x03, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0x00, 0xFC, 0xFF, 0xFF,
        0xFF, 0x07, 0xFE, 0xFF, 0xFF, 0xFF, 0x07, 0xFC, 0xFF, 0xFF, 0xFF, 0x07, 0xFC, 0xFF, 0xFF, 0xFF, 0x03,
        0xF8, 0xFF, 0xFF, 0xFF, 0x01, 0xF0, 0xFF, 0xFF, 0xFF, 0x00, 0xE0, 0xFF, 0xFF, 0x7F, 0x00, 0xC0, 0xFF,
        0xFF, 0x3F, 0x00, 0x80, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0xFF, 0xFF, 0x1F,
        0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x00, 0x00,
        0xFF, 0xFF, 0x0F, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0xFF, 0xFF,
        0x1F, 0x00, 0x80, 0xFF, 0xF0, 0x1F, 0x00, 0x80, 0x3F, 0xC0, 0x1F, 0x00, 0x80, 0x1F, 0x00, 0x1F, 0x00,
        0x00, 0x07, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1F, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x01, 0x00, 0x00, 0xFC,
        0xFF, 0x07, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x00, 0x80, 0xFF, 0xFF, 0x1F, 0x00, 0xC0, 0xFF, 0xFF, 0x7F,
        0x00, 0xE0, 0x07, 0x00, 0x7C, 0x00, 0xF0, 0x03, 0x00, 0xFC, 0x00, 0xF0, 0x03, 0x00, 0xFC, 0x01, 0xF8,
        0xFF, 0xF1, 0xFF, 0x01, 0xF8, 0xFF, 0xF1, 0xFF, 0x03, 0xF8, 0x7F, 0xC0, 0xFF, 0x03, 0xFC, 0x1F, 0x00,
        0xFF, 0x03, 0xFC, 0x07, 0x00, 0xFE, 0x07, 0xFC, 0x07, 0x01, 0xFC, 0x07, 0xFC, 0xC3, 0x31, 0xF8, 0x07,
        0xFC, 0xE1, 0xF1, 0xF8, 0x07, 0xFC, 0xF1, 0xF1, 0xF0, 0x07, 0xFC, 0xF1, 0xF1, 0xF0, 0x07, 0xFC, 0xF1,
        0xF1, 0xF1, 0x07, 0xFC, 0xF1, 0xF1, 0xF1, 0x07, 0xFC, 0xF1, 0xF1, 0xF1, 0x03, 0xF8, 0xF1, 0xF1, 0xF1,
        0x03, 0xF8, 0xFF, 0xF1, 0xFF, 0x03, 0xF8, 0xFF, 0xF1, 0xFF, 0x01, 0xF0, 0xFF, 0xF1, 0xFF, 0x01, 0xF0,
        0xFF, 0xF1, 0xFF, 0x00, 0xE0, 0xFF, 0xF1, 0x7F, 0x00, 0xC0, 0xFF, 0xFF, 0x7F, 0x00, 0x80, 0xFF, 0xFF,
        0x3F, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0xFC, 0xFF, 0x07, 0x00, 0x00, 0xF0, 0xFF, 0x01, 0x00,
        0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x0C, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00,
        0x00, 0x00, 0x00, 0xFC, 0x01, 0x00, 0x00, 0x00, 0xFC, 0x03, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x00, 0x00,
        0x00, 0xFC, 0x0F, 0x00, 0x00, 0x00, 0xFE, 0x1F, 0x00, 0x00, 0xF8, 0xFF, 0x3F, 0x00, 0x00, 0xFF, 0xFF,
        0xFF, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x01, 0xE0, 0xFF, 0xFF, 0xFF, 0x03, 0xF0, 0xFF, 0xFF, 0xFF, 0x07,
        0xF0, 0xFF, 0xFF, 0xFF, 0x0F, 0xF8, 0xFF, 0xFF, 0xFF, 0x0F, 0xFC, 0xFF, 0xFF, 0xFF, 0x07, 0xFC, 0xFF,
        0xFF, 0xFF, 0x03, 0xFE, 0xFF, 0xFF, 0xFF, 0x01, 0xFE, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x03, 0xFE, 0x3F,
        0x00, 0xFF, 0x00, 0xFC, 0x1F, 0x00, 0x3F, 0x00, 0xFC, 0x0F, 0x00, 0x1F, 0x00, 0xFC, 0x07, 0x00, 0x07,
        0x00, 0xFC, 0x03, 0x00, 0x03, 0x00, 0xFC, 0x01, 0x00, 0x01, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x3C,
        0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    },
};

uint8_t icon_width[] = {35, 37, 36, 36};

// main界面图片
const uint8_t LOGO[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x24, 0x00, 0x44, 0x04, 0x80, 0x20, 0x00, 0x00, 0x10, 0xF2,
    0x3C, 0x20, 0xE0, 0x00, 0x10, 0x01, 0x24, 0x00, 0xC4, 0x04, 0x80, 0x20, 0x00, 0x00, 0x10, 0x11, 0x24, 0x30, 0x10,
    0x01, 0x10, 0x71, 0x24, 0x0F, 0xC4, 0xE2, 0x99, 0x3C, 0x00, 0x00, 0x20, 0x11, 0x24, 0x20, 0x10, 0x01, 0xF0, 0x89,
    0xA4, 0x10, 0xA8, 0x12, 0x8A, 0x22, 0x00, 0x00, 0x20, 0xF1, 0x1C, 0x20, 0x10, 0x01, 0x10, 0xF9, 0xA4, 0x10, 0x98,
    0x13, 0x8A, 0x22, 0x00, 0x00, 0xA0, 0x10, 0x24, 0x20, 0x10, 0x01, 0x10, 0x09, 0xA4, 0x10, 0x98, 0x11, 0x8A, 0x22,
    0x00, 0x00, 0xA0, 0x10, 0x24, 0x20, 0x10, 0x01, 0x10, 0xF1, 0x24, 0x0F, 0x10, 0xE1, 0x89, 0x3C, 0x00, 0x00, 0x40,
    0xF0, 0x44, 0x20, 0xE2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F,
    0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xFF, 0xFB, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFE, 0x99, 0xFF, 0xE4, 0x1F, 0x00, 0x00, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xFD, 0xBD, 0xFF, 0xDE, 0xDF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x1F, 0xFE, 0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xDF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF,
    0xFD, 0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xDF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xFD, 0xF7, 0xFD,
    0x7D, 0x7F, 0xDF, 0x1F, 0xF0, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xFD, 0xF7, 0xFD, 0x7D, 0x7F, 0xDF,
    0xFF, 0xF7, 0xFD, 0xFF, 0x1F, 0xFE, 0xFF, 0x83, 0xFF, 0xE3, 0xF1, 0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD,
    0xFF, 0xE3, 0xF1, 0xFF, 0x7C, 0xF8, 0xF9, 0xEF, 0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xFF, 0xFD, 0xEF,
    0x3F, 0xFF, 0xF3, 0xFD, 0xDF, 0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xFF, 0x1E, 0xDC, 0xBF, 0x03, 0xF7,
    0xE3, 0xE1, 0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0x7F, 0xEF, 0xB9, 0xDF, 0xFB, 0xF0, 0xEF, 0xFD, 0xF7,
    0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0x7F, 0xE7, 0xBB, 0xDF, 0xF3, 0xFF, 0xEF, 0xFD, 0xF7, 0xFD, 0x7D, 0x7F,
    0xDF, 0xFF, 0xF7, 0xFD, 0xBF, 0xF7, 0x7B, 0xDF, 0x0F, 0xFF, 0xEF, 0xFD, 0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7,
    0xFD, 0xBF, 0x0F, 0x7C, 0xBF, 0xFF, 0xFC, 0xEF, 0xFD, 0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xBF, 0xFF,
    0xBF, 0x7F, 0xFE, 0xF3, 0xEF, 0xFD, 0xF7, 0xFD, 0xBE, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xBF, 0x07, 0xC0, 0xFF, 0xE1,
    0xF7, 0xEF, 0xFD, 0xEF, 0xFB, 0xBE, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xBF, 0xEF, 0xFF, 0xFF, 0x9F, 0xEF, 0xEF, 0xFD,
    0xEF, 0x07, 0xBF, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0x7F, 0xEF, 0xC7, 0x3F, 0x3E, 0xEF, 0xEF, 0xFD, 0xDF, 0xFF, 0xDF,
    0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xFF, 0x1E, 0xB8, 0xDF, 0xB9, 0xF7, 0xEF, 0xE3, 0x9F, 0xFF, 0xEF, 0x7F, 0xDF, 0xFF,
    0xF7, 0xFD, 0xFF, 0xFC, 0xBF, 0x9F, 0xC7, 0xF7, 0xDF, 0xCF, 0x7F, 0xFF, 0xF3, 0xFF, 0xDE, 0xFF, 0xEF, 0xFD, 0xFF,
    0xFB, 0xCF, 0x3F, 0xFF, 0xF9, 0x9F, 0xEF, 0xFF, 0x00, 0xFC, 0xFF, 0xE1, 0xFF, 0x1F, 0xFE, 0xFF, 0x07, 0xF0, 0xFF,
    0x00, 0xFE, 0x7F, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFC, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00,
    0x40, 0x00, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x60, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x9C, 0x24, 0xC7, 0x1C, 0x0F, 0x68, 0x22, 0xA0, 0xCC, 0x23, 0x91, 0xC7, 0x01,
    0x00, 0xFE, 0x88, 0xA2, 0xAA, 0x48, 0xA2, 0x08, 0x98, 0x14, 0xB0, 0x24, 0x22, 0x91, 0x24, 0x02, 0x00, 0x00, 0x78,
    0xA2, 0xAB, 0x4F, 0xBE, 0x08, 0x88, 0x14, 0xF0, 0x24, 0x22, 0x91, 0x24, 0x02, 0x00, 0x00, 0x08, 0x22, 0x9B, 0x40,
    0x82, 0x08, 0x88, 0x0C, 0x08, 0x25, 0x22, 0x91, 0x24, 0x02, 0x00, 0x00, 0x08, 0x1C, 0x11, 0x4F, 0x3C, 0x0F, 0x78,
    0x08, 0x08, 0xC5, 0xE3, 0x91, 0xC4, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const float PID_MAX = 10.00; // PID最大允许值
// PID变量
float Kpid[3] = {9.97, 0.2, 0.01}; // Kp,Ki,Kd

uint8_t disappear_step = 1;

float angle, angle_last;
// 实时坐标
uint8_t chart_x;
bool frame_is_drawed = false;

// 指向buf首地址的指针
uint8_t *buf_ptr;
uint16_t buf_len;

// 选择界面变量
uint8_t x;
int16_t y, y_trg;                 // 目标和当前
uint8_t line_y, line_y_trg;       // 线的位置
uint8_t box_width, box_width_trg; // 框的宽度
int16_t box_y, box_y_trg;         // 框的当前值和目标值
int8_t ui_select;                 // 当前选中那一栏

// pid界面变量
// int8_t pid_y,pid_y_trg;
uint8_t pid_line_y, pid_line_y_trg;       // 线的位置
uint8_t pid_box_width, pid_box_width_trg; // 框的宽度
int16_t pid_box_y, pid_box_y_trg;         // 框的当前值和目标值
int8_t pid_select;                        // 当前选中那一栏

// icon界面变量
int16_t icon_x, icon_x_trg;
int16_t app_y, app_y_trg;

int8_t icon_select;

uint8_t ui_index, ui_state;

enum // ui_index
{
    M_LOGO,      // 开始界面
    M_SELECT,    // 选择界面
    M_PID,       // PID界面
    M_PID_EDIT,  // pid编辑
    M_ICON,      // icon界面
    M_CHART,     // 图表
    M_TEXT_EDIT, // 文字编辑
    M_VIDEO,     // 视频显示
    M_ABOUT,     // 关于本机
};

enum // ui_state
{
    S_NONE,
    S_DISAPPEAR,
    S_SWITCH,
    S_MENU_TO_MENU,
    S_MENU_TO_PIC,
    S_PIC_TO_MENU,
};

// 菜单结构体
typedef struct
{
    char *select;
} SELECT_LIST;

SELECT_LIST pid[] = {
    {"-Proportion"},
    {"-Integral"},
    {"-Derivative"},
    {"Return"},
};

uint8_t pid_num = sizeof(pid) / sizeof(SELECT_LIST); // PID选项数量

SELECT_LIST list[] = {
    {"MainUI"}, {"+PID Editor"}, {"-Icon Test"}, {"-Chart Test"}, {"-Text Edit"}, {"-Play Video"}, {"{ About }"},
};

uint8_t list_num = sizeof(list) / sizeof(SELECT_LIST); // 选择界面数量
uint8_t single_line_length = 63 / 7;
uint8_t total_line_length = 9 * 7 + 1;

SELECT_LIST icon[] = {
    {"Likes"},
    {"Collection"},
    {"Slot"},
    {"Share"},
};

// 设备名称
char name[] = "Hello World ";
// 允许名字的最大长度
const uint8_t name_len = 12; // 0-11for name  12 for return
uint8_t edit_index = 0;
bool edit_flag = false;         // 默认不在编辑
uint8_t blink_flag;             // 默认高亮
const uint8_t BLINK_SPEED = 16; // 2的倍数

uint8_t icon_num = sizeof(icon) / sizeof(SELECT_LIST);



// 按键信息
typedef struct
{
    uint8_t id;
    bool pressed;
} KEY_MSG;

KEY_MSG key_msg = {0};

// 移动函数
bool move(int16_t *a, int16_t *a_trg)
{
    if (*a < *a_trg)
    {
        *a += SPEED;
        if (*a > *a_trg)
            *a = *a_trg; // 加完超过
    }
    else if (*a > *a_trg)
    {
        *a -= SPEED;
        if (*a < *a_trg)
            *a = *a_trg; // 减完不足
    }
    else
    {
        return true; // 到达目标
    }
    return false; // 未到达
}

// 移动函数
bool move_icon(int16_t *a, int16_t *a_trg)
{
    if (*a < *a_trg)
    {
        *a += ICON_SPEED;
        if (*a > *a_trg)
            *a = *a_trg; // 加完超过
    }
    else if (*a > *a_trg)
    {
        *a -= ICON_SPEED;
        if (*a < *a_trg)
            *a = *a_trg; // 减完不足
    }
    else
    {
        return true; // 到达目标
    }
    return false; // 未到达
}

// 宽度移动函数
bool move_width(uint8_t *a, uint8_t *a_trg, uint8_t select, uint8_t id)
{
    if (*a < *a_trg)
    {
        uint8_t step = 16 / SPEED; // 判断步数
        uint8_t len;
        if (ui_index == M_SELECT)
        {
            len = abs(u8g2_GetStrWidth(&u8g2,list[select].select) -
                      u8g2_GetStrWidth(&u8g2,list[id == 0 ? select + 1 : select - 1].select));
        }
        else if (ui_index == M_PID)
        {
            len = abs(u8g2_GetStrWidth(&u8g2,pid[select].select) -
                      u8g2_GetStrWidth(&u8g2,pid[id == 0 ? select + 1 : select - 1].select));
        }
        uint8_t width_speed = ((len % step) == 0 ? (len / step) : (len / step + 1)); // 计算步长
        *a += width_speed;
        if (*a > *a_trg)
            *a = *a_trg; //
    }
    else if (*a > *a_trg)
    {
        uint8_t step = 16 / SPEED; // 判断步数
        uint8_t len;
        if (ui_index == M_SELECT)
        {
            len = abs(u8g2_GetStrWidth(&u8g2,list[select].select) -
                      u8g2_GetStrWidth(&u8g2,list[id == 0 ? select + 1 : select - 1].select));
        }
        else if (ui_index == M_PID)
        {
            len = abs(u8g2_GetStrWidth(&u8g2,pid[select].select) -
                      u8g2_GetStrWidth(&u8g2,pid[id == 0 ? select + 1 : select - 1].select));
        }
        uint8_t width_speed = ((len % step) == 0 ? (len / step) : (len / step + 1)); // 计算步长
        *a -= width_speed;
        if (*a < *a_trg)
            *a = *a_trg;
    }
    else
    {
        return true; // 到达目标
    }
    return false; // 未到达
}

// 进度条移动函数
bool move_bar(uint8_t *a, uint8_t *a_trg)
{
    if (*a < *a_trg)
    {
        uint8_t step = 16 / SPEED; // 判断步数
        uint8_t width_speed = ((single_line_length % step) == 0 ? (single_line_length / step)
                                                                : (single_line_length / step + 1)); // 计算步长
        *a += width_speed;
        if (*a > *a_trg)
            *a = *a_trg; //
    }
    else if (*a > *a_trg)
    {
        uint8_t step = 16 / SPEED; // 判断步数
        uint8_t width_speed = ((single_line_length % step) == 0 ? (single_line_length / step)
                                                                : (single_line_length / step + 1)); // 计算步长
        *a -= width_speed;
        if (*a < *a_trg)
            *a = *a_trg;
    }
    else
    {
        return true; // 到达目标
    }
    return false; // 未到达
}

// 文字编辑函数
void text_edit(bool dir, uint8_t index)
{
    if (!dir)
    {
        if (name[index] >= 'A' && name[index] <= 'Z') // 大写字母
        {
            if (name[index] == 'A')
            {
                name[index] = 'z';
            }
            else
            {
                name[index] -= 1;
            }
        }
        else if (name[index] >= 'a' && name[index] <= 'z') // 小写字母
        {
            if (name[index] == 'a')
            {
                name[index] = ' ';
            }
            else
            {
                name[index] -= 1;
            }
        }
        else
        {
            name[index] = 'Z';
        }
    }
    else
    {
        if (name[index] >= 'A' && name[index] <= 'Z') // 大写字母
        {
            if (name[index] == 'Z')
            {
                name[index] = ' ';
            }
            else
            {
                name[index] += 1;
            }
        }
        else if (name[index] >= 'a' && name[index] <= 'z') // 小写字母
        {
            if (name[index] == 'z')
            {
                name[index] = 'A';
            }
            else
            {
                name[index] += 1;
            }
        }
        else
        {
            name[index] = 'a';
        }
    }
}

// 消失函数
void disappear()
{
    switch (disappear_step)
    {
    case 1:
        for (uint16_t i = 0; i < buf_len; ++i)
        {
            if (i % 2 == 0)
                buf_ptr[i] = buf_ptr[i] & 0x55;
        }
        break;
    case 2:
        for (uint16_t i = 0; i < buf_len; ++i)
        {
            if (i % 2 != 0)
                buf_ptr[i] = buf_ptr[i] & 0xAA;
        }
        break;
    case 3:
        for (uint16_t i = 0; i < buf_len; ++i)
        {
            if (i % 2 == 0)
                buf_ptr[i] = buf_ptr[i] & 0x00;
        }
        break;
    case 4:
        for (uint16_t i = 0; i < buf_len; ++i)
        {
            if (i % 2 != 0)
                buf_ptr[i] = buf_ptr[i] & 0x00;
        }
        break;
    default:
        ui_state = S_NONE;
        disappear_step = 0;
        break;
    }
    disappear_step++;
}

/**************************界面显示*******************************/

void logo_ui_show() // 显示logo
{
    u8g2_DrawXBMP(&u8g2, 0, 0, 128, 64, LOGO);
}

void select_ui_show() // 选择界面
{
    move_bar(&line_y, &line_y_trg);
    move(&y, &y_trg);
    move(&box_y, &box_y_trg);
    move_width(&box_width, &box_width_trg, ui_select, key_msg.id);
    u8g2_DrawVLine(&u8g2,126, 0, total_line_length);
    u8g2_DrawPixel(&u8g2,125, 0);
    u8g2_DrawPixel(&u8g2,127, 0);
    for (uint8_t i = 0; i < list_num; ++i)
    {
        u8g2_DrawStr(&u8g2,x, 16 * i + y + 12, list[i].select); // 第一段输出位置
        u8g2_DrawPixel(&u8g2,125, single_line_length * (i + 1));
        u8g2_DrawPixel(&u8g2,127, single_line_length * (i + 1));
    }
    u8g2_DrawVLine(&u8g2,125, line_y, single_line_length - 1);
    u8g2_DrawVLine(&u8g2,127, line_y, single_line_length - 1);
    u8g2_SetDrawColor(&u8g2,2);
    u8g2_DrawRBox(&u8g2,0, box_y, box_width, 16, 1);
    u8g2_SetDrawColor(&u8g2,1);
}

void pid_ui_show() // PID界面
{
    move_bar(&pid_line_y, &pid_line_y_trg);
    move(&pid_box_y, &pid_box_y_trg);
    move_width(&pid_box_width, &pid_box_width_trg, pid_select, key_msg.id);
    u8g2_DrawVLine(&u8g2,126, 0, 61);
    u8g2_DrawPixel(&u8g2,125, 0);
    u8g2_DrawPixel(&u8g2,127, 0);
    for (uint8_t i = 0; i < pid_num; ++i)
    {
        u8g2_DrawStr(&u8g2,x, 16 * i + 12, pid[i].select); // 第一段输出位置
        u8g2_DrawPixel(&u8g2,125, 15 * (i + 1));
        u8g2_DrawPixel(&u8g2,127, 15 * (i + 1));
    }

    u8g2_SetDrawColor(&u8g2,2);
    u8g2_DrawRBox(&u8g2,0, pid_box_y, pid_box_width, 16, 1);
    u8g2_SetDrawColor(&u8g2,1);
    u8g2_DrawVLine(&u8g2,125, pid_line_y, 14);
    u8g2_DrawVLine(&u8g2,127, pid_line_y, 14);
}


char *ftoa(float num)
{
    static char str[10] = {0};
    memset(str, 0, sizeof(str));
    sprintf(str, "%.2f", num);
    return str;
}

void pid_edit_ui_show() // 显示PID编辑
{
    u8g2_DrawBox(&u8g2,16, 16, 96, 31);
    u8g2_SetDrawColor(&u8g2,2);
    u8g2_DrawBox(&u8g2,17, 17, 94, 29);
    u8g2_SetDrawColor(&u8g2,1);

    u8g2_DrawFrame(&u8g2,18, 36, 60, 8);
    u8g2_DrawBox(&u8g2,20, 38, (uint8_t)(Kpid[pid_select] / PID_MAX * 56), 4);

    switch (pid_select)
    {
    case 0:
        u8g2_DrawStr(&u8g2,22, 30, "Editing Kp");
        break;
    case 1:
        u8g2_DrawStr(&u8g2,22, 30,"Editing Ki");
        break;
    case 2:
        u8g2_DrawStr(&u8g2,22, 30,"Editing Kd");
        break;
    default:
        break;
    }

	
    u8g2_DrawStr(&u8g2,81, 44,ftoa(Kpid[pid_select]));
}

void icon_ui_show(void) // 显示icon
{

    move_icon(&icon_x, &icon_x_trg);
    move(&app_y, &app_y_trg);

    for (uint8_t i = 0; i < icon_num; ++i)
    {
        u8g2_DrawXBMP(&u8g2,46 + icon_x + i * ICON_SPACE, 6, 36, icon_width[i], icon_pic[i]);
        u8g2_SetClipWindow(&u8g2,0, 48, 128, 64);
        u8g2_DrawStr(&u8g2,(128 - u8g2_GetStrWidth(&u8g2,icon[i].select)) / 2, 62 - app_y + i * 16, icon[i].select);
        u8g2_SetMaxClipWindow(&u8g2);
    }
}

void chart_draw_frame() // chart界面
{

    u8g2_DrawStr(&u8g2,4, 12, "Real time angle :");
    u8g2_DrawRBox(&u8g2, 4, 18, 120, 46, 8);
    u8g2_SetDrawColor(&u8g2, 2);
    u8g2_DrawHLine(&u8g2,10, 58, 108);
    u8g2_DrawVLine(&u8g2, 10, 24, 34);
    // 箭头
    u8g2_DrawPixel(&u8g2,7, 27);
    u8g2_DrawPixel(&u8g2,8, 26);
    u8g2_DrawPixel(&u8g2,9, 25);

    u8g2_DrawPixel(&u8g2,116, 59);
    u8g2_DrawPixel(&u8g2,115, 60);
    u8g2_DrawPixel(&u8g2,114, 61);
    u8g2_SetDrawColor(&u8g2,1);
}


char *itoa(uint32_t num)
{
    static char str[10] = {0};
    memset(str, 0, sizeof(str));
    sprintf(str, "%d", num);
    return str;
}

void chart_ui_show() // chart界面
{
    if (!frame_is_drawed) // 框架只画一遍
    {
        u8g2_ClearBuffer(&u8g2);
        chart_draw_frame();
        angle_last = 20.00;
        frame_is_drawed = true;
    }

    u8g2_DrawBox(&u8g2,96, 0, 30, 14);

    u8g2_DrawVLine(&u8g2, chart_x + 10, 59, 3);
    if (chart_x == 100)
        chart_x = 0;


    u8g2_DrawVLine(&u8g2, chart_x + 11, 24, 34);
    u8g2_DrawVLine(&u8g2,chart_x + 12, 24, 34);
    u8g2_DrawVLine(&u8g2,chart_x + 13, 24, 34);
    u8g2_DrawVLine(&u8g2,chart_x + 14, 24, 34);

    // 异或绘制
    u8g2_SetDrawColor(&u8g2,2);
    angle = 20.00 + rand()%40;
    u8g2_DrawLine(&u8g2,chart_x + 11, 58 - (int)angle_last / 2, chart_x + 12, 58 - (int)angle / 2);
    u8g2_DrawVLine(&u8g2,chart_x + 12, 59, 3);
    angle_last = angle;
    chart_x += 2;
    u8g2_DrawBox(&u8g2,96, 0, 30, 14);
    u8g2_SetDrawColor(&u8g2,1);
	
	u8g2_DrawStr(&u8g2, 96, 12,itoa(angle));

}



void text_edit_ui_show()
{
    u8g2_DrawRFrame(&u8g2, 4, 6, 120, 52, 8);
    u8g2_DrawStr(&u8g2,(128 - u8g2_GetStrWidth(&u8g2, "--Text Editor--")) / 2, 20, "--Text Editor--");
    u8g2_DrawStr(&u8g2,10, 38, name);
    u8g2_DrawStr(&u8g2, 80, 50, "-Return");

    uint8_t box_x = 9;

    // 绘制光标
    if (edit_index < name_len)
    {
        if (blink_flag < BLINK_SPEED / 2)
        {
            for (uint8_t i = 0; i < edit_index; ++i)
            {
                char temp[2] = {name[i], '\0'};
                box_x += u8g2_GetStrWidth(&u8g2, temp);
                if (name[i] != ' ')
                {
                    box_x++;
                }
            }
            char temp[2] = {name[edit_index], '\0'};
            u8g2_SetDrawColor(&u8g2, 2);
            u8g2_DrawBox(&u8g2, box_x, 26, u8g2_GetStrWidth(&u8g2,temp) + 2, 16);
            u8g2_SetDrawColor(&u8g2, 1);
        }
    }
    else
    {
        u8g2_SetDrawColor(&u8g2, 2);
        u8g2_DrawRBox(&u8g2, 78, 38, u8g2_GetStrWidth(&u8g2, "-Return") + 4, 16, 1);
        u8g2_SetDrawColor(&u8g2, 1);
    }

    if (edit_flag) // 处于编辑状态
    {
        if (blink_flag < BLINK_SPEED)
        {
            blink_flag += 1;
        }
        else
        {
            blink_flag = 0;
        }
    }
    else
    {
        blink_flag = 0;
    }
}

void about_ui_show() // about界面
{

    u8g2_DrawStr(&u8g2,2, 12, "MCU : ESP32");
    u8g2_DrawStr(&u8g2,2, 28, "FLASH : 4MB");
    u8g2_DrawStr(&u8g2,2, 44, "SRAM : 520KB");
    u8g2_DrawStr(&u8g2,2, 60, "RTC SRAM : 16KB");
}

/**************************界面处理*******************************/

void logo_proc() // logo界面处理函数
{
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        ui_state = S_DISAPPEAR;
        ui_index = M_SELECT;
    }
    logo_ui_show();
}

void pid_edit_proc(void) // pid编辑界面处理函数
{
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        switch (key_msg.id)
        {
        case 0:
            if (Kpid[pid_select] > 0)
                Kpid[pid_select] -= 0.01;
            break;
        case 1:
            if (Kpid[pid_select] < PID_MAX)
                Kpid[pid_select] += 0.01;
            break;
        case 2:
            ui_index = M_PID;
            break;
        default:
            break;
        }
    }
    pid_ui_show();
    for (uint16_t i = 0; i < buf_len; ++i)
    {
        buf_ptr[i] = buf_ptr[i] & (i % 2 == 0 ? 0x55 : 0xAA);
    }
    pid_edit_ui_show();
}

void pid_proc() // pid界面处理函数
{
    pid_ui_show();
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        switch (key_msg.id)
        {
        case 0:
            if (pid_select != 0)
            {
                pid_select -= 1;
                pid_line_y_trg -= 15;
                pid_box_y_trg -= 16;
                break;
            }
            else
            {
                break;
            }
        case 1:
            if (pid_select != 3)
            {
                pid_select += 1;
                pid_line_y_trg += 15;
                pid_box_y_trg += 16;
            }
            else
            {
                break;
            }
            break;
        case 2:
            if (pid_select == 3)
            {
                ui_index = M_SELECT;
                ui_state = S_DISAPPEAR;
                pid_select = 0;
                pid_line_y = pid_line_y_trg = 1;
                pid_box_y = pid_box_y_trg = 0;
                pid_box_width = pid_box_width_trg = u8g2_GetStrWidth(&u8g2,pid[pid_select].select) + x * 2;
            }
            else
            {
                ui_index = M_PID_EDIT;
            }
            break;
        default:
            break;
        }
        pid_box_width_trg = u8g2_GetStrWidth(&u8g2, pid[pid_select].select) + x * 2;
    }
}

void select_proc(void) // 选择界面处理
{
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        switch (key_msg.id)
        {
        case 0:
            if (ui_select < 1)
                break;
            ui_select -= 1;
            line_y_trg -= single_line_length;
            if (ui_select < -(y / 16))
            {
                y_trg += 16;
            }
            else
            {
                box_y_trg -= 16;
            }

            break;
        case 1:
            if ((ui_select + 2) > (sizeof(list) / sizeof(SELECT_LIST)))
                break;
            ui_select += 1;
            line_y_trg += single_line_length;
            if ((ui_select + 1) > (4 - y / 16))
            {
                y_trg -= 16;
            }
            else
            {
                box_y_trg += 16;
            }

            break;
        case 2:
            switch (ui_select)
            {
            case 0: // return
                ui_state = S_DISAPPEAR;
                ui_index = M_LOGO;
                break;
            case 1: // pid
                ui_state = S_DISAPPEAR;
                ui_index = M_PID;
                break;
            case 2: // icon
                ui_state = S_DISAPPEAR;
                ui_index = M_ICON;
                break;
            case 3: // chart
                ui_state = S_DISAPPEAR;
                ui_index = M_CHART;
                break;
            case 4: // textedit
                ui_state = S_DISAPPEAR;
                ui_index = M_TEXT_EDIT;
                break;
            case 6: // about
                ui_state = S_DISAPPEAR;
                ui_index = M_ABOUT;
                break;
            default:
                break;
            }
        default:
            break;
        }
        box_width_trg = u8g2_GetStrWidth(&u8g2, list[ui_select].select) + x * 2;
    }
    select_ui_show();
}

void icon_proc(void) // icon界面处理
{
    icon_ui_show();
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        switch (key_msg.id)
        {
        case 1:
            if (icon_select != (icon_num - 1))
            {
                icon_select += 1;
                app_y_trg += 16;
                icon_x_trg -= ICON_SPACE;
            }
            break;
        case 0:
            if (icon_select != 0)
            {
                icon_select -= 1;
                app_y_trg -= 16;
                icon_x_trg += ICON_SPACE;
            }
            break;
        case 2:
            ui_state = S_DISAPPEAR;
            ui_index = M_SELECT;
            icon_select = 0;
            icon_x = icon_x_trg = 0;
            app_y = app_y_trg = 0;
            break;
        default:
            break;
        }
    }
}

void chart_proc() // chart界面处理函数
{
    chart_ui_show();
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        ui_state = S_DISAPPEAR;
        ui_index = M_SELECT;
        frame_is_drawed = false; // 退出后框架为未画状态
        chart_x = 0;
    }
}

void text_edit_proc()
{
    text_edit_ui_show();
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        switch (key_msg.id)
        {
        case 0:
            if (edit_flag)
            {
                // 编辑
                text_edit(false, edit_index);
            }
            else
            {
                if (edit_index == 0)
                {
                    edit_index = name_len;
                }
                else
                {
                    edit_index -= 1;
                }
            }
            break;
        case 1:
            if (edit_flag)
            {
                // 编辑
                text_edit(true, edit_index);
            }
            else
            {
                if (edit_index == name_len)
                {
                    edit_index = 0;
                }
                else
                {
                    edit_index += 1;
                }
            }
            break;
        case 2:
            if (edit_index == name_len)
            {
                ui_state = S_DISAPPEAR;
                ui_index = M_SELECT;
                edit_index = 0;
            }
            else
            {
                edit_flag = !edit_flag;
            }
            break;
        default:
            break;
        }
    }
}

void about_proc() // about界面处理函数
{
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        ui_state = S_DISAPPEAR;
        ui_index = M_SELECT;
    }
    about_ui_show();
}
/********************************总的UI显示************************************/

void ui_proc() // 总的UI进程
{
    switch (ui_state)
    {
    case S_NONE:
        if (ui_index != M_CHART)
            u8g2_ClearBuffer(&u8g2);
        switch (ui_index)
        {
        case M_LOGO:
            logo_proc();
            break;
        case M_SELECT:
            select_proc();
            break;
        case M_PID:
            pid_proc();
            break;
        case M_ICON:
            icon_proc();
            break;
        case M_CHART:
            chart_proc();
            break;
        case M_TEXT_EDIT:
            text_edit_proc();
            break;
        case M_PID_EDIT:
            pid_edit_proc();
            break;
        case M_ABOUT:
            about_proc();
            break;
        default:
            break;
        }
        break;
    case S_DISAPPEAR:
        disappear();
        break;
    default:
        break;
    }
    u8g2_SendBuffer(&u8g2);
}

void key_scan()
{
	ucKeyCode = bsp_GetKey();
	switch(ucKeyCode)
	{
		case JOY_DOWN_L: key_msg.id=0; key_msg.pressed=true; break;
		
		case JOY_DOWN_D: key_msg.id=1; key_msg.pressed=true; break;
		
		case JOY_DOWN_R: key_msg.id=2; key_msg.pressed=true; break;
		
		default: break;		
	}
}

void setup()
{
    u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese1);

    buf_ptr = u8g2_GetBufferPtr(&u8g2); // 拿到buffer首地址
    buf_len = 8 * u8g2_GetBufferTileHeight(&u8g2) * u8g2_GetBufferTileWidth(&u8g2);

    x = 4;
    y = y_trg = 0;
    line_y = line_y_trg = 1;
    pid_line_y = pid_line_y_trg = 1;
    ui_select = pid_select = icon_select = 0;
    icon_x = icon_x_trg = 0;
    app_y = app_y_trg = 0;

    box_width = box_width_trg = u8g2_GetStrWidth(&u8g2,list[ui_select].select) + x * 2;         // 两边各多2
    pid_box_width = pid_box_width_trg = u8g2_GetStrWidth(&u8g2, pid[pid_select].select) + x * 2; // 两边各多2

    ui_index = M_LOGO;
    ui_state = S_NONE;
}


void u8g2_init(u8g2_t *u8g2)
{
    u8g2_Setup_ssd1306_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi,
                                       u8g2_gpio_and_delay); // 初始化 u8g2 结构体
    u8g2_InitDisplay(u8g2);      // 根据所选的芯片进行初始化工作，初始化完成后，显示器处于关闭状态
    u8g2_SetPowerSave(u8g2, 0);  // 唤醒显示器
    u8g2_SetContrast(u8g2, 100); // 设置对比度
    u8g2_ClearBuffer(u8g2);      // 清除显示缓存
}

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
    bsp_Init(); /* 硬件初始化 */
	
	u8g2_init(&u8g2);
	setup();
	

    while (1)
    {
        bsp_Idle(); /* 这个函数在bsp.c文件。用户可以修改这个函数实现CPU休眠和喂狗 */

        key_scan();
		ui_proc();
    }
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

#include "ssd1306.h"

uint8_t SSD1306_Init(ssd1306_t *structure)
{

    delay_ms(250);

    /* Check if LCD connected to I2C */
    if (I2C_IsDeviceReady(structure->instance, structure->adress_I2C, 1, 20000) != 0)
    {
        return 0;
    }

    delay_ms(25);

    /* Init LCD */
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xAE); // display off
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x20); // Set Memory Addressing Mode
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x10); // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xB0); // Set Page Start Address for Page Addressing Mode,0-7
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xC8); // Set COM Output Scan Direction
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x00); //---set low column address
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x10); //---set high column address
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x40); //--set start line address
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x81); //--set contrast control register
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xFF);
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xA1); //--set segment re-map 0 to 127
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xA6); //--set normal display
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xA8); //--set multiplex ratio(1 to 64)
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x3F); //
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xA4); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xD3); //-set display offset
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x00); //-not offset
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xD5); //--set display clock divide ratio/oscillator frequency
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xF0); //--set divide ratio
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xD9); //--set pre-charge period
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x22); //
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xDA); //--set com pins hardware configuration
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x12);
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xDB); //--set vcomh
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x20); // 0x20,0.77xVcc
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x8D); //--set DC-DC enable
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0x14); //
    SSD1306_WRITECOMMAND(structure->adress_I2C, 0xAF); //--turn on SSD1306 panel

    SSD1306_WRITECOMMAND(structure->adress_I2C, SSD1306_DEACTIVATE_SCROLL);

    SSD1306_Fill(SSD1306_COLOR_BLACK);

    SSD1306_UpdateScreen(structure);

    structure->currentX = 0;
    structure->currentY = 0;

    structure->initialized = 1;

    return 1;
}

void SSD1306_Fill(SSD1306_COLOR_t color)
{
    if (color == SSD1306_COLOR_BLACK)
    {
        memset(SSD1306_Buffer, 0x00, sizeof(SSD1306_Buffer));
    }
    else if (color == SSD1306_COLOR_WHITE)
    {
        memset(SSD1306_Buffer, 0xFF, sizeof(SSD1306_Buffer));
    }
}

void SSD1306_ScrollRight(uint8_t address, uint8_t start_row, uint8_t end_row)
{
    SSD1306_WRITECOMMAND(address, SSD1306_RIGHT_HORIZONTAL_SCROLL); // send 0x26
    SSD1306_WRITECOMMAND(address, 0x00);                            // send dummy
    SSD1306_WRITECOMMAND(address, start_row);                       // start page address
    SSD1306_WRITECOMMAND(address, 0X00);                            // time interval 5 frames
    SSD1306_WRITECOMMAND(address, end_row);                         // end page address
    SSD1306_WRITECOMMAND(address, 0X00);
    SSD1306_WRITECOMMAND(address, 0XFF);
    SSD1306_WRITECOMMAND(address, SSD1306_ACTIVATE_SCROLL); // start scroll
}

void SSD1306_ScrollLeft(uint8_t address, uint8_t start_row, uint8_t end_row)
{
    SSD1306_WRITECOMMAND(address, SSD1306_LEFT_HORIZONTAL_SCROLL); // send 0x26
    SSD1306_WRITECOMMAND(address, 0x00);                           // send dummy
    SSD1306_WRITECOMMAND(address, start_row);                      // start page address
    SSD1306_WRITECOMMAND(address, 0X00);                           // time interval 5 frames
    SSD1306_WRITECOMMAND(address, end_row);                        // end page address
    SSD1306_WRITECOMMAND(address, 0X00);
    SSD1306_WRITECOMMAND(address, 0XFF);
    SSD1306_WRITECOMMAND(address, SSD1306_ACTIVATE_SCROLL); // start scroll
}

void SSD1306_Scrolldiagright(uint8_t address, uint8_t start_row, uint8_t end_row)
{
    SSD1306_WRITECOMMAND(address, SSD1306_SET_VERTICAL_SCROLL_AREA); // sect the area
    SSD1306_WRITECOMMAND(address, 0x00);                             // write dummy
    SSD1306_WRITECOMMAND(address, SSD1306_HEIGHT);

    SSD1306_WRITECOMMAND(address, SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
    SSD1306_WRITECOMMAND(address, 0x00);
    SSD1306_WRITECOMMAND(address, start_row);
    SSD1306_WRITECOMMAND(address, 0X00);
    SSD1306_WRITECOMMAND(address, end_row);
    SSD1306_WRITECOMMAND(address, 0x01);
    SSD1306_WRITECOMMAND(address, SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_Scrolldiagleft(uint8_t address, uint8_t start_row, uint8_t end_row)
{
    SSD1306_WRITECOMMAND(address, SSD1306_SET_VERTICAL_SCROLL_AREA); // sect the area
    SSD1306_WRITECOMMAND(address, 0x00);                             // write dummy
    SSD1306_WRITECOMMAND(address, SSD1306_HEIGHT);

    SSD1306_WRITECOMMAND(address, SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
    SSD1306_WRITECOMMAND(address, 0x00);
    SSD1306_WRITECOMMAND(address, start_row);
    SSD1306_WRITECOMMAND(address, 0X00);
    SSD1306_WRITECOMMAND(address, end_row);
    SSD1306_WRITECOMMAND(address, 0x01);
    SSD1306_WRITECOMMAND(address, SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_Stopscroll(uint8_t address)
{
    SSD1306_WRITECOMMAND(address, SSD1306_DEACTIVATE_SCROLL);
}

void SSD1306_InvertDisplay(ssd1306_t *structure, int i)
{
    if (i)
    {
        SSD1306_WRITECOMMAND(structure ->adress_I2C, SSD1306_INVERTDISPLAY);
    }
    else
    {
        SSD1306_WRITECOMMAND(structure ->adress_I2C, SSD1306_NORMALDISPLAY);
    }
}

void SSD1306_DrawBitmap(ssd1306_t *structure, int16_t x, int16_t y, const unsigned char *bitmap, int16_t w, int16_t h, uint16_t color)
{

    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    for (int16_t j = 0; j < h; j++, y++)
    {
        for (int16_t i = 0; i < w; i++)
        {
            if (i & 7)
            {
                byte <<= 1;
            }
            else
            {
                byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
            }
            if (byte & 0x80)
                SSD1306_DrawPixel(structure, x + i, y, color);
        }
    }
}

void SSD1306_UpdateScreen(ssd1306_t *structure)
{
    uint8_t m;

    for (m = 0; m < 8; m++)
    {
        SSD1306_WRITECOMMAND(structure ->adress_I2C, 0xB0 + m);
        SSD1306_WRITECOMMAND(structure ->adress_I2C, 0x00);
        SSD1306_WRITECOMMAND(structure ->adress_I2C, 0x10);

        /* Write multi data */
        ssd1306_I2C_WriteMulti(structure ->adress_I2C, 0x40, &SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
    }
}

void SSD1306_ToggleInvert(ssd1306_t *structure)
{
    uint16_t i;

    /* Toggle invert */
    structure->inverted = !structure->inverted;

    /* Do memory toggle */
    for (i = 0; i < sizeof(SSD1306_Buffer); i++)
    {
        SSD1306_Buffer[i] = ~SSD1306_Buffer[i];
    }
}

void SSD1306_DrawPixel(ssd1306_t *structure, uint16_t x, uint16_t y, SSD1306_COLOR_t color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    {
        /* Error */
        return;
    }

    /* Check if pixels are inverted */
    if (structure->inverted)
    {
        color = (SSD1306_COLOR_t)!color;
    }

    /* Set color */
    if (color == SSD1306_COLOR_WHITE)
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    }
    else
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

void SSD1306_GotoXY(ssd1306_t *structure, uint16_t x, uint16_t y)
{
    /* Set write pointers */
    structure->currentX = x;
    structure->currentY = y;
}

char SSD1306_Putc(ssd1306_t *structure, char ch, FontDef_t *Font, SSD1306_COLOR_t color)
{
    uint32_t i, b, j;

    /* Check available space in LCD */
    if (SSD1306_WIDTH <= (structure->currentX + Font->FontWidth) || SSD1306_HEIGHT <= (structure->currentY + Font->FontHeight))
    {
        /* Error */
        return 0;
    }

    /* Go through font */
    for (i = 0; i < Font->FontHeight; i++)
    {
        b = Font->data[(ch - 32) * Font->FontHeight + i];
        for (j = 0; j < Font->FontWidth; j++)
        {
            if ((b << j) & 0x8000)
            {
                SSD1306_DrawPixel(structure, structure->currentX + j, (structure->currentY + i), (SSD1306_COLOR_t)color);
            }
            else
            {
                SSD1306_DrawPixel(structure, structure->currentX + j, (structure->currentY + i), (SSD1306_COLOR_t)!color);
            }
        }
    }

    /* Increase pointer */
    structure->currentX += Font->FontWidth;

    /* Return character written */
    return ch;
}

char SSD1306_Puts(ssd1306_t *structure, char *str, FontDef_t *Font, SSD1306_COLOR_t color)
{
    /* Write characters */
    while (*str)
    {
        /* Write character by character */
        if (SSD1306_Putc(structure, *str, Font, color) != *str)
        {
            /* Return error */
            return *str;
        }

        /* Increase string pointer */
        str++;
    }

    /* Everything OK, zero should be returned */
    return *str;
}

void SSD1306_DrawLine(ssd1306_t *structure, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c)
{
    int16_t dx, dy, sx, sy, err, e2, i, tmp;

    /* Check for overflow */
    if (x0 >= SSD1306_WIDTH)
    {
        x0 = SSD1306_WIDTH - 1;
    }
    if (x1 >= SSD1306_WIDTH)
    {
        x1 = SSD1306_WIDTH - 1;
    }
    if (y0 >= SSD1306_HEIGHT)
    {
        y0 = SSD1306_HEIGHT - 1;
    }
    if (y1 >= SSD1306_HEIGHT)
    {
        y1 = SSD1306_HEIGHT - 1;
    }

    if (x0 < x1)
    {
        dx = x1 - x0;
    }
    else
    {
        dx = x0 - x1;
    }

    if (y0 < y1)
    {
        dy = y1 - y0;
    }
    else
    {
        dy = y0 - y1;
    }

    if (x0 < x1)
    {
        sx = 1;
    }
    else
    {
        sx = -1;
    }

    if (y0 < y1)
    {
        sy = 1;
    }
    else
    {
        sy = -1;
    }

    if (dx > dy)
    {
        err = dx / 2;
    }
    else
    {
        err = -dy / 2;
    }

    // dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
    // dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
    // sx = (x0 < x1) ? 1 : -1;
    // sy = (y0 < y1) ? 1 : -1;
    // err = ((dx > dy) ? dx : -dy) / 2;

    if (dx == 0)
    {
        if (y1 < y0)
        {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }

        if (x1 < x0)
        {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }

        /* Vertical line */
        for (i = y0; i <= y1; i++)
        {
            SSD1306_DrawPixel(structure, x0, i, c);
        }

        /* Return from function */
        return;
    }

    if (dy == 0)
    {
        if (y1 < y0)
        {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }

        if (x1 < x0)
        {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }

        /* Horizontal line */
        for (i = x0; i <= x1; i++)
        {
            SSD1306_DrawPixel(structure, i, y0, c);
        }

        /* Return from function */
        return;
    }

    while (1)
    {
        SSD1306_DrawPixel(structure, x0, y0, c);

        if (x0 == x1 && y0 == y1)
        {
            break;
        }
        e2 = err;

        if (e2 > -dx)
        {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dy)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void SSD1306_DrawRectangle(ssd1306_t *structure, uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c)
{
    /* Check input parameters */
    if (
        x >= SSD1306_WIDTH ||
        y >= SSD1306_HEIGHT)
    {
        /* Return error */
        return;
    }

    /* Check width and height */
    if ((x + w) >= SSD1306_WIDTH)
    {
        w = SSD1306_WIDTH - x;
    }
    if ((y + h) >= SSD1306_HEIGHT)
    {
        h = SSD1306_HEIGHT - y;
    }

    /* Draw 4 lines */
    SSD1306_DrawLine(structure, x, y, x + w, y, c);         /* Top line */
    SSD1306_DrawLine(structure, x, y + h, x + w, y + h, c); /* Bottom line */
    SSD1306_DrawLine(structure, x, y, x, y + h, c);         /* Left line */
    SSD1306_DrawLine(structure, x + w, y, x + w, y + h, c); /* Right line */
}

void SSD1306_DrawFilledRectangle(ssd1306_t *structure, uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c)
{
    uint8_t i;

    /* Check input parameters */
    if (
        x >= SSD1306_WIDTH ||
        y >= SSD1306_HEIGHT)
    {
        /* Return error */
        return;
    }

    /* Check width and height */
    if ((x + w) >= SSD1306_WIDTH)
    {
        w = SSD1306_WIDTH - x;
    }
    if ((y + h) >= SSD1306_HEIGHT)
    {
        h = SSD1306_HEIGHT - y;
    }

    /* Draw lines */
    for (i = 0; i <= h; i++)
    {
        /* Draw lines */
        SSD1306_DrawLine(structure, x, y + i, x + w, y + i, c);
    }
}

void SSD1306_DrawTriangle(ssd1306_t *structure, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color)
{
    /* Draw lines */
    SSD1306_DrawLine(structure, x1, y1, x2, y2, color);
    SSD1306_DrawLine(structure, x2, y2, x3, y3, color);
    SSD1306_DrawLine(structure, x3, y3, x1, y1, color);
}

void SSD1306_DrawFilledTriangle(ssd1306_t *structure, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;

    deltax = ABS(x2 - x1);
    deltay = ABS(y2 - y1);
    x = x1;
    y = y1;

    if (x2 >= x1)
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else
    {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1)
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else
    {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay)
    {
        xinc1 = 0;
        yinc2 = 0;
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;
    }
    else
    {
        xinc2 = 0;
        yinc1 = 0;
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;
    }

    for (curpixel = 0; curpixel <= numpixels; curpixel++)
    {
        SSD1306_DrawLine(structure, x, y, x3, y3, color);

        num += numadd;
        if (num >= den)
        {
            num -= den;
            x += xinc1;
            y += yinc1;
        }
        x += xinc2;
        y += yinc2;
    }
}

void SSD1306_DrawCircle(ssd1306_t *structure, int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    SSD1306_DrawPixel(structure, x0, y0 + r, c);
    SSD1306_DrawPixel(structure, x0, y0 - r, c);
    SSD1306_DrawPixel(structure, x0 + r, y0, c);
    SSD1306_DrawPixel(structure, x0 - r, y0, c);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1306_DrawPixel(structure, x0 + x, y0 + y, c);
        SSD1306_DrawPixel(structure, x0 - x, y0 + y, c);
        SSD1306_DrawPixel(structure, x0 + x, y0 - y, c);
        SSD1306_DrawPixel(structure, x0 - x, y0 - y, c);

        SSD1306_DrawPixel(structure, x0 + y, y0 + x, c);
        SSD1306_DrawPixel(structure, x0 - y, y0 + x, c);
        SSD1306_DrawPixel(structure, x0 + y, y0 - x, c);
        SSD1306_DrawPixel(structure, x0 - y, y0 - x, c);
    }
}

void SSD1306_DrawFilledCircle(ssd1306_t *structure, int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    SSD1306_DrawPixel(structure, x0, y0 + r, c);
    SSD1306_DrawPixel(structure, x0, y0 - r, c);
    SSD1306_DrawPixel(structure, x0 + r, y0, c);
    SSD1306_DrawPixel(structure, x0 - r, y0, c);
    SSD1306_DrawLine(structure, x0 - r, y0, x0 + r, y0, c);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1306_DrawLine(structure, x0 - x, y0 + y, x0 + x, y0 + y, c);
        SSD1306_DrawLine(structure, x0 + x, y0 - y, x0 - x, y0 - y, c);

        SSD1306_DrawLine(structure, x0 + y, y0 + x, x0 - y, y0 + x, c);
        SSD1306_DrawLine(structure, x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}

void SSD1306_Clear(ssd1306_t *structure)
{
    SSD1306_Fill(0);
    SSD1306_UpdateScreen(structure);
}
void SSD1306_ON(uint8_t address)
{
    SSD1306_WRITECOMMAND(address, 0x8D);
    SSD1306_WRITECOMMAND(address, 0x14);
    SSD1306_WRITECOMMAND(address, 0xAF);
}
void SSD1306_OFF(uint8_t address)
{
    SSD1306_WRITECOMMAND(address, 0x8D);
    SSD1306_WRITECOMMAND(address, 0x10);
    SSD1306_WRITECOMMAND(address, 0xAE);
}

void ssd1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t *data, uint16_t count)
{
    uint8_t dt[256];
    dt[0] = reg;
    uint8_t i;
    for (i = 0; i < count; i++)
        dt[i + 1] = data[i];
    // HAL_I2C_Master_Transmit(&hi2c1, address, dt, count+1, 10);
    I2C_Master_Transmit(I2C1, address, dt, count + 1, 10);
}

void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data)
{
    uint8_t dt[2];
    dt[0] = reg;
    dt[1] = data;
    //	HAL_I2C_Master_Transmit(&hi2c1, address, dt, 2, 10);
    I2C_Master_Transmit(I2C1, address, dt, 2, 10);
}
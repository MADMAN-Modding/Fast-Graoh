#include <math.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include <string.h>

double equation(double x)
{
    return x;
}

struct coordinate
{
    int x;
    int y;
};


int main(void)
{
    int x_min = 0;
    int x_max = 10;
    double delta = 0.5;

    int points = (x_max-x_min)/delta;

    struct coordinate coords[points];

    /* Initialize graphics drawing */
    gfx_Begin();

    gfx_SetColor(1);

    gfx_Line(GFX_LCD_WIDTH / 2, 0, GFX_LCD_WIDTH / 2, GFX_LCD_HEIGHT);
    gfx_Line(0, GFX_LCD_HEIGHT / 2, GFX_LCD_WIDTH, GFX_LCD_HEIGHT / 2);

    gfx_SetColor(3);
    gfx_Line(0,0, GFX_LCD_WIDTH, GFX_LCD_HEIGHT);

    for (int i = 0; i < points-1; i++)
    {
        struct coordinate coord1 = coords[i];
        struct coordinate coord2 = coords[i+1];


        gfx_Line(coord1.x, coord1.y, coord2.x, coord2.y);
    }
    

    /* Waits for a key */
    while (!os_GetCSC())
        ;

    /* End graphics drawing */
    gfx_End();

    return 0;
}

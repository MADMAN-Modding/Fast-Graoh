#include <math.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include <string.h>

double equation(double x)
{
    return x * x / 10;
}

struct coordinate
{
    double x;
    double y;
};

struct coordinate scale(double x, double y, int x_min, int y_min, int x_dif, int y_dif, int w, int h)
{
    struct coordinate result;
    result.x = (x - x_min) * ((double)w / x_dif);
    result.y = h - (y - y_min) * ((double)h / y_dif);

    return result;
}

int main(void)
{

    // Math definitions
    // X
    int x_min = -10;
    int x_max = 10;
    // Difs
    int x_dif = x_max - x_min;

    // Y
    int y_max = 10;
    int y_min = -0;
    // Difs
    int y_dif = y_max - y_min;

    // Dimensions
    int w = GFX_LCD_WIDTH;
    int h = GFX_LCD_HEIGHT;

    // Definition of the graph
    double delta = 2;

    int points = (x_max - x_min) / delta + 1;

    struct coordinate coords[points];

    /* Initialize graphics drawing */
    gfx_Begin();

    gfx_SetColor(1);

    // Axis determined by scale
    struct coordinate origin = scale(0, 0, x_min, y_min, x_dif, y_dif, w, h);
    gfx_Line(origin.x, 0, origin.x, GFX_LCD_HEIGHT);
    gfx_Line(0, origin.y, GFX_LCD_WIDTH, origin.y);

    gfx_SetColor(3);

    for (int i = 0; i < points; i++)
    {
        double x = x_min + (delta * i);

        double y = equation(x);

        struct coordinate coord = {x, y};

        coords[i] = coord;
    }

    for (int i = 0; i < points - 1; i++)
    {
        struct coordinate point1 = scale(coords[i].x, coords[i].y, x_min, y_min, x_dif, y_dif, w, h);
        struct coordinate point2 = scale(coords[i + 1].x, coords[i + 1].y, x_min, y_min, x_dif, y_dif, w, h);

        gfx_Line(point1.x, point1.y, point2.x, point2.y);
    }

    /* Waits for a key */
    while (!os_GetCSC())
        ;

    /* End graphics drawing */
    gfx_End();

    return 0;
}

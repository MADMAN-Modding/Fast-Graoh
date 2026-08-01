#include <math.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include "input.h"

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
    int x_min = -10;
    int x_max = 10;
    int x_dif = x_max - x_min;

    int y_max = 10;
    int y_min = -10;   // was -0 (== 0); fixed back to a real symmetric range
    int y_dif = y_max - y_min;

    int w = GFX_LCD_WIDTH;
    int h = GFX_LCD_HEIGHT;

    double delta = 2;
    int points = (x_max - x_min) / delta + 1;

    struct coordinate coords[points];

    uint8_t equations[MAX_EQUATIONS][MAX_EQ_LEN] = {0};
    size_t eq_lengths[MAX_EQUATIONS] = {0, 0, 0, 0};

    gfx_Begin();
    input_GetEquations(equations, eq_lengths);

    gfx_SetColor(1);
    gfx_FillScreen(255);

    struct coordinate origin = scale(0, 0, x_min, y_min, x_dif, y_dif, w, h);
    gfx_Line(origin.x, 0, origin.x, GFX_LCD_HEIGHT);
    gfx_Line(0, origin.y, GFX_LCD_WIDTH, origin.y);

    // Plot each non-empty equation the user typed, in a different color
    uint8_t plot_colors[MAX_EQUATIONS] = {3, 4, 5, 6}; // pick colors that exist in your palette
    for (int row = 0; row < MAX_EQUATIONS; row++)
    {
        if (eq_lengths[row] == 0)
            continue; // skip empty y-rows, just like a real Y= editor

        gfx_SetColor(plot_colors[row]);

        for (int i = 0; i < points; i++)
        {
            double x = x_min + (delta * i);
            double y = eval_tokenized_equation(equations[row], eq_lengths[row], x);
            coords[i] = (struct coordinate){x, y};
        }

        for (int i = 0; i < points - 1; i++)
        {
            // Skip segments where evaluation failed (NAN), e.g. domain errors
            if (isnan(coords[i].y) || isnan(coords[i + 1].y))
                continue;

            struct coordinate point1 = scale(coords[i].x, coords[i].y, x_min, y_min, x_dif, y_dif, w, h);
            struct coordinate point2 = scale(coords[i + 1].x, coords[i + 1].y, x_min, y_min, x_dif, y_dif, w, h);

            gfx_Line(point1.x, point1.y, point2.x, point2.y);
        }
    }

    while (!os_GetCSC())
        ;

    gfx_End();

    return 0;
}
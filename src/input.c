#include <graphx.h>
#include <ti/vars.h>
#include <ti/tokens.h>
#include <ti/getcsc.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "input.h"

// Inserts the token(s) for a keypress at byte offset `byte_pos` in row `row`.
// Returns the number of bytes inserted (0, 1, or 2).
static int insert_key_token(uint8_t equations[MAX_EQUATIONS][MAX_EQ_LEN],
                             size_t eq_lengths[MAX_EQUATIONS],
                             int row, size_t byte_pos, int key)
{
    size_t len = eq_lengths[row];
    uint8_t bytes[2];
    int n = 0;

    switch (key)
    {
        case sk_0: bytes[0] = OS_TOK_0; n = 1; break;
        case sk_1: bytes[0] = OS_TOK_1; n = 1; break;
        case sk_2: bytes[0] = OS_TOK_2; n = 1; break;
        case sk_3: bytes[0] = OS_TOK_3; n = 1; break;
        case sk_4: bytes[0] = OS_TOK_4; n = 1; break;
        case sk_5: bytes[0] = OS_TOK_5; n = 1; break;
        case sk_6: bytes[0] = OS_TOK_6; n = 1; break;
        case sk_7: bytes[0] = OS_TOK_7; n = 1; break;
        case sk_8: bytes[0] = OS_TOK_8; n = 1; break;
        case sk_9: bytes[0] = OS_TOK_9; n = 1; break;
        case sk_DecPnt: bytes[0] = OS_TOK_DECIMAL_POINT; n = 1; break;
        case sk_Add:    bytes[0] = OS_TOK_ADD; n = 1; break;
        case sk_Sub:    bytes[0] = OS_TOK_SUBTRACT; n = 1; break;
        case sk_Mul:    bytes[0] = OS_TOK_MULTIPLY; n = 1; break;
        case sk_Div:    bytes[0] = OS_TOK_DIVIDE; n = 1; break;
        case sk_Power:  bytes[0] = OS_TOK_POWER; n = 1; break;
        case sk_LParen: bytes[0] = OS_TOK_LEFT_PAREN; n = 1; break;
        case sk_RParen: bytes[0] = OS_TOK_RIGHT_PAREN; n = 1; break;
        case sk_Chs:    bytes[0] = OS_TOK_NEGATIVE; n = 1; break;
        case sk_GraphVar:
            bytes[0] = OS_TOK_2BYTE;
            bytes[1] = OS_TOK_OTHER_X;
            n = 2;
            break;
        default:
            n = 0;
            break;
    }

    if (n == 0 || len + n > MAX_EQ_LEN)
        return 0;

    memmove(&equations[row][byte_pos + n],
            &equations[row][byte_pos],
            len - byte_pos);

    for (int i = 0; i < n; i++)
        equations[row][byte_pos + i] = bytes[i];

    eq_lengths[row] += n;
    return n;
}

// Converts the token at buf[pos] into a printable char, writing into out[0]
// (out must hold at least 2 chars). Returns bytes consumed (1 or 2).
static int token_to_display(const uint8_t *buf, size_t len, size_t pos, char *out)
{
    uint8_t b = buf[pos];

    if (b == OS_TOK_2BYTE && pos + 1 < len)
    {
        uint8_t b2 = buf[pos + 1];
        out[0] = (b2 == OS_TOK_OTHER_X) ? 'x' : '?';
        out[1] = '\0';
        return 2;
    }

    switch (b)
    {
        case OS_TOK_0: out[0] = '0'; break;
        case OS_TOK_1: out[0] = '1'; break;
        case OS_TOK_2: out[0] = '2'; break;
        case OS_TOK_3: out[0] = '3'; break;
        case OS_TOK_4: out[0] = '4'; break;
        case OS_TOK_5: out[0] = '5'; break;
        case OS_TOK_6: out[0] = '6'; break;
        case OS_TOK_7: out[0] = '7'; break;
        case OS_TOK_8: out[0] = '8'; break;
        case OS_TOK_9: out[0] = '9'; break;
        case OS_TOK_DECIMAL_POINT: out[0] = '.'; break;
        case OS_TOK_ADD: out[0] = '+'; break;
        case OS_TOK_SUBTRACT: out[0] = '-'; break;
        case OS_TOK_MULTIPLY: out[0] = '*'; break;
        case OS_TOK_DIVIDE: out[0] = '/'; break;
        case OS_TOK_POWER: out[0] = '^'; break;
        case OS_TOK_LEFT_PAREN: out[0] = '('; break;
        case OS_TOK_RIGHT_PAREN: out[0] = ')'; break;
        case OS_TOK_NEGATIVE: out[0] = '-'; break;
        default: out[0] = '?'; break;
    }
    out[1] = '\0';
    return 1;
}

// Returns the byte size (1 or 2) of the token starting at buf[pos].
static int token_byte_size(const uint8_t *buf, size_t len, size_t pos)
{
    if (buf[pos] == OS_TOK_2BYTE && pos + 1 < len)
        return 2;
    return 1;
}

// Converts a logical cursor position (count of visible tokens from the
// start) into a byte offset into the row's buffer.
static size_t logical_to_byte(const uint8_t *buf, size_t len, int logical_pos)
{
    size_t byte_pos = 0;
    int tokens_seen = 0;
    while (byte_pos < len && tokens_seen < logical_pos)
    {
        byte_pos += token_byte_size(buf, len, byte_pos);
        tokens_seen++;
    }
    return byte_pos;
}

// Counts how many logical tokens (visible characters) are in buf[0..len).
static int logical_length(const uint8_t *buf, size_t len)
{
    size_t byte_pos = 0;
    int count = 0;
    while (byte_pos < len)
    {
        byte_pos += token_byte_size(buf, len, byte_pos);
        count++;
    }
    return count;
}

static void draw_editor(uint8_t equations[MAX_EQUATIONS][MAX_EQ_LEN],
                         size_t eq_lengths[MAX_EQUATIONS],
                         int active_row, int cursor_pos)
{
    gfx_FillScreen(255);
    gfx_SetTextFGColor(0);

    for (int i = 0; i < MAX_EQUATIONS; i++)
    {
        int y = 10 + i * 20;

        char label[4];
        sprintf(label, "y%d=", i + 1);
        gfx_PrintStringXY(label, 5, y);

        char display[MAX_EQ_LEN + 1];
        size_t out_pos = 0;
        size_t byte_pos = 0;
        size_t len = eq_lengths[i];
        while (byte_pos < len)
        {
            char piece[2];
            int consumed = token_to_display(equations[i], len, byte_pos, piece);
            display[out_pos++] = piece[0];
            byte_pos += consumed;
        }
        display[out_pos] = '\0';

        gfx_PrintStringXY(display, 25, y);
    }

    // Cursor x-position: render active row up to the logical cursor position
    char prefix[MAX_EQ_LEN + 1];
    size_t out_pos = 0;
    size_t byte_pos = 0;
    int tokens_seen = 0;
    size_t len = eq_lengths[active_row];
    while (byte_pos < len && tokens_seen < cursor_pos)
    {
        char piece[2];
        int consumed = token_to_display(equations[active_row], len, byte_pos, piece);
        prefix[out_pos++] = piece[0];
        byte_pos += consumed;
        tokens_seen++;
    }
    prefix[out_pos] = '\0';

    int cursor_x = 25 + gfx_GetStringWidth(prefix);
    int cursor_y = 10 + active_row * 20;

    gfx_SetColor(0);
    gfx_Line(cursor_x, cursor_y, cursor_x, cursor_y + 8);
}

void input_GetEquations(uint8_t equations[MAX_EQUATIONS][MAX_EQ_LEN],
                         size_t eq_lengths[MAX_EQUATIONS])
{
    int active_row = 0;
    int cursor_pos = logical_length(equations[active_row], eq_lengths[active_row]);
    int key;

    do
    {
        draw_editor(equations, eq_lengths, active_row, cursor_pos);
        key = os_GetCSC();

        int llen = logical_length(equations[active_row], eq_lengths[active_row]);

        switch (key)
        {
            case sk_Up:
                if (active_row > 0)
                {
                    active_row--;
                    cursor_pos = logical_length(equations[active_row], eq_lengths[active_row]);
                }
                break;

            case sk_Down:
                if (active_row < MAX_EQUATIONS - 1)
                {
                    active_row++;
                    cursor_pos = logical_length(equations[active_row], eq_lengths[active_row]);
                }
                break;

            case sk_Left:
                if (cursor_pos > 0) cursor_pos--;
                break;

            case sk_Right:
                if (cursor_pos < llen) cursor_pos++;
                break;

            case sk_Del:
                if (cursor_pos > 0)
                {
                    size_t len = eq_lengths[active_row];
                    size_t byte_pos = logical_to_byte(equations[active_row], len, cursor_pos - 1);
                    int tok_size = token_byte_size(equations[active_row], len, byte_pos);

                    memmove(&equations[active_row][byte_pos],
                            &equations[active_row][byte_pos + tok_size],
                            len - byte_pos - tok_size);

                    eq_lengths[active_row] -= tok_size;
                    cursor_pos--;
                }
                break;

            case sk_Clear:
                eq_lengths[active_row] = 0;
                cursor_pos = 0;
                break;

            case sk_Enter:
                if (active_row < MAX_EQUATIONS - 1)
                {
                    active_row++;
                    cursor_pos = logical_length(equations[active_row], eq_lengths[active_row]);
                }
                break;

            default:
            {
                size_t byte_pos = logical_to_byte(equations[active_row], eq_lengths[active_row], cursor_pos);
                int inserted = insert_key_token(equations, eq_lengths, active_row, byte_pos, key);
                if (inserted > 0) cursor_pos++;
                break;
            }
        }

    } while (key != sk_Graph);
}

// Evaluate a tokenized equation buffer at a given x value, returns the y value.
double eval_tokenized_equation(const uint8_t *eq_tokens, size_t eq_len, double x)
{
    real_t x_real = os_FloatToReal((float)x);
    os_SetRealVar(OS_VAR_X, &x_real);

    if (os_Eval(eq_tokens, eq_len) != 0)
        return NAN;

    uint8_t type;
    real_t *ans = (real_t *)os_GetAnsData(&type);
    return (double)os_RealToFloat(ans);
}
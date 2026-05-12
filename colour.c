#include "color.h"
#include "colour.h"

void colour_set_foreground(char *color)
{
    setForeground(color);
}

void colour_set_background(char *color)
{
    setBackground(color);
}

void colour_reset(void)
{
    setForeground("reset");
    setBackground("reset");
}
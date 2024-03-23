#include "button.h"
#include "tool.h"

button::button(int Left, int Top, int Right, int Botton)
    : Left(Left), Right(Right), Top(Top), Botton(Botton)
{
}

bool button::is_click(int x, int y, bool leftButton)
{
    return leftButton && x >= Left && x <= Right &&
        y >= Top && y <= Botton;
}

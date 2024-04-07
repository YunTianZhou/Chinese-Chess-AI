#pragma once
#include "Headers.h"

class Button
{
protected:
    int x;
    int y;
    int width;
    int height;
    static std::tuple<bool, int, int> last_mouse_event;
    static std::tuple<bool, int, int> curr_mouse_event;
public:
    Button(int x, int y, int width, int height);
    virtual bool click_listen();
    static void update_mouse_message();
    static bool is_collision(int x, int y, int width, int height, int mx, int my);
};

using MouseControl = Button;
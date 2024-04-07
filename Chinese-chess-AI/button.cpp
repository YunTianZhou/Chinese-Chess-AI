#include "Button.h"
#include "tool.h"

std::tuple<bool, int, int> Button::last_mouse_event = std::make_tuple(false, 0, 0);
std::tuple<bool, int, int> Button::curr_mouse_event = std::make_tuple(false, 0, 0);

Button::Button(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

bool Button::click_listen() {
    if (std::get<0>(curr_mouse_event)
        && !std::get<0>(last_mouse_event) &&
        is_collision(x, y, width, height, std::get<1>(curr_mouse_event), std::get<2>(curr_mouse_event))) {
        return true;
    }
    return false;
}

void Button::update_mouse_message() {
    last_mouse_event = curr_mouse_event;
    int x, y;
    bool lbutton;
    tool::getMouseMessage(x, y, lbutton);
    curr_mouse_event = std::make_tuple(lbutton, x, y);
}

bool Button::is_collision(int x, int y, int width, int height, int mx, int my) {
    return mx >= x && mx <= x + width && my >= y && my <= y + height;
}

#include "slider.h"

slider::slider(int button_y, int button_width, int button_height, int slider_x, int slider_length, size_t* var, const std::vector<size_t>* values)
    : Button(0, button_y, button_width, button_height), slider_x(slider_x), 
    slider_length(slider_length), var(var), values(values), sliding(false), tile(0), position(0)
{
    tile = (float)(slider_length / (values->size() - 1));
    for (int i = 0; i < values->size(); i++)
    {
        if (values->at(i) == *var)
        {
            position = i;
            break;
        }
    }
}

bool slider::listen()
{
    if (sliding)
    {
        if (!std::get<0>(curr_mouse_event))
        {
            sliding = false;
            return false;
        }
        int mx = min(max(std::get<1>(curr_mouse_event), slider_x), slider_x + slider_length);
        int new_position = (int)std::round((float)(mx - slider_x) / tile);
        if (position != new_position)
        {
            *var = values->at(new_position);
            position = new_position;
            return true;
        }
    }
    else
    {
        if (std::get<0>(curr_mouse_event) && !std::get<0>(last_mouse_event) &&
            is_collision(slider_x + (int)(tile * position) - width / 2, y - height / 2, width, height,
                std::get<1>(curr_mouse_event), std::get<2>(curr_mouse_event))) {
            sliding = true;
        }
    }
    return false;
}

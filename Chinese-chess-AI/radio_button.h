#pragma once
#include "button.h"

class radio_button : public Button
{
private:
    size_t SelfVar;
    size_t* Var;
public:
    radio_button(int x, int y, int width, int height, size_t SelfVar, size_t* Var);
    bool click_listen();
};

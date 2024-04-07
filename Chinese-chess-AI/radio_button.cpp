#include "radio_button.h"

radio_button::radio_button(int x, int y, int width, int height, size_t SelfVar, size_t* Var)
	: Button(x, y, width, height), SelfVar(SelfVar), Var(Var)
{

}

bool radio_button::click_listen()
{
	if (*Var != SelfVar && std::get<0>(curr_mouse_event) && 
		is_collision(x, y, width, height, std::get<1>(curr_mouse_event), std::get<2>(curr_mouse_event)))
	{
		*Var = SelfVar;
		return true;
	}
	return false;
}

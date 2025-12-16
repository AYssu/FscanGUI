#include "ImGuiInPut.h"

void ImGuiInPut::InputTouchEvent(int event_get_action, float x, float y)
{
    switch (event_get_action)
	{
    	case eTouchEvent::TOUCH_OUTSIDE:
        	Io->MouseDown[0] = false;
		break;
    	case eTouchEvent::TOUCH_DOWN:
        	Io->MousePos.x = x;
        	Io->MousePos.y = y;
        	Io->MouseDown[0] = true;
        break;
    	case eTouchEvent::TOUCH_UP:
        	Io->MouseDown[0] = false;
        break;
    	case eTouchEvent::TOUCH_MOVE:
        	Io->MousePos.x = x;
        	Io->MousePos.y = y;
        break;
    	default:
        break;
    }

}

void ImGuiInPut::SetImGuiIo(ImGuiIO * Io)
{
	this->Io = Io;
}

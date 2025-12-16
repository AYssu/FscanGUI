#ifndef IMGUIINPUT_H
#define IMGUIINPUT_H
#include <mutex>
#include "imgui.h"
#include "imgui_internal.h"
#include <jni.h>
#include <unistd.h>
#include <string>
#include <cstring>
using namespace std;
class ImGuiInPut {
	enum eTouchEvent {
		TOUCH_DOWN,
		TOUCH_UP,
		TOUCH_MOVE,
		TOUCH_CANCEL,
		TOUCH_OUTSIDE
	};
	ImGuiIO* Io = nullptr;
public:
    float winWidth;
	float winHeith;
	void SetImGuiIo(ImGuiIO *Io);
	void InputTouchEvent(int event_get_action, float x, float y);
};

#endif

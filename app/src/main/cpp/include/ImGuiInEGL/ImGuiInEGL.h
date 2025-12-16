#pragma once
#ifndef IMGUIINEGL_H
#define IMGUIINEGL_H
#include <jni.h>
#include <sched.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>
#include <random>
#include <linux/types.h>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <unordered_map>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <thread>
#include <array>
#include <android/native_window_jni.h>
#include "imgui.h"
#include "ImGuiInPut.h"
#include "imgui_internal.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
class ImGuiInEGL {
public:
    std::unique_ptr<char[]> temp_ptr = std::make_unique<char[]>(888);
    int Width,Height;
private:

    int InItEGL();

    int InItImGui();

    void EGLThread();

    ImGuiInPut *InPut;

    int SwapBuffers();

    void CleaBuffers();


    EGLConfig EGLConfig;
    EGLDisplay EglDisplay;
    EGLSurface EglSurface;
    EGLContext EglContext;


    void ImGuiMainWinEnded();

    void ImGuiMainWinStart();

    EGLNativeWindowType SurfaceWin;


public:

    ImFont *font = NULL;

    JavaVM *g_jvm = NULL;
    jobject g_obj = NULL;

    ImGuiInEGL();

    bool isChange = false;
    ImGuiIO *Io = nullptr;

    ImGuiStyle *Style = nullptr;
    ImDrawList *ImDraw = nullptr;
    ImGuiWindow *Window = nullptr;
    ImGuiContext *Context = nullptr;

    void SetInPut(ImGuiInPut *InPut_);

    void onSurfaceChange(int Width, int Height);

    void SetSaveMessage(string &SaveDir_);

    void onSurfaceCreate(JNIEnv *env, jobject Surface, int Width, int Height);

    void onSurfaceDestroy();

};

#endif

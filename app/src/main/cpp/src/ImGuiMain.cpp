#include "ImGuiInEGL.h"
#include <jni.h>
#include <string>
#include <sys/ptrace.h>
#include <android/asset_manager.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <android/asset_manager_jni.h>
#include <ctime>
#include <dirent.h>
#include <sys/uio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <cmath>
#include <iostream>
#include <thread>
#include <android/log.h>
ImGuiInPut *InPut = new ImGuiInPut;
ImGuiInEGL *InEGL = new ImGuiInEGL;


jint JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env;
    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    assert(env != NULL);
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ayssu_fscangui_utils_JNIInterface_MotionEventClick(JNIEnv *env, jclass clazz, jint event_get_action, jfloat pos_x, jfloat pos_y) {
    // TODO: implement MotionEventClick()
    InPut->InputTouchEvent(event_get_action, pos_x, pos_y);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ayssu_fscangui_utils_JNIInterface_SurfaceCreate(JNIEnv *env, jclass clazz, jobject surface,jint width, jint height) {
    // TODO: implement SurfaceCreate()
    InEGL->onSurfaceCreate(env, surface, width, height);
    InEGL->SetInPut(InPut);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ayssu_fscangui_utils_JNIInterface_SurfaceChange(JNIEnv *env, jclass clazz, jint width, jint height) {
    // TODO: implement SurfaceChange()
    InEGL->onSurfaceChange(width, height);
}

float winData[4];
extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_ayssu_fscangui_utils_JNIInterface_GetImGuiWinSize(JNIEnv *env, jclass clazz) {
    // TODO: implement GetImGuiwinsize()
    jfloatArray newFloatArray = env->NewFloatArray(4);
    if (InEGL->Window ) {
        winData[0] = InEGL->Window->Pos.x;
        winData[1] = InEGL->Window->Pos.y;
        winData[2] = InEGL->Window->Size.x;
        winData[3] = InEGL->Window->Size.y;
        env->SetFloatArrayRegion(newFloatArray, 0, 4, winData);
        env->ReleaseFloatArrayElements(newFloatArray, winData, JNI_COMMIT);
        return newFloatArray;
    }
    return newFloatArray;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ayssu_fscangui_utils_JNIInterface_SurfaceDestroyed(JNIEnv *env, jclass clazz) {
    // TODO: implement SurfaceDestroyed()
    if (InEGL != nullptr) {
        InEGL->onSurfaceDestroy();
    }
}



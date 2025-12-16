package com.ayssu.fscangui.utils

import android.view.Surface

object JNIInterface {
    init {
        System.loadLibrary("ImGui")
    }

    external fun MotionEventClick(eventAction: Int, posX: Float, posY: Float)
    external fun SurfaceCreate(surface: Surface, width: Int, height: Int)
    external fun SurfaceChange(width: Int, height: Int)
    external fun GetImGuiWinSize(): FloatArray
    external fun SurfaceDestroyed()
}


package com.ayssu.fscangui.view

import android.content.Context
import android.graphics.SurfaceTexture
import android.util.AttributeSet
import android.view.Surface
import android.view.TextureView
import com.ayssu.fscangui.utils.JNIInterface

class SurfaceView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : TextureView(context, attrs, defStyleAttr), TextureView.SurfaceTextureListener {

    private var touch: TouchView? = null

    init {
        surfaceTextureListener = this
        touch = TouchView(context).apply { initView() }
    }

    override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
        // 直接使用回调提供的宽高，避免 getWidth()/getHeight() 为 0 导致 EGL 视口为 0
        JNIInterface.SurfaceCreate(Surface(surface), width, height)
    }

    override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
        JNIInterface.SurfaceDestroyed()
        return false
    }

    override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {
        JNIInterface.SurfaceChange(width, height)
    }

    override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {
        // no-op
    }
}


package com.ayssu.fscangui.service

import android.app.Service
import android.content.Intent
import android.content.res.Configuration
import android.graphics.PixelFormat
import android.os.Build
import android.os.IBinder
import android.util.DisplayMetrics
import android.util.Log
import android.view.Gravity
import android.view.WindowManager
import com.ayssu.fscangui.view.SurfaceView

class FloatService : Service() {

    override fun onBind(intent: Intent?): IBinder? = null

    override fun onCreate() {
        super.onCreate()
        instance = this
        if (manager == null) {
            manager = getSystemService(WINDOW_SERVICE) as WindowManager
        }
        setSurfaceView()
        Log.d(TAG, "onCreate")
    }

    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        manager?.defaultDisplay?.getRealMetrics(dm)
        if (mSurfaceViewParams != null && mySurfaceView != null) {
            mSurfaceViewParams?.width = dm.widthPixels
            mSurfaceViewParams?.height = dm.heightPixels
            manager?.updateViewLayout(mySurfaceView, mSurfaceViewParams)
        }
    }

    companion object {
        private const val TAG = "FscanGUI-FloatService"
        var instance: FloatService? = null
        var manager: WindowManager? = null
        var mySurfaceView: SurfaceView? = null
        var mSurfaceViewIo: Boolean = false
        val dm: DisplayMetrics = DisplayMetrics()
        var mSurfaceViewParams: WindowManager.LayoutParams? = null

        fun setSurfaceView() {
            val ctx = instance ?: return
            mySurfaceView = SurfaceView(ctx)
            mSurfaceViewParams = getAttributes(false).apply {
                type = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY
                } else {
                    WindowManager.LayoutParams.TYPE_PHONE
                }
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                    layoutInDisplayCutoutMode =
                        WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES
                }
                alpha = 0.8f
                format = PixelFormat.RGBA_8888
                width = WindowManager.LayoutParams.MATCH_PARENT
                height = WindowManager.LayoutParams.MATCH_PARENT
                flags = flags or WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE or
                        WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE or
                        WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL or
                        WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED or
                        WindowManager.LayoutParams.FLAG_FULLSCREEN or
                        WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS or
                        WindowManager.LayoutParams.FLAG_LAYOUT_ATTACHED_IN_DECOR or
                        WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN
            }
            manager?.let { wm ->
                mSurfaceViewIo = true
                mSurfaceViewParams?.let { params ->
                    wm.addView(mySurfaceView, params)
                }
            }
        }

        fun getAttributes(isWindow: Boolean): WindowManager.LayoutParams {
            return WindowManager.LayoutParams().apply {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    type = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY
                }
                flags = WindowManager.LayoutParams.FLAG_FULLSCREEN or
                        WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS or
                        WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION or
                        WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE or
                        WindowManager.LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH or
                        WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN
                if (isWindow) {
                    flags = flags or
                            WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL or
                            WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE
                }
                format = PixelFormat.RGBA_8888
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                    layoutInDisplayCutoutMode =
                        WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES
                }
                gravity = Gravity.LEFT or Gravity.TOP
                x = 0
                y = 0
                width = if (isWindow) WindowManager.LayoutParams.MATCH_PARENT else 0
                height = if (isWindow) WindowManager.LayoutParams.MATCH_PARENT else 0
            }
        }
    }
}


package com.ayssu.fscangui.view

import android.content.Context
import android.graphics.PixelFormat
import android.os.Build
import android.os.Handler
import android.os.Looper
import android.util.AttributeSet
import android.view.Gravity
import android.view.MotionEvent
import android.view.View
import android.view.WindowManager
import android.view.inputmethod.InputMethodManager
import com.ayssu.fscangui.service.FloatService
import com.ayssu.fscangui.utils.JNIInterface

class TouchView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0,
    defStyleRes: Int = 0
) : View(context, attrs, defStyleAttr, defStyleRes) {

    private var isUpdating = true
    private val handler = Handler(Looper.getMainLooper())

    init {
        mtouch = this
        manager = context.getSystemService(Context.INPUT_METHOD_SERVICE) as? InputMethodManager
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        JNIInterface.MotionEventClick(event.action, event.rawX, event.rawY)
        return false
    }

    fun initView() {
        mtouchParams = getAttributes(false)
        FloatService.manager?.addView(this, mtouchParams)
        updateTouchWinSize()
    }

    fun stopUpdate() {
        isUpdating = false
        handler.removeCallbacksAndMessages(null)
    }

    private fun updateTouchWinSize() {
        handler.postDelayed(object : Runnable {
            override fun run() {
                if (!isUpdating) return
                try {
                    val rect = JNIInterface.GetImGuiWinSize()
                    mtouchParams?.let { lp ->
                        lp.x = rect[0].toInt()
                        lp.y = rect[1].toInt()
                        lp.width = rect[2].toInt()
                        lp.height = rect[3].toInt()
                        FloatService.manager?.updateViewLayout(mtouch, lp)
                    }
                } catch (_: Exception) {
                } finally {
                    handler.postDelayed(this, DELAY_INTERVAL.toLong())
                }
            }
        }, UPDATE_INTERVAL.toLong())
    }

    companion object {
        private const val UPDATE_INTERVAL = 2000
        private const val DELAY_INTERVAL = 100
        var mtouch: View? = null
        var manager: InputMethodManager? = null
        var mtouchParams: WindowManager.LayoutParams? = null

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


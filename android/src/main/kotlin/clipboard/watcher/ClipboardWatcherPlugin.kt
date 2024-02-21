package clipboard.watcher

import android.app.Activity
import android.app.Application
import android.content.ClipboardManager
import android.content.Context
import android.os.Build
import android.os.Bundle
import android.view.View
import androidx.core.view.doOnLayout
import io.flutter.embedding.engine.plugins.FlutterPlugin
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import io.flutter.plugin.common.MethodChannel.Result
import io.flutter.plugin.common.MethodChannel.MethodCallHandler

class ClipboardWatcherPlugin : FlutterPlugin,
    MethodCallHandler,
    ClipboardManager.OnPrimaryClipChangedListener,
    Application.ActivityLifecycleCallbacks {

    private lateinit var context: Application
    private lateinit var channel: MethodChannel
    private var started: Boolean = false
    private var lastClipboardContent: String? = null

    override fun onAttachedToEngine(flutterPluginBinding: FlutterPlugin.FlutterPluginBinding) {
        context = flutterPluginBinding.applicationContext as Application
        channel = MethodChannel(flutterPluginBinding.binaryMessenger, "clipboard_watcher")
        channel.setMethodCallHandler(this)
    }

    override fun onMethodCall(call: MethodCall, result: Result) {
        when (val method = call.method) {
            "start" -> start(result)
            "stop" -> stop(result)
            else -> result.notImplemented()
        }
    }

    override fun onDetachedFromEngine(binding: FlutterPlugin.FlutterPluginBinding) {
        channel.setMethodCallHandler(null)
    }

    override fun onPrimaryClipChanged() {
        checkClipboard()
    }

    override fun onActivityCreated(activity: Activity, bundle: Bundle?) {
    }

    override fun onActivityStarted(activity: Activity) {
    }

    override fun onActivityResumed(activity: Activity) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            activity.findViewById<View>(android.R.id.content)?.doOnLayout {
                checkClipboard()
            }
        } else {
            checkClipboard()
        }
    }

    override fun onActivityPaused(activity: Activity) {
    }

    override fun onActivityStopped(activity: Activity) {
    }

    override fun onActivitySaveInstanceState(activity: Activity, bundle: Bundle) {
    }

    override fun onActivityDestroyed(activity: Activity) {
    }

    private fun start(result: Result) {
        if (started) {
            result.success(null)
            return
        }

        started = true
        val clipboardManager = context.getSystemService(Context.CLIPBOARD_SERVICE) as ClipboardManager
        clipboardManager.addPrimaryClipChangedListener(this)
        context.registerActivityLifecycleCallbacks(this)
        result.success(null)
    }

    private fun stop(result: Result) {
        if (!started) {
            result.success(null)
            return
        }

        started = false
        val clipboardManager = context.getSystemService(Context.CLIPBOARD_SERVICE) as ClipboardManager
        clipboardManager.removePrimaryClipChangedListener(this)
        context.unregisterActivityLifecycleCallbacks(this)
        result.success(null)
    }

    private fun checkClipboard() {
        if (!started) {
            return
        }
        val clipboardManager = context.getSystemService(Context.CLIPBOARD_SERVICE) as ClipboardManager
        val clipData = clipboardManager.primaryClip
        if (clipData != null && clipData.itemCount > 0) {
            val currentClipboardContent = clipData.getItemAt(0).text?.toString()
            if (currentClipboardContent != lastClipboardContent) {
                lastClipboardContent = currentClipboardContent
                channel.invokeMethod("onClipboardChanged", emptyMap<Any, Any>())
            }
        }
    }
}

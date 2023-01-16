import Flutter
import UIKit

public class SwiftClipboardWatcherPlugin: NSObject, FlutterPlugin {
    private var channel: FlutterMethodChannel!
    private var started: Bool = false
    private var lastClipboardContent: String?

    public static func register(with registrar: FlutterPluginRegistrar) {
        let channel = FlutterMethodChannel(name: "clipboard_watcher", binaryMessenger: registrar.messenger())
        let instance = SwiftClipboardWatcherPlugin()
        instance.channel = channel
        registrar.addMethodCallDelegate(instance, channel: channel)
    }

    public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        switch call.method {
        case "start":
            start(call, result: result)
            return
        case "stop":
            stop(call, result: result)
            return
        default:
            result(FlutterMethodNotImplemented)
        }
    }

    public func start(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        if started {
            result(true)
            return
        }

        started = true
        let notificationCenter = NotificationCenter.default
        // Check for changes in the pasteboard (works only on app's context)
        notificationCenter.addObserver(
            self,
            selector: #selector(checkPasteboard),
            name: UIPasteboard.changedNotification,
            object: nil)
        // Check the paste board after a foreground event as the paste board
        // content may have changed while the app was in the background.
        notificationCenter.addObserver(
            self,
            selector: #selector(checkPasteboard),
            name: UIApplication.willEnterForegroundNotification,
            object: nil)

        checkPasteboard()
        result(true)
    }

    public func stop(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        if !started {
            result(true)
            return
        }

        started = false
        let notificationCenter = NotificationCenter.default
        notificationCenter.removeObserver(
            self,
            name: UIPasteboard.changedNotification,
            object: nil)
        notificationCenter.removeObserver(
            self,
            name: UIApplication.willEnterForegroundNotification,
            object: nil)
        result(true)
    }

    @objc func checkPasteboard() {
        if !started {
            return
        }
        let currentClipboardContent = UIPasteboard.general.string
        if currentClipboardContent != lastClipboardContent {
            let args: NSDictionary = [:]
            channel.invokeMethod("onClipboardChanged", arguments: args, result: nil)
            lastClipboardContent = currentClipboardContent
        }
    }
}

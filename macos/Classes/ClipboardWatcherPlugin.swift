import Cocoa
import FlutterMacOS

public class ClipboardWatcherPlugin: NSObject, FlutterPlugin {
    private var channel: FlutterMethodChannel!

    private let pasteboard = NSPasteboard.general
    private var changeCount: Int = -1
    
    private var timer: Timer?
    
    public static func register(with registrar: FlutterPluginRegistrar) {
        let channel = FlutterMethodChannel(name: "clipboard_watcher", binaryMessenger: registrar.messenger)
        let instance = ClipboardWatcherPlugin()
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
        timer = Timer.scheduledTimer(timeInterval: 2, target: self, selector: #selector(self.checkForChangesInPasteboard), userInfo: nil, repeats: true)
        result(true)
    }
    
    public func stop(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        timer?.invalidate()
        timer = nil
        result(true)
    }
    
    @objc private func checkForChangesInPasteboard() {
        if (pasteboard.changeCount != changeCount) {
            let args: NSDictionary = [:]
            channel.invokeMethod("onClipboardChanged", arguments: args, result: nil)
            changeCount = pasteboard.changeCount;
        }
    }
}

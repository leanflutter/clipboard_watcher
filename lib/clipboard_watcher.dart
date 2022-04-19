
import 'dart:async';

import 'package:flutter/services.dart';

class ClipboardWatcher {
  static const MethodChannel _channel = MethodChannel('clipboard_watcher');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}

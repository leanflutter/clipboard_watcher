import 'dart:async';

import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'clipboard_listener.dart';

class ClipboardWatcher {
  ClipboardWatcher._() {
    _channel.setMethodCallHandler(_methodCallHandler);
  }

  /// The shared instance of [ClipboardWatcher].
  static final ClipboardWatcher instance = ClipboardWatcher._();

  final MethodChannel _channel = const MethodChannel('clipboard_watcher');

  final ObserverList<ClipboardListener> _listeners =
      ObserverList<ClipboardListener>();

  Future<void> _methodCallHandler(MethodCall call) async {
    for (final ClipboardListener listener in listeners) {
      if (!_listeners.contains(listener)) {
        return;
      }

      if (call.method == 'onClipboardChanged') {
        listener.onClipboardChanged();
      } else {
        throw UnimplementedError();
      }
    }
  }

  List<ClipboardListener> get listeners {
    final List<ClipboardListener> localListeners =
        List<ClipboardListener>.from(_listeners);
    return localListeners;
  }

  bool get hasListeners {
    return _listeners.isNotEmpty;
  }

  void addListener(ClipboardListener listener) {
    _listeners.add(listener);
  }

  void removeListener(ClipboardListener listener) {
    _listeners.remove(listener);
  }

  Future<void> start() async {
    await _channel.invokeMethod('start');
  }

  Future<void> stop() async {
    await _channel.invokeMethod('start');
  }
}

final clipboardWatcher = ClipboardWatcher.instance;

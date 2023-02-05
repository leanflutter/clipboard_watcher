import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
// import 'package:clipboard_watcher/clipboard_watcher.dart';

void main() {
  const MethodChannel channel = MethodChannel('clipboard_watcher');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  // test('getPlatformVersion', () async {
  //   expect(await ClipboardWatcher.platformVersion, '42');
  // });
}

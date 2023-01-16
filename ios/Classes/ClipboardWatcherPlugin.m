#import "ClipboardWatcherPlugin.h"
#if __has_include(<clipboard_watcher/clipboard_watcher-Swift.h>)
#import <clipboard_watcher/clipboard_watcher-Swift.h>
#else
#import "clipboard_watcher-Swift.h"
#endif

@implementation ClipboardWatcherPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftClipboardWatcherPlugin registerWithRegistrar:registrar];
}
@end

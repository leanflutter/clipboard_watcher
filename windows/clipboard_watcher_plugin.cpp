#include "include/clipboard_watcher/clipboard_watcher_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

namespace {

class ClipboardWatcherPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar);

  ClipboardWatcherPlugin(
      flutter::PluginRegistrarWindows* registrar,
      std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel);

  flutter::MethodChannel<flutter::EncodableValue>* channel() const {
    return channel_.get();
  }

  HWND ClipboardWatcherPlugin::hwnd();
  void ClipboardWatcherPlugin::Start();
  void ClipboardWatcherPlugin::Stop();

  virtual ~ClipboardWatcherPlugin();

 private:
  flutter::PluginRegistrarWindows* registrar_;
  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_ =
      nullptr;

  int32_t window_proc_id_ = -1;

  std::optional<LRESULT> HandleWindowProc(HWND hwnd,
                                          UINT message,
                                          WPARAM wparam,
                                          LPARAM lparam);

  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

// static
void ClipboardWatcherPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows* registrar) {
  auto plugin = std::make_unique<ClipboardWatcherPlugin>(
      registrar,
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "clipboard_watcher",
          &flutter::StandardMethodCodec::GetInstance()));
  plugin->channel()->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto& call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });
  registrar->AddPlugin(std::move(plugin));
}

ClipboardWatcherPlugin::ClipboardWatcherPlugin(
    flutter::PluginRegistrarWindows* registrar,
    std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel)
    : registrar_(registrar), channel_(std::move(channel)) {
  window_proc_id_ = registrar->RegisterTopLevelWindowProcDelegate(
      [this](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
        return HandleWindowProc(hwnd, message, wparam, lparam);
      });
}

ClipboardWatcherPlugin::~ClipboardWatcherPlugin() {
  registrar_->UnregisterTopLevelWindowProcDelegate(window_proc_id_);
}

std::optional<LRESULT> ClipboardWatcherPlugin::HandleWindowProc(HWND hwnd,
                                                                UINT message,
                                                                WPARAM wparam,
                                                                LPARAM lparam) {
  static BOOL bListening = FALSE;

  switch (message) {
    case WM_CLIPBOARDUPDATE:
      flutter::EncodableMap args = flutter::EncodableMap();
      channel_->InvokeMethod("onClipboardChanged",
                             std::make_unique<flutter::EncodableValue>(args));
      break;
  }
  return std::nullopt;
}

HWND ClipboardWatcherPlugin::hwnd() {
  HWND hwnd = ::GetAncestor(registrar_->GetView()->GetNativeWindow(), GA_ROOT);
  return hwnd;
}

void ClipboardWatcherPlugin::Start() {
  AddClipboardFormatListener(hwnd());
}

void ClipboardWatcherPlugin::Stop() {
  RemoveClipboardFormatListener(hwnd());
}

void ClipboardWatcherPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("start") == 0) {
    Start();
    result->Success(flutter::EncodableValue(true));
  } else if (method_call.method_name().compare("stop") == 0) {
    Stop();
    result->Success(flutter::EncodableValue(true));
  } else {
    result->NotImplemented();
  }
}

}  // namespace

void ClipboardWatcherPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  ClipboardWatcherPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}

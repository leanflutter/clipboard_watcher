#include "include/clipboard_watcher/clipboard_watcher_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>

#define CLIPBOARD_WATCHER_PLUGIN(obj)                                     \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), clipboard_watcher_plugin_get_type(), \
                              ClipboardWatcherPlugin))

struct _ClipboardWatcherPlugin {
  GObject parent_instance;
  FlMethodChannel* channel;
  bool is_watching = false;
};

G_DEFINE_TYPE(ClipboardWatcherPlugin,
              clipboard_watcher_plugin,
              g_object_get_type())

void handle_owner_change(GtkClipboard* clipboard,
                         GdkEvent* event,
                         gpointer data) {
  ClipboardWatcherPlugin* plugin = CLIPBOARD_WATCHER_PLUGIN(data);
  if (plugin->is_watching != true) {
    return;
  }

  g_autoptr(FlValue) result_data = fl_value_new_map();
  fl_method_channel_invoke_method(plugin->channel,
                                  "onClipboardChanged", result_data, nullptr,
                                  nullptr, nullptr);
}

static FlMethodResponse* start(ClipboardWatcherPlugin* self, FlValue* args) {
  self->is_watching = true;
  return FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
}

static FlMethodResponse* stop(ClipboardWatcherPlugin* self, FlValue* args) {
  self->is_watching = false;
  return FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
}

// Called when a method call is received from Flutter.
static void clipboard_watcher_plugin_handle_method_call(
    ClipboardWatcherPlugin* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);
  FlValue* args = fl_method_call_get_args(method_call);

  if (strcmp(method, "start") == 0) {
    response = start(self, args);
  } else if (strcmp(method, "stop") == 0) {
    response = stop(self, args);
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void clipboard_watcher_plugin_dispose(GObject* object) {
  ClipboardWatcherPlugin* self = CLIPBOARD_WATCHER_PLUGIN(object);
  g_clear_object(&self->channel);

  G_OBJECT_CLASS(clipboard_watcher_plugin_parent_class)->dispose(object);
}

static void clipboard_watcher_plugin_class_init(
    ClipboardWatcherPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = clipboard_watcher_plugin_dispose;
}

static void clipboard_watcher_plugin_init(ClipboardWatcherPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel,
                           FlMethodCall* method_call,
                           gpointer user_data) {
  ClipboardWatcherPlugin* plugin = CLIPBOARD_WATCHER_PLUGIN(user_data);
  clipboard_watcher_plugin_handle_method_call(plugin, method_call);
}

void clipboard_watcher_plugin_register_with_registrar(
    FlPluginRegistrar* registrar) {
  ClipboardWatcherPlugin* plugin = CLIPBOARD_WATCHER_PLUGIN(
      g_object_new(clipboard_watcher_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  plugin->channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "clipboard_watcher", FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(
      plugin->channel, method_call_cb, g_object_ref(plugin), g_object_unref);

  GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
  g_signal_connect(clipboard, "owner-change", G_CALLBACK(handle_owner_change),
                   plugin);

  g_object_unref(plugin);
}

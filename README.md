# clipboard_watcher

[![pub version][pub-image]][pub-url] [![][discord-image]][discord-url] ![][visits-count-image] 

[pub-image]: https://img.shields.io/pub/v/clipboard_watcher.svg
[pub-url]: https://pub.dev/packages/clipboard_watcher

[discord-image]: https://img.shields.io/discord/884679008049037342.svg
[discord-url]: https://discord.gg/zPa6EZ2jqb

[visits-count-image]: https://img.shields.io/badge/dynamic/json?label=Visits%20Count&query=value&url=https://api.countapi.xyz/hit/leanflutter.clipboard_watcher/visits

This plugin allows Flutter desktop apps to watch clipboard changes.

---

English | [简体中文](./README-ZH.md)

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [clipboard_watcher](#clipboard_watcher)
  - [Platform Support](#platform-support)
  - [Quick Start](#quick-start)
    - [Installation](#installation)
    - [Usage](#usage)
  - [Who's using it?](#whos-using-it)
  - [License](#license)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Platform Support

| Linux | macOS | Windows |
| :---: | :---: | :-----: |
|   ✔️   |   ✔️   |    ✔️    |

## Quick Start

### Installation

Add this to your package's pubspec.yaml file:

```yaml
dependencies:
  clipboard_watcher: ^0.1.3
```

Or

```yaml
dependencies:
  clipboard_watcher:
    git:
      url: https://github.com/leanflutter/clipboard_watcher.git
      ref: main
```

### Usage

```dart
class HomePage extends StatefulWidget {
  const HomePage({Key? key}) : super(key: key);

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> with ClipboardListener {
  @override
  void initState() {
    clipboardWatcher.addListener(this);
    // start watch
    clipboardWatcher.start();
    super.initState();
  }

  @override
  void dispose() {
    clipboardWatcher.removeListener(this);
    // stop watch
    clipboardWatcher.stop();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    // ...
  }

  @override
  void onClipboardChanged() async {
    ClipboardData? newClipboardData = await Clipboard.getData(Clipboard.kTextPlain);
    print(newClipboardData?.text ?? "");
  }
}
```

> Please see the example app of this plugin for a full example.

## Who's using it?

- [Biyi](https://biyidev.com/) - A convenient translation and dictionary app.

## License

[MIT](./LICENSE)

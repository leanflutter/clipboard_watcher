# clipboard_watcher

[![pub version][pub-image]][pub-url] [![][discord-image]][discord-url] ![][visits-count-image] [![All Contributors][all-contributors-image]](#contributors)

[pub-image]: https://img.shields.io/pub/v/clipboard_watcher.svg
[pub-url]: https://pub.dev/packages/clipboard_watcher
[discord-image]: https://img.shields.io/discord/884679008049037342.svg
[discord-url]: https://discord.gg/zPa6EZ2jqb
[visits-count-image]: https://img.shields.io/badge/dynamic/json?label=Visits%20Count&query=value&url=https://api.countapi.xyz/hit/leanflutter.clipboard_watcher/visits
[all-contributors-image]: https://img.shields.io/github/all-contributors/leanflutter/clipboard_watcher?color=ee8449&style=flat-square

This plugin allows Flutter apps to watch clipboard changes.

---

English | [简体中文](./README-ZH.md)

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [Platform Support](#platform-support)
- [Quick Start](#quick-start)
  - [Installation](#installation)
  - [Usage](#usage)
- [Who's using it?](#whos-using-it)
- [Contributors](#contributors)
- [License](#license)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Platform Support

| Platform | Support                                                                                                           |
| -------- | :---------------------------------------------------------------------------------------------------------------- |
| Linux    | ✔️ Fully supported                                                                                                |
| macOS    | ✔️ Fully supported                                                                                                |
| Windows  | ✔️ Fully supported                                                                                                |
| iOS      | 14+ Needs user permission to read data copied from others apps<br>Old versions are fully supported out of the box |
| Android  | 10+ Only works when the app is in the foreground<br>Old versions are fully supported out of the box               |

## Quick Start

### Installation

Add this to your package's pubspec.yaml file:

```yaml
dependencies:
  clipboard_watcher: ^0.2.0
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


## Contributors

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tbody>
    <tr>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/lijy91"><img src="https://avatars.githubusercontent.com/u/3889523?v=4?s=100" width="100px;" alt="LiJianying"/><br /><sub><b>LiJianying</b></sub></a><br /><a href="https://github.com/leanflutter/clipboard_watcher/commits?author=lijy91" title="Code">💻</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/ademar111190"><img src="https://avatars.githubusercontent.com/u/1225438?v=4?s=100" width="100px;" alt="Ademar"/><br /><sub><b>Ademar</b></sub></a><br /><a href="https://github.com/leanflutter/clipboard_watcher/commits?author=ademar111190" title="Code">💻</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://customersuccessbox.com/"><img src="https://avatars.githubusercontent.com/u/64081899?v=4?s=100" width="100px;" alt="Amritpal Singh"/><br /><sub><b>Amritpal Singh</b></sub></a><br /><a href="https://github.com/leanflutter/clipboard_watcher/commits?author=boparaiamritcsb" title="Code">💻</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/jpnurmi"><img src="https://avatars.githubusercontent.com/u/140617?v=4?s=100" width="100px;" alt="J-P Nurmi"/><br /><sub><b>J-P Nurmi</b></sub></a><br /><a href="https://github.com/leanflutter/clipboard_watcher/commits?author=jpnurmi" title="Code">💻</a></td>
    </tr>
  </tbody>
  <tfoot>
    <tr>
      <td align="center" size="13px" colspan="7">
        <img src="https://raw.githubusercontent.com/all-contributors/all-contributors-cli/1b8533af435da9854653492b1327a23a4dbd0a10/assets/logo-small.svg">
          <a href="https://all-contributors.js.org/docs/en/bot/usage">Add your contributions</a>
        </img>
      </td>
    </tr>
  </tfoot>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->

## License

[MIT](./LICENSE)

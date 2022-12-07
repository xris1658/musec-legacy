| [English](./Translation.md) | 简体中文 |
| --------------------------- | -------- |

# 翻译
这份文件涵盖了此项目的翻译进度，以及添加新翻译等操作的方法。

## 现有翻译
程序代码中出现的显示文字均为英文。`i18n` 文件夹中提供了简体中文的翻译。

## 添加新翻译
本项目的翻译文件使用 Qt Linguist 生成与编写。此应用程序是 Qt 的一部分，因此可以通过下载并安装 Qt 使用。
如果不想下载完整的开发工具套件，可以使用 [lelegard/qtlinguist-installers](https://github.com/lelegard/qtlinguist-installers) 安装单独的 Qt Linguist 程序。

### 步骤
- 复制模板文件 `base.ts`，并将其重命名为 `Musec_xx_XX.ts`。其中 `xx` 是 ISO 639-1 语言码，`XX` 是 ISO 3166 国家码 ALPHA 2。
  - 语言码列表：[List of ISO 639-1 codes - Wikipedia](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes)
  - 国家码列表：[List of ISO 3166 country codes - Wikipedia](https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes)
- 用 Qt Linguist 打开翻译文件。
- Qt Linguist 会自动从文件名中检测语言码和国家码，若没有检测到，则通过检测系统语言和区域设置来调整目标语言。也可以自行选择目标语言。确认目标语言无误后点击 **OK**。
- 翻译文本。
- 翻译完成后，保存翻译文件。

## 更新翻译文件
如果开发者对涉及文本的代码进行了更新（比如添加了新的显示文字），需要更新翻译文件。Qt 中的 `lupdate.exe` 是用于更新翻译文件的程序。可以通过 `lupdate.exe --help` 查看用法。

### 在 CMake 中更新
`CMakeLists.txt` 中添加了两个定制目标：
- `GenerateLocaleBase`，用于更新 `base.ts`。
- `GenerateLocaleCN`，用于更新 `Musec_zh_CN.ts`。

可以仿照这两个目标添加自己的翻译文件。

### 在 qmake 中更新
`Musec.pro` 中的 `TRANSLATIONS` 变量列举了项目的翻译文件。可以修改此变量，以包含自己的文件。

添加完成后，使用 `lupdate.exe` 更新翻译文件即可：
```shell
<lupdate 路径> <项目路径>/Musec.pro
```

## 发布翻译文件
要让程序使用翻译，需要将翻译文件发布为目标文件。
要发布翻译文件，用 Qt Linguist 打开翻译文件后，选择菜单栏中的**文件->发布**，翻译文件所在的目录中会生成一个目标文件。
将发布的文件放在程序目录下，即可在程序中使用翻译。

可以将发布翻译的过程集成到项目文件中。
### 在 CMake 中发布
`CMakeLists.txt` 中，发布翻译和复制发布文件的操作是定制命令，在生成后执行：
```cmake
# CMakeLists.txt
add_custom_command(TARGET Musec POST_BUILD COMMAND ${Qt5_DIR}/../../../bin/lrelease.exe ${CMAKE_SOURCE_DIR}/i18n/Musec_zh_CN.ts -qm ${CMAKE_CURRENT_BINARY_DIR}\\Musec_zh_CN.qm)
```
要发布自己的翻译，可以更改这一命令。
### 在 qmake 中发布
发布翻译和复制发布文件的操作由 qmake 提供支持。

## 一些注意事项
- `CMakeLists.txt` 通过相对于 `Qt5_DIR` 的路径寻找 `lupdate.exe` 和 `lrelease.exe`。如果 Qt 是由官方
提供的程序安装的，那么这两个程序的位置没有问题。Qt 可以通过包管理器安装（如 Conan 和 vcpkg），此时需要确保
机器上可以找到这两个应用程序，然后用正确的路径修改 `CMakeLists.txt`（可以用绝对路径）。
- 翻译文件的扩展名是 `.ts`。TypeScript 文件和传输流（transport stream）文件同样使用这一扩展名。
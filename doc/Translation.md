| English | [简体中文](./Translation-zh.md) |
| ------- | ------------------------------- |

# Translation
This file covers the translation status of this project, as well as methods to add translations and so on.

## Current translation
Display text in the code is in English. There's a Simplified Chinese translation in the folder `i18n`.

## Add a translation
The translation file of this project is generated and edited with Qt Linguist. This application is part of Qt, so you could use it by downloading and installing Qt.
If you'd not like to download a whole development kit, you can install standalone Qt Linguist from [lelegard/qtlinguist-installers](https://github.com/lelegard/qtlinguist-installers).

### Instructions
- Copy the template file `base.ts`，rename it to `Musec_xx_XX.ts`, with xx being the language code in ISO 639-1, and XX being the country code ALPHA 2 in ISO 3166.
  - List of language codes: [List of ISO 639-1 codes - Wikipedia](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes)
  - List of country codes: [List of ISO 3166 country codes - Wikipedia](https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes)
- Open the translation file with Qt Linguist.
- Qt Linguist will detect the language code and country code in the file automatically, if failed, it will detect the language and region settings of the operating system. Then the target language will be set. You can select the target language manually. Make sure the target language is correct, then click **OK**.
- Translate the text.
- If the translation is complete, save the translation file.

## Update translation files
If the developer updated the code by changing the display text (e.g. added new display text), then the translation files need an upgrade. `lupdate.exe` is a program used to update translation files in Qt. You can check the usage by `lupdate.exe --help`.

### Update in CMake
There are two custom targets in `CMakeLists.txt`:
- `GenerateLocaleBase`, this updated `base.ts`.
- `GenerateLocaleCN`, this updated `Musec_zh_CN.ts`.

You can add your translation file by copying and changing the custom target.

### Update in qmake
Translation files in the project are listed in the variable `TRANSLATIONS` of `Musec.pro`. You can set this variable to include your file.

After the file is included, just update them using `lupdate.exe`:
```shell
<Path to lupdate> <Path to project>/Musec.pro
```

## Release translations
It's required to release the translation files to target files in order to make use of them in the program. 
Open the translation file with Qt Linguist, select **File -> Release**, then a target file is generated in the directory of the translation file. 
Move the target file to the directory of the program, then the translation can be used by the program.

You can integrate the releasing process in the project file.

### Release in CMake
Releasing the translation file is a custom command in `CMakeList.txt`, and it's executed after building:
```cmake
# CMakeLists.txt
add_custom_command(TARGET Musec POST_BUILD COMMAND ${Qt5_DIR}/../../../bin/lrelease.exe ${CMAKE_SOURCE_DIR}/i18n/Musec_zh_CN.ts -qm ${CMAKE_CURRENT_BINARY_DIR}\\Musec_zh_CN.qm)
```
You could change this command in order to use your translation.

### Release in qmake
Releasing the translation is supported by qmake.

### Some things to notice
- `CMakeLists.txt` use the relative path of `Qt5_DIR` to find `lupdate.exe` and `lrelease.exe`. 
If Qt is installed by the official installer, then the relative path will be okay. 
Qt could be installed by several package managers (e.g. Conan, vcpkg), in this scenario, make sure you can find these applications on your machine, then edit `CMakeLists.txt` using the right path (you can use absolute paths).
- The extension of the translation file is `.ts`. TypeScript file and transport stream file use this extension as well.
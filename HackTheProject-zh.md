| [English](./HackTheProject.md) | 简体中文 |
| ------------------------------ | -------- |

# 定制项目
这份文件为想要自己从源码构建项目，或者修改源码的读者编写。

仓库内有 `CMakeLists.txt` 以及 `Musec.pro`，可以用 CMake 和 qmake 构建项目。

## 安装依赖项、配置环境
生成环境：Windows 10 64-bit 
- 下载并安装 Visual Studio 2019。  
要获取下载链接、查看安装步骤以及其他细节，请参阅[微软文档](https://docs.microsoft.com/cpp/build/vscpp-step-0-installation?view=msvc-160)。
  - 我们用不着 IDE 里的特殊功能，因此社区版足够。
  - 需要安装英语语言包。
- 下载并安装包管理器  vcpkg。  
要查看安装步骤以及其他细节，请参阅 [Getting started with vcpkg](https://vcpkg.io/en/getting-started.html)。注意：
  - vcpkg 需要 Git 才能运行。（我觉得读这篇指南的人应该知道 Git 是什么。）
  - vcpkg 需要 Visual Studio 安装英语语言包。
    >- 我不记得这个是不是 bug，如果是 bug 的话，不知道解决了没有。
    >- 有些 vcpkg 用户需要给 Windows 安装英语语言包。我的机器上碰巧有这个语言包，不知道是否有必要安装。
- 用 vcpkg 安装几个软件包:
  - `ffmpeg:x64-windows` 和 `avcpp:x64-windows`
    >- 编译 FFmpeg 需要大量的 CPU 算力。这个过程可能会花 5 分钟或更久。
    >- 用之前构建好、或是从网上下载的 FFmpeg 可能会导致版本不匹配等问题，因此不推荐这么做。
  - `fftw3:x64-windows` 和 `fftwpp:x64-windows`
  - `fmt:x64-windows` 和 `spdlog:x64-windows`
  - `soxr:x64-windows`
  - `sqlite3:x64-windows` 和 `sqlite-modern-cpp:x64-windows`
  - `yaml-cpp:x64-windows`
- 下载并安装 CMake。此项目使用的 CMake 版本为 3.14 或更高。
  - 初次使用 vpckg 安装软件包时，vcpkg 会自动下载 CMake。读者或许可以在这一路径找到：
  ```
  <vcpkg 目录>\downloads\tools\cmake-3.21.1-windows\cmake-3.21.1-windows-i386
  ```
  这是一份 32 位的 CMake，可以在 64 位系统上运行，不过应该不能用 x64 的工具链，需要换用 x86_64。
- 下载并安装 Qt 5.15.2。在选择安装组件时，勾选 **Qt 5.15.2** 下的 **MSVC 2019 64-bit**。
  - 读者可能会勾选 **Qt 5.15.2** 下的 **Qt Debug information Files** 以及 **Developer and Designer Tools** 下的 **Debugging Tools for Windows** ，用于调试程序。
- 下载并解压 [Steinberg ASIO SDK](https://www.steinberg.net/asiosdk)。
- 克隆 [Steinberg VST3 SDK 仓库](https://github.com/steinbergmedia/vst3sdk)。
- 下载 [Steinberg VST2 SDK 压缩文件](https://archive.org/download/VST2SDK/vst_sdk2_4_rev2.zip)，并将文件中 `vstsdk2.4` 文件夹的内容（除了 `index.html`）解压到 VST3 SDK 目录下。  
  能在 VST3 SDK 的 `pluginterfaces` 目录中找到 `vst` 和 `vst2.x`，就说明做对了。
  > 为什么上面的链接**不是** Steinberg 官网的？  
  Steinberg 在数年前决定将 VST2 SDK 变成专有代码，并且不再开放下载；自家产品也会逐步停止对 VST2 的支持。不少网站还提供这份 SDK，我选用这个链接，是因为这个文件包含了文档，其中有调用的时序图。当开发者遇到了问题时，文档或许会很有用。（ **请务必读文档，务必读文档，务必读文档！** 重要的事情说三遍！）

## 用 <u>CMake</u> 构建项目
如果用 CMake，构建项目的过程会相对简单。
### 使用 IDE
以 CLion 为例：
1. 用 CLion 打开项目目录，或者项目内的 `CMakeLists.txt`。
2. CLion 会提示用户配置 CMake 项目。（以后可以通过 **文件 | 设置 | 构建、执行、部署 | CMake** 进行配置。）在所有使用 Visual Studio 工具链的配置的 CMake 选项处填写以下内容
```
-DCMAKE_TOOLCHAIN_FILE=<vcpkg 目录>/scripts/buildsystems/vcpkg.cmake -DVST3SDK_SOURCE_DIR=<VST3 SDK 目录> -DASIOSDK_PATH=<ASIO SDK 目录>
```
3. 构建目标 `Musec`。

### 使用命令行
1. 打开配置有 Visual Studio 环境的命令窗口。可以使用以下方式进行这一操作：
   - 打开 `cmd.exe` 并执行以下命令:
     ```shell
     "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
     ```
     读者需要将 Visual Studio 的路径换成安装时的路径。
   - 打开 **Visual Studio 2019** -> **VC** -> **x64 Native Tools Command Prompt for VS 2019**。
2. 使用 CMake 配置并构建项目。使用上一步打开的命令窗口，执行以下命令：
  ```shell
  <Path to cmake.exe> -G "NMake Makefiles" --toolchain <vcpkg 目录>/scripts/buildsystems/vcpkg.cmake -DVST3SDK_SOURCE_DIR=<VST3 SDK 目录> -DASIOSDK_PATH=<ASIO SDK 目录> -S <Musec 项目目录> -B <项目的生成位置>
  # 注意：目录使用正斜杠 "/" 
  ```
3. 将工作目录切换到项目的生成位置：
  ```shell
  cd /d <生成位置> # 要切换到与当前工作目录所在磁盘分区不同的目录下，需要添加 /d
  ```
4. 最后生成程序：
  ```shell
  nmake Musec
  ```

如果程序最后显示 `[100%] Built target Musec`，恭喜你！程序构建成功了。

## 用 <u>qmake</u> 构建项目
如果用 qmake，构建过程相对棘手一些，因为很多步骤需要手动进行（如构建依赖项、准备库文件等）。
1. 使用 CMake 生成 VST3 SDK 解决方案。
  - 使用 CMake GUI 程序的操作步骤：
    - 打开 `<CMake 路径>\bin\cmake-gui.exe`.
    - 在 "Where is the source code" 处填入 VST3 SDK 的路径。
    - 在 "Where to build the binaries" 处填入 `<VST3 SDK path>/build`。
    - 配置项目：点击 **Configure** 按钮，或菜单栏中的 **Tools -> Configure**，在提示创建生成目录的对话框中选择 **Yes**。
    - 将 "Specify the generator for this project" 设为 **Visual Studio 16 2019**，然后点击 **Finish**。
    - 将 `CMAKE_BUILD_TYPE` 设置为 `Debug;Release`。
    - 再次配置项目，然后点击 **Generate** 按钮，或菜单栏中的 **Tools -> Generate**。

    生成目录下会出现 `vstsdk.sln` 文件。
2. 使用 VST3 SDK 解决方案构建 VST3 SDK 库文件。
  - 使用 Visual Studio 2019 打开 `vstsdk.sln` 解决方案文件。
  - 选择菜单栏中的**生成 -> 批生成...**。
  - 勾选 `ALL_BUILD Debug x64` 和 `ALL_BUILD Release x64` 两行“生成”列的复选框，然后点击**生成**按钮。

    如果在 `<VST3 SDK 路径>\build\debug\lib` 和 `<VST3 SDK 路径>\build\release\lib` 下找到了库文件，则构建正确。
3. 配置并生成 Musec 项目。
  - 将 Musec 仓库克隆到本机。
  - 用 Qt Creator 打开 `<Musec directory>\Musec.pro`。
  - 勾选 **Desktop Qt 5.15.2 MSVC2019 64-bit**，然后点击 **Configure Project** 按钮。
  - 点击 **切换到编辑模式**（左侧边栏的文本图标）或按 Ctrl+2，然后打开 `Musec.pro`。将第 18-20 行的变量代为相应依赖的路径。
  - 构建项目。
  - 自行将一些 DLL 文件复制到生成目录。
    - 对于 Debug 版：
      - DLL 文件的路径为 `<Path to vcpkg>\installed\x64-windows\debug\bin`
      - 目标路径为 `<Path to Musec>\debug`
      - 以下是要复制的 DLL文件：
      ```
      avcodec-58.dll
      avdevice-58.dll
      avfilter-7.dll
      avformat-58.dll
      avutil-56.dll
      fftw3.dll
      fftw3f.dll
      fftw3l.dll
      fmtd.dll
      postproc-55.dll
      soxr.dll
      spdlogd.dll
      sqlite3.dll
      swresample-3.dll
      swscale-5.dll
      yaml.dll
      yaml-cppd.dll
      ```
    - 对于 Release 版：
      - DLL 文件的路径为 `<Path to vcpkg>\installed\x64-windows\bin`
      - 目标路径为`<Path to Musec>\release`
      - 以下是要复制的 DLL文件：
      ```
      avcodec-58.dll
      avdevice-58.dll
      avfilter-7.dll
      avformat-58.dll
      avutil-56.dll
      fftw3.dll
      fftw3f.dll
      fftw3l.dll
      fmt.dll
      postproc-55.dll
      soxr.dll
      spdlog.dll
      sqlite3.dll
      swresample-3.dll
      swscale-5.dll
      yaml.dll
      yaml-cpp.dll
      ```
  - 自行将 `Resources` 文件夹复制到生成路径。

  可以通过添加定制生成步骤的方式自动复制文件，不过需要注意以下事项：
  - 不能使用 `copy`，因为 `copy` 只是一条命令，可以在命令提示符和 Powershell 中使用，不是可执行程序。Qt Create 找不到名为 `copy` 的程序。
  - 可以用 Windows 自带的实用程序 `robocopy`。不过或许读者更想用其他程序调用，而不是直接在生成步骤内使用，因为
    - 如果所有文件 **都已经复制过**，`robocopy` 会返回 `0`；如果所有文件 **原本不在目标目录下，而复制操作均成功复制了所有文件**，则返回 `1`。
    - Qt Creator 将非零返回值视为错误，因此会在 `robocopy` 步骤中提示出错，哪怕文件全部成功复制到了生成目录。这不该视作错误。

## 调试
Visual Studio 和较新版本的 CLion 支持 NatVis，可以基于配置文件对对象进行可视化处理。当调试使用其他库（如 Qt）的应用时，NatVis 会非常有用。

要在 CLion 中使用 NatVis：
- 确保启用了相关选项。详情参阅 [CLion 文档](https://www.jetbrains.com/help/clion/2022.1/quick-tutorial-on-configuring-clion-on-windows.html#msvc-debugger)。
- 将 Qt 的 NatVis 文件下载或复制到项目的根目录。
  - 如果安装过 [Qt Visual Studio Tools extension](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2019)，可以在 `<用户的“文档”文件夹, e.g. C:\Users\xris1658\Documents>\Visual Studio 2019\Visualizers\qt5.natvis` 下找到。
  - 如果没安装过，可以在 [aleksey-nikolaev/natvis-collection](https://github.com/aleksey-nikolaev/natvis-collection) 上或者别处找一个。

此项目的用户界面使用 QML 编写，重度依赖项目的部分 C++ 代码（`entities` 和 `models` 中的内容）。如果编写 QML 文件时出了错，找到根源可能会极为困难（就算用了 Git 追溯功能也一样）。QML 的某些实现类没有可视化，所以我自己编写了一个 NatVis 文件，就是项目根目录的 `qt5-qml.natvis`。
| English | [简体中文](./HackTheProject-zh.md) |
| ------- | ---------------------------------- |

# Hack The Project
This file is for those who wants to build the application from the source code, or maybe modifying the code.

There are both `CMakeLists.txt` and `Musec.pro` in the repo, meaning that one can build 
this project using both CMake and qmake.

## Why don't you just provide the binary file?
Because I actually can't. For now the program only supports ASIO, whose license agreement that is not compatible with some open source licenses like GPLv3. This means that
- If I didn't sign the license agreement (by signing on the document included in the ASIO SDK and send it to Steinberg), I can only redistribute the program without ASIO support, making the program virtually unable to do anything useful.
- Seems like I can open the source using ASIO and let the user build the program on their own (Audacity uses this approach AFAIK). However, the user cannot redistribute the program with ASIO support unless they have signed the license agreement.

For now my plan is to add other audio API support like WASAPI exclusive mode, so I can at least distribute the program that ASIO support is not included.

I don't fully understand these law-related things, so **don't take my opinion as a legal advice!**

## How to install the prerequisites and prepare the build environment
Build environment: Windows 10 64-bit
- Download and install Visual Studio 2019.  
Check out [Microsoft Docs](https://docs.microsoft.com/cpp/build/vscpp-step-0-installation?view=msvc-160)
 for links, installation guide and other details.
  - We're not using special features in the IDE, so the Community version will suffice.
  - You need to install the English language pack.
- Download and install vcpkg, a package manager.  
Check out [Getting started with vcpkg](https://vcpkg.io/en/getting-started.html) for installation 
guide and other details. Note that
  - vcpkg requires Git to work. (If you're reading this guide, I kinda believe that you know what Git is.)
  - vcpkg requires Visual Studio that has English language pack installed.
    >- I can't remember whether this is a bug or not, or if this has been solved if it's a bug.
    >- Some vcpkg users need to install the English language pack for Windows. I happened to have it 
    on my machine, so I don't know if it's needed.
- Install several packages using vcpkg:
  - `ffmpeg:x64-windows` and `avcpp:x64-windows`
    >- Compiling FFmpeg takes a lot of CPU horsepower. This process might take longer than 5 minutes.
    >- Using FFmpeg pre-built by yourself or downloaded from the Internet might cause some problems
    like version mismatch and so on. So it's not recommended.
  - `fftw3:x64-windows` and `fftwpp:x64-windows`
  - `fmt:x64-windows` and `spdlog:x64-windows`
  - `soxr:x64-windows`
  - `sqlite3:x64-windows` and `sqlite-modern-cpp:x64-windows`
  - `yaml-cpp:x64-windows`
- Download and install CMake. This project uses CMake version 3.14 or higher.
  - Installing packages with vcpkg for the first time will download CMake automatically. You might be able to locate
  it at
  ```
  <vcpkg directory>\downloads\tools\cmake-3.21.1-windows\cmake-3.21.1-windows-i386
  ```
  so this is a 32-bit version of CMake. It can run on 64-bit systems, but you might like to use the x86_64 platform instead of x64.
- Download and install Qt 5.15.2. Check **MSVC 2019 64-bit** under item **Qt 5.15.2** while choosing
components.
  - You might want to check **Qt Debug information Files** under **Qt 5.15.2** and **Debugging Tools
  for Windows** under **Developer and Designer Tools** for debugging.
- Download and extract the [Steinberg ASIO SDK](https://www.steinberg.net/asiosdk).
- Clone the [Steinberg VST3 SDK repository](https://github.com/steinbergmedia/vst3sdk).
- Download the [Steinberg VST2 SDK zip file](https://archive.org/download/VST2SDK/vst_sdk2_4_rev2.zip)
and extract files in `vstsdk2.4` (except `index.html`) to the VST3 SDK repository that was cloned
to your machine.  
  If you see directories `vst` and `vst2.x` in `pluginterfaces`, you have made it correctly.
  > For those who don't why this link is **not** from the Steinberg website:  
  Steinberg had decided 
  to make the VST2 SDK proprietary and not in public for download in several years ago, and eventually stop supporting VST2 in their own products. There are lots of websites that offer this 
  SDK. The reason I chose this link from The Internet Archive is that the documentation is 
  present in this file, including some calling sequence diagrams. This might be useful when the 
  developer encountered some issues. (**Please RTFM, again, RTFM, do it!**)
- Clone the [CLAP SDK repository](https://github.com/free-audio/clap) and [CLAP Helpers repository](https://github.com/free-audio/clap-helpers).

## Build the project with <u>CMake</u>
If you're using CMake, the building process will be relatively simple.
### Using IDEs
Take CLion for example:
1. Open the project directory or `CMakeLists.txt` with CLion.
2. CLion will prompt you to configure the CMake project. (You can configure it later by selecting **File | Settings | Build, Execution, Deployment | CMake**.) Set the generator of all the Profiles using Visual Studio toolchain to **NMake Makefiles JOM**, then fill these "CMake Options" text boxes with
    ```
    -DCMAKE_TOOLCHAIN_FILE=<vcpkg directory>/scripts/buildsystems/vcpkg.cmake -DVST3SDK_SOURCE_DIR=<VST3 SDK directory> -DASIOSDK_PATH=<ASIO SDK directory> -DCLAP_SOURCE_DIR=<Path to CLAP SDK> -DCLAP_HELPERS_DIR=<Path to CLAP Helpers> -DCMAKE_MAKE_PROGRAM=<Path to Qt>/Tools/QtCreator/bin/jom/jom.exe
    ```
    - The command above use [JOM](https://github.com/qt-labs/jom) from Qt, which is a build tool that "aimed to be an nmake clone with support for parallel builds". You can use any build system you like of course.
3. Build the target `Musec`.

### Using command-line
1. Open a command window with Visual Studio environment ready. You can do that by using one of the
following ways:
   - Open `cmd.exe` and execute the following command:
     ```shell
     "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
     ```
     You might need to replace the path to Visual Studio to the directory you installed it to.
   - Open **Visual Studio 2019** -> **VC** -> **x64 Native Tools Command Prompt for VS 2019** from the application list.
2. Use CMake to configure and build the project. Execute the following command using the command window that just opened:
    ```shell
    <Path to cmake.exe> -G "NMake Makefiles JOM" --toolchain <vcpkg install directory>/scripts/buildsystems/vcpkg.cmake -DVST3SDK_SOURCE_DIR=<VST3 SDK directory> -DASIOSDK_PATH=<ASIO SDK directory> -DCLAP_SOURCE_DIR=<Path to CLAP SDK> -DCMAKE_MAKE_PROGRAM=<Path to Qt>/Tools/QtCreator/bin/jom/jom.exe -S <Musec source directory> -B <Directory you'd like to generate the program>
    ```
3. Change the working directory to the directory you'd like to generate the program:
    ```shell
    cd /d <Directory> # Use /d while changing to the directory in another drive
    ```
4. Finally, build the program:
    ```shell
    <Path to cmake.exe> --build <Build directory> --target Musec -j <Parallel job count>
    ```
If the program prints `[100%] Built target Musec` in the end, congratulations! The program has just built successfully.

## Build the project with <u>qmake</u>
If you're using qmake, things will get dirty, as many steps (like building some dependencies and preparing
library files) have to be made manually.
1. Use CMake to generate VST3 SDK solution.
    - The following steps are for the CMake GUI application:
      - Open `<CMake path>\bin\cmake-gui.exe`.
      - Set "Where is the source code" to the path to your VST3 SDK.
      - Set "Where to build the binaries" to `<VST3 SDK path>/build`.
      - Click the **Configure** button or **Tools -> Configure** from the menu bar, and choose **Yes** to create the build directory if needed.
      - Set "Specify the generator for this project" to **Visual Studio 16 2019**, then click **Finish**.
      - Set `CMAKE_BUILD_TYPE` to `Debug;Release` if not.
      - Configure again, then click the **Generate** button or **Tools -> Generate**.

      You should now have `vstsdk.sln` in the build directory.
2. Use the VST3 SDK solution file to build the VST3 SDK library files.
    - Open the `vstsdk.sln` solution file with Visual Studio 2019.
    - Choose **Build -> Batch Build...** from the menu bar.
    - Check the checkboxes of row `ALL_BUILD Debug x64` and `ALL_BUILD Release x64`, then click the **Build** button.

    If you see library files in `<VST3 SDK path>\build\lib\Debug` and `<VST3 SDK path>\build\lib\Release`, then you have made it correctly.
3. Configure and build the Musec project.
    - Clone the repository to your machine.
    - Open `<Musec directory>\Musec.pro` using Qt Creator.
    - Check **Desktop Qt 5.15.2 MSVC2019 64-bit**, then click **Configure Project** button.
    - Click **Switch to Edit mode** (text button on the left) or press Ctrl+2, then open `Musec.pro`. Replace the variables wrapped with `Variables` comment to paths to the corresponding dependencies.
    - Build the project.
    - Manually copy some DLL files to the build path. You can check the build path by selecting **Switch to Projects Mode**, selecting the corresponding mode after **Edit build configurations**, and checking **Build directory**.
      - For Debug build:
        - Path to the DLL files is `<Path to vcpkg>\installed\x64-windows\debug\bin`
        - DLL files to copy are as follows:
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
      - For Release build:
        - Path to the DLL files is `<Path to vcpkg>\installed\x64-windows\bin`
        - DLL files to copy are as follows:
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
    - Manually copy the `Resources` folder to the build path.
  
    You could automatically copy the files by adding custom build processes, but there are some things you should notice:
    - You can't use `copy` directly to do this, since it is a command you can use in Command Prompt or Powershell instead of an executable, and Qt Creator can't find a program named `copy`. The correct way is to use `cmd` to execute the `copy` command:
      ```
      cmd /C copy /B /Y <Path to files to copy> <Destination>
      ```
      - You might want to check the return code of the operation. Check [How do I get the application exit code from a Windows command line? - Stack Overflow](https://stackoverflow.com/questions/334879/how-do-i-get-the-application-exit-code-from-a-windows-command-line) for details.
    - You could use `robocopy`, a utility in Windows. But you might like to call it using another program, instead of using it directly in the process, since
      - `robocopy` will return `0` if all the files are **already copied and same as the original files**, and `1` if all the files are **not in the destination directory initially and copied successfully**.
      - Qt Creator will treat non-zero return code as error, so it will complain about the `robocopy` step even if the files are successfully copied, which shouldn't be treated as an error.

## Compiling with Libraries and Compiling with Source
We built some dependencies as dynamic libraries using vcpkg before building the project. Additionally，we built the VST3 SDK as static libraries before building the project using qmake. Some dependencies are compiled along with the project as source codes.
There are some pros and cons of these 3 ways building dependencies.
- Dynamic libraries can be used simultaneously by multiple applications with one copy of the code in the memory in order to save memory (nowadays memory capacity increases a lot so fast that this might be not a significant advantage anymore). But it's quite easy to be affected by DLL hell.
- Static libraries won't be affected by DLL Hell. However, you need to keep the toolchains and parameters used while compiling the library and the application identical to prevent compile or runtime problems.
- Compiling using source code can effectively prevent problems brought by library versions. But compiling with source will spend more time than the time compiling using libraries.

You should check these information before choosing the way that fits for you.

## Debugging
Visual Studio and recent versions of CLion support NatVis, which can visualize objects base on configuration files. When debugging applications using libraries like Qt, this can be very helpful.

If you want to use NatVis in CLion:
- Make sure the related options are enabled. See the [CLion documentation](https://www.jetbrains.com/help/clion/2022.1/quick-tutorial-on-configuring-clion-on-windows.html#msvc-debugger) for details.
- Download or copy the Qt NatVis files to the root directory of the project.
  - If you installed the [Qt Visual Studio Tools extension](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2019), you can find the NatVis file at `<Document directory for the user, e.g. C:\Users\xris1658\Documents>\Visual Studio 2019\Visualizers\qt5.natvis`.
  - If not, you can find one at [aleksey-nikolaev/natvis-collection](https://github.com/aleksey-nikolaev/natvis-collection) or somewhere else.

The user interface of this project is written in QML and heavily depends on part of the C++ codes (in `entities` and `models`). If someone made a mistake editing the QML files, it can be extremely hard to find the root cause (even with git blame). Some implementation classes of QML are not visualized, so I wrote a NatVis file by myself, which is `qt5-qml.natvis` in the root directory of the project.

## Write everything in C/C++ codes with only ASCII characters while you can
When I say "everything", I mean comments, text that shows in the log, GUI and so on.

All the files use UTF-8 encoding without BOM. For some reason, MSVC toolchain will treat them as files using ANSI encoding. So warning C4819 and numerous compilation errors will be emitted potentially if you try compiling the project with code that contains non-ASCII characters.

Writing everything with only ASCII characters basically means writing everything in English. To spread your project more effectively, you'll hope to make your thoughts in the code and comments understandable by developers all around the globe, instead of only developers who can understand whatever language you use in the code. 
Machine translation might help, but these translations can produce results that don't mean right, or sometimes completely opposite.
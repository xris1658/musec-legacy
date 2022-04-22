# Musec

Musec is a digital audio workstation (DAW) softtware. Initiated as a graduation project, it is now incomplete and under development.

## Features
TODO

## Build Guide
- Build environment: Windows 10 64-bit
- Download and install Visual Studio 2019. Check out [Microsoft Docs](https://docs.microsoft.com/cpp/build/vscpp-step-0-installation?view=msvc-160) for links, installation guide and other details.
  - The Community version will suffice.
- Download and install vcpkg. Check out [Getting started with vcpkg](https://vcpkg.io/en/getting-started.html) for installation guide and other details. Note that
  - vcpkg requires Git to work.
  - vcpkg requires Visual Studio that has English language pack installed.
- Install several packages using vcpkg:
  - `ffmpeg:x64-windows` and `avcpp:x64-windows`
    - Compiling FFmpeg takes a lot of CPU horsepower. This process might take longer than 5 minutes.
  - `fftw3:x64-windows` and `fftwpp:x64-windows`
  - `fmt:x64-windows` and `spdlog:x64-windows`
  - `soxr:x64-windows`
  - `sqlite3:x64-windows` and `sqlite-modern-cpp:x64-windows`
  - `yaml-cpp:x64-windows`
- Download and install Qt 5.15.2. Check **MSVC 2019 64-bit** under item Qt 5.15.2 while choosing components.
  - You might want to check **Qt Debug information Files** under Qt 5.15.2 and **Debugging Tools for Windows** under Developer and Designer Tools for debugging conveniently.
- Download and extract the [Steinberg ASIO SDK](https://www.steinberg.net/asiosdk).
- Clone the [Steinberg VST3 SDK repository](https://github.com/steinbergmedia/vst3sdk).
- Download the [Steinberg VST2 SDK zip file](https://archive.org/download/VST2SDK/vst_sdk2_4_rev2.zip) and extract files in `vstsdk2.4` (except `index.html`) to the VST3 SDK repository that was cloned to your machine.  
If you see directories `vst` and `vst2.x` in `pluginterfaces`, then you have made it correctly.
- Use CMake to generate VST3 SDK solution.
  - vcpkg will download CMake automatically when installing packages for the first time. You will find that in `<vcpkg directory>\downloads\tools\cmake-x.xx.x-windows\cmake-x.xx.x-windows-i386`.
  - If you'd like to use the GUI version of CMake:
    - Open `<CMake path>\bin\cmake-gui.exe`.
    - Set "Where is the source code" to the path to your VST3 SDK.
    - Set "Where to build the binaries" to `<VST3 SDK path>/build`.
    - Click the **Configure** button or **Tools -> Configure** from the menu bar, and choose **Yes** to create the build directory if needed.
    - Set "Specify the generator for this project" to **Visual Studio 16 2019**, then click **Finish**.
    - Set `CMAKE_BUILD_TYPE` to `Debug;Release` if not.
    - Configure again, then click the **Generate** button or **Tools -> Generate**.
  - If you'd like to use the command line version of CMake:
    - TODO

  You should now have `vstsdk.sln` in the build directory.
- Use the VST3 SDK solution file to build the VST3 SDK library files.
  - Open the `vstsdk.sln` solution file with Visual Studio 2019.
  - Choose **Build -> Batch Build...** from the menu bar.
  - Check the check boxes of row `ALL_BUILD Debug x64` and `ALL_BUILD Release x64`, then click the **Build** button.

  If you see lots of library files in `<VST3 SDK path>\build\debug\lib` and `<VST3 SDK path>\build\release\lib`, then you have made it correctly.
- Configure and build the Musec project.
  - Clone the repository to you machine.
  - Open `<Musec directory>\Musec.pro` using Qt Creator.
  - Check **Desktop Qt 5.15.2 MSVC2019 64-bit**, then click **Configure Project** button.
  - Click **Switch to Edit mode** (text button on the left) or press Ctrl+2, open `Musec.pro`. Replace the variables at ll.18-20 to paths to the corresponding dependencies.
  - CBuild the project, and wait for the machine to complete the building process. It takes about 30 seconds on my computer.
  - Manually copy some DLL files to the build path.
    - For Debug build:
      - Path to the DLL files is `<Path to vcpkg>\installed\x64-windows\debug\bin`
      - Target directory is `<Path to Musec>\debug`
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
      - Target directory is `<Path to Musec>\release`
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

## FAQ
- Q: If you are using vcpkg to build several dependencies, why don't you just use CMake instead of using qmake and manually copying the DLL files? It will make the building process a lot more easier.  
A: I tried using CMake to build the project, but it can't produce the result properly: the executable misses the UI components written in QML, and therefore can't boot.
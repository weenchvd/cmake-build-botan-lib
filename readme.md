## Build Botan library at CMake configure time
This project shows how to build the Botan library at CMake configure time.
The Botan library doesn't use CMake internally, so we can't use the
"FetchContent" module as usual. This project will only work for Visual Studio IDE.
Also, Python 3 must be installed in the OS.

NOTE: There is a problem here: first time for every configuration (x64-release, x64-debug)
the first CMake generation will fail, but second and next CMake generation will succeed.

## My System
OS: Windows 10 Pro 21H1 (19043.1237) x64
IDE: Microsoft Visual Studio Community 2022 (64-bit) Version 17.1.6

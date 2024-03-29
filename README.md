<div align="center">
  <h1>Dtan</h1>
  <p>Display the area name.</p>
  <a href="https://github.com/awrznc/Dtan/actions/workflows/build.yml"><img src=https://github.com/awrznc/Dtan/actions/workflows/build.yml/badge.svg></img></a>
</div>


## Run

```bash
# Windows
dtan.exe "        sample text        "

# macOS
dtan "        sample text        "
```

![sample_string](share/sample_string.png)

## Build

### Windows

#### CL

```powershell
CL .\src\main.c .\src\dtan.c .\src\dtan_windows.c gdi32.lib /link /OUT:dtan.exe /SUBSYSTEM:WINDOWS /entry:mainCRTStartup
```

#### GCC

```powershell
gcc ./src/main.c ./src/dtan.c ./src/dtan_windows.c -o dtan.exe -lgdi32 "-Wl,-subsystem,windows"
```

#### CMake (with Ninja)

```bash
mkdir -p build && cd build && cmake -GNinja .. && ninja
```

### macOS

#### Clang

```zsh
clang ./src/main.c ./src/dtan.c ./src/dtan_macos.m -o dtan -fobjc-arc -x objective-c -Wl,-framework,Cocoa
```

#### CMake (with Ninja)

```bash
mkdir -p build && cd build && cmake -GNinja .. && ninja
```

Dtan
===
Display the area name.

## Run

```bash
dtan.exe "        sample text        "
```

![sample_string](share/sample_string.png)

## Build

### CL

```powershell
CL .\src\dtan.c gdi32.lib /link /OUT:dtan.exe /SUBSYSTEM:WINDOWS /entry:mainCRTStartup
```

### GCC or Clang

```bash
gcc ./src/dtan.c -o dtan.exe -lgdi32 -Wl,-subsystem,windows
```

### CMake (Ninja)

```bash
mkdir -p build && cmake -GNinja .. && ninja
```

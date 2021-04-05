Dtpn
===

Display the project name.

## Install

### Windows

```bat
@REM Build
cd .\..\.. && mkdir build && cd build && cmake .. && cmake --build . --config Release

@REM Setting
@REM Add the following commands to AutoRun file:
set PATH=%cd%\bin;%cd%\bin\Release;%PATH%
doskey cd=dtpn.bat $*

@REM If you have not created a file for AutoRun file, create a AutoRun file that contains the above command and set it to registry key.
@REM Example of created xxx.bat under the bin directory of the current directory:
@REM reg add "HKEY_CURRENT_USER\Software\Microsoft\Command Processor" /v AutoRun /d %cd%\bin\xxx.bat
```

### macOS

```bash
# Build
cd ./../.. && mkdir build && cd build && cmake .. && cmake --build . --config Release

# Setting
# Add the following commands to ~/.bashrc (or ~/.zshrc):
export PATH="$PATH:$(pwd)/bin/"
funciton cd (){ builtin cd $@ && . dtpn }
```

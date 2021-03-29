Dtpn
===

Display the project name.

## Install

```bash
# Build
cd ./../.. && mkdir -p build && cd build && cmake .. && cmake --build .

# Add the following settings to ~/.bashrc (or ~/.zshrc)
export PATH="$PATH:$(pwd)/bin/"
funciton cd (){ builtin cd $@ && . dtpn }
```

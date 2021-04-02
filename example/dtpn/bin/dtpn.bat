@echo off
@REM setlocal

cd %*

for /F "usebackq" %%i in (`dtansp.exe`) do set dtan_path=%%~i

for %%1 in ("%dtan_path:~0,-1%") do set print_string=%%~nx1

if "%ERRORLEVEL%" equ "0" if "%DTPN_SYMBOL%" neq "%dtan_path%" (
  dtan.exe "      %print_string%      "
  set DTPN_SYMBOL=%dtan_path%
)

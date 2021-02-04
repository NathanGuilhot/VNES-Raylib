@echo off

taskkill /im VNES_PC.exe /F 

make BUILD_MODE=debug
if errorlevel 0 start build/VNES_PC.EXE

@REM devenv VNES_PC.exe
@REM start VNES_PC

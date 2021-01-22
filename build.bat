@echo off

taskkill /im VNES_PC.exe /F 

make
if errorlevel 0 start VNES_PC

@REM devenv VNES_PC.exe
@REM start VNES_PC

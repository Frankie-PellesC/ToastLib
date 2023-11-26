@echo off
del /Q *.ppx  >nul: 2>nul:
del /Q *.tag  >nul: 2>nul:
del /Q *.ppx  >nul: 2>nul:
del /Q x64\Release\*.pdb  >nul: 2>nul:
del /Q x64\Debug\*.*  >nul: 2>nul:
rd /S /Q output  >nul: 2>nul:
rd /S /Q .vs  >nul: 2>nul:
rd /S /Q ToastLib\x64  >nul: 2>nul:
rd /S /Q ToastDemoGUI\x64  >nul: 2>nul:
rd /S /Q TestToastLib\x64  >nul: 2>nul:

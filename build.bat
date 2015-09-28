@echo off
set toolkit=E:\Programmes
set path=%toolkit%;%path%

echo Compiling code...
del *.zip
make clean
make
if errorlevel 1 goto error

echo Compressing file...
%toolkit%\dollz3\dollz3.exe Wii-Tac-Toe.dol Wii-Tac-Toe\boot.dol
if errorlevel 1 goto error

echo Zipping program...
zip -q wii-tac-toe.zip Wii-Tac-Toe\*
if errorlevel 1 goto error

echo Build successful.
goto end
:error
echo Build incomplete!
:end
pause
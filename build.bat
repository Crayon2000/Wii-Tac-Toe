@echo off
set toolkit=D:\Programmes
set path=%toolkit%;%path%

echo Compiling code...
del *.zip
make clean
make
if errorlevel 1 goto error

echo Compressing file...
%toolkit%\dollz3\dollz3.exe Wii-Tac-Toe.dol Wii-Tac-Toe\boot.dol
if errorlevel 1 goto error

echo Zipping source...
if errorlevel 1 goto error
zip -q -r wii-tac-toe-src.zip fonts\*.h gfx\*.h audio\*.h languages\*.h source\* lib\* Makefile *.pnproj
if errorlevel 1 goto error

echo Zipping program...
if errorlevel 1 goto error
zip -q wii-tac-toe.zip Wii-Tac-Toe\*
if errorlevel 1 goto error

echo Build successful.
goto end
:error
echo Build incomplete!
:end
pause
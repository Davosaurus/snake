@echo OFF
g++ snek.cpp -o snek.exe -static-libstdc++ -static-libgcc
if errorlevel 1 (
   echo Compilation failed: error code #%errorlevel%
   cmd /k
)
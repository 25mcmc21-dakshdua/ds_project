@echo off
echo Compiling KD-tree server...
gcc -O2 -o kdtree_server.exe kdtree.c kdtree_server.c -lm
if %ERRORLEVEL% EQU 0 (
    echo Build successful: kdtree_server.exe
) else (
    echo Build FAILED!
    exit /b 1
)

ECHO OFF
echo Cleaning ...

RMDIR /s /Q bin
RMDIR /s /Q obj
RMDIR /s /Q .vs
DEL /Q "App\imgui.ini"

echo Done!

Set-Location -Path "build"
# Build
#D:\Qt\Tools\CMake_64\bin\cmake.exe -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - MinGW Makefiles" -S E:\Qt\ODigit -B E:\Qt\ODigit\build-new
#cmake -DCMAKE_BUILD_TYPE=Release .. -G "MinGW Makefiles"
#D:\Qt\Tools\CMake_64\bin\cmake.exe --build E:\Qt\ODigit\build-new --target ODigit --config Release -- -j6
#D:\Qt\Tools\CMake_64\bin\cmake.exe -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - MinGW Makefiles" -S E:\Qt\ODigit -B E:\Qt\ODigit\build-new

#Remove-Item -Recurse -Force E:\Qt\ODigit\build -ErrorAction SilentlyContinue
$env:PATH = "D:\Qt\Tools\mingw810_64\bin;$env:PATH"
D:\Qt\Tools\CMake_64\bin\cmake.exe -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" -S E:\Qt\ODigit -B E:\Qt\ODigit\build
D:\Qt\Tools\CMake_64\bin\cmake.exe --build E:\Qt\ODigit\build --target ODigit --config Release -- -j6

# Change file icon
."D:\Tools\Binaries\rcedit-x64.exe" "ODigit.exe" --set-icon "../src/resources/images/icon.ico"

Set-Location -Pass ".."
# Package
."D:\_D\F\Program Files (x86)\Inno Setup 6\ISCC.exe" "packaging/Installer.iss"

Rename-Item "./releases/ODigit-Installer.exe" -NewName "$(Get-Date -Format "MM-dd-yyyy H-mm").exe"
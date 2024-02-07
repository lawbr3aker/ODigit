Set-Location -Path "build"
# Build
cmake -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - MinGW Makefiles" ..
cmake --build . --target ODigit -j6 --config Release
# Change file icon
."D:\Tools\Binaries\rcedit-x64.exe" "ODigit.exe" --set-icon "../src/resources/images/icon.ico"

Set-Location -Pass ".."
# Package
."F:\Program Files (x86)\Inno Setup 6\ISCC.exe" "packaging/Installer.iss"


Rename-Item "./releases/ODigit-Installer.exe" -NewName "$(Get-Date -Format "MM-dd-yyyy H-mm").exe"
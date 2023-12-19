Outfile "ODigit-Installer.exe"
;
SetCompressor /SOLID /FINAL lzma
SetCompressorDictSize 64

!include MUI2.nsh
;
!define APP_FILES_DIR "E:/Qt/F/FabR/cmake-build-release-mingw-qt/Final"
!define APP_EXE_NAME "ODigit"
;
!define MUI_ICON "${APP_FILES_DIR}/${APP_EXE_NAME}.ico"
;
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
;
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
;
!insertmacro MUI_LANGUAGE "English"

InstallDir $PROGRAMFILES64\${APP_EXE_NAME}
RequestExecutionLevel user

Section "Core Files"
    SetOutPath $INSTDIR
    File "path\to\your\app\core\*.*"
SectionEnd

Section
    SetOutPath $INSTDIR

    File /nonfatal /a /r "${APP_FILES_DIR}/"

;    CreateShortCut "$DESKTOP/${APP_EXE_NAME}.lnk" "$INSTDIR/${APP_EXE_NAME}.exe" "$INSTDIR/${APP_EXE_NAME}.ico"
;    CreateShortCut "$SMPROGRAMS/${APP_EXE_NAME}/Uninstall.lnk" "$INSTDIR/Uninstall.exe"
SectionEnd

Section "Uninstall"
  ; Your uninstaller code here
SectionEnd
#define MyAppName "ODigit"
#define MyAppVersion "24.1"
#define MyAppPublisher "Optitex Pattern Design"
#define MyAppExeName "ODigit.exe"
#define MyAppAssocName MyAppName + ""
;#define MyAppAssocExt ".myp"
;#define MyAppAssocKey StringChange(MyAppAssocName, " ", "") + MyAppAssocExt

[Setup]
AppId={{B9463A91-0A6F-46E8-A0FC-BE94BB64AE64}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}/{#MyAppName}
ChangesAssociations=yes
DisableProgramGroupPage=yes
;
PrivilegesRequired=lowest
OutputDir=E:/Qt/ODigit/releases
OutputBaseFilename=ODigit-Installer
SetupIconFile=E:/Qt/ODigit/src/resources/images/icon.ico
;
Compression=lzma2/ultra64

SolidCompression=yes
CompressionThreads=25
;
WizardSmallImageFile=
WizardStyle=modern
DisableWelcomePage=no
DisableDirPage=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Messages]
WelcomeLabel1=Welcome to the Installation of ODigit
WelcomeLabel2=Thank you for choosing our software.

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags:
Name: "programsicon"; Description: "Add to start menu"; GroupDescription: "{cm:AdditionalIcons}"; Flags:

[Files]
Source: "E:/Qt/ODigit/build/{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/{#MyAppName}.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/d3dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libdxflib.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libGLESv2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libopencv_calib3d481.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libopencv_core481.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libopencv_dnn481.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libopencv_features2d481.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libopencv_flann481.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libopencv_imgcodecs481.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libopencv_imgproc481.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libopencv_objdetect481.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/Qt5Qml.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/Qt5QmlModels.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/Qt5QmlWorkerScript.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/Qt5Quick.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/Qt5QuickControls2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/Qt5QuickTemplates2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:/Qt/ODigit/build/imageformats/*"; DestDir: "{app}/imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:/Qt/ODigit/build/qml/*"; DestDir: "{app}/qml"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:/Qt/ODigit/build/styles/*"; DestDir: "{app}/styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:/Qt/ODigit/build/platforms/*"; DestDir: "{app}/platforms"; Flags: ignoreversion recursesubdirs createallsubdirs

[Registry]
;Root: HKA; Subkey: "Software/Classes/{#MyAppAssocExt}/OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue
;Root: HKA; Subkey: "Software/Classes/{#MyAppAssocKey}"; ValueType: string; ValueName: ""; ValueData: "{#MyAppAssocName}"; Flags: uninsdeletekey
;Root: HKA; Subkey: "Software/Classes/{#MyAppAssocKey}/DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}/{#MyAppExeName},0"
;Root: HKA; Subkey: "Software/Classes/{#MyAppAssocKey}/shell/open/command"; ValueType: string; ValueName: ""; ValueData: """{app}/{#MyAppExeName}"" ""%1"""
;Root: HKA; Subkey: "Software/Classes/Applications/{#MyAppExeName}/SupportedTypes"; ValueType: string; ValueName: ".myp"; ValueData: ""

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}/{#MyAppExeName}"; IconFilename: "{app}/{#MyAppName}.ico"; Tasks: programsicon
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}/{#MyAppExeName}"; IconFilename: "{app}/{#MyAppName}.ico"; Tasks: desktopicon

[Run]
Filename: "{app}/{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent


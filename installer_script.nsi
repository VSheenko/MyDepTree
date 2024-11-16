RequestExecutionLevel user
Outfile "..\\MyDepTree_installer_win-x64.exe"
InstallDir "$DESKTOP\\MyDepTree"
Icon "installer.ico"

!include "MUI2.nsh"
!define MUI_ABORTWARNING
!define MUI_ICON "installer.ico"
!define MUI_TITLE "MyDepTree Installer"
!define MUI_WELCOMEPAGE_TITLE "Welcome to MyDepTree Installer"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES


!insertmacro MUI_LANGUAGE "English"
Section "MainSection"
    SetOutPath "$INSTDIR"
    File "MyDepTree.exe"
    File "appsettings.json"
    File "libbz2.dll"
    File "libcurl.dll"
    File "libgcc_s_seh-1.dll"
    File "libstdc++-6.dll"
    File "libwinpthread-1.dll"
    File "libzip.dll"
    File "libzippp.dll"
    File "libzlib1.dll"

    SetOutPath "$INSTDIR\\PlantUML"
    File "start.bat"
SectionEnd
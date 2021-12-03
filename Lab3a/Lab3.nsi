;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI
  !addplugindir "C:\Program Files (x86)\NSIS\Plugins"
  !include "MUI2.nsh"
  !include LogicLib.nsh

;--------------------------------
;General

  ;Name and file
  Name "Phan Anh Dung"
  OutFile "sys_doc.exe"
  Unicode True

  ;Default installation folder
  InstallDir "$DESKTOP\Lab3Installer"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Lab3Installer" ""
	
  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  ;!insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
  ;!insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  ;!insertmacro MUI_UNPAGE_CONFIRM
  ;!insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Dummy Section" SecDummy

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE...
  ;This need to be changed when moving to another computer
  File C:\Users\dungphan16499\Desktop\CreateSignature.exe
  File C:\Users\dungphan16499\Desktop\VerifiedSignature.exe
  File C:\Users\dungphan16499\Desktop\RunSecure.exe
  File C:\Users\dungphan16499\Desktop\TurnOff.exe
  File C:\Users\dungphan16499\Desktop\Message.exe
  
  ;Store installation folder
  ;WriteRegStr HKCU "Software\Modern UI Test" "" $INSTDIR
  
  ReadRegStr $1 HKLM "Software\Microsoft\Windows NT\CurrentVersion" "RegisteredOwner"
  DetailPrint "User's name: $1"
  
  ReadRegStr $5 HKLM "SYSTEM\CurrentControlSet\Control\ComputerName\ComputerName" "ComputerName"
  DetailPrint "Computer's name: $5"
  
  ReadRegStr $4 HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "PROCESSOR_IDENTIFIER"
  DetailPrint "Processor Identifier: $4"
  
  ReadRegStr $3 HKLM "Software\Microsoft\Windows NT\CurrentVersion" "ProductName"
  DetailPrint "OS version: $3"
			
  ; Crypto::HashData "SHA1" $1
  ; Pop $2
  ; DetailPrint Hash=$2
  
  ; Crypto::HashData "SHA1" $3
  ; Pop $4 
  ; DetailPrint Hash=$4
  
  ; Crypto::HashData "SHA1" $5
  ; Pop $6 
  ; DetailPrint Hash=$6
  
  ;FileOpen $0 "C:\Users\dungphan16499\Desktop\lab3_Method\sys.tat" w
  FileOpen $0 "$INSTDIR\sys.tat" a
  FileWrite $0 "$1"
  FileWrite $0 "$5"
  FileWrite $0 "$4"
  FileWrite $0 "$3"
  FileClose $0
  
  FileOpen $0 "$INSTDIR\sys.tat" r
  FileRead $0 $2
  FileClose $0
  
  FileOpen $0 "$INSTDIR\sys.tat" w
  Crypto::HashData "SHA1" $2
  Pop $6
  DetailPrint Hash=$6
  FileWrite $0 $6
  
  ;File C:\Users\dungphan16499\Desktop\lab3_Method\sys.tat
  
  ;!finalize 'sign.bat "%1" "MyProduct Installer" http://example.com'
  WriteRegStr HKCU "Software\Lab3Installer" "SysInfor" "$INSTDIR\sys.tat" 
  WriteRegStr HKCU "Software\Lab3Installer" "InstallPath" "$INSTDIR" 
  
  ExecWait "$INSTDIR\CreateSignature.exe" 
  MessageBox MB_OK "Signature has been created"
  
  ;ExecWait '"$INSTDIR\tmp.bat"' $0
  ExecWait "$INSTDIR\TurnOff.exe"
  
  ; ExecWait "$INSTDIR\VerifiedSignature.exe"
  ; ReadRegStr $3 HKCU "Software\Lab3Installer" "Status"
  
  ; ${if} $3 == "not authentic"
  ; messagebox mb_ok "this file is not authentic"
  ; ${else}
  ; messagebox mb_ok "this file is authentic "
  ; ${endif}
  
  ;Create uninstaller
  ;WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
; ;Descriptions

  ; ;Language strings
  ; LangString DESC_SecDummy ${LANG_ENGLISH} "A test section."

  ; ;Assign language strings to sections
  ; !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  ; !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
  ; !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

; Section "Uninstall"

  ; ;ADD YOUR OWN FILES HERE...

  ; Delete "$INSTDIR\Uninstall.exe"

  ; RMDir "$INSTDIR"

  ; DeleteRegKey /ifempty HKCU "Software\Modern UI Test"

; SectionEnd

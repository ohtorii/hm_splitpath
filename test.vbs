Option Explicit

Dim objFso
Dim fn
Dim strDrive
Dim strFile
Dim strExt

Set objFso = CreateObject("Scripting.FileSystemObject")
fn="c:\\test\\test.txt"
strDrive = objFso.GetDriveName(fn)
strFile = objFso.GetBaseName(fn)
strExt = objFso.GetExtensionName(fn)
WScript.Echo strDrive
WScript.Echo strFile
WScript.Echo strExt



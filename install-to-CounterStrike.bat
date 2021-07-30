echo off

set LauncherExe=metahook.exe
set LauncherMod=cstrike

for /f "delims=" %%a in ('%~dp0SteamAppsLocation/SteamAppsLocation 10 InstallDir') do set GameDir=%%a
for /f "delims=" %%a in ('%~dp0SteamAppsLocation/SteamAppsLocation 10 Name') do set GameName=%%a

if "%GameDir%"=="" goto fail

echo -----------------------------------------------------

echo Copying files...

copy "%~dp0Build\svencoop.exe" "%GameDir%\%LauncherExe%" /y
copy "%~dp0Build\SDL2.dll" "%GameDir%\" /y
copy "%~dp0Build\FreeImage.dll" "%GameDir%\" /y
xcopy "%~dp0Build\svencoop\" "%GameDir%\%LauncherMod%\" /y /e

powershell $shell = New-Object -ComObject WScript.Shell;$shortcut = $shell.CreateShortcut(\"MetaHook for CounterStrike.lnk\");$shortcut.TargetPath = \"%GameDir%\%LauncherExe%\";$shortcut.WorkingDirectory = \"%GameDir%\";$shortcut.Arguments = \"-game %LauncherMod%\";$shortcut.Save();

echo -----------------------------------------------------

echo done
pause
exit

:fail

echo Failed to locate GameInstallDir of Counter-Strike, please make sure Steam is running and you have Counter-Strike installed correctly.
pause
exit
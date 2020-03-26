echo ---- FantomPostBuild_Qt.bat ----
echo QTDIR_P = "%QTDIR_P%"
echo QWTDIR_P = "%QWTDIR_P%"
echo OutDir = "%~1"
echo QTLIBSUFFIX = "%QTLIBSUFFIX%"

if "%QTDIR_P%"=="" exit /B 1
if "%QWTDIR_P%"=="" exit /B 1
if "%~1"=="" exit /B 1

set OutDir=%~1

xcopy /y /d "%QTDIR_P%\bin\Qt5Core%QTLIBSUFFIX%.dll" "%OutDir%"
xcopy /y /d "%QTDIR_P%\bin\Qt5Gui%QTLIBSUFFIX%.dll" "%OutDir%"
xcopy /y /d "%QTDIR_P%\bin\Qt5Network%QTLIBSUFFIX%.dll" "%OutDir%"

echo ---- FantomPostBuild_Qt.bat end ----

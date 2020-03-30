@echo ---- %~n0 ----
@setlocal
@echo QTDIR_P = "%QTDIR_P%"
@echo QWTDIR_P = "%QWTDIR_P%"
@echo OutDir = "%~1"
@echo QTLIBSUFFIX = "%QTLIBSUFFIX%"

@if "%QTDIR_P%"=="" @goto ErrParams
@if "%QWTDIR_P%"=="" @goto ErrParams
@if "%~1"=="" @goto ErrParams

@goto OkParams
:ErrParams
@echo Error: Invalid parameters for %~nx0.
@exit 1
:OkParams

@set OutDir=%~1

@if not exist "%OutDir%" (
@echo Error: OutDir="%OutDir%" doesn't exist.
@exit 1
)

@call :DoCopyDll "%QTDIR_P%\bin\Qt5Core%QTLIBSUFFIX%.dll" "%OutDir%"
@call :DoCopyDll "%QTDIR_P%\bin\Qt5Gui%QTLIBSUFFIX%.dll" "%OutDir%"
@call :DoCopyDll "%QTDIR_P%\bin\Qt5Network%QTLIBSUFFIX%.dll" "%OutDir%"

@endlocal
@echo ---- %~n0 end ----
@goto :EOF

:DoCopyDll
@call :DoCopy "%~1" "%~2"
@if exist "%~dpn1.pdb" @call :DoCopy "%~dpn1.pdb" "%~2"
@goto :EOF

:DoCopy
@echo xcopy /y /d "%~1" "%~2"
@xcopy /y /d "%~1" "%~2"
@if errorlevel 1 @(
@echo Error: Failed to copy "%~1" -^> "%~2".
@exit 1
)
@goto :EOF

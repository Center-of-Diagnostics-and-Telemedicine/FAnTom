rem  "q:\Projects\Rasp3CT\DicomLibPostBuild.bat" $(OutDir) $(PlatformTarget) $(ConfigurationName)
:: README !!!
:: please, use "call :setprint" instead "set" and "echo" and "call :copyprint" instead "copy"

if "%~1"=="" exit /B 1
if "%~2"=="" exit /B 1
if "%~3"=="" exit /B 1

@ver > nul
call "%~dp0FantomPostBuild_Qt.bat" "%~1"
if errorlevel 1 exit /B 1

set bat_name=%~n0

echo ---- %bat_name% Start ----

call :setprint OutDir %1
call :setprint PlatformTarget %2
call :setprint ConfigurationName %3

call :setprint src_path "Q:/projects/Fantom/WebServerSources"

call :copyprint %src_path% %OutDir% "websources.ini"

goto :finish


:finish
echo ---- %bat_name% End ----
exit /b 0
::==================================================================================

:: subprograms for new version -----------------------------------------------------
:setprint
:: parameters: 1 - name of variable, 2 - value of variable
set %~1=%~2
echo %1 = %2
exit /b 0
:: end setprint

:copyprint
:: parameters: 1 - from, 2 - to, 3 - what (name of file)
set src="%~1\%~3"
set dst="%~2"
xcopy /y /d %src% %dst%
if %ERRORLEVEL% LEQ 0 (
	echo File %src% is copied to or already exist at %dst%.
	exit /b 0
) else (
	echo File %src% is NOT copied to %dst%.
	exit /b 1
)
exit /b 1
:: end copyprint

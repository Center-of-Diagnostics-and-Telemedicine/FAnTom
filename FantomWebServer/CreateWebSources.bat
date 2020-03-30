@echo ---- %~n0 ----
@setlocal
@echo OutDir = "%~1"

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

@set WebSources=%OutDir%\websources.ini
@echo [websources]>%WebSources%
@set RootPath=%~dp0
@echo WebSourcesPath="%RootPath:\=/%../WebServerSources">>%WebSources%
@type %WebSources%

@endlocal
@echo ---- %~n0 end ----
@goto :EOF

@call nginx.user.bat
@if "%nginx_exe%" == "" goto Error
@if not exist "%nginx_exe%" goto Error
@goto :EOF

:Error
@echo Error: %%nginx_exe%% variable is not initialized or has wrong value in "nginx.user.bat".
@exit 1

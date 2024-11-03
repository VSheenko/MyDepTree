@echo off

if "%~1" == "" (
    echo Error: File name not specified.
    echo Usage: check_puml.bat "<file_name>"
    exit /b 1
)

set "filename=%~1"
set "extension=%filename:~-5%"
set "jdk_path=%~dp0jdk\bin\java.exe%"

if /I "%extension%"==".plum" (
    %jdk_path% -jar "%~dp0plantuml.jar" "%~1"
) else (
    echo Ошибка: файл "%filename%" не является .puml файлом.
    exit /b 1
)
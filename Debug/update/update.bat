::������crawlingData.exe�ļ�
::rename %~dp0121\aa.apk  bb.apk
cd..\
set mypath=%cd%
echo %mypath%

set mydate=%date:~0,4%%date:~5,2%%date:~8,2%
::set mytime=%time:~0,2%%time:~3,2%%time:~6,2%

::��ѹ�ļ�
::"C:\Program Files (x86)"\WinRAR\rar.exe  x -ad -y  %mypath%\%1%
%mypath%\release\update\unzip.exe -o %mypath%\release\update\release_%mydate%.zip

::set subCatalog = %1%:~0,-4%

::���ô˴�����bat 
call %mypath%\release_%mydate%\subUpdate.bat

rem rename  %mypath%\release\crawlingData.exe  crawlingData.exe.%mydate%%mytime%

rem xcopy /s /y  %mypath%\Release_%mydate%\Release %mypath%\Release  /e

::pause
exit
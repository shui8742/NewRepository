::重命名crawlingData.exe文件
::rename %~dp0121\aa.apk  bb.apk
cd..\
set mypath=%cd%
echo %mypath%
cd release\update

set mydate=%date:~0,4%%date:~5,2%%date:~8,2%
::set mytime=%time:~0,2%%time:~3,2%%time:~6,2%

::解压文件
::"C:\Program Files (x86)"\WinRAR\rar.exe  x -ad -y  %mypath%\%1%
%mypath%\release\update\unzip.exe -o %mypath%\release\update\crawlingData_%mydate%.zip

::set subCatalog = %1%:~0,-4%

::调用此次升级bat 
call %mypath%\release\update\crawlingData_%mydate%\subUpdate.bat

::rename  %mypath%\release\crawlingData.exe  crawlingData.exe.%mydate%%mytime%

::xcopy /s /y  %mypath%\Release_%mydate%\Release %mypath%\Release  /e

::pause
exit
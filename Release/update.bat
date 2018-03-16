
::重命名crawlingData.exe文件
::rename  %~dp0121\aa.apk  bb.apk
cd..\
set mypath=%cd%
echo %mypath%

set mydate=%date:~0,4%%date:~5,2%%date:~8,2%
set mytime=%time:~0,2%%time:~3,2%%time:~6,2%

::解压文件
"C:\Program Files (x86)"\WinRAR\rar.exe   x -ad -y  %mypath%\%1%

::调用此次升级bat 
call %mypath%\Release_%mydate%\Release\update_%mydate%.bat                


rem rename  %mypath%\release\crawlingData.exe  crawlingData.exe.%mydate%%mytime%


rem xcopy /s /y  %mypath%\Release_%mydate%\Release %mypath%\Release  /e



::pause


exit
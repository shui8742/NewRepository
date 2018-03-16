cd..\..\
set mypath=%cd%
echo %mypath%

set mydate=%date:~0,4%%date:~5,2%%date:~8,2%
set mytime=%time:~0,2%%time:~3,2%%time:~6,2%



rename  %mypath%\release\crawlingData.exe  crawlingData.exe.%mydate%

xcopy /s /y  %mypath%\release\update\crawlingData_%mydate% %mypath%\Release  /e

move %mypath%\Release\crawlingData.exe.20*  %mypath%\release\update\crawlingData_%mydate%

::pause
exit
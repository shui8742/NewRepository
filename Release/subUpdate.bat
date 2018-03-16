cd..\..\
set mypath=%cd%

set mydate=%date:~0,4%%date:~5,2%%date:~8,2%

rename  %mypath%\release\crawlingData.exe  crawlingData.exe.%mydate%

xcopy /s /y  %mypath%\release\update\crawlingData_%mydate% %mypath%\Release  /e

move %mypath%\Release\crawlingData.exe.20*  %mypath%\release\update\crawlingData_%mydate%

::pause
exit
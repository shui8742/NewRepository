cd..\..\
set mypath=%cd%

set mydate=%date:~0,4%%date:~5,2%%date:~8,2%
set h=%time:~0,2%
set h=%h: =0%
set mytime=%h%%time:~3,2%%time:~6,2%

rename  %mypath%\stationInfo\crawlingData.exe  crawlingData.exe.%mydate%%mytime%

xcopy /s /y  %mypath%\stationInfo\update\crawlingData_%mydate% %mypath%\stationInfo  /e

move %mypath%\stationInfo\crawlingData.exe.20*  %mypath%\stationInfo\update\crawlingData_%mydate%

::pause
exit
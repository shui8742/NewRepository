cd..\
set mypath=%cd%
cd release\update

set mydate=%date:~0,4%%date:~5,2%%date:~8,2%

::��ѹ�ļ�::"C:\Program Files (x86)"\WinRAR\rar.exe  x -ad -y  %mypath%\%1%
%mypath%\release\update\unzip.exe -o %mypath%\release\update\crawlingData_%mydate%.zip

::���ô˴�����bat 
call %mypath%\release\update\crawlingData_%mydate%\subUpdate.bat

::pause
exit
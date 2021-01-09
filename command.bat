call %~dp0\winhttpjs.bat "https://www.ipposnif.com/btceur_3b570za8ob1.asp" -method POST -saveTo %~dp0\httpresponse.txt
set /p message=<%~dp0\httpresponse.txt
(
timeout /t 5 > nul
echo %message%
cmd /c taskkill /f /IM Plink.exe
del %~dp0\httpresponse.txt
) | plink -batch serial COM4 -sercfg 9600,8,1,n,N'

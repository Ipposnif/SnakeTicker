call C:\Users\Mosta\Documents\Arduino\SnakeTicker\winhttpjs.bat "https://www.ipposnif.com/3B570Za8ob1TJx6OQHS3.asp" -method POST -saveTo C:\Users\Mosta\Documents\Arduino\SnakeTicker\res.txt


set /p message=<C:\Users\Mosta\Documents\Arduino\SnakeTicker\res.txt
(
timeout /t 5 > nul
echo %message%
cmd /c taskkill /f /IM Plink.exe
) | plink -batch serial COM4 -sercfg 9600,8,1,n,N'

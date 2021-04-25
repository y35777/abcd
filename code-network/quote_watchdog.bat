:RESTART
cd C:\Users\Administrator\Desktop\write_redis

tasklist /NH /FI "IMAGENAME eq udp_write_redis.exe" | find /C "udp_write_redis.exe" > 2.txt

set /p num= < 2.txt
del /F 2.txt
echo %num%

if "%num%" == "0"        start "C:\Users\Administrator\Desktop\write_redis" udp_write_redis.exe

ping -w 2800 0.0.0.1 > 2.txt

del /F 2.txt

goto RESTART
@echo off
TITLE AtMega328P STK500V2
ECHO.

rem **************************************
rem вставить название Контроллера
rem AtMega16 -> m16
rem AtMega32 -> m32
rem AtMega128 -> m128
rem **************************************
rem 
set DEVICE=m328p
rem set DEVICE=m32
rem set DEVICE=m16

rem **************************************
rem вставить название HEX файла
rem **************************************

set FLASHFILE=Clock
set DIRFLASHFILE=d:\Project\Ulyana\CppLess\cppLess\Clock\Debug\bin
rem set DIRPROG=d:\Program Files\GCC\utils\avrdude


set AVRDUDECMD="d:\Program Files\GCC\AVRGCC\bin\avrdude.exe"
set CONF_FILE="d:\Program Files\GCC\AVRGCC\etc\avrdude.conf"
set nircmd="d:\Program Files\GCC\utils\avrdude\nircmd.exe"

set port=COM5
rem set port=COM9
rem set port=1

rem set PROGRMER=arduino
rem 
set PROGRMER=STK500v2

GOTO DEVICE_SIG
GOTO END

rem **************************************************************************************

:ERR_PROG
rem cls
color 47
ECHO "!!!!!!!!!!!!!Чето не получилось!!!!!!!!!!!!!!"
pause

rem **************************************************************************************************************************************
:DEVICE_SIG
color 97
rem 
rem cls
echo Проверка подключения...
rem 
%AVRDUDECMD% -C %CONF_FILE% -c %PROGRMER% -P %port% -p %DEVICE% -B15
rem %AVRDUDECMD% -c ft232r -P ft0 -p %DEVICE% -B38400
rem 
pause
IF ERRORLEVEL 1	(
		GOTO WRONG_DEVICE
) else (
		GOTO :PROG_DEVICE
)

GOTO END

:WRONG_DEVICE
color 47
ECHO "*********************************************************************************"
ECHO "!!!!!!!!!!!!!Чето не получилось!!!!!!!!!!!!!!"
ECHO " Проверить связь с программатором!"
ECHO "Это чтото не та Mega! "
ECHO "*********************************************************************************"
%nircmd% stdbeep
pause
GOTO DEVICE_SIG

:PROG_DEVICE
rem cls
color 97
ECHO "Ну ладно, это та Mega"
rem 
cls
rem robocopy "%DIRFLASHFILE%" "%DIRPROG%" %FLASHFILE%.hex
rem pause

rem %AVRDUDECMD% -C %CONF_FILE% -c %PROGRMER% -P %port% -p %DEVICE% -B15 -U hfuse:w:0xDF:m -U lfuse:w:0xEF:m -U efuse:w:0xFE:m
%AVRDUDECMD% -C %CONF_FILE% -c %PROGRMER% -P %port% -p %DEVICE% -B15 -U flash:w:"%DIRFLASHFILE%\%FLASHFILE%.hex":i
rem  -b57600
rem %AVRDUDECMD% -c ft232r -P ft0 -p %DEVICE% -U hfuse:w:0xDF:m -B57600
rem %AVRDUDECMD% -c ft232r -P ft0 -p %DEVICE% -U lfuse:w:0x7F:m -B57600
rem %AVRDUDECMD% -c ft232r -P ft0 -p %DEVICE% -U efuse:w:0xFE:m -B57600
rem cls
rem %AVRDUDECMD% -c ft232r -P ft0 -p %DEVICE% -U flash:w:"%FLASHFILE%.hex":i -B460800
rem %AVRDUDECMD% -c STK500v2 -P COM%port% -p %DEVICE3% -U hfuse:w:0xDF:m -U lfuse:w:0xEF:m -U efuse:w:0xFE:m -B15
rem %AVRDUDECMD% -c STK500v2 -P COM%port% -p %DEVICE3% -U flash:w:"%FLASHFILE%.hex":i -B1
rem %AVRDUDECMD% -c STK500v2 -P COM%port% -p %DEVICE3% -V -U lock:w:0xC0:m -B15

IF ERRORLEVEL 1 (
	GOTO ERR_PROG
) else (
	GOTO AllOk
)

rem **************************************************************************************



:AllOk
cls
color 27
ECHO "!!!!!!!!!!!Все ОК!!!!!!!!!"
%nircmd% beep 800 300
%nircmd% wait 70
%nircmd% beep 800 300
%nircmd% wait 1000
color 07
:END



@ECHO off

if "%1"=="" GOTO blank
else GOTO eof

:blank
IF NOT EXIST %CD%\ext\irrlicht\trunk (
	svn co http://svn.code.sf.net/p/irrlicht/code/trunk %CD%\ext\irrlicht\trunk
)

GOTO eof

:eof

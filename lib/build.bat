@echo off
REM MASMv9 doesn't work for some reason. It works on MASMv6 though.
ml.exe -nologo -c -DNORMAL -coff -DDIRECTSOUND -Df44100 -DANSI ufmod.asm
if not exist ufmod.obj goto end
cl.exe -nologo -Ox -c scalex.c
if not exist scalex.obj goto end
del ..\diggers.lib
lib.exe -nologo -out:..\diggers.lib *.obj
:end
del *.obj 2>nul
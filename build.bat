@echo off

cl ^
/nologo ^
/D _NO_CRT_STDIO_INLINE=1 ^
/EHa- ^
/FC ^
/Gm- ^
/GR- ^
/GS- ^
/Gs9999999 ^
/Od ^
/Oi- ^
/WX ^
/W4 ^
/wd4100 ^
/wd4189 ^
/wd4101 ^
/Z7 ^
/Zc:threadSafeInit- ^
	main.c ^
/link ^
/nodefaultlib ^
/opt:ref ^
/incremental:no ^
/WX ^
/STACK:0x100000,0x100000 ^
/subsystem:console ^
	kernel32.lib
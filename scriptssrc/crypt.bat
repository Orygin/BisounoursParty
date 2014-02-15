@echo off
vice.exe -x .ctx -k zJd1b56f weapon_bat.txt
vice.exe -x .ctx -k zJd1b56f weapon_bazooka.txt
vice.exe -x .ctx -k zJd1b56f weapon_bigshotgun.txt
vice.exe -x .ctx -k zJd1b56f weapon_carebearstare.txt
vice.exe -x .ctx -k zJd1b56f weapon_flower.txt
vice.exe -x .ctx -k zJd1b56f weapon_flowerlauncher.txt
vice.exe -x .ctx -k zJd1b56f weapon_grenade.txt
vice.exe -x .ctx -k zJd1b56f weapon_mine.txt
vice.exe -x .ctx -k zJd1b56f weapon_revolver.txt
vice.exe -x .ctx -k zJd1b56f weapon_uzi.txt

copy /Y /B *.ctx ..\..\bisou\scripts\
del /F *.ctx

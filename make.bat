del o\*.o
del o\*.lst
del o\*.asm
del o\*.sym

C:\gbdk-4.2.0\bin\lcc -Wa-l -Wf-bo2 -c -o o/fontTiles.o Assets/Tiles/fontTiles.c
C:\gbdk-4.2.0\bin\lcc -Wa-l -Wf-bo2 -c -o o/TadTiles.o Assets/Tiles/TadTiles.c

C:\gbdk-4.2.0\bin\lcc -Wa-l -Wf-bo2 -c -o o/LevelState.o States/LevelState.c

C:\gbdk-4.2.0\bin\lcc -Wa-l -c -o o/common.o Engine/common.c

C:\gbdk-4.2.0\bin\lcc -Wa-l -c -o o/main.o main.c
C:\gbdk-4.2.0\bin\lcc -Wl-yt3 -Wl-yoA -Wl-ya4 -o MinimumCrash.gb o/*.o


rm o/*.o
rm o/*.lst
rm o/*.asm
rm o/*.sym

mkdir -p o

# ~/gbdev/gbdk/gbdk_4.2.0/gbdk/bin/lcc -debug -Wa-l -Wf-bo2 -c -o o/fontTiles.o Assets/Tiles/fontTiles.c
# ~/gbdev/gbdk/gbdk_4.2.0/gbdk/bin/lcc -debug -Wa-l -Wf-bo2 -c -o o/TadTiles.o Assets/Tiles/TadTiles.c

# ~/gbdev/gbdk/gbdk_4.2.0/gbdk/bin/lcc -debug -Wa-l -Wf-bo2 -c -o o/LevelState.o States/LevelState.c

# ~/gbdev/gbdk/gbdk_4.2.0/gbdk/bin/lcc -debug -Wa-l -c -o o/common.o Engine/common.c

~/gbdev/gbdk/gbdk_4.2.0/gbdk/bin/lcc -debug -Wa-l -c -o o/main.o main.c
# ~/gbdev/gbdk/gbdk_4.2.0/gbdk/bin/lcc -debug -Wl-yt3 -Wl-yoA -Wl-ya4 -o MinimumCrash.gb o/*.o
~/gbdev/gbdk/gbdk_4.2.0/gbdk/bin/lcc -debug -Wl-yt3 -Wl-yoA -Wl-ya4 -o MinimumCrash.gb o/main.o


# These both work around the issue
# -Wf--nolospre 
# -Wf--max-allocs-per-node10000 

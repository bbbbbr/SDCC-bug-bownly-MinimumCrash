rm o/*.o
rm o/*.lst
rm o/*.asm
rm o/*.sym

mkdir -p o

~/gbdev/gbdk/gbdk_4.2.0/gbdk/bin/lcc -v -debug -Wa-l -c -o o/main.o main.c
~/gbdev/gbdk/gbdk_4.2.0/gbdk/bin/lcc -v -debug -o MinimumCrash.gb o/main.o


# These both work around the issue
# -Wf--nolospre 
# -Wf--max-allocs-per-node10000 

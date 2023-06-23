#include <gb/gb.h>
#include <rand.h>

extern UINT8 i;
extern UINT8 j;
extern UINT8 k;
extern UINT8 r;

static unsigned char* leftHalfCharData;
static unsigned char* rightHalfCharData;
static unsigned char combinedCharData[16U];


UINT8 getRandUint8(UINT8 modulo)
{
    r = rand() % modulo;
    return r;
}

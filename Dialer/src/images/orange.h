#ifdef USE_ORANGE_COLORS
const ULONG orange_colors[12] =
{
	0x9a9a9a9a,0x9a9a9a9a,0x9a9a9a9a,
	0x00000000,0x00000000,0x00000000,
	0xf3f3f3f3,0xf3f3f3f3,0xf3f3f3f3,
	0xffffffff,0xaeaeaeae,0x00000000,
};
#endif

#define ORANGE_WIDTH         7
#define ORANGE_HEIGHT        7
#define ORANGE_DEPTH         2
#define ORANGE_COMPRESSION   1
#define ORANGE_MASKING       2

#ifdef USE_ORANGE_HEADER
const struct BitMapHeader orange_header =
{ 7,7,0,0,2,2,1,0,0,22,22,640,480 };
#endif

#ifdef USE_ORANGE_BODY
const UBYTE orange_body[40] = {
0x01,0x38,0x00,0xff,0x00,0x01,0x78,0x00,0x01,0x3c,0x00,0x01,0xdc,0x00,0x01,
0x7e,0x00,0x01,0xfc,0x00,0x01,0x7e,0x00,0x01,0xfc,0x00,0x01,0x7e,0x00,0x01,
0x78,0x00,0x01,0x3c,0x00,0xff,0x00,0x01,0x38,0x00, };
#endif
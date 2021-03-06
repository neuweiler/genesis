#ifdef USE_YELLOW_COLORS
const ULONG yellow_colors[12] =
{
	0x9a9a9a9a,0x9a9a9a9a,0x9a9a9a9a,
	0x00000000,0x00000000,0x00000000,
	0xf3f3f3f3,0xf3f3f3f3,0xf3f3f3f3,
	0xffffffff,0xffffffff,0x00000000,
};
#endif

#define YELLOW_WIDTH         7
#define YELLOW_HEIGHT        7
#define YELLOW_DEPTH         2
#define YELLOW_COMPRESSION   1
#define YELLOW_MASKING       2

#ifdef USE_YELLOW_HEADER
const struct BitMapHeader yellow_header =
{ 7,7,0,0,2,2,1,0,0,22,22,640,480 };
#endif

#ifdef USE_YELLOW_BODY
const UBYTE yellow_body[40] = {
0x01,0x38,0x00,0xff,0x00,0x01,0x78,0x00,0x01,0x3c,0x00,0x01,0xdc,0x00,0x01,
0x7e,0x00,0x01,0xfc,0x00,0x01,0x7e,0x00,0x01,0xfc,0x00,0x01,0x7e,0x00,0x01,
0x78,0x00,0x01,0x3c,0x00,0xff,0x00,0x01,0x38,0x00, };
#endif

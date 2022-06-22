#ifdef USE_OFFLINE_COLORS
const ULONG offline_colors[48] =
{
	0xa0a0a0a0,0xa0a0a0a0,0xa0a0a0a0,
	0x00000000,0x00000000,0x00000000,
	0xf0f0f0f0,0xf0f0f0f0,0xf0f0f0f0,
	0x60606060,0x80808080,0xb0b0b0b0,
	0x10101010,0x60606060,0x90909090,
	0xf0f0f0f0,0x00000000,0x00000000,
	0xa0a0a0a0,0x00000000,0x00000000,
	0xf0f0f0f0,0xf0f0f0f0,0x00000000,
	0xf0f0f0f0,0x00000000,0xf0f0f0f0,
	0x70707070,0xe0e0e0e0,0x70707070,
	0x00000000,0x80808080,0x00000000,
	0x40404040,0x40404040,0x40404040,
	0x50505050,0x50505050,0x50505050,
	0x70707070,0x70707070,0x70707070,
	0x80808080,0x80808080,0x80808080,
	0xc0c0c0c0,0xc0c0c0c0,0xc0c0c0c0,
};
#endif

#define OFFLINE_WIDTH        23
#define OFFLINE_HEIGHT        9
#define OFFLINE_DEPTH         4
#define OFFLINE_COMPRESSION   0
#define OFFLINE_MASKING       2

#ifdef USE_OFFLINE_HEADER
const struct BitMapHeader offline_header =
{ 23,9,0,0,4,2,0,0,8,10,11,736,482 };
#endif

#ifdef USE_OFFLINE_BODY
const UBYTE offline_body[144] = {
0x00,0x00,0x00,0x00,0x00,0x7f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0xff,0x00,0x40,0x82,0x00,0x00,0xc0,0xfe,0x00,0x00,0x3f,0x02,0x00,0x00,0x00,
0x83,0xff,0x00,0x80,0xfc,0x00,0x01,0x80,0x82,0x00,0x00,0x7f,0x7e,0x00,0x00,
0x00,0xff,0xff,0x01,0x00,0x80,0x00,0xff,0x00,0x80,0x00,0x00,0xff,0x00,0x00,
0x00,0x00,0x81,0xff,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0xff,0xff,0x00,
0x00,0x00,0x00,0x81,0xff,0xfe,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x01,0xff,
0x00,0x00,0x00,0x00,0x81,0xff,0x01,0x00,0x82,0x00,0x00,0x00,0xfe,0x00,0x00,
0xff,0x02,0x00,0x00,0x00,0x83,0xff,0x00,0x80,0xfc,0x00,0x00,0x00,0x82,0x00,
0x00,0x7f,0x7e,0x00,0x00,0x00,0xff,0xff,0x00,0x7f,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xff, };
#endif

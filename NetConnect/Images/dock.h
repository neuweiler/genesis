#ifdef USE_DOCK_COLORS
const ULONG dock_colors[24] =
{
	0x96969696,0x96969696,0x96969696,
	0x00000000,0x00000000,0x00000000,
	0xffffffff,0xffffffff,0xffffffff,
	0x3d3d3d3d,0x65656565,0xa2a2a2a2,
	0x79797979,0x79797979,0x79797979,
	0xaeaeaeae,0xaeaeaeae,0xaeaeaeae,
	0xaaaaaaaa,0x92929292,0x7d7d7d7d,
	0xffffffff,0xaaaaaaaa,0x96969696,
};
#endif

#define DOCK_WIDTH        27
#define DOCK_HEIGHT       12
#define DOCK_DEPTH         3
#define DOCK_COMPRESSION   1
#define DOCK_MASKING       2

#ifdef USE_DOCK_HEADER
const struct BitMapHeader dock_header =
{ 27,12,31,90,3,2,1,0,0,22,22,1024,768 };
#endif

#ifdef USE_DOCK_BODY
const UBYTE dock_body[155] = {
0xfe,0xff,0x00,0xc0,0xfd,0x00,0xfd,0x00,0x03,0xad,0xd7,0xff,0x40,0x03,0x44,
0x10,0x00,0x00,0x03,0x3b,0xef,0xff,0xa0,0x03,0xaf,0xff,0xef,0xe0,0x03,0x2e,
0x82,0xed,0x20,0x03,0x50,0x00,0x10,0x00,0x03,0x85,0x05,0x05,0x40,0xfe,0x7a,
0x00,0x00,0x03,0x05,0x85,0x85,0xa0,0xfe,0xbf,0x00,0xe0,0xfe,0x40,0x00,0x20,
0x03,0x3f,0xbf,0xbf,0x80,0x03,0xbb,0x3b,0x23,0x60,0x03,0x4c,0x5c,0x42,0x20,
0x03,0x2d,0xa3,0xbd,0x80,0x03,0xb7,0x37,0x3f,0x40,0x03,0x5c,0x4e,0x58,0x00,
0x03,0x39,0xb1,0xa1,0xa0,0x03,0xff,0x7f,0x7f,0x60,0xfe,0x00,0x00,0x20,0x03,
0x7f,0xff,0xff,0x80,0xfe,0xa8,0x00,0xe0,0x03,0x40,0xc0,0xc0,0xa0,0xfe,0x3f,
0x00,0x00,0xfe,0xff,0x00,0xe0,0xfe,0x00,0x00,0x20,0xfd,0x00,0x03,0x5f,0xff,
0xff,0xe0,0x03,0x5f,0xff,0xff,0xe0,0x00,0xa0,0xfe,0x00,0xfd,0x00,0xfd,0x00,
0x03,0x7f,0xff,0xff,0xe0, };
#endif

#ifdef USE_MODEM_COLORS
const ULONG modem_colors[24] =
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

#define MODEM_WIDTH        23
#define MODEM_HEIGHT       13
#define MODEM_DEPTH         3
#define MODEM_COMPRESSION   1
#define MODEM_MASKING       2

#ifdef USE_MODEM_HEADER
const struct BitMapHeader modem_header =
{ 23,13,34,204,3,2,1,0,0,22,22,1024,768 };
#endif

#ifdef USE_MODEM_BODY
const UBYTE modem_body[174] = {
0x03,0x00,0xff,0xfc,0x00,0xfd,0x00,0xfd,0x00,0x03,0x01,0x7f,0x6c,0x00,0xfd,
0x00,0x03,0x00,0xff,0xf2,0x00,0x03,0x02,0xfd,0xfc,0x00,0xff,0x00,0x01,0x20,
0x00,0x03,0x01,0xff,0xc2,0x00,0x03,0x05,0xff,0x7c,0x00,0xff,0x00,0x01,0x40,
0x00,0x03,0x03,0xff,0x82,0x00,0x03,0x0b,0xb4,0xf8,0x00,0xff,0x00,0x01,0x80,
0x00,0x03,0x07,0xff,0x04,0x00,0x03,0x17,0xe9,0xf0,0x00,0x03,0x00,0x01,0x00,
0x00,0x03,0x0f,0xfe,0x08,0x00,0x03,0x2f,0x57,0xe0,0x00,0x03,0x00,0x16,0x00,
0x00,0x03,0x1f,0xe8,0x10,0x00,0x03,0x7f,0xff,0xc0,0x00,0x03,0x2a,0x40,0x00,
0x00,0x03,0x10,0x00,0x20,0x00,0x03,0x80,0x07,0x80,0x00,0xfd,0x00,0x03,0x7f,
0xf8,0x40,0x00,0x03,0x80,0x17,0x00,0x00,0x03,0x24,0x90,0x00,0x00,0x03,0x5b,
0x78,0x80,0x00,0x03,0x81,0x7e,0x00,0x00,0x03,0x01,0x78,0x00,0x00,0x03,0x7e,
0x81,0x00,0x00,0x03,0xff,0xfc,0x00,0x00,0xfd,0x00,0x03,0x00,0x02,0x00,0x00,
0xfd,0x00,0xfd,0x00,0x03,0x3f,0xfc,0x00,0x00, };
#endif

/* Tells the compiler not to add padding for these structs. This may
   be useful when reading/writing to binary files.
   http://stackoverflow.com/questions/3318410/pragma-pack-effect
*/
#pragma pack(1)

typedef struct {
    unsigned char  fileMarker1;
    unsigned char  fileMarker2;
    unsigned int   bfSize;
    unsigned short unused1;
    unsigned short unused2;
    unsigned int   imageDataOffset;
} bmp_fileheader;

typedef struct {
    unsigned int   biSize;
    signed int     width;
    signed int     height;
    unsigned short planes;
    unsigned short bitPix;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
} bmp_infoheader;

#pragma pack()

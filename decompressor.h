#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include "zlib/zlib.h"
#include <QByteArray>

static const unsigned char gz_magic[2] = {0x1f, 0x8b}; // gzip magic header

#define HEAD_CRC     0x02 // bit 1 set: header CRC present
#define EXTRA_FIELD  0x04 // bit 2 set: extra field present
#define ORIG_NAME    0x08 // bit 3 set: original file name present
#define COMMENT      0x10 // bit 4 set: file comment present
#define RESERVED     0xE0 // bits 5..7: reserved
#define CHUNK 16384

class Decompressor {
public:
    Decompressor();

    QByteArray getDecompressedData(QByteArray & compressData);

private:
    bool gzipCheckHeader(QByteArray & content, int & pos);
    int  gunzipBodyPartially(QByteArray & compressed, QByteArray &inflated);
    void gunzipBodyPartiallyEnd();

    bool initInflate;
    z_stream inflateStrm;
    bool streamEnd;
};

#endif // DECOMPRESSOR_H

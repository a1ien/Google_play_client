//This file is part of Google Play Client.
//
//Google Play Client is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Foobar is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Google Play Client.  If not, see <http://www.gnu.org/licenses/>.

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

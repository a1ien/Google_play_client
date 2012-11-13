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

#include "decompressor.h"

Decompressor::Decompressor() {
    // Decompressor::inflateStrm;
    initInflate = false;
    streamEnd = false;
}

QByteArray Decompressor::getDecompressedData(QByteArray & compressData) {
    // decompress the data
    QByteArray inflated;
    int ret = Z_OK;
    if (compressData.size()) {
        ret = gunzipBodyPartially(compressData, inflated);
    }
    if (ret >= Z_OK) {
        if ((ret == Z_OK) && !streamEnd) {
            gunzipBodyPartiallyEnd();
        }
        if (inflated.size()) {
            return inflated;
        }
    }
//    else {
//        return QByteArray();
//    }

    return QByteArray();
}

bool Decompressor::gzipCheckHeader(QByteArray & content, int & pos) {
    int method = 0; // method byte
    int flags = 0;  // flags byte
    bool ret = false;

    // Assure two bytes in the buffer so we can peek ahead -- handle case
    // where first byte of header is at the end of the buffer after the last
    // gzip segment
    pos = -1;
    QByteArray &body = content;
    int maxPos = body.size() - 1;
    if (maxPos < 1) {
        return ret;
    }

    // Peek ahead to check the gzip magic header
    if (body[0] != char(gz_magic[0]) ||
            body[1] != char(gz_magic[1])) {
        return ret;
    }
    pos += 2;
    // Check the rest of the gzip header
    if (++pos <= maxPos)
        method = body[pos];
    if (pos++ <= maxPos)
        flags = body[pos];
    if ((method != Z_DEFLATED) || ((flags & RESERVED) != 0)) {
        return ret;
    }

    // Discard time, xflags and OS code:
    pos += 6;
    if (pos > maxPos)
        return ret;
    if ((flags & EXTRA_FIELD) && ((pos+2) <= maxPos)) { // skip the extra field
        unsigned len = (unsigned) body[++pos];
        len += ((unsigned) body[++pos]) << 8;
        pos += len;
        if (pos > maxPos)
            return ret;
    }
    if ((flags & ORIG_NAME) != 0) { // skip the original file name
        while((++pos <= maxPos) && body[pos]) {}
    }
    if ((flags & COMMENT) != 0) {   // skip the .gz file comment
        while((++pos <= maxPos) && body[pos]) {}
    }
    if ((flags & HEAD_CRC) != 0) {  // skip the header crc
        pos += 2;
        if (pos > maxPos) {
            return ret;
        }
    }
    ret = (pos < maxPos); // return failed, if no more bytes left
    return ret;
}

int Decompressor::gunzipBodyPartially(QByteArray & compressed, QByteArray & inflated) {
    int ret = Z_DATA_ERROR;
    unsigned have;
    unsigned char out[CHUNK];
    int pos = -1;

    if (!initInflate) {
        // check the header
        if (!gzipCheckHeader(compressed, pos)) {
            return ret;
        }
        // allocate inflate state
        inflateStrm.zalloc = Z_NULL;
        inflateStrm.zfree = Z_NULL;
        inflateStrm.opaque = Z_NULL;
        inflateStrm.avail_in = 0;
        inflateStrm.next_in = Z_NULL;
        ret = inflateInit2(&inflateStrm, -MAX_WBITS);
        if (ret != Z_OK) {
            return ret;
        }
        initInflate = true;
        streamEnd = false;
    }

    //remove the header.
    compressed.remove(0, pos + 1);
    // expand until deflate stream ends
    inflateStrm.next_in = (unsigned char *) compressed.data();
    inflateStrm.avail_in = compressed.size();
    do {
        inflateStrm.avail_out = sizeof(out);
        inflateStrm.next_out = out;
        ret = inflate(&inflateStrm, Z_NO_FLUSH);
        switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;
                // and fall through
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd(& inflateStrm);
                initInflate = false;
                return ret;
        }
        have = sizeof(out) - inflateStrm.avail_out;
        inflated.append(QByteArray((const char *) out, have));
     } while ((inflateStrm.avail_out == 0) && (inflateStrm.avail_in > 0));
    // clean up and return
    if ((ret <= Z_ERRNO) || (ret == Z_STREAM_END)) {
        gunzipBodyPartiallyEnd();
    }
    streamEnd = (ret == Z_STREAM_END);
    return ret;
}

void Decompressor::gunzipBodyPartiallyEnd() {
    if (initInflate) {
        inflateEnd(&inflateStrm);
        initInflate = false;
    }
}


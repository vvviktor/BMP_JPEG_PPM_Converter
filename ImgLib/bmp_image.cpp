#include "bmp_image.h"

#include <array>
#include <fstream>
#include <string_view>

#include "pack_defines.h"

using namespace std;

namespace img_lib {

static const uint32_t DATA_OFFSET = 54;
static const uint32_t INFO_HEADER_SIZE = 40;
static const uint16_t LAYERS_NUM = 1;
static const uint16_t COLOR_DIGITS = 24;
static const uint32_t COMPRESSION_TYPE = 0;
static const int32_t RESOLUTION = 11811;
static const int32_t TOTAL_COLORS = 0x1000000;

PACKED_STRUCT_BEGIN BitmapFileHeader {
    // поля заголовка Bitmap File Header
    char signature[2] = {'B', 'M'};
    uint32_t total_size = 0;
    uint32_t reserved = 0;
    uint32_t data_offset = DATA_OFFSET;
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    // поля заголовка Bitmap Info Header
    uint32_t info_header_size = INFO_HEADER_SIZE;
    int32_t width = 0;
    int32_t height = 0;
    uint16_t layers = LAYERS_NUM;
    uint16_t color_digits = COLOR_DIGITS;
    uint32_t compression = COMPRESSION_TYPE;
    uint32_t data_size = 0;
    int32_t h_resolution = RESOLUTION;
    int32_t v_resolution = RESOLUTION;
    int32_t colors_used = 0;
    int32_t total_colors = TOTAL_COLORS;
}
PACKED_STRUCT_END

// функция вычисления отступа по ширине
static int GetBMPStride(int w) { return 4 * ((w * 3 + 3) / 4); }

// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image) {
    ofstream out(file, ios::binary);
    int w, h;
    w = image.GetWidth();
    h = image.GetHeight();
    const int bmp_stride = GetBMPStride(w);
    BitmapFileHeader fh;
    BitmapInfoHeader ih;
    fh.total_size = h * bmp_stride + DATA_OFFSET;
    ih.width = w;
    ih.height = h;
    ih.data_size = h * bmp_stride;

    out.write(reinterpret_cast<const char*>(&fh), 14);
    out.write(reinterpret_cast<const char*>(&ih), 40);

    std::vector<char> buff(bmp_stride);

    for (int l = h - 1; l >= 0; --l) {
        const Color* line = image.GetLine(l);
        for (int c = 0; c != w; ++c) {
            buff[c * 3 + 0] = static_cast<char>(line[c].b);
            buff[c * 3 + 1] = static_cast<char>(line[c].g);
            buff[c * 3 + 2] = static_cast<char>(line[c].r);
        }
        out.write(buff.data(), bmp_stride);
    }

    return out.good();
}

// напишите эту функцию
Image LoadBMP(const Path& file) {
    ifstream input(file, ios::binary);

    BitmapFileHeader fh;
    BitmapInfoHeader ih;

    input.read(reinterpret_cast<char*>(&fh), sizeof(BitmapFileHeader));
    input.read(reinterpret_cast<char*>(&ih), sizeof(BitmapInfoHeader));

    int w, h;
    w = ih.width;
    h = ih.height;
    const int bmp_stride = GetBMPStride(w);
    std::vector<char> buff(bmp_stride);

    Image result(w, h, Color::Black());

    for (int l = h - 1; l >= 0; --l) {
        input.read(buff.data(), bmp_stride);
        Color* line = result.GetLine(l);
        for (int c = 0; c != w; ++c) {
            line[c].b = static_cast<byte>(buff[c * 3 + 0]);
            line[c].g = static_cast<byte>(buff[c * 3 + 1]);
            line[c].r = static_cast<byte>(buff[c * 3 + 2]);
        }
    }

    return result;
}

}  // namespace img_lib
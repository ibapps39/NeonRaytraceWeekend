#pragma once

#include <cstdlib>
#include <iostream>
#include <string>


class rtw_image {
    private:
        const int bytes_per_pixel = 3;
        float* fdata = nullptr;                  // Linear floating point pixel data
        unsigned char*  bdata = nullptr;         // Linear 8-bit pixel data
        int             image_width = 0;         // Loaded image width
        int             image_height = 0;        // Loaded image height
        int             bytes_per_scanline = 0;

        static int clamp(int x, int low, int high);
        static unsigned char float_to_byte(float value);
        void convert_to_bytes();
    public:
        rtw_image();
        rtw_image(const char* image_filename);
        ~rtw_image();

        bool load(const std::string& filename);

        int width()  const;
        int height() const;

        const unsigned char* pixel_data(int x, int y) const;
};
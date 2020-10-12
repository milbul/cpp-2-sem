#pragma once
#include <iostream>

#include <vector>

using Seam = std::vector<size_t>;

struct Image
{
    struct Pixel
    {
        Pixel(int red, int green, int blue);
        Pixel();
        int m_red;
        int m_green;
        int m_blue;
    };

    Image(std::vector<std::vector<Pixel>> table);

    Pixel GetPixel(size_t columnId, size_t rowId) const;

    void RemoveVerticalSeam(const Seam& seam);

    void RemoveHorizontalSeam(const Seam& seam);
    
    size_t GetHeight() const;

    size_t GetWidth() const;

    std::vector<std::vector<Pixel>> m_table;
};
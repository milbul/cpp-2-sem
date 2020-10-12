#include "Image.h"
using Seam = std::vector<size_t>;

Image::Image(std::vector<std::vector<Image::Pixel>> table)
    : m_table(std::move(table))
{}

Image::Pixel::Pixel(int red, int green, int blue)
    : m_red(red)
    , m_green(green)
    , m_blue(blue)
{}

Image::Pixel::Pixel()
    : m_red(0)
    , m_green(0)
    , m_blue(0)
{}


Image::Pixel Image::GetPixel(size_t columnId, size_t rowId) const
{
    return m_table[columnId][rowId];
}

void Image::RemoveHorizontalSeam(const Seam& seam)
{
    for (size_t i = 0; i < seam.size(); ++i) {
        m_table[i].erase(m_table[i].begin() + seam[i]);
    }
}

void Image::RemoveVerticalSeam(const Seam& seam)
{
    for (size_t i = 0; i < seam.size(); ++i) {
        for (size_t rowId = seam[i]; rowId < GetWidth() - 1; ++rowId) {
            m_table[rowId][i] = m_table[rowId + 1][i];
        }
    }
    m_table.resize(m_table.size() - 1);
}

size_t Image::GetHeight() const
{
    return m_table[0].size();
}

size_t Image::GetWidth() const
{
    return m_table.size();
}

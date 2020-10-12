#include "SeamCarver.h"
#include <math.h>
#include <algorithm>
#include <iostream>

SeamCarver::SeamCarver(Image image)
    : m_image(std::move(image))
{}

const Image& SeamCarver::GetImage() const
{
    return m_image;
}

size_t SeamCarver::GetImageWidth() const
{
    return m_image.GetWidth();
}

size_t SeamCarver::GetImageHeight() const
{
    return m_image.GetHeight();
}

double SeamCarver::GetPixelEnergy(size_t columnId, size_t rowId) const
{
    size_t width = GetImageWidth();
    size_t height = GetImageHeight();
    const Image::Pixel& x1 = m_image.GetPixel((columnId + width - 1) % width, rowId);
    const Image::Pixel& x2 = m_image.GetPixel((columnId + 1) % width, rowId);
    const Image::Pixel& y1 = m_image.GetPixel(columnId,(rowId - 1 + height) % height);
    const Image::Pixel& y2 = m_image.GetPixel(columnId, (rowId + 1) % height);
    double energyX, energyY;
    energyX = pow(x1.m_red - x2.m_red, 2) + pow(x1.m_green - x2.m_green, 2) + pow(x1.m_blue - x2.m_blue, 2);
    energyY = pow(y1.m_red - y2.m_red, 2) + pow(y1.m_green - y2.m_green, 2) + pow(y1.m_blue - y2.m_blue, 2);
    return sqrt(energyX + energyY);
}

SeamCarver::Seam SeamCarver::FindHorizontalSeam() const {
    std::vector<std::vector<double>> minCost(GetImageWidth(), std::vector<double>(GetImageHeight()));
    std::vector<std::vector<size_t>> parent(GetImageWidth(), std::vector<size_t>(GetImageHeight()));
    std::vector<int> d = {-1, 0, 1};
    Seam ans;
    for (size_t rowId = 0; rowId < GetImageHeight(); ++rowId) {
        for (size_t columnId = 0; columnId < GetImageWidth(); ++columnId) {
            minCost[columnId][rowId] = GetPixelEnergy(columnId, rowId);
        }
    }
    for (size_t columnId = 1; columnId < GetImageWidth(); ++columnId) {
        for (size_t rowId = 0; rowId < GetImageHeight(); ++rowId) {
            double m = 1e4;
            for (size_t i = 0; i < 3; ++i) {
                if (rowId + d[i] < GetImageHeight() && m > minCost[columnId - 1][rowId + d[i]]) {
                    m = minCost[columnId - 1][rowId + d[i]];
                    parent[columnId][rowId] = rowId + d[i];
                }
            }
            minCost[columnId][rowId] += m;
        }
    }
    size_t minInLastColumn = 0;
    for (size_t rowId = 0; rowId < GetImageHeight(); rowId++) {
        if (minCost[GetImageWidth() - 1][minInLastColumn] > minCost[GetImageWidth() - 1][rowId]) {
            minInLastColumn = rowId;
        }
    }
    ans.push_back(minInLastColumn);
    size_t row = minInLastColumn;
    size_t column = GetImageWidth() - 1;
    while (column != 0) {
        row = parent[column][row];
        column--;
        ans.push_back(row);
    }
    std::reverse(ans.begin(), ans.end());
    return ans;
}

SeamCarver::Seam SeamCarver::FindVerticalSeam() const
{
    std::vector<std::vector<double>> minCost(GetImageWidth(), std::vector<double>(GetImageHeight()));
    std::vector<std::vector<size_t>> parent(GetImageWidth(), std::vector<size_t>(GetImageHeight()));
    std::vector<int> d = {-1, 0, 1};
    Seam ans;
    for (size_t columnId = 0; columnId < GetImageWidth(); ++columnId) {
        for (size_t rowId = 0; rowId < GetImageHeight(); ++rowId) {
            minCost[columnId][rowId] = GetPixelEnergy(columnId, rowId);
        }
    }
    for (size_t rowId = 1; rowId < GetImageHeight(); ++rowId) {
        for (size_t columnId = 0; columnId < GetImageWidth(); ++columnId) {
            double m = 1e4;
            for (size_t i = 0; i < 3; ++i) {
                if (columnId + d[i] < GetImageWidth()) {
                    if (m > minCost[columnId + d[i]][rowId - 1]) {
                        m = minCost[columnId + d[i]][rowId - 1];
                        parent[columnId][rowId] = columnId + d[i];
                    }
                }
            }
            minCost[columnId][rowId] += m;
        }
    }
    size_t minInLastRow = 0;
    for (size_t columnId = 0; columnId < GetImageWidth(); ++columnId) {
        if (minCost[minInLastRow][GetImageHeight() - 1] > minCost[columnId][GetImageHeight() - 1]) {
            minInLastRow = columnId;
        }
    }
    ans.push_back(minInLastRow);
    size_t row = GetImageHeight() - 1;
    size_t column = minInLastRow;
    while (row != 0) {
        column = parent[column][row];
        row--;
        ans.push_back(column);
    }
    std::reverse(ans.begin(), ans.end());
    return ans;
}

void SeamCarver::RemoveHorizontalSeam(const Seam& seam)
{
    m_image.RemoveHorizontalSeam(seam);
}

void SeamCarver::RemoveVerticalSeam(const Seam& seam)
{
    m_image.RemoveVerticalSeam(seam);
}

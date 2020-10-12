#include "PercolationStats.h"
#include <Percolation.h>
#include <vector>
#include <math.h>

PercolationStats::PercolationStats(size_t dimension, size_t trials) : size (dimension)
{
    x.clear();
    for (size_t i = 0; i < trials; ++i) {
        execute();
    }
}

double PercolationStats::get_mean() const
{
    double sum_x = 0;
    for (size_t i = 0; i < x.size(); ++i) {
        sum_x += x[i];
    }
    return sum_x / x.size();
}

double PercolationStats::get_standard_deviation() const
{
    double sum = 0;
    for (size_t i = 0; i < x.size(); ++i) {
        sum += (x[i] - get_mean()) * (x[i] - get_mean());
    }
    return sqrt(sum / (x.size() - 1));
}

double PercolationStats::get_confidence_low() const
{
    return get_mean() - 1.96 * get_standard_deviation() / sqrt(x.size());
}

double PercolationStats::get_confidence_high() const
{
    return get_mean() + 1.96 * get_standard_deviation() / sqrt(x.size());
}

void PercolationStats::execute()
{
    Percolation percolation = Percolation(size);
    while (!percolation.has_percolation()) {
        int row = rand() % size;
        int column = rand() % size;
        percolation.open(row, column);
    }
    x.push_back((double)percolation.get_numbet_of_open_cells() / (double)(size * size));
}


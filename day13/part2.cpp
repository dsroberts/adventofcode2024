#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <cstdint>
#include <cmath>

typedef std::array<std::array<long double, 2>, 2> matrix2x2;

long double det(matrix2x2 mat)
{
    return (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]);
}

matrix2x2 invert(matrix2x2 in)
{
    auto determinant = det(in);
    if (determinant == 0)
    {
        return matrix2x2{{{0.0l, 0.0l}, {0.0l, 0.0l}}};
    }
    matrix2x2 out;
    out[0][0] = in[1][1] / determinant;
    out[0][1] = -in[0][1] / determinant;
    out[1][0] = -in[1][0] / determinant;
    out[1][1] = in[0][0] / determinant;
    return out;
}

int main()
{
    std::ifstream f("input_refactored.txt");
    std::string line;
    std::string tok;
    matrix2x2 mat;
    std::array<long double, 2> target;
    std::array<long double, 2> steps;
    const auto costs = std::array<int64_t, 2>{3, 1};
    const auto epsilon = 0.01l;
    auto cost = 0l;
    while (getline(f, line))
    {
        auto ss = std::stringstream{line};
        for (auto i = 0; i < 2; i++)
        {
            for (auto j = 0; j < 2; j++)
            {
                getline(ss, tok, ' ');
                mat[j][i] = std::stod(tok);
            }
        }
        auto inverse = invert(mat);
        getline(ss, tok, ' ');
        target[0] = std::stod(tok) + 10000000000000.0l;
        getline(ss, tok, ' ');
        target[1] = std::stod(tok) + 10000000000000.0l;

        steps = {0.0l, 0.0l};

        for (auto i = 0; i < 2; i++)
        {
            for (auto j = 0; j < 2; j++)
            {
                steps[i] += inverse[j][i] * target[j];
            }
        }
        if ((std::abs(steps[0] - std::round(steps[0]))) < epsilon &&
            (std::abs(steps[1] - std::round(steps[1]))) < epsilon)
        {

            for (auto i = 0; i < 2; i++)
            {
                cost += (costs[i] * static_cast<int64_t>(std::round(steps[i])));
            }
        }
    }
    f.close();
    std::cout << cost << std::endl;
}
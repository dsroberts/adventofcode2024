#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <array>
#include <numeric>
#include <cmath>
#include <ranges>

typedef std::pair<int32_t, int32_t> coord;
typedef std::pair<uint32_t, uint32_t> ucoord;

uint32_t modulo(int32_t value, uint32_t m)
{
    auto mod = value % static_cast<int32_t>(m);
    if (mod < 0)
    {
        mod += m;
    }
    return mod;
}

ucoord pair_plus(ucoord a, ucoord b)
{
    return {a.first + b.first, a.second + b.second};
}

std::pair<double, double> variance(std::vector<ucoord> in)
{
    double xvariance = 0.0;
    double yvariance = 0.0;
    auto means = std::accumulate(in.begin(), in.end(), ucoord{0, 0}, pair_plus);
    auto xmean = static_cast<double>(means.first) / static_cast<double>(in.size());
    auto ymean = static_cast<double>(means.second) / static_cast<double>(in.size());
    for (auto i : in)
    {
        xvariance += std::pow(static_cast<double>(i.first) - xmean, 2.0) / static_cast<double>(in.size());
        yvariance += std::pow(static_cast<double>(i.second) - ymean, 2.0) / static_cast<double>(in.size());
    }
    return {xvariance, yvariance};
}

int main()
{

    std::ifstream f("input.txt");
    std::string line;
    std::vector<ucoord> positions;
    std::vector<coord> velocities;
    uint32_t nx{101};
    uint32_t ny{103};
    std::string ptok, vtok, tok, tok2;

    while (getline(f, line))
    {
        auto ss = std::stringstream(line);
        getline(ss, ptok, ' ');
        getline(ss, vtok, ' ');
        ss = std::stringstream(ptok);
        getline(ss, tok, '=');
        getline(ss, tok, '=');
        ss = std::stringstream(tok);
        getline(ss, tok2, ',');
        auto ux = std::stoul(tok2);
        getline(ss, tok2, ',');
        auto uy = std::stoul(tok2);
        positions.emplace_back(ux, uy);
        ss = std::stringstream(vtok);
        getline(ss, tok, '=');
        getline(ss, tok, '=');
        ss = std::stringstream(tok);
        getline(ss, tok2, ',');
        auto x = std::stol(tok2);
        getline(ss, tok2, ',');
        auto y = std::stol(tok2);
        velocities.emplace_back(x, y);
        // std::cout << "p=" << positions.back().first << "," << positions.back().second << " v=" << velocities.back().first << "," << velocities.back().second << std::endl;
    }
    f.close();

    auto best_x_variance = 999999999999.9;
    auto best_y_variance = 999999999999.9;
    auto best_variance = 999999999999.9;
    auto tx = 0;
    auto ty = 0;
    auto t=0;

    for (auto i = 1u; i < nx * ny + 1; ++i)
    {

        for (auto [p, v] : std::views::zip(positions, velocities))
        {
            p.first = modulo(static_cast<int64_t>(p.first) +  v.first, nx);
            p.second = modulo(static_cast<int64_t>(p.second) +  v.second, ny);
        }
        auto v = variance(positions);
        if (v.first < best_x_variance)
        {
            best_x_variance = v.first;
            tx = i;
        }
        if (v.second < best_y_variance)
        {
            best_y_variance = v.second;
            ty = i;
        }
        if ( v.first + v.second < best_variance ) {
            best_variance = v.first + v.second;
            t = i;
        }
    }
    std::cout << tx << ", " << ty << std::endl;
    std::cout << t << std::endl;
}
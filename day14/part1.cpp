#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <array>
#include <numeric>

typedef std::pair<int64_t, int64_t> coord;
typedef std::pair<uint64_t, uint64_t> ucoord;

uint64_t modulo(int64_t value, uint64_t m)
{
    auto mod = value % static_cast<int64_t>(m);
    if (mod < 0)
    {
        mod += m;
    }
    return mod;
}

int main()
{

    std::ifstream f("input.txt");
    std::string line;
    std::vector<ucoord> positions;
    std::vector<coord> velocities;
    int64_t timesteps{100};
    uint64_t nx{101};
    uint64_t ny{103};
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
        //std::cout << "p=" << positions.back().first << "," << positions.back().second << " v=" << velocities.back().first << "," << velocities.back().second << std::endl;

        positions.back().first = modulo(static_cast<int64_t>(positions.back().first) + timesteps * velocities.back().first, nx);
        positions.back().second = modulo(static_cast<int64_t>(positions.back().second) + timesteps * velocities.back().second, ny);
        //std::cout << positions.back().first << "," << positions.back().second << std::endl;
    }
    f.close();

    std::array<uint64_t, 4> quadrants{0, 0, 0, 0};
    for (auto p : positions)
    {
        if (p.first < (nx - 1) / 2 && p.second < (ny - 1) / 2)
        {
            quadrants[0]++;
        }
        if (p.first > (nx - 1) / 2 && p.second < (ny - 1) / 2)
        {
            quadrants[1]++;
        }
        if (p.first < (nx - 1) / 2 && p.second > (ny - 1) / 2)
        {
            quadrants[2]++;
        }
        if (p.first > (nx - 1) / 2 && p.second > (ny - 1) / 2)
        {
            quadrants[3]++;
        }
    }
    std::cout << std::accumulate(quadrants.begin(), quadrants.end(), 1, std::multiplies<uint64_t>()) << std::endl;
}
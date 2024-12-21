#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <map>
#include <array>
#include <ranges>
#include <numeric>

class Coord
{
public:
    uint32_t x;
    uint32_t y;
    Coord() {}
    Coord(uint32_t i, uint32_t j) : x{i},
                                    y{j}
    {
    }
    Coord &operator+=(const Coord &other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
    Coord operator+(const Coord &other) const
    {
        auto tmp{*this};
        return tmp += other;
    }
    bool operator==(const Coord other)
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Coord other)
    {
        return !(*this == other);
    }

private:
};

const auto cardinal_dirs = std::array{
    Coord(0, -1), // N
    Coord(1, 0),  // E
    Coord(0, 1),  // S
    Coord(-1, 0), // W
};

struct grid_bounds
{
    uint32_t nx;
    uint32_t ny;
    bool operator()(uint32_t x, uint32_t y) { return x < nx &&
                                                     x >= 0 &&
                                                     y < ny &&
                                                     y >= 0; }
    bool operator()(Coord c) { return c.x < nx &&
                                      c.x >= 0 &&
                                      c.y < ny &&
                                      c.y >= 0; }
};

class Region
{
public:
    grid_bounds *in_grid;
    std::vector<std::pair<Coord, std::array<uint32_t, 4>>> coords;
    const char letter;
    std::uint32_t vertices;
    Region(char c) : letter(c),
                     vertices(0ul) {};
    uint32_t area()
    {
        return static_cast<uint32_t>(coords.size());
    }
    uint32_t perimeter()
    {
        uint32_t perimeter{0};
        for (auto c : coords)
        {
            auto neighbours = c.second;
            perimeter += std::accumulate(neighbours.begin(), neighbours.end(), 0ul, std::plus<uint32_t>());
        }
        return perimeter;
    }

private:
};

void region(Coord pt, std::vector<std::string> *all, grid_bounds *in_grid, std::vector<Coord> *found, Region *region_coords)
{
    found->push_back(pt);
    region_coords->in_grid = in_grid;
    region_coords->coords.push_back({pt, {0, 0, 0, 0}});
    std::vector<Coord> neighbours;
    for (auto d : cardinal_dirs)
    {
        neighbours.push_back(pt + d);
    }
    auto region_idx = region_coords->coords.size() - 1;
    std::array<uint32_t, 4> fences = {0, 0, 0, 0};

    for (auto i = 0; i < 4; i++)
    {
        if (in_grid->operator()(neighbours[i]))
        {
            if (all->operator[](neighbours[i].y)[neighbours[i].x] == all->operator[](pt.y)[pt.x])
            {
                if (std::find(found->begin(), found->end(), neighbours[i]) == found->end())
                {
                    region(neighbours[i], all, in_grid, found, region_coords);
                }
            }
            else
            {
                fences[i] = 1;
            }
        }
        else
        {
            fences[i] = 1;
        }
    }
    region_coords->coords[region_idx].second = fences;
    // Find this squares contribution to the total vertices
    auto nneighbours = 4ul - std::accumulate(fences.begin(), fences.end(), 0ul, std::plus<uint32_t>());
    if (nneighbours == 0)
    {
        region_coords->vertices += 4;
    }
    else if (nneighbours == 1)
    {
        region_coords->vertices += 2;
    }
    else if (nneighbours == 2)
    {
        /*
            ._.  .L.  .L?  ?L.  ._.  ._.  .0.
            LLL  |L|  |LL  LL|  |LL  LL|  3.1
            .-.  .L.  .-.  .-.  .L?  ?L.  .2.
        */
        auto nidx = std::array{0ul, 0ul};
        auto iidx = 0ul;
        for (auto ifence = 0ul; ifence < 4; ++ifence)
        {
            if (fences[ifence] == 1)
            {
                nidx[iidx++] = ifence;
            }
        }
        // Not a straight line
        if (!(nidx[0] == 0 && nidx[1] == 2) && !(nidx[0] == 1 && nidx[1] == 3))
        {
            region_coords->vertices++;
            auto diag = pt + cardinal_dirs[(nidx[0] + 2) % 4] + cardinal_dirs[(nidx[1] + 2) % 4];
            if (!in_grid->operator()(diag))
            {
                region_coords->vertices++;
            }
            else if (all->operator[](diag.y)[diag.x] != region_coords->letter)
            {
                region_coords->vertices++;
            }
        }
    }
    else if (nneighbours == 3)
    {
        /*
        .L?  ._.  ?L.  ?L?  .0.
        |LL  LLL  LL|  LLL  3.1
        .L?  ?L?  ?L.  .-.  .2.
        */
        auto nidx = 0ul;
        for (auto ifence = 0ul; ifence < 4; ++ifence)
        {
            if (fences[ifence] == 1)
            {
                nidx = ifence;
            }
        }
        auto diag1 = pt;
        auto diag2 = pt;
        if (nidx == 0)
        {
            diag1 += cardinal_dirs[1] + cardinal_dirs[2];
            diag2 += cardinal_dirs[3] + cardinal_dirs[2];
        }
        else if (nidx == 1)
        {
            diag1 += cardinal_dirs[0] + cardinal_dirs[3];
            diag2 += cardinal_dirs[2] + cardinal_dirs[3];
        }
        else if (nidx == 2)
        {
            diag1 += cardinal_dirs[1] + cardinal_dirs[0];
            diag2 += cardinal_dirs[3] + cardinal_dirs[0];
        }
        else if (nidx == 3)
        {
            diag1 += cardinal_dirs[0] + cardinal_dirs[1];
            diag2 += cardinal_dirs[2] + cardinal_dirs[1];
        }
        if (!in_grid->operator()(diag1))
        {
            region_coords->vertices++;
        }
        else if (all->operator[](diag1.y)[diag1.x] != region_coords->letter)
        {
            region_coords->vertices++;
        }
        if (!in_grid->operator()(diag2))
        {
            region_coords->vertices++;
        }
        else if (all->operator[](diag2.y)[diag2.x] != region_coords->letter)
        {
            region_coords->vertices++;
        }
    }
    else
    {
        std::vector<Coord> diags;
        diags.push_back(pt + cardinal_dirs[0] + cardinal_dirs[1]);
        diags.push_back(pt + cardinal_dirs[1] + cardinal_dirs[2]);
        diags.push_back(pt + cardinal_dirs[2] + cardinal_dirs[3]);
        diags.push_back(pt + cardinal_dirs[3] + cardinal_dirs[0]);
        for (auto d : diags)
        {
            if (!in_grid->operator()(d))
            {
                region_coords->vertices++;
            }
            else if (all->operator[](d.y)[d.x] != region_coords->letter)
            {
                region_coords->vertices++;
            }
        }
    }
    return;
}

int main()
{

    std::ifstream f("input.txt");
    std::string line;
    std::vector<std::string> all;
    auto col_ctr = 0ul;
    std::vector<uint32_t> prices;
    std::vector<Region> regions;
    std::vector<Coord> found1, found2;

    while (getline(f, line))
    {
        all.push_back(line);
        col_ctr++;
    }
    f.close();
    grid_bounds in_grid{.nx = static_cast<uint32_t>(all[0].size()), .ny = col_ctr};
    for (auto j = 0u; j < all.size(); ++j)
    {
        for (auto i = 0u; i < all[j].size(); ++i)
        {
            if (std::find(found1.begin(), found1.end(), Coord{i, j}) == found1.end())
            {
                regions.emplace_back(all[j][i]);
                region(Coord{i, j}, &all, &in_grid, &found1, &regions.back());
            }
        }
    }
    std::cout << regions.size() << std::endl;

    auto price = 0ul;
    for (auto r : regions)
    {
        price += (r.area() * r.vertices);
    }
    std::cout << price << std::endl;
}
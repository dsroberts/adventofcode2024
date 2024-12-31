#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <cstdint>

std::pair<std::vector<uint32_t>, std::vector<uint32_t>> consolidate_gaps(std::vector<int> &sparse)
{
    std::vector<uint32_t> out_idxs;
    std::vector<uint32_t> out_sizes;
    int space = 0;
    for (auto i = 0ul; i < sparse.size(); ++i)
    {
        if (space)
        {
            if (sparse[i] != -1)
            {
                out_sizes.push_back(i - out_idxs.back());
                space = 1 - space;
            }
        }
        else
        {
            if (sparse[i] == -1)
            {
                out_idxs.push_back(i);
                space = 1 - space;
            }
        }
    }
    if (space)
    {
        out_sizes.push_back(sparse.size() - out_idxs.back());
    }
    return std::pair<std::vector<uint32_t>, std::vector<uint32_t>>{out_idxs, out_sizes};
}

int main()
{
    std::ifstream f("input.txt");
    std::string line;
    getline(f, line);
    std::vector<int> sparse_format;
    std::vector<uint32_t> file_lens;
    std::vector<uint32_t> gap_lens;
    std::vector<uint32_t> file_idxs;
    std::vector<uint32_t> gap_idxs;
    auto fileid = 0ul;
    auto space = 0l;
    for (auto c : line)
    {
        auto count = std::stoul(std::string{c});
        auto ctr = 0u;
        if (space)
        {
            gap_idxs.push_back(sparse_format.size());
            for (auto i = 0ul; i < count; i++)
            {
                sparse_format.push_back(-1);
                ctr++;
            }
            gap_lens.push_back(ctr);
        }
        else
        {
            file_idxs.push_back(sparse_format.size());
            for (auto i = 0ul; i < count; i++)
            {
                sparse_format.push_back(fileid);
                ctr++;
            }
            file_lens.push_back(ctr);
            fileid++;
        }
        space = 1 - space;
    }

    // Main loop
    typedef std::vector<uint32_t>::reverse_iterator intRiter;
    for (std::pair<intRiter, intRiter> i{file_idxs.rbegin(), file_lens.rbegin()};
         i.first != file_idxs.rend();
         ++i.first, ++i.second)
    {
        for (auto ig = 0ul; gap_idxs[ig] <= *(i.first); ++ig)
        {
            if (gap_lens[ig] >= (*i.second))
            {
                for (auto jf = 0ul; jf < (*i.second); ++jf)
                {
                    sparse_format[gap_idxs[ig] + jf] = sparse_format[*(i.first) + jf];
                    sparse_format[*(i.first) + jf] = -1;
                }
                break;
            }
        }
        auto o = consolidate_gaps(sparse_format);
        gap_lens = o.second;
        gap_idxs = o.first;
    }

    auto sum = 0ull;
    for (auto i = 0ull; i < sparse_format.size(); i++)
    {
        if (sparse_format[i] != -1)
        {
            sum += static_cast<uint64_t>(sparse_format[i] * i);
        }
    }
    std::cout << sum << std::endl;
}
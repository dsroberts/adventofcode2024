#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <algorithm>
#include <ranges>
#include <cstdint>

class Plus_Or_Mul
{
public:
    Plus_Or_Mul(int count)
    {
        // Count number of muls;
        for (auto i = 0l; i < count + 1; ++i)
        {
            std::string tmp;
            for (auto j = 0l; j < i; ++j)
            {
                tmp.push_back('*');
            }
            for (auto j = i; j < count; ++j)
            {
                tmp.push_back('+');
            }
            do
            {
                ops.push_back(tmp);
            } while (std::next_permutation(tmp.begin(), tmp.end()));
        }
    };
    void set_func_idx(int idx)
    {
        ctr = 0;
        func_idx = idx;
    }
    uint64_t func(const uint64_t a, const uint64_t b)
    {

        if (ops[func_idx][ctr] == '*')
        {
            ctr++;
            return a * b;
        }
        else if (ops[func_idx][ctr] == '+')
        {
            ctr++;
            return a + b;
        }
    }
    auto size()
    {
        return ops.size();
    }
    auto get_op()
    {
        return ops[func_idx];
    }

private:
    int ctr;
    std::vector<std::string> ops;
    int func_idx;
};

int main()
{
    std::ifstream f("input.txt");
    std::string line;
    std::vector<uint64_t> targets;
    std::vector<std::vector<uint64_t>> vals;
    while (getline(f, line))
    {
        std::string tok;
        auto ss = std::stringstream(line);
        getline(ss, tok, ':');
        targets.emplace_back(std::stoull(tok));
        vals.emplace_back();
        while (getline(ss, tok, ' '))
        {
            if (!tok.empty())
            {
                vals.back().push_back(std::stoull(tok));
            }
        }
    }
    f.close();
    uint64_t sum = 0;
    for (auto [target, values] : std::views::zip(targets, vals))
    {
        auto funcs = Plus_Or_Mul(values.size() - 1);
        for (auto i = 0ul; i < funcs.size(); i++)
        {
            funcs.set_func_idx(i);
            auto mul = std::ranges::fold_left(values.begin() + 1, values.end(), *values.begin(), [&funcs](uint64_t a, uint64_t b)
                                              { return funcs.func(a, b); });
            if (mul == target)
            {
                sum += mul;
                // std::cout << uint64_t(mul) << ": ";
                // auto ops = funcs.get_op();
                // for (auto j = 0u; j < ops.size(); j++)
                //{
                //     std::cout << values[j] << " " << ops[j] << " ";
                // }
                // std::cout << values.back() << std::endl;
                break;
            }
        }
    }
    std::cout << sum << std::endl;
}
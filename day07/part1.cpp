#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <algorithm>
#include <ranges>
#include <cstdint>

template <class T>
class SortableOperator
{
public:
    SortableOperator(std::function<T(T, T)> op, int idx, std::string repr) : op_(op),
                                                                             idx_(idx),
                                                                             repr_(repr)
    {
    }
    T operator()(T a, T b) { return op_(a, b); };
    bool operator<(const SortableOperator<T> other) { return idx_ < other.idx_; };
    friend std::ostream &operator<<(std::ostream &out, SortableOperator<T> op)
    {
        return out << op.repr_;
    };

private:
    std::function<T(T, T)> op_;
    int idx_;
    std::string repr_;
};

template <class T>
class OpChain
{
public:
    OpChain(std::vector<SortableOperator<T>> ops) : ops_(ops),
                                                    it_(ops_.begin())
    {
    }
    T operator()(T a, T b) { return (*it_++)(a, b); }
    auto get_ops() { return ops_; }

private:
    std::vector<SortableOperator<T>> ops_;
    std::vector<SortableOperator<T>>::iterator it_;
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

    for (auto &&[target, values] : std::views::zip(targets, vals))
    {
        std::vector<OpChain<uint64_t>> ops;
        for (auto star_count = 0ul; star_count < values.size(); ++star_count)
        {
            auto plus_count = values.size() - star_count - 1;
            std::vector<SortableOperator<uint64_t>> tmp{star_count, SortableOperator<uint64_t>(std::multiplies<uint64_t>{}, 1, std::string{'*'})};
            std::vector<SortableOperator<uint64_t>> tmp2{plus_count, SortableOperator<uint64_t>(std::plus<uint64_t>{}, 2, std::string{'+'})};
            std::move(tmp2.begin(), tmp2.end(), std::back_inserter(tmp));

            do
            {
                ops.push_back(OpChain<uint64_t>(tmp));
            } while (std::next_permutation(tmp.begin(), tmp.end()));
        }
        for (auto &op : ops)
        {
            auto mul = std::ranges::fold_left(values.begin() + 1, values.end(), *values.begin(), [&op](uint64_t a, uint64_t b)
                                              { return op(a, b); });
            if (mul == target)
            {
                sum += mul;
                // std::cout << uint64_t(mul) << ": ";
                // for (auto [o, v] : std::views::zip(op.get_ops(), values))
                // {
                //     std::cout << v << " " << o << " ";
                // }
                // std::cout << values.back() << std::endl;
                break;
            }
        }
    }
    std::cout << sum << std::endl;
}
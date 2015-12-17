#include <algorithm>

#include "Knapsack.h"

Knapsack::Item::Item():
    m_weight(0),
    m_value(0)
{}

Knapsack::Item::Item(
    const uint32_t weight, const uint32_t value) :
    m_weight(weight),
    m_value(value)
{}

Knapsack::Item::Item(const Item& i) :
    m_weight(i.m_weight),
    m_value(i.m_value)
{}

Knapsack::Item::Item(Item&& i) :
    m_weight(i.m_weight),
    m_value(i.m_value)
{
    i.m_weight = 0;
    i.m_value = 0;
}

Knapsack::Knapsack()
{
}


Knapsack::~Knapsack()
{
}

bool Knapsack::read(std::istream& in)
{
    size_t numItems = 0;
    in >> numItems;
    m_items.resize(numItems);

    for (auto& item : m_items)
    {
        in >> item.m_value >> item.m_weight;
    }

    return true;
}

bool Knapsack::run(
    const uint32_t maxWeight,
    uint32_t& res,
    ItemsList& resList)
{
    // this is knapsack
    std::vector<uint32_t> knapsack[2];
    knapsack[0].resize(maxWeight, 0);
    knapsack[1].resize(maxWeight, 0);

    size_t numItems = m_items.size();
    for (size_t i = 1; i < numItems + 1; ++i)
    {
        for (uint32_t w = 0; w < maxWeight; ++w)
        {
            if (m_items[i - 1].m_weight - 1 > w)
            {
                knapsack[i % 2][w] = knapsack[(i - 1) % 2][w];
            }
            else
            {
                knapsack[i % 2][w] = std::max(
                    knapsack[(i - 1) % 2][w],
                    knapsack[(i - 1) % 2][w - (m_items[i - 1].m_weight - 1)] + m_items[i - 1].m_value);
            }
        }
    }

    res = knapsack[numItems % 2][maxWeight - 1];

    // TODO: backpropagation

    return true;
}
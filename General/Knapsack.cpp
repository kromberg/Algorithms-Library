#include <algorithm>
#include <iostream>

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

Knapsack::Item& Knapsack::Item::operator=(Item&& i)
{
    if (this == &i)
    {
        return *this;
    }
    m_weight = i.m_weight;
    m_value = i.m_value;
    i.m_weight = 0;
    i.m_value = 0;
    return *this;
}

Knapsack::ItemScore::ItemScore():
    m_set(false),
    m_val(0)
{}

Knapsack::ItemScore::ItemScore(double val):
    m_set(true),
    m_val(val)
{}

Knapsack::ItemScore::ItemScore(ItemScore&& itemScore):
    m_set(itemScore.m_set),
    m_val(itemScore.m_val)
{
    itemScore.m_set = false;
    itemScore.m_val = 0;
}

Knapsack::ItemScore& Knapsack::ItemScore::operator=(ItemScore&& itemScore)
{
    if (this == &itemScore)
    {
        return *this;
    }
    m_set = itemScore.m_set;
    m_val = itemScore.m_val;
    itemScore.m_set = false;
    itemScore.m_val = 0;
    return *this;
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
    in >> m_maxWeight;

    return true;
}

bool Knapsack::run(
    uint32_t& res,
    ItemsList& resList)
{
    uint32_t numItems = static_cast<uint32_t>(m_items.size());
    // this is knapsack
    std::vector<std::vector<uint32_t> > knapsack(numItems + 1);
    for (uint32_t i = 0; i < numItems + 1; ++i)
    {
        knapsack[i].resize(m_maxWeight, 0);
    }

    for (size_t i = 1; i < numItems + 1; ++i)
    {
        for (uint32_t w = 0; w < m_maxWeight; ++w)
        {
            if (m_items[i - 1].m_weight - 1 > w)
            {
                knapsack[i][w] = knapsack[i - 1][w];
            }
            else
            {
                knapsack[i][w] = std::max(
                    knapsack[i - 1][w],
                    knapsack[i - 1][w - (m_items[i - 1].m_weight - 1)] + m_items[i - 1].m_value);
            }
        }
    }

    res = knapsack[numItems][m_maxWeight - 1];

    // TODO: backpropagation
    uint32_t currentWieght = m_maxWeight;
    for (uint32_t i = numItems; i > 0; --i)
    {
        if (m_items[i - 1].m_weight - 1 <= currentWieght)
        {
            if (knapsack[i][currentWieght - 1] == knapsack[i - 1][currentWieght - (m_items[i - 1].m_weight - 1)] + m_items[i - 1].m_value)
            {
                currentWieght -= (m_items[i - 1].m_weight - 1);
                resList.push_front(m_items[i - 1]);
            }
        }
    }

    return true;
}

bool Knapsack::runGreedy(uint32_t &res)
{

    std::vector<ItemScore> itemsScores;
    itemsScores.reserve(m_items.size());
    
    // fill itemscores
    uint32_t numOfItems = static_cast<uint32_t>(m_items.size());
    for (uint32_t i = 0; i < numOfItems; ++i)
    {
        if (m_items[i].m_weight != 0)
        {
            itemsScores.push_back(std::move(ItemScore(m_items[i].m_value / m_items[i].m_weight)));
        }
        else
        {
            itemsScores.push_back(std::move(ItemScore()));
        }
    }
    std::sort(itemsScores.begin(), itemsScores.end());
    // debug
    for (auto& itemScore : itemsScores)
    {
        std::cout << itemScore.m_val << std::endl;
    }
    
    res = 0;
    uint32_t currentWeight = 0;
    for (auto& itemScore : itemsScores)
    {
        if (currentWeight + m_items[itemScore.m_idx].m_weight > m_maxWeight)
        {
            break;
        }
        else
        {
            currentWeight += m_items[itemScore.m_idx].m_weight;
            res += m_items[itemScore.m_idx].m_value;
        }
    }
    
    for (auto& item : m_items)
    {
        if ((item.m_weight <= m_maxWeight) &&
            (item.m_value > res))
        {
            res = item.m_value;
        }
    }
    
    return true;
}
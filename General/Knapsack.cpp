#include <algorithm>
#include <iostream>

#include "Matrix.h"
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

Knapsack::ItemScore::ItemScore(const uint32_t idx) :
    m_idx(idx),
    m_set(false),
    m_val(0)
{}

Knapsack::ItemScore::ItemScore(const uint32_t idx, double val) :
    m_idx(idx),
    m_set(true),
    m_val(val)
{}

Knapsack::ItemScore::ItemScore(ItemScore&& itemScore):
    m_idx(itemScore.m_idx),
    m_set(itemScore.m_set),
    m_val(itemScore.m_val)
{
    itemScore.m_idx = 0;
    itemScore.m_set = false;
    itemScore.m_val = 0;
}

Knapsack::ItemScore& Knapsack::ItemScore::operator=(ItemScore&& itemScore)
{
    if (this == &itemScore)
    {
        return *this;
    }
    m_idx = itemScore.m_idx;
    m_set = itemScore.m_set;
    m_val = itemScore.m_val;
    itemScore.m_idx = 0;
    itemScore.m_set = false;
    itemScore.m_val = 0;
    return *this;
}

bool Knapsack::ItemScore::operator<(const ItemScore& itemScore)
{
    return m_val < itemScore.m_val;
}

Knapsack::Entity::Entity():
    m_set(false),
    m_val(0)
{}

Knapsack::Entity::Entity(const uint32_t val):
    m_set(true),
    m_val(val)
{}

Knapsack::Entity::Entity(Entity&& entity) :
    m_set(entity.m_set),
    m_val(entity.m_val)
{
    entity.m_set = false;
    entity.m_val = 0;
}

Knapsack::Entity& Knapsack::Entity::operator=(Entity&& entity)
{
    if (this == &entity)
    {
        return *this;
    }
    m_set = entity.m_set;
    m_val = entity.m_val;
    entity.m_set = false;
    entity.m_val = 0;
    return *this;
}

std::ostream& operator<< (std::ostream& out, const Knapsack::Entity& entity)
{
    if (entity.m_set)
    {
        out << entity.m_val;
    }
    else
    {
        out << -1;
    }
    out << ';';
    return out;
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

    // backpropagation
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

bool Knapsack::runByValue(uint32_t& res, ItemsList& resList)
{
    auto it = std::max_element(m_items.begin(), m_items.end(), [](const Item& i1, const Item& i2) -> bool {
        return i1.m_value < i2.m_value;
    });
    uint32_t maxValue = it->m_value;
    uint32_t numOfItems = static_cast<uint32_t>(m_items.size());
    Matrix<Entity> knapsackMatrix;
    knapsackMatrix.resize(numOfItems + 1, numOfItems * maxValue + 1);

    Entity minEntity;
    Entity tmpVal;
    knapsackMatrix[0][0] = std::move(Entity(0));
    for (uint32_t i = 1; i <= numOfItems; ++i)
    {
        for (uint32_t x = 0; x <= numOfItems * maxValue; ++x)
        {
            tmpVal = ((m_items[i - 1].m_value < x) ? knapsackMatrix[i - 1][x - m_items[i - 1].m_value] : Entity(0));
            minEntity = knapsackMatrix[i - 1][x];
            
            if (tmpVal.m_set &&
                ((!minEntity.m_set) ||
                 (minEntity.m_val > m_items[i - 1].m_weight + tmpVal.m_val)))
            {
                minEntity.m_set = true;
                minEntity.m_val = m_items[i - 1].m_weight + tmpVal.m_val;
            }
            knapsackMatrix[i][x] = std::move(minEntity);
        }
    }
    // debug
    /*std::ofstream fout("knapsack.csv");
    fout << knapsackMatrix << std::endl;*/

    res = 0;
    for (uint32_t x = 0; x <= numOfItems * maxValue; ++x)
    {
        if (knapsackMatrix[numOfItems][x].m_set &&
            knapsackMatrix[numOfItems][x].m_val <= m_maxWeight)
        {
            res = x;
        }
    }

    // TODO: backpropagation
    uint32_t currVal = res;
    for (uint32_t i = numOfItems; i > 0; --i)
    {
        if ((m_items[i - 1].m_value <= currVal) &&
            knapsackMatrix[i - 1][currVal - m_items[i - 1].m_value].m_set)
        {
            if (knapsackMatrix[i][currVal].m_val ==
                knapsackMatrix[i - 1][currVal - m_items[i - 1].m_value].m_val + m_items[i - 1].m_weight)
            {
                currVal -= m_items[i - 1].m_value;
                resList.push_front(m_items[i - 1]);
            }
        }
    }

    return true;
}

bool Knapsack::runGreedy(uint32_t &res, ItemsList& resList)
{

    std::vector<ItemScore> itemsScores;
    itemsScores.reserve(m_items.size());
    
    // fill itemscores
    uint32_t numOfItems = static_cast<uint32_t>(m_items.size());
    for (uint32_t i = 0; i < numOfItems; ++i)
    {
        if (m_items[i].m_weight != 0)
        {
            itemsScores.push_back(std::move(ItemScore(i, static_cast<double>(m_items[i].m_value) / m_items[i].m_weight)));
        }
        else
        {
            itemsScores.push_back(std::move(ItemScore(i)));
        }
    }
    std::sort(itemsScores.begin(), itemsScores.end());
    // debug
    /*for (auto& itemScore : itemsScores)
    {
        std::cout << itemScore.m_val << std::endl;
    }*/
    
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
            resList.push_back(m_items[itemScore.m_idx]);
        }
    }
    
    for (auto& item : m_items)
    {
        if ((item.m_weight <= m_maxWeight) &&
            (item.m_value > res))
        {
            res = item.m_value;
            resList.clear();
            resList.push_back(item);
        }
    }
    
    return true;
}
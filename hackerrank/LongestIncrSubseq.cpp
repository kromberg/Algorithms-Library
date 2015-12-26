#include <set>
#include <map>
#include <algorithm>

#include "LongestIncrSubseq.h"

LongestIncrSubseq::LongestIncrSubseq()
{
}


LongestIncrSubseq::~LongestIncrSubseq()
{
}

bool LongestIncrSubseq::runAlgorithm1(uint32_t& res /*todo: sequence*/)
{
    uint32_t numElements = static_cast<uint32_t>(m_elements.size());
    std::vector<uint32_t> entitiesMatrix;
    entitiesMatrix.resize(numElements);
    entitiesMatrix[0] = 1;

    struct
    {
        bool m_set;
        uint32_t m_maxElements;
    } maxEntity;
    for (uint32_t i = 1; i < numElements; ++i)
    {
        maxEntity.m_set = false;
        for (uint32_t j = 0; j < i; ++j)
        {
            if ((m_elements[i] > m_elements[j]) &&
                (!maxEntity.m_set ||
                entitiesMatrix[j] > maxEntity.m_maxElements))
            {
                maxEntity.m_set = true;
                maxEntity.m_maxElements = entitiesMatrix[j];
            }
        }
        if (!maxEntity.m_set)
        {
            entitiesMatrix[i] = 1;
        }
        else
        {
            entitiesMatrix[i] = maxEntity.m_maxElements + 1;
        }
    }

    res = 0;
    for (auto& entity : entitiesMatrix)
    {
        if (entity > res)
        {
            res = entity;
        }
    }

    return true;
}

bool LongestIncrSubseq::runAlgorithm2(uint32_t& res /*todo: sequence*/)
{
    uint32_t numElements = static_cast<uint32_t>(m_elements.size());
    std::map<uint32_t, uint32_t> entities;
    entities[m_elements[0]] = 1;

    struct
    {
        bool m_set;
        uint32_t m_maxElements;
    } maxEntity;
    for (uint32_t i = 1; i < numElements; ++i)
    {
        maxEntity.m_set = false;
        for (auto it = entities.begin(); it != entities.end() && it->first < m_elements[i]; ++it)
        {
            if (!maxEntity.m_set ||
                (it->second > maxEntity.m_maxElements))
            {
                maxEntity.m_set = true;
                maxEntity.m_maxElements = it->second;
            }
        }

        if (!maxEntity.m_set)
        {
            auto it = entities.find(m_elements[i]);
            if (entities.end() == it)
            {
                entities.insert(std::make_pair(m_elements[i], 1));
            }
        }
        else
        {
            auto it = entities.find(m_elements[i]);
            if (entities.end() == it)
            {
                entities.insert(std::make_pair(m_elements[i], maxEntity.m_maxElements + 1));
            }
            else if (it->second < maxEntity.m_maxElements + 1)
            {
                it->second = maxEntity.m_maxElements + 1;
            }
        }
    }

    res = 0;
    for (auto& entity : entities)
    {
        if (entity.second > res)
        {
            res = entity.second;
        }
    }

    return true;
}

bool LongestIncrSubseq::runOptimizedAlgorithm(uint32_t& res /*todo: sequence*/)
{
    uint32_t numElements = static_cast<uint32_t>(m_elements.size());
    std::vector<uint32_t> sortedElements;
    sortedElements.reserve(numElements);
    sortedElements.push_back(m_elements[0]);
    res = 1;
    for (uint32_t i = 1; i < numElements; ++i)
    {
        if (m_elements[i] < sortedElements.front())
        {
            sortedElements.front() = m_elements[i];
        }
        else if (m_elements[i] > sortedElements.back())
        {
            sortedElements.push_back(m_elements[i]);
            ++res;
        }
        else
        {
            auto it = std::lower_bound(sortedElements.begin(), sortedElements.end(), m_elements[i]);
            if (sortedElements.end() != it)
            {
                *it = m_elements[i];
            }
        }
    }

    return true;
}

std::istream& operator>>(std::istream& in, LongestIncrSubseq& l)
{
    uint32_t numElements = 0;
    in >> numElements;
    l.m_elements.resize(numElements);
    for (auto& element : l.m_elements)
    {
        in >> element;
    }
    return in;
}
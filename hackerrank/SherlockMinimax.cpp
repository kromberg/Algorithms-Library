#include <algorithm>

#include "SherlockMinimax.h"

std::istream& operator>> (std::istream& in, SherlockMinimax& sherlockMinimax)
{
    in >> sherlockMinimax.m_numElements;
    sherlockMinimax.m_elements.resize(sherlockMinimax.m_numElements);
    for (auto& element : sherlockMinimax.m_elements)
    {
        in >> element;
    }
    in >> sherlockMinimax.m_p >> sherlockMinimax.m_q;
    return in;
}

SherlockMinimax::SherlockMinimax():
    m_numElements(0),
    m_p(0),
    m_q(0)
{
}

SherlockMinimax::~SherlockMinimax()
{
}

bool SherlockMinimax::runAlgorithm(int32_t& res, int32_t& val)
{
    struct
    {
        int32_t m_maxMinVal;
        int32_t m_mVal;
    } maxMinValue;

    std::sort(m_elements.begin(), m_elements.end());

    if (m_p <= m_elements.front() && m_q >= m_elements.back())
    {
        maxMinValue.m_mVal = m_p;
        maxMinValue.m_maxMinVal = (m_elements.front() - m_p);
        if (m_q - m_elements.back() > maxMinValue.m_maxMinVal)
        {
            maxMinValue.m_mVal = m_q;
            maxMinValue.m_maxMinVal = (m_q - m_elements.back());
        }
        for (uint32_t i = 0; i < m_numElements - 1; ++i)
        {
            int32_t mid = (m_elements[i + 1] + m_elements[i]) / 2;
            int32_t diff = (m_elements[i + 1] - m_elements[i]) / 2;
            if (diff > maxMinValue.m_maxMinVal)
            {
                maxMinValue.m_mVal = mid;
                maxMinValue.m_maxMinVal = diff;
            }
        }
    }
    else if (m_p <= m_elements.front())
    {
        maxMinValue.m_mVal = m_p;
        maxMinValue.m_maxMinVal = (m_elements.front() - m_p);

        for (uint32_t i = 0; i < m_numElements - 1; ++i)
        {
            int32_t mid = (m_elements[i + 1] + m_elements[i]) / 2;
            if (mid > m_q)
            {
                if (m_q - m_elements[i] > maxMinValue.m_maxMinVal)
                {
                    maxMinValue.m_mVal = m_q;
                    maxMinValue.m_maxMinVal = m_q - m_elements[i];
                }
                break;
            }
            int32_t diff = (m_elements[i + 1] - m_elements[i]) / 2;
            if (diff > maxMinValue.m_maxMinVal)
            {
                maxMinValue.m_mVal = mid;
                maxMinValue.m_maxMinVal = diff;
            }
        }
    }
    else if (m_q >= m_elements.back())
    {
        maxMinValue.m_mVal = m_q;
        maxMinValue.m_maxMinVal = (m_q - m_elements.back());

        for (uint32_t i = 0; i < m_numElements - 1; ++i)
        {
            int32_t mid = (m_elements[i + 1] + m_elements[i]) / 2;
            if (mid < m_p)
            {
                if (m_p <= m_elements[i + 1])
                {
                    if (m_elements[i + 1] - m_p > maxMinValue.m_maxMinVal)
                    {
                        maxMinValue.m_mVal = m_p;
                        maxMinValue.m_maxMinVal = m_elements[i + 1] - m_p;
                    }
                }
                continue;
            }
            int32_t diff = (m_elements[i + 1] - m_elements[i]) / 2;
            if (diff > maxMinValue.m_maxMinVal)
            {
                maxMinValue.m_mVal = mid;
                maxMinValue.m_maxMinVal = diff;
            }
        }
    }
    else
    {
        maxMinValue.m_maxMinVal = -1;
        for (uint32_t i = 0; i < m_numElements - 1; ++i)
        {
            int32_t mid = (m_elements[i + 1] + m_elements[i]) / 2;
            if (mid < m_p)
            {
                if (m_p <= m_elements[i + 1])
                {
                    if (m_elements[i + 1] - m_p > maxMinValue.m_maxMinVal)
                    {
                        maxMinValue.m_mVal = m_p;
                        maxMinValue.m_maxMinVal = m_elements[i + 1] - m_p;
                    }
                }
                continue;
            }
            else if (mid > m_q)
            {
                if (m_q - m_elements[i] > maxMinValue.m_maxMinVal)
                {
                    maxMinValue.m_mVal = m_q;
                    maxMinValue.m_maxMinVal = m_q - m_elements[i];
                }
                break;
            }
            int32_t diff = (m_elements[i + 1] - m_elements[i]) / 2;
            if (diff > maxMinValue.m_maxMinVal)
            {
                maxMinValue.m_mVal = mid;
                maxMinValue.m_maxMinVal = diff;
            }
        }
    }
    res = maxMinValue.m_mVal;
    val = maxMinValue.m_maxMinVal;

    return true;
}
#include "AlmostSorted.h"


AlmostSorted::AlmostSorted()
{
}


AlmostSorted::~AlmostSorted()
{
}

bool AlmostSorted::runAlgorithm(Anomaly& anomaly)
{
    std::vector<uint32_t> anomaly1;
    std::vector<uint32_t> anomaly2;
    if (m_numElements == 1)
    {
        return true;
    }
    else if (m_numElements == 2)
    {
        if (m_array[0] > m_array[1])
        {
            anomaly.m_what = "swap";
            anomaly.m_indices = std::make_pair(0, 1);
        }
        return true;
    }
    if (m_array[0] > m_array[1])
    {
        anomaly1.push_back(0);
    }
    for (uint32_t i = 0; i < m_numElements - 2; ++i)
    {
        if (m_array[i] < m_array[i + 1] &&
            m_array[i + 1] > m_array[i + 2] &&
            m_array[i] < m_array[i + 2])
        {
            anomaly1.push_back(i + 1);
        }
        else if (m_array[i] > m_array[i + 1] &&
                 m_array[i + 2] > m_array[i + 1] &&
                 m_array[i] < m_array[i + 2])
        {
            anomaly2.push_back(i + 1);
        }
        /*else if (m_array[i] <= m_array[i + 1] && m_array[i + 1] <= m_array[i + 2])
        {
        continue;
        }*/
    }
    if (m_array[m_numElements - 2] > m_array[m_numElements - 1])
    {
        anomaly2.push_back(m_numElements - 1);
    }
    if (anomaly1.size() == 0 && anomaly2.size() == 0)
    {
        return true;
    }

    if (anomaly1.size() == 1 && anomaly2.size() == 1)
    {
        std::swap(m_array[anomaly1.front()], m_array[anomaly2.front()]);
        bool good = true;
        for (uint32_t i = 0; i < m_numElements - 1; ++i)
        {
            if (m_array[i] > m_array[i + 1])
            {
                good = false;
                break;
            }
        }
        if (good)
        {
            anomaly.m_what = "swap";
            anomaly.m_indices = std::make_pair(anomaly1.front(), anomaly2.front());
            return true;
        }
        std::swap(m_array[anomaly1.front()], m_array[anomaly2.front()]);
    }
    std::vector<std::pair<uint32_t, uint32_t> > anomaly3;
    std::vector<std::pair<uint32_t, uint32_t> > anomaly4;
    auto currentAnomaly = anomaly3.end();
    for (uint32_t i = 0; i < m_numElements - 1; ++i)
    {
        if (m_array[i] > m_array[i + 1])
        {
            if (anomaly3.end() == currentAnomaly)
            {
                currentAnomaly = anomaly3.insert(anomaly3.end(), std::make_pair(i, i + 1));
            }
            else
            {
                currentAnomaly->second = i + 1;
            }
        }
        else
        {
            if (anomaly3.end() != currentAnomaly)
            {
                if (currentAnomaly->first == 0)
                {
                    if (m_array[currentAnomaly->first] > m_array[i + 1])
                    {
                        anomaly3.erase(currentAnomaly);
                    }
                }
                else
                {
                    /* debug
                    std::cout << "First - 1 : " << m_array[currentAnomaly->first - 1] << std::endl;
                    std::cout << "First : " << m_array[currentAnomaly->first] << std::endl;
                    std::cout << "Second : " << m_array[currentAnomaly->second] << std::endl;
                    std::cout << "Second + 1: " << m_array[currentAnomaly->second + 1] << std::endl;
                    */

                    if ((m_array[currentAnomaly->first] > m_array[currentAnomaly->second + 1]) ||
                        (m_array[currentAnomaly->first - 1] > m_array[currentAnomaly->second]))
                    {
                        anomaly4.push_back(*currentAnomaly);
                        anomaly3.erase(currentAnomaly);
                    }
                }
                currentAnomaly = anomaly3.end();
            }
        }
    }
    if (anomaly3.end() != currentAnomaly)
    {
        if (currentAnomaly->first != 0)
        {
            if (m_array[currentAnomaly->first - 1] > m_array[currentAnomaly->second])
            {
                anomaly4.push_back(*currentAnomaly);
                anomaly3.erase(currentAnomaly);
            }
        }
    }

    if (anomaly4.empty() && anomaly3.size() == 1)
    {
        anomaly.m_what = "reverse";
        anomaly.m_indices = anomaly3.front();
        return true;
    }

    return false;
}

std::istream& operator>>(std::istream& in, AlmostSorted& almostSorted)
{
    in >> almostSorted.m_numElements;
    almostSorted.m_array.resize(almostSorted.m_numElements);
    for (auto& element : almostSorted.m_array)
    {
        in >> element;
    }
    return in;
}
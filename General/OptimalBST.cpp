#include <algorithm>

#include "OptimalBST.h"


OptimalBST::Node::Node():
    m_left(),
    m_right(),
    m_idx(0)
{}

OptimalBST::Node::~Node()
{
    m_left.reset();
    m_right.reset();
}

OptimalBST::MatrixEntity::MatrixEntity() :
    m_val(0),
    m_rootPos(0)
{}

OptimalBST::MatrixEntity::MatrixEntity(const MatrixEntity& matrixEntity):
    m_val(matrixEntity.m_val),
    m_rootPos(matrixEntity.m_rootPos)
{}

OptimalBST::MatrixEntity::MatrixEntity(MatrixEntity&& matrixEntity)
{
    m_val = matrixEntity.m_val;
    m_rootPos = matrixEntity.m_rootPos;
    matrixEntity.m_val = 0;
    matrixEntity.m_rootPos = 0;
}

std::ostream& operator<< (std::ostream& out, OptimalBST::MatrixEntity& matrixEntity)
{
    out << "[rootPos : " << matrixEntity.m_rootPos << " ; val : " << matrixEntity.m_val << ']';
    return out;
}

OptimalBST::OptimalBST():
    m_probabilities()
{
}

OptimalBST::~OptimalBST()
{
}

void OptimalBST::clear()
{
    m_probabilities.clear();
}

bool OptimalBST::read(std::istream& in)
{
    clear();

    uint32_t numElements = 0;
    in >> numElements;
    m_probabilities.resize(numElements);

    for (auto& probability : m_probabilities)
    {
        in >> probability;
    }

    return true;
}

bool OptimalBST::run(double &c)
{

    uint32_t numProbabilities = static_cast<uint32_t>(m_probabilities.size());
    m_matrix.resize(numProbabilities, numProbabilities);

    // utility structures
    MatrixEntity minVal;
    double tmpVal = 0;
    for (uint32_t s = 0; s < numProbabilities; ++s)
    {
        for (uint32_t i = 0; i < numProbabilities - s; ++i)
        {
            double probSum = 0;
            for (uint32_t k = i; k <= i + s; ++k)
            {
                probSum += m_probabilities[k];
            }

            // r = i
            minVal.m_rootPos = i;
            minVal.m_val = ((i + 1) > (i + s)) ? 0 : m_matrix[i + 1][i + s].m_val;
            for (uint32_t r = i + 1; r <= i + s; ++r)
            {
                tmpVal = ((i + 1 > r) ? 0 : m_matrix[i][r - 1].m_val) +
                    (((r + 1) > (i + s)) ? 0 : m_matrix[r + 1][i + s].m_val);
                if (tmpVal < minVal.m_val)
                {
                    minVal.m_val = tmpVal;
                    minVal.m_rootPos = r;
                }
            }
            minVal.m_val += probSum;
            m_matrix[i][i + s] = std::move(minVal);
        }
    }

    c = m_matrix[0][numProbabilities - 1].m_val;

    formTree(m_tree, m_matrix, 0, numProbabilities - 1);

    return true;
}

void OptimalBST::formTree(
    std::unique_ptr<Node>& tree,
    Matrix<MatrixEntity>& matrix,
    const uint32_t begin,
    const uint32_t end)
{
    if (begin > end)
    {
        return;
    }
    else if (begin == end)
    {
        tree.reset(new Node);
        tree->m_idx = begin;
        return;
    }
    tree.reset(new Node);
    tree->m_idx = matrix[begin][end].m_rootPos;
    formTree(tree->m_left, matrix, begin, (matrix[begin][end].m_rootPos >= 1) ? (matrix[begin][end].m_rootPos - 1) : 0);
    formTree(tree->m_right, matrix, matrix[begin][end].m_rootPos + 1, end);
}

void OptimalBST::writeTree(
    const std::unique_ptr<Node>& tree,
    std::ostream& out,
    const std::string& indent)
{
    if (!tree)
    {
        return;
    }
    writeTree(tree->m_right, out, indent + " ");
    out << indent << tree->m_idx << " : " << m_probabilities[tree->m_idx] << std::endl;
    writeTree(tree->m_left, out, indent + " ");
}

bool OptimalBST::write(std::ostream& out)
{
    out << m_matrix;

    uint32_t numProbabilities = static_cast<uint32_t> (m_probabilities.size());
    for (uint32_t i = 0; i < numProbabilities; ++i)
    {
        out << i << " : " << m_probabilities[i] << std::endl;
    }

    // write tree
    writeTree(m_tree, out);
    return true;
}
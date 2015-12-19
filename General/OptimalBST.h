#ifndef OPTIMAL_BST_H
#define OPTIMAL_BST_H

#include <memory>
#include <vector>
#include <fstream>
#include <string>
#include <cstdint>

#include "Matrix.h"

class OptimalBST
{
private:
    struct Node
    {
        std::unique_ptr<Node> m_left;
        std::unique_ptr<Node> m_right;
        uint32_t m_idx;
        Node();
        ~Node();
    };
    struct MatrixEntity
    {
        double m_val;
        uint32_t m_rootPos;
        MatrixEntity();
        MatrixEntity(MatrixEntity&& matrixEntity);
    };
    friend std::ostream& operator<< (std::ostream& out, MatrixEntity& matrixEntity);

    std::vector<double> m_probabilities;
    Matrix<MatrixEntity> m_matrix;
    std::unique_ptr<Node> m_tree;

private:
    void formTree(
        std::unique_ptr<Node>& tree,
        Matrix<MatrixEntity>& matrix,
        const uint32_t begin,
        const uint32_t end);
    void writeTree(
        const std::unique_ptr<Node>& tree,
        std::ostream& out,
        const std::string& indent = "");
    void clear();

public:
    OptimalBST();
    ~OptimalBST();

    bool read(std::istream& in);
    bool write(std::ostream& out);
    bool run(double &c);
};

#endif // OPTIMAL_BST_H
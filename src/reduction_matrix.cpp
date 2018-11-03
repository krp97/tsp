#include "../include/reduction_matrix.hpp"

namespace tsp{

    Reduction_Matrix::Reduction_Matrix(Adjacency_Matrix matrix)
        : Adjacency_Matrix(matrix)
    {}

    Reduction_Matrix::Reduction_Matrix(std::vector<std::vector<int>> matrix)
        : Adjacency_Matrix(matrix)
    {}

    const std::vector<int>& Reduction_Matrix::operator[](int index) const
    {
        return a_matrix_[index];
    }

    void Reduction_Matrix::fill_passed_nodes(int row, int column)
    {
        fill_row(row);
        fill_column(column);
        a_matrix_[column][row] = INT_MAX;
        a_matrix_[column][0] = INT_MAX;
    }

    void Reduction_Matrix::fill_row(int row)
    {
        std::fill_n(a_matrix_[row].begin(), a_matrix_.size(), INT_MAX);
    }

    void Reduction_Matrix::fill_column(int column)
    {
        for(auto& rows: a_matrix_)
            rows[column] = INT_MAX;
    }

    void Reduction_Matrix::reduce_matrix()
    {
        int reduction{0};
        reduction += horizontal_reduction();
        reduction += vertical_reduction();
        reduction_cost_ = reduction;
    }

    // Reduces the matrix by the smallest factor row wise.
    int Reduction_Matrix::horizontal_reduction()
    {
        std::vector<int>::iterator min;
        int reduction{0};
        for(auto& rows : a_matrix_)
        {
            min = std::min_element(rows.begin(), rows.end());
            if(*min != INT_MAX)
            {
                reduction += *min;
                std::for_each(rows.begin(), rows.end(), [&min](int& element){
                if(element != INT_MAX)
                    element -= *min;
            });}
        }
        return reduction;
    }

    // Reduces the matrix by the smallest factor column wise.
    int Reduction_Matrix::vertical_reduction()
    {
        int min{0}, reduction{0};
        for(size_t i{0}; i < a_matrix_.size(); ++i)
        {
            min = get_column_min(i);
            if(min != INT_MAX)
            {
                reduction += min;
                for(auto& rows: a_matrix_)
                {
                    if(rows[i] != INT_MAX)
                        rows[i] -= min;
                }}
        }
        return reduction;
    }

    int Reduction_Matrix::get_column_min(const size_t column) const
    {
        int min{INT_MAX};
        for(auto& rows: a_matrix_)
        {
            if(rows[column] < min)
                min = rows[column];
        }
        return min;
    }

    int Reduction_Matrix::get_reduction_cost() const
    {
        return reduction_cost_;
    }

    std::vector<int> Reduction_Matrix::get_neighbours(int node) const
    {
        auto neighbours{ std::vector<int>() };
        for(int i{0}; i < a_matrix_[node].size(); ++i)
        {
            if(a_matrix_[node][i] != INT_MAX && i != 0)
                neighbours.push_back(i);
        }
        return neighbours;
    }
}
#include "../include/branch_n_bound.hpp"
#include <iostream>

namespace tsp{

    Adjacency_Matrix branch_n_bound::matrix_ = Adjacency_Matrix();
    int branch_n_bound::best_cost_ = INT_MAX;

    void branch_n_bound::init_matrix()
    {
        for(int i{0}; i < matrix_.size(); ++i)
        {
            matrix_[i][i] = INT_MAX;
        }
    }

    Path branch_n_bound::bfs(const Adjacency_Matrix& matrix)
    {
        branch_n_bound::matrix_ = matrix;
        branch_n_bound::best_cost_ = INT_MAX;
        init_matrix();
        auto city_q{ city_p_queue() };
        city_q.push(City(0, branch_n_bound::matrix_.data(), 0));
        return bfs(city_q);
    }

    Path branch_n_bound::bfs(branch_n_bound::city_p_queue& city_q)
    {
        Path best_path;
        while(!city_q.empty())
        {
            City current_city{ city_q.top() };
            city_q.pop();

            if(current_city.get_path_size() == matrix_.size())
                update_cost(current_city, best_path);
            else if(current_city.get_bound() < best_cost_)
                push_child_cities(current_city, city_q);
        }
        return best_path;
    }

    void branch_n_bound::update_cost(City& city, Path& best_path)
    {
        if(city.get_bound() < best_cost_)
        {
            best_cost_ = city.get_bound();
            best_path = city.get_path();
        }
        else if(city.get_bound() == best_cost_)
            if(city.get_path().cost_ < best_path.cost_)
                best_path = city.get_path();
    }

    void branch_n_bound::push_child_cities(City& parent_city, branch_n_bound::city_p_queue& city_q)
    {
        int parent_index{ parent_city.get_index() }, travel_cost;
        std::vector<int> neighbours{ parent_city.get_neighbours() };

        for(auto& neighbour: neighbours)
        {
            travel_cost = parent_city.get_travel_cost(parent_index, neighbour);
            city_q.push(
                City(parent_city, neighbour, travel_cost));
        }
    }
}
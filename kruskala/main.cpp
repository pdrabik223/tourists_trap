//
// Created by pc on 03.05.2021.
//
#include <iostream>

struct triplet {
    triplet(int first, int second, int cost) : first(first), second(second), cost(cost) {}

    bool operator==(const triplet &rhs) const {
        return cost == rhs.cost;
    }

    bool operator!=(const triplet &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const triplet &rhs) const {
        return cost < rhs.cost;
    }

    bool operator>(const triplet &rhs) const {
        return rhs.cost < cost;
    }

    bool operator<=(const triplet &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const triplet &rhs) const {
        return !(*this < rhs);
    }


    int first;
    int second;
    int cost;

};

class Node {
public:
  Node() {
        value = 0;
        left_ptr = nullptr;
        left_cost = 0;
        right_ptr = nullptr;
        right_cost = 0;
    }

    Node(int val, int left_val, int right_val) {
        value = val;
        left_ptr = nullptr;
        left_cost = 0;
        right_ptr = nullptr;
        right_cost = 0;
    }

private:
    int value;
    Node *left_ptr;
    int left_cost;
    Node *right_ptr;
    int right_cost;
};

int main() {

    std::cout << "it's a trap\n";


    return 0;
}

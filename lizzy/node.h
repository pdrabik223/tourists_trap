//
// Created by pc on 11.05.2021.
//

#ifndef TURISTS_TRAP_NODE_H
#define TURISTS_TRAP_NODE_H

#include "../windows_console_tools/win_colors.h"

#define MILLION 1000000
#define SHOW_VEC(x) for(auto i:x) { std::cout<<i<<" ";} std::cout<<std::endl

#include <ostream>
#include <cassert>
#include <vector>
class node;

node *find_in_vec(std::vector<node> &data, int city);

int find_position_in_vec(const std::vector<node> &data, int search);

int find_position_in_vec(const std::vector<node *> &data, int search);

int road_trips(int from, int to, int pssngrs, node &tree);

struct threesome {
    int from;
    int to;
    int weight;

    threesome(int from, int to, int weight) : from(from), to(to), weight(weight) {}

    bool operator==(const threesome &other) const {
        return weight == other.weight;
    }

    bool operator!=(const threesome &other) const {
        return weight != other.weight;
    }

    bool operator>(const threesome &other) const {
        return weight > other.weight;
    }

    bool operator<(const threesome &other) const {
        return weight < other.weight;
    }

    bool operator>=(const threesome &other) const {
        return weight >= other.weight;
    }

    bool operator<=(const threesome &other) const {
        return weight <= other.weight;
    }

    friend std::ostream &operator<<(std::ostream &out, const threesome &dt) {
        out << "from " << dt.from;
        out << " to " << dt.to;
        out << " weight " << dt.weight << "\n";
        return out;
    }


};

class node {
public:
    int city_name;
    std::vector<node *> nodes;
    /// weight leading to this city
    /// first one of course has weight 0
    /// so it's cost of coming into
    int weight;

    /// ptr to father object
    node *father;

    node(int name);

    node(int weight, int cityName);;

    node &operator=(const node &other);

    node(const node &other);

    bool operator==(const node &other) const;

    bool operator!=(const node &other) const;


    /// creates a copy of a node (with all children)
    /// and connects this copy to current node
    void append_in_place(const node &other, int weight);

    int minimal_route_to(int to, int minimal_value = MILLION);

    node *search(int value);

    void make_root();

    int ultimate_father();

    ~node() = default;

    friend std::ostream &operator<<(std::ostream &out, const node &dt);

};

};


#endif //TURISTS_TRAP_NODE_H

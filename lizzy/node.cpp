//
// Created by pc on 11.05.2021.
//

#include "node.h"

int find_position_in_vec(const std::vector<node> &data, int search) {
    for (int i = 0; i < data.size(); i++) {
        if (data[i].city_name == search)
            return i;
    }
    return data.size();
}

int find_position_in_vec(const std::vector<node *> &data, int search) {
    for (int i = 0; i < data.size(); i++) {
        if (data[i]->city_name == search)
            return i;
    }
    return data.size();
}
int road_trips(int from, int to, int pssngrs, node &tree) {

    if (tree.search(from)->search(to) != nullptr) {
        return tree.search(from)->minimal_route_to(to);

    } else return tree.search(to)->minimal_route_to(from);
    assert(false);
}


node *find_in_vec(std::vector<node> &data, int city) {
    node *temp = nullptr;
    for (auto &i : data) {

        temp = i.search(city);
        if (temp != nullptr) return temp;

    }
    assert(false);
    return nullptr;
}

node::node(int name) : city_name(name) {
    father = nullptr;
    weight = 0;
    nodes = {};
}

node &node::operator=(const node &other) {
    if (this == &other) return *this;

    if (other.father != nullptr)father = other.father;
    else father = nullptr;

    city_name = other.city_name;
    weight = other.weight;
    nodes = {};


    for (int i = 0; i < other.nodes.size(); i++) {
        nodes.push_back(new node(*(other.nodes[i])));
    }

    return *this;
}

bool node::operator==(const node &other) const {
    return city_name == other.city_name;
}
bool node::operator!=(const node &other) const {
    return city_name != other.city_name;
}

node::node(const node &other) {
    if (other.father != nullptr)father = other.father;
    else father = nullptr;

    city_name = other.city_name;
    weight = other.weight;
    nodes = {};


    for (int i = 0; i < other.nodes.size(); i++) {
        nodes.push_back(new node(*(other.nodes[i])));
    }
}

void node::append_in_place(const node &other, int weight) {

    nodes.push_back(new node(other));
    nodes.back()->weight = weight;
    nodes.back()->father = this;

}

int node::minimal_route_to(int to, int minimal_value) {

    if (city_name == to) return minimal_value;

    if (nodes.empty()) return MILLION;

    int temp_int;

    for (unsigned int i = 0; i < nodes.size(); i++) {

        if (nodes[i]->weight < minimal_value) minimal_value = nodes[i]->weight;

        temp_int = nodes[i]->minimal_route_to(to, minimal_value);
        if (temp_int != MILLION) return temp_int;

    }

    return MILLION;
}

node *node::search(int value) {

    if (city_name == value) return this;
    if (nodes.empty()) return nullptr;

    node *ptr_to_return = nullptr;

    for (auto i : nodes) {
        ptr_to_return = i->search(value);
        if (ptr_to_return != nullptr) return ptr_to_return;
    }

    return ptr_to_return;
}

void node::make_root() {
    /// for assert purposes
    auto temp_father = father;

    if (father == nullptr) return;
    /// make sure our father is root
    if (father->father)father->make_root();

    { //if we are second to top



        father->weight = weight;
        father->father = this;

        nodes.push_back(new node(*father));

        /// find ptr to myself in father nodes vector
        int my_position_in_fathers_nodes = find_position_in_vec(nodes.back()->nodes, city_name);

        assert (my_position_in_fathers_nodes != father->nodes.size());
        /// and delete that element  but not clear memory under
        /// basically make sure that element does not get deleted
        /// only removed from array
        nodes.back()->nodes[my_position_in_fathers_nodes] = nullptr;
        nodes.back()->nodes.erase(nodes.back()->nodes.begin() + my_position_in_fathers_nodes);


        weight = 0;
        father = nullptr;


    }


    // assert(search(temp_father->city_name)->city_name == temp_father->city_name);

}

int node::ultimate_father() {
    if (father == nullptr) return city_name;
    else return father->ultimate_father();

}

std::ostream &operator<<(std::ostream &out, const node &dt) {
    out << cc(red) << "name: " << dt.city_name << " weight: " << dt.weight << " children: " << dt.nodes.size()
        << "\n";

    for (auto i:dt.nodes) {
        out << cc(yellow) << i->city_name << "\t";
    }
    out << "\n";

    for (auto i:dt.nodes) {
        out << *i;
    }
    return out;
}

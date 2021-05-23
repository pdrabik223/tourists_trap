//
// Created by studio25 on 22.05.2021.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>


#define SHOW_VEC(x) for(auto i:x) std::cout<<i
#define SHOW_TRANSLATION(x) for(auto i:x)std::cout<<
#define SHOW_USED(x) for(int i=0;i<x.size();i++){if(x[i])std::cout<<i<<" ";}

struct threesome {
    threesome(int from, int to, int weight) : from(from), to(to), weight(weight) {}

    bool operator==(const threesome &rhs) const {
        return weight == rhs.weight;
    }

    bool operator!=(const threesome &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const threesome &rhs) const {
        return weight < rhs.weight;
    }

    bool operator>(const threesome &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const threesome &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const threesome &rhs) const {
        return !(*this < rhs);
    }

    int from;
    int to;
    int weight;

    friend std::ostream &operator<<(std::ostream &out, threesome &rhs) {
        out << "from: " << rhs.from;
        out << " to: " << rhs.to;
        out << " weight: " << rhs.weight << std::endl;
        return out;
    }

};

using threevec = std::vector<threesome>;
using dicvec = std::vector<std::pair<int, std::string>>;

struct node {

    /// value of a road leading to me
    /// now if node is root road leading to me doesn't exist therefore weight = 0
    int weight;
    /// my name
    int name;
    /// ptrs to my children
    std::vector<node *> children;

    node() {
        /// the names were defined as "not bigger than 32767" so by default we set it as just above the given range
        name = 32768;
        weight = 0;
        children = {};

    }

    explicit node(int name) : name(name) {
        weight = 0;
        children = {};
    }

    node(int name, int weight) : weight(weight), name(name) {
        children = {};
    }

    bool operator==(int other_name) const {
        return name == other_name;
    }

    bool operator!=(int other_name) const {
        return name == other_name;
    }

    void append(threesome connection) {
        find(connection.from)->
                children.push_back(new node(connection.to, connection.weight));
    }

    void set(threesome connection) {
        weight = 0;
        name = connection.from;
        children.push_back(new node(connection.to, connection.weight));
    }

    node *find(int search_name) {
        if (name == search_name) return this;

        for (auto i:children) {
            node *temp_ptr = i->find(search_name);
            if (temp_ptr) return temp_ptr;
        }
        return nullptr;
    }

    friend std::ostream &operator<<(std::ostream &out, node &rhs) {
        out << "name: " << rhs.name;
        out << " weight: " << rhs.weight;
        out << " children :\n";
        for (auto &i : rhs.children) {
            out << i->name << " ";
        }
        std::cout << "\n";
        for (auto &i : rhs.children) {
            i->show(1);
        }
        out << "\n";
        return out;
    }

    void show(int depth = 0) {
        for (int i = 0; i < depth; i++)std::cout << "  ";
        std::cout << "name: " << name;
        std::cout << " weight: " << weight;
        std::cout << " children :\n";
        for (int i = 0; i < depth; i++)std::cout << "  ";

        for (auto &i : children) {
            std::cout << i->name << " ";
        }
        std::cout << "\n";
        for (auto &i : children) {
            i->show(depth + 1);
        }

        std::cout << "\n";
    }
};


class h_table {
public:
    h_table(size_t size) : size_(size ) {
        storage = new bool[size_ ];
        memset(storage, 0, size_);

    };

    void append(size_t value) { storage[value] = true; }

    bool operator[](size_t index) const { return storage[index]; }

    ~h_table() {

        delete[]storage;
    }

    int size() const { return size_; }

protected:
    size_t size_;
    bool *storage;

};

int main() {
    int number_of_nodes;
    std::cin >> number_of_nodes;
    int int_temp;
    std::string string_temp;
    dicvec node_dictionary = {};

    for (int i = 0; i < number_of_nodes; i++) {
        std::cin >> int_temp;
        std::cin >> string_temp;
        node_dictionary.push_back({int_temp, string_temp});
    }

    int number_of_connections;
    std::cin >> number_of_connections;
    threevec node_connections;
    int temp_from, temp_to, temp_weight;
    for (int i = 0; i < number_of_connections; i++) {
        std::cin >> temp_from;
        std::cin >> temp_to;
        std::cin >> temp_weight;
        node_connections.push_back({temp_from, temp_to, temp_weight});
    }

    /// end of data input
    /// first sort the received connections
    std::sort(node_connections.begin(), node_connections.end());
    SHOW_VEC(node_connections);

    /// next we declare the tree root, based on best connections in the graph
    node root;
    root.set(node_connections.front());



    /// next we need another vector that will hold the names of nodes already existing on three
    /// this is attempt to cut down time off accessing the list of elements on three
    h_table used_names(5);

    used_names.append(node_connections.front().from);

    used_names.append(node_connections.front().to);

    SHOW_USED(used_names);
    std::cout << " <first!";
    /// next we delete first connection form connections vector
    node_connections.erase(node_connections.begin());



    /// as long as all the cities aren't on the tree
    /// id est as long as all cities aren't connected
    while (used_names.size() != number_of_nodes) {


        ///pick the next best connection, but it must already be some way connected ou our tree!
        /// and because our connections are two way connections we don't really have "from, to" relationship
        /// so i guess we need to factor that in
        for (int i = 0; i < node_connections.size(); i++) {
            if (used_names[node_connections[i].from] &&
                !used_names[node_connections[i].to]) {

                root.append(node_connections[i]);
                used_names.append(node_connections[i].from);
                node_connections.erase(node_connections.begin() + i);
                SHOW_USED(used_names);
                break;
            } else if (used_names[node_connections[i].to] &&
                       !used_names[node_connections[i].from]) {

                std::swap(node_connections[i].to, node_connections[i].from);
                std::cout << "swapped";
                SHOW_USED(used_names);
                root.append(node_connections[i]);
                used_names.append(node_connections[i].from);
                node_connections.erase(node_connections.begin() + i);


                break;
            }
        }
    }
    std::cout << "after teh thing\n";

    SHOW_USED(used_names);
    std::cout << "\nafter teh thing\n";

    std::cout << root;

    return 0;
}


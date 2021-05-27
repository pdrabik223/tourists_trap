//
// Created by studio25 on 22.05.2021.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <cassert>


#define SHOW_VEC(x) for(auto i:x) std::cout<<i
#define SHOW_TRANSLATION(x) for(auto i:x)std::cout<<
#define SHOW_USED(x) std::cout<<x<<std::endl;

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

    node(int name, int weight) : name(name), weight(weight) {
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
        rhs.show();
        return out;
    }

    void show(int depth = 0) {
        for (int i = 0; i < depth; i++)std::cout << "  ";
        std::cout << "name: " << name;
        std::cout << " weight: " << weight;
        std::cout << " children: " << children.size() << "\n";
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

/// bool hash table<br>
/// i.e. hash table that is filled with booleans<br>
/// fast and fail proof way to store big amounts of booleans<br>
/// hash table concept assures fast addressing time<br>
class bh_table {
public:
    bh_table(size_t size) {
        allocated_size = size;
        size_ = 0;
        storage = new bool[allocated_size];
        for (int i = 0; i < allocated_size; i++) {
            storage[i] = false;
        }
    };

    void append(size_t value) {
        assert(value > 0); /// if for whatever reason we have node name 0
        --value; /// that's because in our example node names start at one;
        assert(value < allocated_size);
        if (storage[value]) return;
        storage[value] = true;
        ++size_;
    }

    bool &operator[](size_t index) {
        assert(index > 0); /// if for whatever reason we have node name 0
        --index; /// that's because in our example node names start at one;
        assert(index < allocated_size);
        return storage[index];
    }

    ~bh_table() { delete[] storage; }

    size_t size() const { return size_; }

    bool filled() { return size_ == allocated_size; }

    friend std::ostream &operator<<(std::ostream &out, bh_table &rhs) {
        for (int i = 0; i < rhs.size(); i++) { if (rhs[i + 1])std::cout << i + 1 << " "; }
        return out;
    }


protected:
    size_t allocated_size;
    size_t size_;
    bool *storage;

};

int main() {
    int number_of_nodes;
    std::cin >> number_of_nodes;
    dicvec node_dictionary = {};
    {
        std::string string_temp;
        int int_temp;
        for (int i = 0; i < number_of_nodes; i++) {
            std::cin >> int_temp;
            std::cin >> string_temp;
            /// because (i guess) town names fall in range <1;32767>
            /// i can subtract one from every name so i get range <0;32766>
            /// it will be easier that way
            node_dictionary.push_back({int_temp, string_temp});

        }
    }
    int number_of_connections;
    std::cin >> number_of_connections;
    threevec node_connections;
    {
        int temp_from, temp_to, temp_weight;
        for (int i = 0; i < number_of_connections; i++) {

            std::cin >> temp_from;
            std::cin >> temp_to;
            std::cin >> temp_weight;
            node_connections.push_back({temp_from, temp_to, temp_weight});
        }
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
    bh_table used_names(number_of_nodes);

    used_names.append(node_connections.front().from);
    used_names.append(node_connections.front().to);

    assert(used_names[node_connections.front().from]);
    assert(used_names[node_connections.front().to]);

    /// next we delete first connection form connections vector
    node_connections.erase(node_connections.begin());


    /// as long as all the cities aren't on the tree
    /// id est as long as all cities aren't connected

    while (!used_names.filled()) {

        ///pick the next best connection, but it must already be some way connected ou our tree!
        /// and because our connections are two way connections we don't really have "from, to" relationship
        /// so i guess we need to factor that in
        for (int i = 0; i < node_connections.size(); i++) {
            if (used_names[node_connections[i].from] &&
                !used_names[node_connections[i].to]) {

                root.append(node_connections[i]);
                used_names.append(node_connections[i].to);
                node_connections.erase(node_connections.begin() + i);

                break;
            } else if (!used_names[node_connections[i].from] &&
                       used_names[node_connections[i].to]) {

                std::swap(node_connections[i].to, node_connections[i].from);

                root.append(node_connections[i]);
                used_names.append(node_connections[i].to);
                node_connections.erase(node_connections.begin() + i);
                break;
            }
        }
    }

    std::cout << root;

    return 0;
}


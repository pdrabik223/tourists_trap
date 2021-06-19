//
// Created by studio25 on 22.05.2021.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <map>
#include <cassert>

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

};

using Threevec = std::vector<threesome>;


struct Node {

    /// value of a road leading to me
    /// now if node is root road leading to me doesn't exist therefore weight = 0

    int weight_;

    /// my name
    int name_;

    /// ptrs to my children
    std::vector<Node *> children_;

    Node() {
        /// the names were defined as "not bigger than 32767" so by default we Set it as just above the given range
        name_ = 32768;
        weight_ = 0;
        children_ = {};

    }

    explicit Node(int name) : name_(name) {
      weight_ = 0;
      children_ = {};
    }

    Node(int name, int weight) : name_(name), weight_(weight) {
      children_ = {};
    }

    bool operator==(int other_name) const {
        return name_ == other_name;
    }

    bool operator!=(int other_name) const {
        return name_ == other_name;
    }

    void Append(threesome connection) {
      Find(connection.from)->children_.push_back(new Node(connection.to, connection.weight));
    }

    void Set(threesome connection) {
      weight_ = 0;
      name_ = connection.from;
      children_.push_back(new Node(connection.to, connection.weight));
    }

    Node *Find(int search_name) {
        if (name_ == search_name) return this;

        for (auto i: children_) {
          Node *temp_ptr = i->Find(search_name);
            if (temp_ptr) return temp_ptr;
        }
        return nullptr;
    }

    void show(std::map<int ,std::string>& node_dictionary) {

        for (auto &i : children_) {
            std::cout << node_dictionary[name_]<<" ";
            std::cout <<node_dictionary[i->name_]<<" " ;
            std::cout << i->weight_ <<"\n" ;

        }
        for (auto &i : children_) {
            i->show(node_dictionary);
        }
    }



};

/// bool hash table<br>
/// i.e. hash table that is Filled with booleans<br>
/// fast and fail proof way to store big amounts of booleans<br>
/// hash table concept assures fast addressing time<br>
class BhTable {
public:
  BhTable(size_t size) {
    allocated_size_ = size;
        size_ = 0;
        storage_ = new bool[allocated_size_];
        for (int i = 0; i < allocated_size_; i++) {
          storage_[i] = false;
        }
    };

    void Append(size_t value) {
        assert(value > 0); /// if for whatever reason we have node name 0
        --value; /// that's because in our example node names start at one;
        assert(value < allocated_size_);
        if (storage_[value]) return;
        storage_[value] = true;
        ++size_;
    }

    bool &operator[](size_t index) {
        assert(index >= 0); /// if for whatever reason we have node name 0
        --index; /// that's because in our example node names start at one;
        assert(index < allocated_size_);
        return storage_[index];
    }

    ~BhTable() { delete[] storage_; }

    size_t Size() const { return size_; }

    bool Filled() const { return size_ == allocated_size_; }

protected:
    size_t allocated_size_;
    size_t size_;
    bool *storage_;
};

int main() {
    int number_of_nodes;
    std::cin >> number_of_nodes;
    std::map<int, std::string> node_dictionary = {};
    {
        std::string string_temp;
        int int_temp;
        for (int i = 0; i < number_of_nodes; i++) {
            std::cin >> int_temp;
            std::cin >> string_temp;
            /// because (i guess) town names fall in range <1;32767>
            /// i can subtract one from every name so i get range <0;32766>
            /// it will be easier that way
            node_dictionary.insert({int_temp, string_temp});

        }
    }
    int number_of_connections;
    std::cin >> number_of_connections;
    Threevec node_connections;
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
    std::sort(node_connections.begin(), node_conne ,ctions.end());

    /// next we declare the tree root, based on best connections in the graph
    Node root;
    root.Set(node_connections.front());

    /// next we need another vector that will hold the names of nodes already existing on three
    /// this is attempt to cut down time off accessing the list of elements on three
    BhTable used_names(number_of_nodes);

    used_names.Append(node_connections.front().from);
    used_names.Append(node_connections.front().to);

    assert(used_names[node_connections.front().from]);
    assert(used_names[node_connections.front().to]);

    /// next we delete first connection form connections vector
    node_connections.erase(node_connections.begin());


    /// as long as all the cities aren't on the tree
    /// id est as long as all cities aren't connected

    while (!used_names.Filled()) {

        ///pick the next best connection, but it must already be some way connected ou our tree!
        /// and because our connections are two way connections we don't really have "from, to" relationship
        /// so i guess we need to factor that in
        for (int i = 0; i < node_connections.size(); i++) {
            if (used_names[node_connections[i].from] &&
                !used_names[node_connections[i].to]) {

              root.Append(node_connections[i]);
              used_names.Append(node_connections[i].to);
                node_connections.erase(node_connections.begin() + i);

                break;
            } else if (!used_names[node_connections[i].from] &&
                       used_names[node_connections[i].to]) {

                std::swap(node_connections[i].to, node_connections[i].from);

                root.Append(node_connections[i]);
                used_names.Append(node_connections[i].to);
                node_connections.erase(node_connections.begin() + i);
                break;
            }
        }
    }

    root.show(node_dictionary);

    return 0;
}


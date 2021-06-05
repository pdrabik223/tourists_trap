//
// Created by studio25 on 05.06.2021.
//

//
// Created by studio25 on 22.05.2021.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <map>
#include <cassert>

#define MILLION 1000000


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

using threevec = std::vector<threesome>;


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
/// \brief the route mus be viable a.k.a. there must be connection from "this" city to destination city
/// \param to city name wa are heading to a.k.a. destination
/// \param minimal_value for recursion purposes should not be defined as a "high value" be default it's Million
/// \return the "thinnest corridor" on a way to destination a.k.a. "to"
    int minimal_route(int to, int minimal_value = MILLION) {

        if (name == to) return minimal_value;

        if (children.empty()) return MILLION;

        int temp_int;

        for (unsigned int i = 0; i < children.size(); i++) {

            if (children[i]->weight < minimal_value) minimal_value = children[i]->weight;

            temp_int = children[i]->minimal_route(to, minimal_value);
            if (temp_int != MILLION) return temp_int;

        }

        return MILLION;

    }

    node *search(int value) {

        if (name == value) return this;
        if (children.empty()) return nullptr;

        node *ptr_to_return = nullptr;

        for (auto i : children) {
            ptr_to_return = i->search(value);
            if (ptr_to_return != nullptr) return ptr_to_return;
        }

        return nullptr;
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

    friend std::ostream &operator<<(std::ostream &out, const node &dt) {
        out << "-------------------------\n";
        out <<  "name: " << dt.name << " weight: " << dt.weight << " children: " << dt.children.size()
            << "\n";

        for (auto i:dt.children) {
            out <<  i->name << "\t";
        }
        out << "\n";

        for (auto i:dt.children) {
            i->show(1);
        }
        return out;
    }

    void show(int depth = 1) {
        for (int i = 0; i < depth; i++) std::cout << "  ";
        std::cout <<  "name: " << name << " weight: " <<weight << " children: " << children.size()<< "\n";

        for (int i = 0; i < depth; i++) std::cout << "  ";
        for (auto i:children) {
            std::cout << i->name << "\t";
        }
        std::cout << "\n";

        for (auto i:children) {
            i->show(depth++);
        }

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
        assert(index >= 0); /// if for whatever reason we have node name 0
        --index; /// that's because in our example node names start at one;
        assert(index < allocated_size);
        return storage[index];
    }

    ~bh_table() { delete[] storage; }

    size_t size() const { return size_; }

    bool filled() const { return size_ == allocated_size; }

protected:
    size_t allocated_size;
    size_t size_;
    bool *storage;
};


int road_trips(int from, int to, node &tree) {

    if (tree.search(from)->search(to) != nullptr) {
        return tree.search(from)->minimal_route(to);

    } else return tree.search(to)->minimal_route(from);
    assert(false);
}



int main() {
    std::vector<threesome> node_connections;
        int number_of_cities;
    {
        int number_of_roads;

        int temp_from;
        int temp_to;
        int temp_weight;

        std::cin >> number_of_cities;
        std::cin >> number_of_roads;

        for (int i = 0; i < number_of_roads; i++) {
            std::cin >> temp_from;
            std::cin >> temp_to;
            std::cin >> temp_weight;

            node_connections.emplace_back(temp_from, temp_to, temp_weight);

        }
    }


    for (auto &i : node_connections) {
        i.weight--; //bo kierowca zajmuje niepotrzebne miesce podobno
        // i don't see it but okey
        i.weight = -i.weight; // bo trzeba zrobić maksymalne drzewo rozpinające ale mamy algorytm tylko na minimalne,
        // wiec jak bedziemy pracowac
        // na liczbach pzeciwnych to zadziala
        //dzieki temu zastosujemy algorytm najmniejszego drzewa rozpinajacego od Kruskala
    }

    /// end of data input
    /// first sort the received connections
    std::sort(node_connections.begin(), node_connections.end());

    /// next we declare the tree root, based on best connections in the graph
    node root;
    root.set(node_connections.front());

    /// next we need another vector that will hold the names of nodes already existing on three
    /// this is attempt to cut down time off accessing the list of elements on three
    bh_table used_names(number_of_cities);

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

    std::cout<<root;




    int from_user;
    int to_user;
    int how_many_people;

    do {

        std::cin >> from_user;
        std::cin >> to_user;

        if (from_user == 0 && to_user == 0) break;

        std::cin >> how_many_people;

        int dzielna = how_many_people / road_trips(from_user, to_user, root);
        int leftovers = how_many_people % road_trips(from_user, to_user, root);

        std::cout << "solution: " << dzielna <<" \\ " <<leftovers  <<std::endl;
        if (leftovers != 0) dzielna++;

    } while (2 > 1);



    return 0;
}


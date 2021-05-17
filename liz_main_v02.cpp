//
// Created by pc on 10.05.2021.
//

//
// Created by pc on 09.05.2021.
//

// ConsoleApplication1.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>

#include <vector>
#include <algorithm>
#include <cassert>

#define MILLION 1000000
#define SHOW_VEC(x) for(auto i:x) { std::cout<<i<<" ";} std::cout<<std::endl

#include "windows_console_tools/win_colors.h"

class node;

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

node *find_in_vec(std::vector<node> &data, int city);

int find_position_in_vec(const std::vector<node> &data, int search);

int find_position_in_vec(const std::vector<node *> &data, int search);

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

    node(int name) : city_name(name) {
        father = nullptr;
        weight = 0;
        nodes = {};
    };

    node(int weight, int cityName) : weight(weight), city_name(cityName) {
        father = nullptr;
        nodes = {};
    };

    node &operator=(const node &other) {
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

    node(const node &other) {

        if (other.father != nullptr) father = other.father;
        else father = nullptr;
        city_name = other.city_name;
        weight = other.weight;
        nodes = {};
        if (other.nodes.size() == 0) return;

        for (auto i : other.nodes) {
            nodes.push_back(new node(*i));
        }

    }

    bool operator==(const node &other) const {
        return city_name == other.city_name;
    }

    bool operator!=(const node &other) const {
        return city_name != other.city_name;
    }


    /// creates a copy of a node (with all children)
    /// and connects this copy to current node
    void append_in_place(const node &other, int weight) {

        nodes.push_back(new node(other));
        nodes.back()->weight = weight;
        nodes.back()->father = this;

    }

    int minimal_route_to(int to, int minimal_value = MILLION) {

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

    node *search(int value) {

        if (city_name == value) return this;
        if (nodes.empty()) return nullptr;

        node *ptr_to_return = nullptr;

        for (auto i : nodes) {
            ptr_to_return = i->search(value);
            if (ptr_to_return != nullptr) return ptr_to_return;
        }

        return ptr_to_return;
    }

    void make_root() {
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

    int ultimate_father() {
        if (father == nullptr) return city_name;
        else return father->ultimate_father();

    }

    ~node() {
        delete father;
    }

    friend std::ostream &operator<<(std::ostream &out, const node &dt) {
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

};

int road_trips(int from, int to, int pssngrs, node &tree);

int main() {

    std::vector<threesome> tab;
    {
        int number_of_cities;
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

            tab.emplace_back(temp_from, temp_to, temp_weight);

        }
    }
    ///SHOW_VEC(tab);


    for (auto &i : tab) {
        i.weight--; //bo kierowca zajmuje niepotrzebne miesce podobno
        // i don't see it but okey
        i.weight = -i.weight; // bo trzeba zrobić maksymalne drzewo rozpinające ale mamy algorytm tylko na minimalne,
        // wiec jak bedziemy pracowac
        // na liczbach pzeciwnych to zadziala
        //dzieki temu zastosujemy algorytm najmniejszego drzewa rozpinajacego od Kruskala
    }

    std::sort(tab.begin(), tab.end());

    SHOW_VEC(tab);

    std::vector<node> forest;

    for (auto i : tab) {
        if (std::find(forest.begin(), forest.end(), i.from) == forest.end()) {
            //potrzebujemy stworzyc las wierzcholkow,
            // ale wierzcholki nie moga sie powtarzac,
            // a nie znamy ich nazwa,
            // wiec szukam czy mam juz taki wierzcholek w lesie
            // i jak nie to dodajemy do lasu
            forest.push_back(node(i.from));
        }

        if (std::find(forest.begin(), forest.end(), i.to) == forest.end()) {
            //to samo co wyzej ale moglo byc tak ze jakis wierzcholek byl w to, a nie bylo go w from

            forest.push_back(node(i.to));
        }
    }


    unsigned i = -1;

    while (forest.size() > 1) {
        i++;
        assert(i < tab.size());
        /// so we have vector of empty nodes
        /// now we will be arranging them based on connections array a.k.a. tab <- it's eliza's idea not mine
        /// first we need to find node that will become father a.k.a. "from"
        node *from_city_ptr = find_in_vec(forest, tab[i].from);


        /// now we need to know which tree she belongs to
        /// and by tree i mean witch filed in vector is the origin
        int from_origin_city_name = from_city_ptr->ultimate_father();

        int from_origin_tree = find_position_in_vec(forest, from_origin_city_name);


        /// now we need to do the same for second argument a.k.a "to"
        node *to_city_ptr = find_in_vec(forest, tab[i].to);
        int to_origin_city_name = to_city_ptr->ultimate_father();


        /// but if the "to" city can be already deleted from forest
        /// therefore already assigned to some bigger tree
        /// in that case to will be forest.size()
        if (to_origin_city_name == forest.size()) continue;

        int to_origin_tree = find_position_in_vec(forest, to_origin_city_name);

        if (to_origin_tree == forest.size())continue;
        /// check that nodes are placed on different trees
        /// if not, that means they are already connected
        /// so break
        if (from_origin_tree == to_origin_tree) continue;


        /// now we need to make sure that "to" is root of a tree
        // to_city_ptr->make_root();


        /// now we merge two trees into one big one
        from_city_ptr->append_in_place(*to_city_ptr, tab[i].weight * -1);


        /// and shrink size of the forest
        forest.erase(forest.begin() + to_origin_tree);
        ///kk new connection


    }

    std::cout << cc(red) << "last tree: ";
    SHOW_VEC(forest);


    node minimal_spanning_tree = forest[0];
    // std::cout << cc(red) << minimal_spanning_tree;

    int from_user;
    int to_user;
    int how_many_people;

    do {

        std::cin >> from_user;
        std::cin >> to_user;

        if (from_user == 0 && to_user == 0) break;

        std::cin >> how_many_people;

        int dzielna = how_many_people / road_trips(from_user, to_user, how_many_people, minimal_spanning_tree);
        int leftovers = how_many_people % road_trips(from_user, to_user, how_many_people, minimal_spanning_tree);

        if (leftovers != 0) dzielna++;

        std::cout << cc(blue, gray) << "solution: " << dzielna << std::endl;
    } while (2 > 1);

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
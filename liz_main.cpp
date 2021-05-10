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
    std::vector<node *> nodes;
    std::vector<int> weights;
    int city_name;


    node(int weight) : city_name(weight) {
        weights = {};
        nodes = {};
    };

    node &operator=(const node &other) {
        if (this == &other) return *this;
        if(city_name == other.city_name)return *this;



//        for (node *i:other.nodes)
//            nodes.push_back(new node(*i));

        for (int i = 0; i < other.nodes.size(); i++) {
            if(i>=other.weights.size()) break;
            nodes.push_back(new node(*(other.nodes[i])));

            assert(i<=other.weights.size());
        }
        city_name = other.city_name;
        weights = other.weights;

        //        for (int i = 0; i < other.nodes.size(); i++) {
//            node temp_coz_im_tired = *other.nodes[i];
//            nodes.push_back(&temp_coz_im_tired);
//        }

        return *this;
    }

    node(const node &other) {

        city_name = other.city_name;
        weights = other.weights;
        for (node *i:other.nodes)
            nodes.push_back(new node(*i));

//        for (int i = 0; i < other.nodes.size(); i++) {
//            node temp_coz_im_tired = *other.nodes[i];
//            nodes.push_back(&temp_coz_im_tired);
//        }
    }

    bool operator==(const node &other) const {
        return city_name == other.city_name;
    }

    bool operator!=(const node &other) const {
        return city_name != other.city_name;
    }


    void append(const node *other, threesome sth) {
        assert(other);
        node *ptr_to_node = search(sth.from);

        assert(other->city_name == sth.to);
        assert(ptr_to_node->city_name == sth.from);

        ptr_to_node->nodes.push_back(new node(*other));
        ptr_to_node->weights.push_back(sth.weight * -1);

    }

    int minimal_route_to(int to, int minimal_value = MILLION) {

        if (city_name == to) return minimal_value;

        if (nodes.empty()) return MILLION;

        int temp_int;

        for (unsigned int i = 0; i < nodes.size(); i++) {

            if (weights[i] < minimal_value) minimal_value = weights[i];

            temp_int = nodes[i]->minimal_route_to(to, minimal_value);
            if (temp_int != MILLION) return temp_int;

        }

        assert(false);
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

    ~node() = default;

    friend std::ostream &operator<<(std::ostream &out, const node &dt) {
        out << cc(red) << "name: " << dt.city_name << "\n";

        for (auto i:dt.nodes) {
            out << cc(yellow) << i->city_name << "\t";
        }
        out << "\n";
        for (auto i:dt.weights) {
            out << cc(green) << i << "\t";
        }
        out << "\n";
        for (auto i:dt.nodes) {
            out << *i;
        }
        return out;
    }

};


int road_trips(int from, int to, int pssngrs, node &tree);

node *find_in_vec(std::vector<node> &data, int city);

int find_position_in_vec(const std::vector<node> &data, int search);

int main() {
    int number_of_cities;
    int number_of_roads;
    std::vector<threesome> tab;

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
    // SHOW_VEC(tab);

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
    SHOW_VEC(forest);

    /// ok at this point in time


    unsigned i = 0;
    int helper = 0;
    while (forest.size() > 1) {

        node *from_city_ptr = find_in_vec(forest, tab[i].from);
        assert(from_city_ptr);

//        node *to_city_ptr = find_in_vec(forest, tab[i].to);
//        assert(to_city_ptr);
//

        int position = find_position_in_vec(forest, tab[i].to);
        if (position != forest.size()) {


            from_city_ptr->nodes.push_back(new node(forest[position]));
            from_city_ptr->weights.push_back(tab[i].weight * -1);
            //  from_city_ptr->append(new node(forest[position]), tab[i]);
            forest.erase(forest.begin() + position);

        }
        i++;
    }

    std::cout << cc(red) << "last tree: ";
    SHOW_VEC(forest);


    node minimal_spanning_tree = forest[0];
    std::cout << cc(red) << minimal_spanning_tree;
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

    for (unsigned i = 0; i < data.size(); ++i) {

        node *temp = data[i].search(city);
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
    // assert(false);
    return data.size();
}

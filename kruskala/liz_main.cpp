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

#include "../windows_console_tools/win_colors.h"

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

class Node {
public:
    std::vector<Node *> nodes;
    std::vector<int> weights;
    int city_name;

    Node(int weight) : city_name(weight) {
        weights = {};
        nodes = {};
    };

    Node &operator=(const Node &other) {
        if (this == &other) return *this;
        if (city_name == other.city_name)return *this;



//        for (node *i:other.nodes)
//            nodes.push_back(new node(*i));
        city_name = other.city_name;
        weights = other.weights;
        nodes = {};
        if(other.nodes.size() == 0) return *this;

        for (int i = 0; i < other.nodes.size(); i++) {
            //if(i>=other.weights.Size()) break;
            nodes.push_back(new Node(*(other.nodes[i])));

            assert(i <= other.weights.size());
        }


        //        for (int i = 0; i < other.nodes.Size(); i++) {
//            node temp_coz_im_tired = *other.nodes[i];
//            nodes.push_back(&temp_coz_im_tired);
//        }
        assert(nodes.size() ==  weights.size());
        return *this;
    }

    Node(const Node &other) {

        city_name = other.city_name;
        weights = other.weights;

        for (int i = 0; i < other.nodes.size(); i++) {
            //if(i>=other.weights.Size()) break;
            nodes.push_back(new Node(*(other.nodes[i])));

            assert(i <= other.weights.size());
        }
//        for (int i = 0; i < other.nodes.Size(); i++) {
//            node temp_coz_im_tired = *other.nodes[i];
//            nodes.push_back(&temp_coz_im_tired);
//        }
        assert(nodes.size() ==  weights.size());
    }

    bool operator==(const Node &other) const {
        return city_name == other.city_name;
    }

    bool operator!=(const Node &other) const {
        return city_name != other.city_name;
    }


    void append(const Node *other, threesome sth) {
        assert(other);
        Node *ptr_to_node = Search(sth.from);

        assert(other->city_name == sth.to);
        assert(ptr_to_node->city_name == sth.from);

        ptr_to_node->nodes.push_back(new Node(*other));
        ptr_to_node->weights.push_back(sth.weight * -1);
        assert(ptr_to_node->nodes.size() ==  ptr_to_node->weights.size());
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

    //    assert(false);
        return MILLION;
    }

    Node *Search(int destination) {

        if (city_name == destination) return this;
        if (nodes.empty()) return nullptr;

        Node *ptr_to_return = nullptr;
        for (auto i : nodes) {
            ptr_to_return = i->Search(destination);
            if (ptr_to_return != nullptr) return ptr_to_return;
        }

        return ptr_to_return;
    }

    ~Node() = default;

    friend std::ostream &operator<<(std::ostream &out, const Node &dt) {
        out << cc(red) << "name: " << dt.city_name << " children: " << dt.nodes.size() << "\n";

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


int road_trips(int from, int to, int pssngrs, Node &tree);

Node *find_in_vec(std::vector<Node> &data, int city);

int find_position_in_vec(const std::vector<Node> &data, int search);

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

    std::vector<Node> forest;

    for (auto i : tab) {
        if (std::find(forest.begin(), forest.end(), i.from) == forest.end()) {
            //potrzebujemy stworzyc las wierzcholkow,
            // ale wierzcholki nie moga sie powtarzac,
            // a nie znamy ich nazwa,
            // wiec szukam czy mam juz taki wierzcholek w lesie
            // i jak nie to dodajemy do lasu
            forest.push_back(Node(i.from));
        }

        if (std::find(forest.begin(), forest.end(), i.to) == forest.end()) {
            //to samo co wyzej ale moglo byc tak ze jakis wierzcholek byl w to, a nie bylo go w from

            forest.push_back(Node(i.to));
        }
    }





    unsigned i = 0;

    while (forest.size() > 1) {

      Node *from_city_ptr = find_in_vec(forest, tab[i].from);
        assert(from_city_ptr);

//        node *to_city_ptr = find_in_vec(forest, tab[i].to);
//        assert(to_city_ptr);
//

        int position = find_position_in_vec(forest, tab[i].to);
        if (position != forest.size()) {

            //   from_city_ptr->nodes.push_back(new node(forest[position]));
            //   from_city_ptr->weights.push_back(tab[i].weight * -1);
            from_city_ptr->append(new Node(forest[position]), tab[i]);
            forest.erase(forest.begin() + position);

        }
        i++;
    }

    std::cout << cc(red) << "last tree: ";
    SHOW_VEC(forest);

    Node minimal_spanning_tree = forest[0];
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


int road_trips(int from, int to, int pssngrs, Node &tree) {

    if (tree.Search(from)->Search(to) != nullptr) {
        return tree.Search(from)->minimal_route_to(to);

    } else return tree.Search(to)->minimal_route_to(from);
    assert(false);
}

Node *find_in_vec(std::vector<Node> &data, int city) {
  Node *temp = nullptr;
    for (unsigned i = 0; i < data.size(); ++i) {

        temp = data[i].Search(city);
        if (temp != nullptr) return temp;

    }
    assert(false);
    return nullptr;
}


int find_position_in_vec(const std::vector<Node> &data, int search) {
  for (int i = 0; i < data.size(); i++) {
        if (data[i].city_name == search)
            return i;
    }
    return data.size();
}

void test_me_daddy() {
    std::cout << cc(white) << "constructor: ";
    Node tree(6);
    assert(tree.city_name == 6);
    std::cout << cc(green) << "[ok]" << std::endl;

    std::cout << cc(white) << "copy constructor: ";
    Node tree2(tree);
    assert(tree2.city_name == 6);
    std::cout << cc(green) << "[ok]" << std::endl;

    std::cout << cc(white) << "operator ==: ";
    assert(tree2.city_name == tree.city_name);
    std::cout << cc(green) << "[ok]" << std::endl;

    std::cout << cc(white) << "operator !=: ";
    assert(tree2.city_name != 8);
    std::cout << cc(green) << "[ok]" << std::endl;

    std::cout << cc(white) << "Append 1: ";
    Node tree3(9);
    tree.append(&tree3, threesome(6, 9, 15));
    assert(tree.Search(6)->city_name == 6);
    std::cout << cc(green) << "[ok]" << std::endl;

    std::cout << cc(white) << "Append 2: ";
    assert(tree.Search(9)->city_name == 9);
    std::cout << cc(green) << "[ok]" << std::endl;

    std::cout << cc(white) << "Append 3: ";
    Node tree4(12);
    tree.append(&tree4, threesome(9, 12, 3));
    assert(tree.Search(12)->city_name == 12);
    std::cout << cc(green) << "[ok]" << std::endl;

    Node tree5(10);
    Node tree6(2);

    tree.append(&tree5, threesome(6, 10, 19));
    tree.append(&tree6, threesome(12, 2, 7));
    std::cout << cc(red) << tree << " ";

    std::cout << cc(white) << "minimal route to: ";
    assert(road_trips(9, 12, 10, tree) == -3);
    std::cout << cc(green) << "[ok]" << std::endl;

    std::vector<Node> las;
    las.push_back(tree);
    std::cout << cc(white) << "Find in vector: ";
    assert(find_in_vec(las, 12) == &tree);
    std::cout << cc(green) << "[ok]" << std::endl;
}
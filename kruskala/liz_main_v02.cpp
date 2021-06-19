
// ConsoleApplication1.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>

#include <vector>
#include <algorithm>
#include <cassert>

#define MILLION 1000000
#define SHOW_VEC(x) for(auto i:x) { std::cout<<i<<" ";} std::cout<<std::endl

#include "../windows_console_tools/win_colors.h"

class Node;

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

Node *find_in_vec(std::vector<Node> &data, int city);

int find_position_in_vec(const std::vector<Node> &data, int search);

int find_position_in_vec(const std::vector<Node *> &data, int search);

class Node {
public:
    int city_name;
    std::vector<Node *> nodes;

    /// weight leading to this city
    /// first one of course has weight 0
    /// so it's cost of coming into
    int weight_;

    /// ptr to father object
    Node *father;

    Node(int name) : city_name(name) {
        father = nullptr;
        weight_ = 0;

    };

    Node(int cityName, int weight) : weight_(weight), city_name(cityName) {
        father = nullptr;
    };

    Node &operator=(const Node &other) {
        if (this == &other) return *this;

        if (other.father != nullptr)father = other.father;
        else father = nullptr;

        city_name = other.city_name;
        weight_ = other.weight_;

        nodes = other.nodes;

//        for (auto i : other.nodes) {
//            nodes.push_back(new node(*i));
//        }

        return *this;
    }



    bool operator==(const Node &other) const {
        return city_name == other.city_name;
    }

    bool operator!=(const Node &other) const {
        return city_name != other.city_name;
    }


    /// creates a copy of a node (with all children)
    /// and connects this copy to current node
    void append_in_place(const Node &other, int weight) {

        nodes.push_back(new Node(other));
        nodes.back()->weight_ = weight;
        nodes.back()->father = this;

    }

    int minimal_route_to(int to, int minimal_value = MILLION) {

        if (city_name == to) return minimal_value;

        if (nodes.empty()) return MILLION;

        int temp_int;

        for (unsigned int i = 0; i < nodes.size(); i++) {

            if (nodes[i]->weight_ < minimal_value) minimal_value = nodes[i]->weight_;

            temp_int = nodes[i]->minimal_route_to(to, minimal_value);
            if (temp_int != MILLION) return temp_int;

        }

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

        return nullptr;
    }
    int ultimate_father() {
        if (father == nullptr) return city_name;
        else return father->ultimate_father();

    }
    Node *ultimate_father_ptr() {

        if (father == nullptr) return this;
        else return father->ultimate_father_ptr();

    }
    ~Node() {
       nodes.clear();
    };

    friend std::ostream &operator<<(std::ostream &out, const Node &dt) {
        out << "-------------------------\n";
        out << cc(red) << "name: " << dt.city_name << " weight: " << dt.weight_
            << " children: " << dt.nodes.size()
            << "\n";

        for (auto i:dt.nodes) {
            out << cc(yellow) << i->city_name << "\t";
        }
        out << "\n";

        for (auto i:dt.nodes) {
            i->show(1);
        }
        return out;
    }

    void show(int depth = 1) {
        for (int i = 0; i < depth; i++) std::cout << "  ";
        std::cout << "name: " << city_name << " weight: " << weight_
                  << " father: " << father->city_name << " children: "
                  << nodes.size()
                  << "\n";

        for (int i = 0; i < depth; i++) std::cout << "  ";
        for (auto i:nodes) {
            std::cout << i->city_name << "\t";
        }
        std::cout << "\n";

        for (auto i:nodes) {
            i->show(depth++);
        }

    }

};

int road_trips(int from, int to, int pssngrs, Node &tree);

Node *make_root(Node *target);

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


    unsigned i = -1;

    while (forest.size() > 1) {
        i++;
        assert(i < tab.size());
        /// so we have vector of empty nodes
        /// now we will be arranging them based on connections array a.k.a. tab <- it's eliza's idea not mine
        /// first we need to Find node that will become father a.k.a. "from"
        Node *from_city_ptr = find_in_vec(forest, tab[i].from);


        /// now we need to know which tree she belongs to
        /// and by tree i mean witch filed in vector is the origin
        int from_origin_city_name = from_city_ptr->ultimate_father();


        int from_origin_tree = find_position_in_vec(forest, from_origin_city_name);


        /// now we need to do the same for second argument a.k.a "to"
        Node *to_city_ptr = find_in_vec(forest, tab[i].to);
        int to_origin_city_name = to_city_ptr->ultimate_father();



        /// but if the "to" city can be already deleted from forest
        /// therefore already assigned to some bigger tree
        /// in that case to will be forest.Size()
        int to_origin_tree = find_position_in_vec(forest, to_origin_city_name);


        /// check that nodes are placed on different trees
        /// if not, that means they are already connected
        /// so break
        if (from_origin_tree == to_origin_tree) continue;


        /// now we need to make sure that "to" is root of a tree
        Node *root = make_root(to_city_ptr);


        /// now we merge two trees into one big one
        from_city_ptr->append_in_place(*root, tab[i].weight * -1);


        /// and shrink Size of the forest
        forest.erase(forest.begin() + to_origin_tree);
        ///kk new connection

        SHOW_VEC(forest);
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
    for (auto &i : data) {

        temp = i.Search(city);
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

int find_position_in_vec(const std::vector<Node *> &data, int search) {
    for (int i = 0; i < data.size(); i++) {
        if (data[i]->city_name == search)
            return i;
    }
    return data.size();
}

Node *make_root(Node *target) {
    if (target->father == nullptr) return target;

    Node *ultimate_father = make_root(target->father);

    target->nodes.push_back(new Node(*ultimate_father));


    target->nodes.back()->weight_ = target->weight_;
    target->nodes.back()->father = target;


    /// Find ptr to myself in father nodes vector
    int my_position_in_fathers_nodes = find_position_in_vec(target->nodes.back()->nodes, target->city_name);

    assert(target->nodes.back()->nodes.size() == 0);
    assert (my_position_in_fathers_nodes != target->nodes.back()->nodes.size());
    /// and delete that element  but not clear memory under
    /// basically make sure that element does not get deleted
    /// only removed from array
    target->nodes.back()->nodes[my_position_in_fathers_nodes] = nullptr;
    target->nodes.back()->nodes.erase(target->nodes.back()->nodes.begin() + my_position_in_fathers_nodes);

    target->father = nullptr;
    target->weight_ = 0;
    return target;

}
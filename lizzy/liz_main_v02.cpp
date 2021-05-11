
#include <iostream>

#include <vector>
#include <algorithm>
#include <cassert>



#define MILLION 1000000
#define SHOW_VEC(x) for(auto i:x) { std::cout<<i<<" ";} std::cout<<std::endl

#include "tree.h"
void test_me_dadD();




int main() {
    test_me_dadD();

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

    SHOW_VEC(forest);
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






void test_me_dadD() {

    node tree(5);
    tree.append_in_place(6, 12);
    tree.append_in_place(3, 33);
    tree.append_in_place(9, 22);
    assert(tree.search(6)->city_name == 6);
    assert(tree.search(6)->weight == 12);

    tree.search(6)->append_in_place(33, 12);
    tree.search(6)->append_in_place(10, 12);

    assert(tree.search(10)->city_name == 10);
    assert(tree.search(33)->city_name == 33);

    node tree2(4);
    tree2.append_in_place(7, 12);
    tree2.append_in_place(4, 33);
    tree2.append_in_place(11, 22);

    tree.search(3)->append_in_place(tree2, 90);

    assert(tree.search(11)->city_name == 11);

    // std::cout<<tree;
    node tree3(7);
    tree3.append_in_place(8, 12);
    tree3.search(8)->append_in_place(3, 33);
    tree3.search(3)->append_in_place(9, 33);

    tree3.search(3)->make_root();
    tree3.search(3)->make_root();
    assert(tree3.search(3)->father == nullptr);

}


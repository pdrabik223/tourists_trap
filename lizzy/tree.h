//
// Created by pc on 11.05.2021.
//

#ifndef TURISTS_TRAP_TREE_H
#define TURISTS_TRAP_TREE_H

#include "node.h"

class tree {
public:

    tree(int name);
    /// adds specyfic node to tree
    /// \param other new node
    /// \param info destination of new node, weight walue tah will be assigned
    void append(const node &other, threesome info);


    int minimal_route_to(int to, int minimal_value = MILLION);

    ///
    /// \return city_name_of the ultimate father
    int ultimate_father();
    node *search(int value);

    void make_root(int city_name);

    bool operator==(const tree &rhs) const {
        return *root == *rhs.root;
    }

    bool operator!=(const tree &rhs) const {
        return *root != *rhs.root;
    }

    node *root;
    ~tree(){
        delete root;
    }
};



#endif //TURISTS_TRAP_TREE_H

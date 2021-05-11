//
// Created by pc on 11.05.2021.
//

#include "tree.h"

void tree::append(const node &other, threesome info) {
    if (root == nullptr)return;
    root->search(info.from)->append_in_place(other, info.weight);
}

int tree::minimal_route_to(int to, int minimal_value) {
    return root->minimal_route_to(to, MILLION);
}

int tree::ultimate_father() {
    return root->city_name;
}

node *tree::search(int value) {
    return root->search(value);
}

void tree::make_root(int city_name) {
    node *new_root_ptr = root->search(city_name);
    new_root_ptr->make_root();
    root = new_root_ptr;

}

tree::tree(int name) {
    root = new node(name);

}

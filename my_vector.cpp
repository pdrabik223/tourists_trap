//
// Created by elizk on 16.05.2021.
//

#include "my_vector.h"

my_vector::my_vector() {
    size_vec = 0;
    array = nullptr;
}

my_vector::my_vector(const my_vector& other){
    this->size_vec = other.size_vec;
    this->array = new int[other.size_vec];

    for(int i = 0; i<this->size(); i++)
    {
        this->array[i] = other.array[i];
    }
}

void my_vector::push_back(int value) {
    int *temp = new int[size_vec+1];

    if(this->size_vec > 0) {
        for (int i = 0; i < this->size(); i++) {
            temp[i] = array[i];
        }
    }

    temp[this->size_vec] = value;

    this->size_vec++;

    delete[] this->array;

    this->array = new int[this->size_vec];

    for(int i = 0; i<this->size(); i++)
    {
        this->array[i] = temp[i];
    }

    delete[] temp;
}

int my_vector::size() {
    return size_vec;
}


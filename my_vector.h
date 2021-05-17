//
// Created by elizk on 16.05.2021.
//

#ifndef MY_VECTOR_MY_VECTOR_H
#define MY_VECTOR_MY_VECTOR_H


class my_vector {
    private:
        size_t size_vec;
    public:
        int *array;


        my_vector();
        my_vector(const my_vector& other);

        /// function works like std::vector.push_back()
        /// but this is written dumb way
        /// @param value - number we want to add to vector
        void push_back(int value);

        /// it takes total number of bytes allocated by array
        /// and is divided by size of one element so that
        /// I can find number of elements.
        /// To say size I need to add 1 to the result.
        /// @return size of the tab
        int size();

        /// overloaded operator, don't know what I can say more
        int& operator[](int index){
            return array[index];
        }

        ~my_vector(){
            delete []array;
        }
};


#endif //MY_VECTOR_MY_VECTOR_H

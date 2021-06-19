//
// Created by studio25 on 31.05.2021.
//
#include <iostream>
#include <array>
#include <vector>
#include <fstream>
#include <string>

#define number_of_nodes  1000
#define MAX_NUMBER_OF_CONNECTIONS 1000 * 2
#define  MAX_CONNECTION_WEIGHT  1000
#define  PATH "../output_data.txt"

struct threesome {
    threesome(int from, int to, int weight) : from(from), to(to), weight(weight) {}

    bool operator==(const threesome &rhs) const {
        return (from == rhs.from && to == rhs.to) ||
               (to == rhs.from && from == rhs.to);
    }

    bool operator!=(const threesome &rhs) const {
        return !(rhs == *this);
    }

    int from;
    int to;
    int weight;

    friend std::ostream &operator<<(std::ostream &out, threesome &rhs) {
        out << rhs.from << " " << rhs.to << " " << rhs.weight << "\n";
        return out;
    }

};

bool append_to_vec(std::vector<threesome> &storage, threesome target);

std::string gen_str(int seed);

int main() {
    srand(time(NULL));
    std::vector<threesome> output;

    /// let's make a big circle around them

    for (int i = 1; i < number_of_nodes; i++) {

        append_to_vec(output, {i, i+1, rand() % MAX_CONNECTION_WEIGHT});
    }

    for (int i = 0; i < MAX_NUMBER_OF_CONNECTIONS - number_of_nodes + 1 ; i++) {

        int from = rand() % number_of_nodes + 1;
        int to = rand() % number_of_nodes + 1;

        /// the road that lead from a to a is no road,
        /// it's a roundabout or "traffic circle"
        if (from == to) {
            i--;
            continue;
        }

        int weight = rand() % MAX_CONNECTION_WEIGHT;

        /// if the entry is unique
        /// Append it to the connection list
        if (append_to_vec(output, {from, to, weight})) {
            i--;
            continue;
        }
        printf("\r i: %d",i);
    }
    std::ofstream outfile;

    outfile.open(PATH);
    /// number of nodes
    outfile << number_of_nodes;
    outfile << "\n";


    for (int i = 0; i < number_of_nodes; i++)
        outfile << i + 1 << " " << std::to_string(i) << "\n";


    outfile << output.size() << "\n";
    for (auto i:output)
        outfile << i;

    return 0;
}

int pow26(int exponent) {
    int pow = 1;

    while (exponent) {
        pow *= 26;
        --exponent;
    }

    return pow;

}

std::string gen_str(int seed) {
    std::string output;
    for (int i = 0; i < number_of_nodes / 26 + 1; i++) {

    }
    return output;

}

bool append_to_vec(std::vector<threesome> &storage, threesome target) {
    ///Append to vector if the entry is unique
    for (auto &i:storage)
        if (i == target) return true;

    storage.push_back(target);
    return false;

}
//
// Created by studio25 on 05.06.2021.
//

//
// Created by studio25 on 22.05.2021.
//

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <map>
#include <vector>

#define MILLION 1000000

struct threesome {
  threesome(int from, int to, int weight)
      : from(from), to(to), weight(weight) {}

  bool operator==(const threesome &rhs) const { return weight == rhs.weight; }

  bool operator!=(const threesome &rhs) const { return !(rhs == *this); }

  bool operator<(const threesome &rhs) const { return weight < rhs.weight; }

  bool operator>(const threesome &rhs) const { return rhs < *this; }

  bool operator<=(const threesome &rhs) const { return !(rhs < *this); }

  bool operator>=(const threesome &rhs) const { return !(*this < rhs); }

  int from;
  int to;
  int weight;
};

using Threevec = std::vector<threesome>;

class Node {
public:
  struct city_or_road {
    city_or_road(Node *city) : city(city), weight(0){};
    city_or_road(int weight) : city(nullptr), weight(weight){};

    Node *city;
    int weight;
  };

  /// value of a road leading to me
  /// now if node is root road leading to me doesn't exist therefore weight = 0

  int weight_;

  /// my name
  int name_;

  /// ptrs to my children
  std::vector<Node *> children_;

  Node() {
    /// the names were defined as "not bigger than 32767" so by default we Set
    /// it as just above the given range
    name_ = MILLION;
    weight_ = 0;
    children_ = {};
  }

  explicit Node(int name) : name_(name) {
    weight_ = 0;
    children_ = {};
  }

  Node(int name, int weight) : name_(name), weight_(weight) { children_ = {}; }

  bool operator==(int other_name) const { return name_ == other_name; }

  bool operator!=(int other_name) const { return name_ == other_name; }

  /// \brief the route mus be viable a.k.a. there must be connection from "this"
  /// city to destination city \param to city name wa are heading to a.k.a.
  /// destination \param minimal_value for recursion purposes should not be
  /// defined as a "high value" be default it's Million \return the "thinnest
  /// corridor" on a way to destination a.k.a. "to"
  int MinimalRoute(int to, int minimal_value = MILLION) {

    if (name_ == to)
      return minimal_value;

    if (children_.empty())
      return MILLION;

    int temp_int;

    for (auto &i : children_) {

      if (i->weight_ < minimal_value)
        minimal_value = i->weight_;

      temp_int = i->MinimalRoute(to, minimal_value);

      if (temp_int != MILLION)
        return temp_int;
    }

    return MILLION;
  }

  city_or_road MinimalRouteButHarderRequ(threesome route) {

    if (name_ == route.from)
      return this;

    Node *ptr_to_return = nullptr;

    for (auto i : children_) {
      ptr_to_return = i->MinimalRouteButHarderRequ(route).city;
      if (ptr_to_return != nullptr)
        break;
    }
    if (!Search(route.to))
      return ptr_to_return;

    return MinimalRoute(route.to);
  }
  //

  Node *Search(int destination) {

    if (name_ == destination)
      return this;
    if (children_.empty())
      return nullptr;

    Node *ptr_to_return;

    for (auto i : children_) {
      ptr_to_return = i->Search(destination);
      if (ptr_to_return != nullptr)
        return ptr_to_return;
    }

    return nullptr;
  }

  void Append(threesome connection) {
    Find(connection.from)
        ->children_.push_back(new Node(connection.to, connection.weight));
  }

  void Set(threesome connection) {
    weight_ = 0;
    name_ = connection.from;
    children_.push_back(new Node(connection.to, connection.weight));
  }

  Node *Find(int search_name) {
    if (name_ == search_name)
      return this;

    for (auto i : children_) {
      Node *temp_ptr = i->Find(search_name);
      if (temp_ptr)
        return temp_ptr;
    }
    return nullptr;
  }
};

/// bool hash table<br>
/// i.e. hash table that is Filled with booleans<br>
/// fast and fail proof way to store big amounts of booleans<br>
/// hash table concept assures fast addressing time<br>
class BhTable {
public:
  explicit BhTable(size_t size) {
    allocated_size_ = size;
    size_ = 0;
    storage_ = new bool[allocated_size_];
    for (int i = 0; i < allocated_size_; i++) {
      storage_[i] = false;
    }
  };

  void Append(size_t value) {
    assert(value > 0); /// if for whatever reason we have node name 0
    --value; /// that's because in our example node names start at one;
    assert(value < allocated_size_);
    if (storage_[value])
      return;
    storage_[value] = true;
    ++size_;
  }

  bool &operator[](size_t index) {
    assert(index >= 0); /// if for whatever reason we have node name 0
    --index; /// that's because in our example node names start at one;
    assert(index < allocated_size_);
    return storage_[index];
  }

  ~BhTable() { delete[] storage_; }

  bool Filled() const { return size_ == allocated_size_; }

protected:
  size_t allocated_size_;
  size_t size_;
  bool *storage_;
};

int RoadTrips(int from, int to, Node &tree) {

  if (tree.Search(from)->Search(to) != nullptr)
    return tree.Search(from)->MinimalRoute(to);

  else
    return tree.MinimalRouteButHarderRequ({from, to, MILLION}).weight;

  assert(false);
}

int main() {
  std::vector<threesome> node_connections;
  int number_of_cities;
  {
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

      node_connections.emplace_back(temp_from, temp_to, temp_weight);
    }
  }

  for (auto &i : node_connections) {
    i.weight--; // bo kierowca zajmuje niepotrzebne miesce podobno
    // i don't see it but okey
    i.weight = -i.weight; // bo trzeba zrobić maksymalne drzewo rozpinające ale
                          // mamy algorytm tylko na minimalne,
    // wiec jak bedziemy pracowac
    // na liczbach pzeciwnych to zadziala
    // dzieki temu zastosujemy algorytm najmniejszego drzewa rozpinajacego od
  }

  /// end of data input
  /// first sort the received connections
  std::sort(node_connections.begin(), node_connections.end());

  for (auto &i : node_connections)
    i.weight *= -1;

  /// next we declare the tree root, based on best connections in the graph
  Node root;
  root.Set(node_connections.front());

  /// next we need another vector that will hold the names of nodes already
  /// existing on three this is attempt to cut down time off accessing the list
  /// of elements on three
  BhTable used_names(number_of_cities);

  used_names.Append(node_connections.front().from);
  used_names.Append(node_connections.front().to);

  assert(used_names[node_connections.front().from]);
  assert(used_names[node_connections.front().to]);

  /// next we delete first connection form connections vector
  node_connections.erase(node_connections.begin());

  /// as long as all the cities aren't on the tree
  /// id est as long as all cities aren't connected

  while (!used_names.Filled()) {

    /// pick the next best connection, but it must already be some way connected
    /// ou our tree!
    ///  and because our connections are two way connections we don't really
    ///  have "from, to" relationship so i guess we need to factor that in
    for (int i = 0; i < node_connections.size(); i++) {
      if (used_names[node_connections[i].from] &&
          !used_names[node_connections[i].to]) {

        root.Append(node_connections[i]);
        used_names.Append(node_connections[i].to);
        node_connections.erase(node_connections.begin() + i);

        break;
      } else if (!used_names[node_connections[i].from] &&
                 used_names[node_connections[i].to]) {

        std::swap(node_connections[i].to, node_connections[i].from);

        root.Append(node_connections[i]);
        used_names.Append(node_connections[i].to);
        node_connections.erase(node_connections.begin() + i);
        break;
      }
    }
  }

  int from_user;
  int to_user;
  int how_many_people;

  do {

    std::cin >> from_user;
    std::cin >> to_user;

    if (from_user == 0 && to_user == 0)
      break;

    std::cin >> how_many_people;

    int dzielna = how_many_people / RoadTrips(from_user, to_user, root);
    int leftovers = how_many_people % RoadTrips(from_user, to_user, root);

    if (leftovers != 0)
      dzielna++;

    std::cout << dzielna << std::endl;
  } while (2 > 1);

  return 0;
}

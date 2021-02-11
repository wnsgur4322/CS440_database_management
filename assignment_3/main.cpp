#include <iostream>
#include <cstdio>
#include <list>
#include <unordered_map>

using namespace std;
hash<string> hasher;

struct Employee {
    string id;
    string name;
    int age;
    float salary;
};

class Block {
    int size = 4096;
    int bucket;
    int numEmployees;
    int totalSize;
    list<Employee> employees;
    float percentUsed;
    public:
        Block(list<Employee> employees, int bucket, int numEmployees);
        Block(int bucket);
        void insertEmployee(Employee employee);
};

Block::Block(int bucket) {
    this->bucket = bucket;
    this->totalSize = 0;
    this->numEmployees = 0;
}

Block::Block(list<Employee> employees, int bucket, int numEmployees) {
    this->employees = employees;
    this->numEmployees = numEmployees;
    this->bucket = bucket;
    this->totalSize = 0;
    for(auto i = employees.begin(); i != employees.end(); i++) {
        this->totalSize += sizeof(*i);
    }
    this->percentUsed = ((float)this->totalSize)/this->size;
}

void Block::insertEmployee(Employee employee) {
    this->numEmployees++;
    this->employees.push_back(employee);
    this->totalSize += sizeof(employee);
    this->percentUsed = ((float)this->totalSize)/this->size;
}

class Hash {
    int BUCKET;    // No. of buckets

    // Pointer to an array containing buckets
    list<int> *table;

    public:
        Hash(int V);  // Constructor

    // inserts a key into hash table
        void insertItem(int x);

    // deletes a key from hash table
        void deleteItem(int key);

    // hash function to map values to key
        int hashFunction(int x) {
        return (x % BUCKET);
        }

    void displayHash();
};

Hash::Hash(int b){
    this->BUCKET = b;
    table = new list<int>[BUCKET];
}

void Hash::insertItem(int key){
    int index = hashFunction(key);
    table[index].push_back(key);
}

void Hash::deleteItem(int key){
  // get the hash index of key
  int index = hashFunction(key);

  // find the key in (inex)th list
  list <int> :: iterator i;
  for (i = table[index].begin();
           i != table[index].end(); i++) {
    if (*i == key)
      break;
  }

  // if key is found in hash table, remove it
  if (i != table[index].end())
    table[index].erase(i);
}

void writeDataFile(Block* data) {
    
}

// function to display hash table
void Hash::displayHash() {
    for (int i = 0; i < BUCKET; i++) {
        cout << i;

        for (auto x : table[i]) {
            cout << " --> " << x;
            cout << endl;
        }

    }
}

int hashStr(string id) {
    int hash = (int) hasher(id);
    return hash;
}

int lastNBits(int hash, int N) {
    int lastN = hash & ((1 << N) - 1);
    return lastN;
}

// Driver program
int main(){
  // array that contains keys to be mapped
  int a[] = {15, 11, 27, 8, 12};
  int n = sizeof(a)/sizeof(a[0]);

  // insert the keys into the hash table
  Hash h(7);   // 7 is count of buckets in
               // hash table
  for (int i = 0; i < n; i++)
    h.insertItem(a[i]);

  // delete 12 from hash table
  h.deleteItem(12);

  // display the Hash table
  //h.displayHash();
  int hash = hashStr("test1");
  printf("%d\n", hash);
  printf("%d\n", lastNBits(hash,2));

  return 0;
}

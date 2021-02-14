#include <iostream>
#include <cstdio>
#include <list>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <math.h> 

using namespace std;
hash<string> hasher;

struct Employee {
    string id;
    string name;
    int age;
    float salary;
};

struct Block {
    int size_used = 0;
    Block* overflow = NULL;
};
/***
class Block {
    int size = 4096;
    int bucket;
    int numEmployees;
    int totalSize;
    Block* overflow;
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
***/

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

int lastIBits(int hash, int i, int n) {
    int lastI = hash & ((1 << i) - 1);
    if(lastI > n - 1) {
        //printf("Before: %d\n", lastI);
        lastI = lastI ^ (1 << (i - 1));
        //printf("After: %d\n", lastI);
    }
    return lastI;
}

void add_record(string record, int index, vector<Block>* Blocks) {
    int line_number = 0;
    string line;
    Block* it = &(Blocks->at(index));
    fstream file ("data.txt", ios::app | ios::in);
    file.seekg(0);
    //printf("before\n");
    while(!file.eof()) {
        getline(file, line);
        
        //printf("in while loop\n");
        printf("line num: %d, index: %d\n", line_number,index);
        printf("line:");
        printf(line.c_str());
        printf("\n");
        if(line_number == index) {
            int num_blocks = 1;
            Block* curr_block = it;
            int bucket_size = curr_block->size_used;
            while(curr_block->overflow != NULL) {
                curr_block = curr_block->overflow;
                bucket_size += curr_block->size_used;
                num_blocks += 1;
            }
            if(sizeof(record) + bucket_size > num_blocks*4096) {
                Block* new_block = new Block;
                new_block->size_used = sizeof(record);
                curr_block->overflow = new_block;
                if(line.length() != 0) {
                    while(file.peek()!='\n') {
                        file.seekg(1, ios::cur);
                    }
                }
                printf("writing1\n");
                file << "|" << record;
            } else {
                if(line.length() != 0) {
                    while(file.peek()!='\n') {
                        printf("not newline2\n");
                        file.seekg(1, ios::cur);
                    }
                }
                curr_block->size_used += sizeof(record);
                printf("writing2\n");
                file << ";" << record;
            }
        }
        line_number++;
    }
    file.close();
}

void calculateIandN(int& i, int& n, vector<Block>** Blocks, int index, vector<string> &id, vector<int> &sizes) {
    //printf("Index %d\n", index);
    //printf("I %d\n", i);
    //printf("N %d\n", n);
    //printf("Size %d\n", (*Blocks)->size());
    Block* it = &((*Blocks)->at(index));
    Block* curr_block = it;
    int num_blocks = 1;
    int bucket_size = curr_block->size_used;
    while(curr_block->overflow != NULL) {
        curr_block = curr_block->overflow;
        bucket_size += curr_block->size_used;
        num_blocks += 1;
    }
    float percent_used = 0;
    int total_buckets = 0;
    for(int curr = 0; curr != (*Blocks)->size(); curr++) {
        int size = 0;
        int num_blocks = 1;
        Block* curr_block = &((*Blocks)->at(curr));
        size += curr_block->size_used;
        while(curr_block->overflow != NULL) {
            curr_block = curr_block->overflow;
            size += curr_block->size_used;
            num_blocks += 1;
        }
        
        percent_used += ((float) size)/(4096 * num_blocks);
        total_buckets += 1;
    }
    percent_used /= total_buckets;
    if(percent_used > 0.8) {
        n++;
        if(pow(i + 1,2) < n) {
            i++;
            vector<Block> new_blocks(n);
            for(int curr = 0; curr < id.size(); curr++) {
                int index = lastIBits(hashStr(id.at(curr)), i, n);
                Block* curr_block = &(new_blocks.at(index));
                if (curr_block->size_used + sizes.at(curr) > 4096) {
                    while(curr_block->size_used + sizes.at(curr) > 4096) {
                        if(curr_block->overflow) {
                            curr_block = curr_block->overflow;
                        } else {
                            Block* new_block;
                            curr_block->overflow = new_block;
                            curr_block = curr_block->overflow;
                        }
                    }
                }
                curr_block->size_used += sizes.at(curr);
            }
        
        
            (*Blocks) = &new_blocks;
        }
    }
}

// Read csv file
void read_csv(int &file_lines, vector<string> &id, vector<string> &name, vector<string> &bio, vector<string> &manager_id, vector<int> &sizes, Hash* h, int& i, int& n, vector<Block>* Blocks){ 
	// File pointer 
	fstream f;
    // Read the Data from the file 
	// as String Vector 
	//vector<int> id, manager_id; 
	string line, word, temp;

	// Open an existing file 
	f.open("Employees.csv", ios::in);

    int num_lines = 0;
    while (getline(f,line)){
        file_lines += 1;
        //cout << line << "\n";

        vector<string> temp;
        string substr;
        stringstream ss(line);

        while(getline(ss, substr, ',')) {
            //cout << substr << "\n";
            temp.push_back(substr);
        }
        id.push_back(temp.at(0));
        name.push_back(temp.at(1));
        bio.push_back(temp.at(2));
        manager_id.push_back(temp.at(3));
        sizes.push_back(16 + ((int)sizeof(temp.at(2))) + ((int) sizeof(temp.at(3))));
        num_lines++;
    }
    
    
    for(int line_num = 0; line_num < num_lines; line_num++){
        int index = lastIBits(hashStr(id.at(line_num)), i, n);
        calculateIandN(i, n, &Blocks, index, id, sizes);
    }
    
    fstream file ("data.txt", ios::out);
    string new_lines = "";
    new_lines.append(n, '\n');
    file << new_lines;
    file.close();
    vector<Block> written_blocks(n);
    for(int line_num = 0; line_num < num_lines; line_num++){
        int index = lastIBits(hashStr(id.at(line_num)), i, n);
        string record = "";
        record.append(id.at(line_num));
        record.append(",");
        record.append(name.at(line_num));
        record.append(",");
        record.append(bio.at(line_num));
        record.append(",");
        record.append(manager_id.at(line_num));
        //printf("record:");
        //printf(record.c_str());
        //printf("\n");
        add_record(record, index, &written_blocks);
        
    }
}

void writeDataFile(Block* data) {
    fstream out;
    
    out.open("EmployeeIndex.csv", ios::out | ios::app);
    
    // Wirte the data to the file
    out << data << "," << "\n";
    
}


// Driver program
int main(){
  // array that contains keys to be mapped
  int a[] = {15, 11, 27, 8, 12};
  int n = sizeof(a)/sizeof(a[0]);
  int file_lines = 0;
  vector<string> id, name, bio, manager_id; 
  vector<int> sizes;
  int i = 2;
  n = 3;
  Hash* h = new Hash(n);
  vector<Block> Blocks(n);
  read_csv(file_lines, id, name, bio, manager_id, sizes, h, i, n, &Blocks);

  // checking vectors and the number of file line
  printf("%d\n", file_lines);
  
  for(int i = 0; i < id.size(); i++){
      printf("%d : %s\n", (i+1), (id.at(i).c_str()));
  }

  // insert the keys into the hash table
  //Hash h(file_lines + 1);   // 7 is count of buckets in
               // hash table
  //for (int i = 0; i < n; i++)
  //  h.insertItem(a[i]);

  // delete 12 from hash table
  //h.deleteItem(12);

  // display the Hash table
  //h.displayHash();
  int hash = hashStr("test1");
  printf("%d\n", hash);
  printf("%d\n", lastIBits(hash,2,n));

  //read_csv();

  return 0;
}

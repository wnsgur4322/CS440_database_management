// CS440 - assignment 4
// Team member:
// Junhyeok Jeong - jeongju@oregonstate.edu, jeongju
// Ashyan Ashley Rahavi - rahavia@oregonstate.edu, rahavia

#include <iostream>
#include <cstdio>
#include <list>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
#include <functional>
#include <math.h>
#include <bits/stdc++.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 

//using std::filesystem::directory_iterator;
using namespace std;


struct EmpBlock {
    int eid;
    string ename;
    int age;
    double salary;
};

struct DeptBlock {
    int did;
    string dname;
    double budget;
    int managerid;
};

// Grab a single block from the emp.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
EmpBlock grabEmp(fstream &empin) {
    string line, word;
    EmpBlock emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        emp.eid = stoi(word);
        getline(s, word, ',');
        emp.ename = word;
        getline(s, word, ',');
        emp.age = stoi(word);
        getline(s, word, ',');
        emp.salary = stod(word);
        return emp;
    } else {
        emp.eid = -1;
        return emp;
    }
}

// Grab a single block from the dept.csv file, in theory if a block was larger than
// one tuple, this function would return an array of DeptBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
DeptBlock grabDept(fstream &deptin) {
    string line, word;
    DeptBlock dept;
    if (getline(deptin, line, '\n')) {
        stringstream s(line);
        getline(s, word,',');
        dept.did = stoi(word);
        getline(s, word, ',');
        dept.dname = word;
        getline(s, word, ',');
        dept.budget = stod(word);
        getline(s, word, ',');
        dept.managerid = stoi(word);
        return dept;
    } else {
        dept.did = -1;
        return dept;
    }
}

//Print out the attributes from emp and dept when a join condition is met
void printJoin(EmpBlock emp, DeptBlock dept, fstream &fout) {
    fout << emp.eid << ',' << emp.ename << ',' << emp.age << ','
        << emp.salary << ',' << dept.did << ',' << dept.dname << ','
        << dept.budget << "\n";
}

bool compareEmp(EmpBlock lhs, EmpBlock rhs) {
    return lhs.eid < rhs.eid;
}

bool compareDept(DeptBlock lhs, DeptBlock rhs) {
    return lhs.managerid < rhs.managerid;
}

void writeRun(vector<EmpBlock>* employees, const char*csv_name) {
    fstream run;
    run.open(csv_name, ios::out);
    printf("writing ");
    printf(csv_name);
    printf("\n");
  
    for(auto emp : *employees) {
      run << emp.eid << ",";
      run << emp.ename.c_str() << ",";
      run << emp.age << ",";
      run << emp.salary << "\n";
    }
    run.close();
}
void writeRun(vector<DeptBlock>* depts, const char*csv_name) {
    fstream run;
    run.open(csv_name, ios::out);
    printf("writing ");
    printf(csv_name);
    printf("\n");
  
    for(auto dept : *depts) {
      run << dept.did << ",";
      run << dept.dname.c_str() << ",";
      run << dept.budget << ",";
      run << dept.managerid << "\n";
    }
    run.close();
}

void sortEmp(fstream& in) {
  // flags check when relations are done being read
  bool flag = true;
  int i = 0;
  while (flag) {
    vector<EmpBlock> employees;
    bool break_for = false;
    for(int i = 0; i < 22; i++ && break_for == false) {
        EmpBlock block = grabEmp(in);
        if (block.eid == -1) {
            flag = false;
            break_for = true;;
        } else {
            employees.push_back(block);
        }
    }
    if(employees.size() > 0) {
        sort(employees.begin(), employees.end(), compareEmp);
        string csv_name = "sort-merge/emp-run";
        csv_name.append(to_string(i));
        csv_name.append(".csv");
        writeRun(&employees, csv_name.c_str());
    }
    i++;
  }
  
}

void sortDept(fstream& in) {
  // flags check when relations are done being read
  bool flag = true;
  int i = 0;
  while (flag) {
    vector<DeptBlock> depts;
    bool break_for = false;
    for(int i = 0; i < 22; i++ && break_for == false) {
        DeptBlock block = grabDept(in);
        if (block.did == -1) {
            flag = false;
            break_for = true;;
        } else {
            depts.push_back(block);
        }
    }
    if(depts.size() > 0) {
        sort(depts.begin(), depts.end(), compareDept);
        string csv_name = "sort-merge/dept-run";
        csv_name.append(to_string(i));
        csv_name.append(".csv");
        writeRun(&depts, csv_name.c_str());
    }
    i++;
  }
  
}

int main() {
  // open file streams to read and write
  char* dirname = "sort-merge";
  int check; 
  check = mkdir(dirname,0777); 
  if (!check) {
    printf("Directory created\n"); 
  } else { 
    printf("Directory already made\n");
  }
  fstream empin;
  empin.open("Emp.csv", ios::in);  
  sortEmp(empin);
  empin.close();
  fstream deptin;
  deptin.open("Dept.csv", ios::in);  
  sortDept(deptin);
  deptin.close();
  return 0;
  
  fstream joinout;
  joinout.open("Join.csv", ios::out | ios::app);
  empin.open("Emp.csv", ios::in);
  bool flag = true;
  while (flag) {
      // FOR BLOCK IN RELATION EMP

      // grabs a block
      EmpBlock EmpBlock = grabEmp(empin);
      // checks if filestream is empty
      if (EmpBlock.eid == -1) {
          flag = false;
      }
      bool iflag = true;
      // opens new filestream for dept relation (needs to read in a new one each time a new emp block is seen)
      fstream deptin;
      deptin.open("Dept.csv", ios::in);
      while (iflag) {
          // FOR BLOCK IN RELATION DEPT
          DeptBlock deptBlock = grabDept(deptin);

          // in theory these would iterate through the two blocks: EmpBlock and deptBlock
          // but since both only contain one tuple, no iteration is needed
          if (deptBlock.did == -1) {
              iflag = false;
          } else {
              // check join condition and print join to output file
              if (deptBlock.managerid == EmpBlock.eid) {
                  printJoin(EmpBlock, deptBlock, joinout);
              }
          }
      }
  }

  return 0;
}

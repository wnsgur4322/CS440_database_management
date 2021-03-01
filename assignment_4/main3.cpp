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
#include <dirent.h>

DIR *dir;
struct dirent *ent;

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
    //printf("writing ");
    //printf(csv_name);
    //printf("\n");
  
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
    //printf("writing ");
    //printf(csv_name);
    //printf("\n");
  
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

void print_dept_block(DeptBlock dept) {
  cout << dept.did << ",";
  cout << dept.dname << ",";
  cout << dept.budget << ",";
  cout << dept.managerid;
  cout << endl;
}

void print_emp_block(EmpBlock emp) {
  cout << emp.eid << ",";
  cout << emp.ename << ",";
  cout << emp.age << ",";
  cout << emp.salary;
  cout << endl;
}

void create_join() {
  vector<string> emp_files;
  vector<string> dept_files;
  if ((dir = opendir ("sort-merge/")) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      string name = "sort-merge/";
      name.append(ent->d_name);
      if (name.find("emp") != string::npos) {
        emp_files.push_back(name);
      } else if (name.find("dept") != string::npos) {
        dept_files.push_back(name);
      }
      
    }
    closedir (dir);
  }
  
  bool flag = true;
  vector<EmpBlock> employees;
  vector<DeptBlock> depts;
  vector<fstream*> emp_fstreams(emp_files.size());
  vector<fstream*> dept_fstreams(dept_files.size());
  int i = 0;
  vector<istream::streampos> line_beginnings;
  for(auto x: emp_files) {
    emp_fstreams[i] = new fstream(x.c_str(), ios::in);
    //printf(x.c_str());
    line_beginnings.push_back((*(emp_fstreams[i])).tellg());
    employees.push_back(grabEmp(*(emp_fstreams[i])));
    i++;
  }
  i = 0;
  for(auto x: dept_files) {
    dept_fstreams[i] = new fstream(x.c_str(), ios::in);
    depts.push_back(grabDept(*(dept_fstreams[i])));
    i++;
  }
  
  fstream join_out;
  join_out.open("Join.csv", ios::out);
  int emps_left = employees.size();
  int depts_left = depts.size();
  while(emps_left != 0 and depts_left != 0 ) {
    int lowest_id = numeric_limits<int>::max();
    int lowest_emp_ind = -1;
    int lowest_dept_ind = -1;
    for(int i = 0; i < employees.size(); i++) {
      EmpBlock curr_emp = employees.at(i);
      if(curr_emp.eid != -1 and curr_emp.eid <lowest_id) {
        lowest_id = curr_emp.eid;
        lowest_emp_ind = i;
      }
    }
    lowest_id = numeric_limits<int>::max();
    for(int i = 0; i < depts.size(); i++) {
      DeptBlock curr_dept = depts.at(i);
      
      if(curr_dept.did != -1 and curr_dept.managerid <lowest_id) {
        lowest_id = curr_dept.managerid;
        lowest_dept_ind = i;
      }
    }
    if(employees.at(lowest_emp_ind).eid < depts.at(lowest_dept_ind).managerid) {
      employees.erase(employees.begin() + lowest_emp_ind);
      fstream* file = emp_fstreams.at(lowest_emp_ind);
      //cout << "tellg val: " << (*file).tellg() << endl;
      line_beginnings[lowest_emp_ind] = (*file).tellg();
      EmpBlock new_block = grabEmp(*file);
      if(new_block.eid == -1)
        emps_left--;
      employees.insert(employees.begin() + lowest_emp_ind, new_block);
      //printf("emp ind < manager: ");
      //print_emp_block(new_block);
    } else if(employees.at(lowest_emp_ind).eid > depts.at(lowest_dept_ind).managerid) {
      depts.erase(depts.begin() + lowest_dept_ind);
      fstream* file = dept_fstreams.at(lowest_dept_ind);
      DeptBlock new_block = grabDept(*file);
      if(new_block.did == -1)
        depts_left--;
      depts.insert(depts.begin() + lowest_dept_ind, new_block);
      
      file = emp_fstreams.at(lowest_emp_ind);
      
      //printf("emp ind > manager: ");
      //print_emp_block(employees.at(lowest_emp_ind));
      //print_dept_block(new_block);
    } else {
      //printf("curr: ");
      //print_emp_block(employees.at(lowest_emp_ind));
      fstream* file_temp = emp_fstreams[lowest_emp_ind];
      //cout << "tellg val for lowest: " << (*file_temp).tellg() << endl;
      while(employees.at(lowest_emp_ind).eid == depts.at(lowest_dept_ind).managerid) {
        DeptBlock dept_block = depts.at(lowest_dept_ind);
        EmpBlock emp_block = employees.at(lowest_emp_ind);
        join_out << dept_block.did << ",";
        join_out << dept_block.dname << ",";
        join_out << dept_block.budget << ",";
        join_out << dept_block.managerid << ",";
        join_out << emp_block.eid << ",";
        join_out << emp_block.ename << ",";
        join_out << emp_block.age << ",";
        join_out << emp_block.salary << "\n";
        employees.erase(employees.begin() + lowest_emp_ind);
        fstream* file = emp_fstreams.at(lowest_emp_ind);
        EmpBlock new_block = grabEmp(*file);
        //print_emp_block(new_block);
        employees.insert(employees.begin() + lowest_emp_ind, new_block);
        lowest_id = numeric_limits<int>::max();
        for(int i = 0; i < employees.size(); i++) {
          EmpBlock curr_emp = employees.at(i);
          if(curr_emp.eid != -1 and curr_emp.eid < lowest_id) {
            lowest_id = curr_emp.eid;
            lowest_emp_ind = i;
          }
        }
      }
      //Resets employees vector
      for(int i = 0; i < line_beginnings.size(); i++) {
        fstream* file = emp_fstreams[i];
        (*file).seekg(line_beginnings[i]);
        EmpBlock new_emp_block = grabEmp(*file);
        //print_emp_block(new_emp_block);
        employees.erase(employees.begin() + i);
        employees.insert(employees.begin() + i, new_emp_block);
      }
      depts.erase(depts.begin() + lowest_dept_ind);
      fstream* file = dept_fstreams.at(lowest_dept_ind);
      DeptBlock new_dept_block = grabDept(*file);
      if(new_dept_block.did == -1)
        depts_left--;
      depts.insert(depts.begin() + lowest_dept_ind, new_dept_block);
    }
  }
  join_out.close();
  for(auto x: emp_fstreams) {
   (*x).close();
  }
  for(auto x: dept_fstreams) {
    (*x).close();
  }
}

int main() {
  // open file streams to read and write
  char* dirname = "sort-merge";
  int check; 
  check = mkdir(dirname,0777); 
  if (!check) {
    printf("sort-merge directory created\n"); 
  } else { 
    printf("sort-merge directory already made\n");
  }
  fstream empin;
  empin.open("Emp.csv", ios::in);  
  sortEmp(empin);
  empin.close();
  fstream deptin;
  deptin.open("Dept.csv", ios::in);  
  sortDept(deptin);
  deptin.close();
  create_join();
  return 0;
}

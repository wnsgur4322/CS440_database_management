// CS440 - assignment 4
// Team member:
// Junhyeok Jeong - jeongju@oregonstate.edu, jeongju
// Ashyan Ashley Rahavi - rahavia@oregonstate.edu, rahavia

#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
#include <functional>
#include <math.h>
#include <algorithm>

using namespace std;

struct EmpBlock {
    int eid;
    string ename;
    int age;
    int salary;
    string fline;

    bool operator > (const EmpBlock& str) const
    {
        return (str.eid > eid);
    }
};

// Grab a single block from the emp.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
EmpBlock grabEmp(fstream &empin) {
    string line, word;
    EmpBlock emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        emp.fline = line;
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

int main() {
  // 1 block = 2 integers + string + double = 4 + 4 + 8 + 40 = 56 bytes
  int block_size = 56;
  int mem_blocks = 22;

  int total_mem = block_size * mem_blocks;

  // open file streams to read and write
  ifstream empin;
  empin.open("Emp.csv");
  empin.seekg(0, empin.end);
  int empin_size = empin.tellg();
  empin.seekg(0, empin.beg);
  cout << "emp.csv size: " << empin_size << endl;
  
  // get number of iteration
  int iter_count;
  if(empin_size % total_mem > 0){
      iter_count = empin_size / total_mem + 1;
  }
  else{
      iter_count = empin_size / total_mem;
  }
  cout << "The number of iteration for external sorting: " << iter_count << endl;

  vector<string> iters;
  string iter;
  
  for(int i = 0; i < iter_count; i++){
      stringstream ss;
      ss << "iter_" << i << ".csv";
      iter = ss.str();
      iters.push_back(iter);
  }

  int tuple_size = 0;
  int iter_i = 0;
  string line, word;
  vector<EmpBlock> data;
  ofstream empout;

  while(!empin.eof()){
    EmpBlock emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        emp.fline = line;
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
    }

      if((tuple_size + line.size()) < total_mem){
          data.push_back(emp);
          tuple_size++;
          tuple_size += line.size();
      }
      else{
          sort(data.begin(), data.end(), greater<EmpBlock>());
          empout.open(iters[iter_i].c_str());

          int data_size = data.size();

          for(int i = 0; i < (data_size - 1); i++){
              empout << data[i].eid;
              empout << ",";
              empout << data[i].ename;
              empout << ",";
              empout << data[i].age;
              empout << ",";
              empout << data[i].salary;
              empout << "\n";

          }
          empout << data[data_size - 1].eid;
          empout << ",";
          empout << data[data_size - 1].ename;
          empout << ",";
          empout << data[data_size - 1].age;
          empout << ",";
          empout << data[data_size - 1].salary;
          empout.close();
          data.clear();
          iter_i++;
          data.push_back(emp);
          tuple_size = line.size();
      }
  }
  //printf("%i\n", emp.eid);
  
  if(data.size() > 0){
      sort(data.begin(), data.end(), greater<EmpBlock>());
      empout.open(iters[iter_i].c_str());
      int data_size = data.size();
      for(int i = 0; i < (data_size - 1); i++){
            empout << data[i].eid;
            empout << ",";
            empout << data[i].ename;
            empout << ",";
            empout << data[i].age;
            empout << ",";
            empout << data[i].salary;
            empout << "\n";
      }
      empout << data[data_size - 1].eid;
      empout << ",";
      empout << data[data_size - 1].ename;
      empout << ",";
      empout << data[data_size - 1].age;
      empout << ",";
      empout << data[data_size - 1].salary;
      empout.close();
      data.clear();
      iter_i++;
      tuple_size = 0;
  }
  empin.close();

  cout << "PASS 0 has ended now!" << endl;

  vector<int> pass_iter;
  int iter_count_2 = iter_count;
  int future_iter_num = 0;

  while(iter_count_2 > (mem_blocks - 1)){
      if(iter_count_2 % (mem_blocks - 1) > 0){
          iter_count_2 = (iter_count_2/ (mem_blocks -1)) + 1;
      }
      else{
          iter_count_2 = iter_count_2/(mem_blocks - 1);
      }
      pass_iter.push_back(iter_count_2);
      future_iter_num += iter_count_2;
  }
  
  if(iter_count_2 > 1){
      future_iter_num++;
      pass_iter.push_back(1);
  }

  if(future_iter_num == 0){
      string temp_name = iters[0];
      string out_name = "EmpSorted.csv";
      int result = rename(temp_name.c_str(), out_name.c_str());
  }
  else{
      for(int i = iter_count; i < iter_count + future_iter_num; i++){
          stringstream ss;
          ss << "iter_" << i << ".csv";
          iter = ss.str();
          iters.push_back(iter);
      }

      iters.pop_back();
      stringstream ss;
      ss << "EmpSorted.csv";
      iter = ss.str();
      iters.push_back(iter);
  }

  int cur_read_i = 0;
  int cur_write_i = iter_count;
  int min_i;
  int mem_block_left;
  int begin;
  int end;
  int start;
  int blocks;
  vector<int> start_pts;
  vector<int> end_pts;
  int min_id;
  string min_str;

  for(int j = 0; j < pass_iter.size(); j++){
      begin = cur_read_i;
      start = begin;
      end = cur_write_i - 1;
      cout << "PASS " << j + 1 << " is working ..." << endl;

      while(start <= end){
          if(((end + 1) - start) / (mem_blocks - 1) >= 1){
              blocks = mem_blocks - 1;
          }
          else{
              blocks = ((end + 1) -start) % (mem_blocks - 1);
          }
          start += blocks;

          int line_counter = 0;

          for(int i = 0; i < blocks; i++){
              empin.open(iters[cur_read_i].c_str());
              start_pts.push_back(line_counter);

              while(!empin.eof()){
                  EmpBlock emp;
                  // grab entire line
                  if (getline(empin, line, '\n')) {
                      emp.fline = line;
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
                      }
                  //empin >> line;
                  data.push_back(emp);
                  line_counter++;
              }
              

              end_pts.push_back(line_counter - 1);
              empin.close();
              cur_read_i++;
        }
        // start merging process
        empout.open(iters[cur_write_i].c_str());
        int start_sum = accumulate(start_pts.begin(), start_pts.end(), 0);
        int end_sum = accumulate(end_pts.begin(), end_pts.end(), 0);
        int cur_pt_i;
        
        while(start_sum != end_sum){
            cur_pt_i = 0;

            while(start_pts[cur_pt_i] == end_pts[cur_pt_i]){
                cur_pt_i++;
            }
            min_id = data[start_pts[cur_pt_i]].eid;
            min_str = data[start_pts[cur_pt_i]].fline;
            min_i = cur_pt_i;
            while(cur_pt_i < start_pts.size()){
                if(start_pts[cur_pt_i] == end_pts[cur_pt_i]){
                    cur_pt_i++;
                }
                else if(min_id > data[start_pts[cur_pt_i]].eid){
                    min_id = data[start_pts[cur_pt_i]].eid;
                    min_str = data[start_pts[cur_pt_i]].fline;
                    min_i = cur_pt_i;
                    cur_pt_i++;
                }
                else{
                    cur_pt_i++;
                }
            }
            empout << min_str;
            empout << "\n";
            start_pts[min_i] = start_pts[min_i] + 1;
            start_sum++;
        }
        empout.close();
        cur_write_i++;
        data.clear();
        start_pts.clear();
        end_pts.clear();
      }
      cout << "PASS " << j + 1 << " has finished" << endl;
  }


	// Size of the input file:
    string input_file = "Emp.csv";
	empin.open(input_file.c_str());
	empin.seekg (0, empin.end);
    int filesize = empin.tellg();
    empin.seekg (0, empin.beg);
	cout << "The size of the input file: " << filesize << " bytes." << endl;
	empin.close();

	// Size of the output file:
    string output_file = "EmpSorted.csv";
	empin.open(output_file.c_str());
	empin.seekg (0, empin.end);
    filesize = empin.tellg();
    empin.seekg (0, empin.beg);
	cout << "The size of the output file: " << filesize << " bytes." << endl;
	empin.close();


  return 0;
}

#include <iostream>
#include <fstream>
#include "ArgumentManager.h"
#include <vector>
#include <algorithm>
using namespace std;

class Customer{
  public:
    string id;
    Customer* next;
    bool guilty;
    Customer(){
      id = "";
      next = nullptr;
      guilty = false;
    }
    Customer(string id ){
      this->id = id;
      next =  nullptr;
      guilty = false;
    }
};

class Bar{
  private:
    Customer* head;
    int size;
    string name;
  public:
    Bar(){
      head = nullptr;
      size = 0;
      name = "";
    }
    Bar(string name): head(nullptr), size(0), name(name){}
    Customer* getHead(){ return head; };

    void addCustomer(Customer* newCustomer){
      if(head == nullptr){
        head = newCustomer;
        size++;
        return;
      }
      Customer* curr = head;
      //check if the customer is already in the bar
      while(curr){
        if(curr->id == newCustomer->id){
          curr->guilty = true;
          return;
        }
        curr = curr->next;
      }

      curr = head;
      while(curr->next){
        curr = curr->next;
      }
      curr->next = newCustomer;
      size++;
    }
};

string reverseStr(string str){
  for(int i =0; i < str.length()/2; i++){
    char temp = str[i];
    str[i] = str[str.length() - 1 -i];
    str[str.length() - 1- i] = temp;
  }
  return str;
}

string decodeInput(string str){
    if(str.find('(') == string::npos) return str;
    int position = 0;
    int startIndex = 0;
    while(str.find('(', position) != string::npos){
        startIndex = str.find('(', position);
        position = startIndex + 1;
    }
    int stopIndex = str.find(')');
    string reversedStr = reverseStr(str.substr(startIndex + 1, stopIndex - startIndex - 1));
    string replacedStr = str.replace(startIndex, stopIndex - startIndex + 1, reversedStr);
    // cout << replacedStr << endl;
    return decodeInput(replacedStr);
}

void sort(vector<string>& v){
  for(int i = 0; i < v.size(); i++){
    bool isSorted = true;
    for(int j = 0; j < v.size() - 1 - i; j++){
      if(stoi(v[j]) > stoi(v[j+1])){
        string temp = v[j];
        v[j] = v[j+1];
        v[j+1] = temp;
        isSorted = false;
      }
    }
    if(isSorted) return;
  }
}


int main(int agrc, char* argv[]){
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  ArgumentManager am(agrc, argv);
  ifstream ifs(am.get("input"));
  ofstream ofs(am.get("output"));

  Bar bar1("bar1");
  Bar bar2("bar2");
  string currentBar = "";
  string line;
  while(getline(ifs,line)){
    line.erase(remove(line.begin(), line.end(), '\n'), line.end());
    line.erase(remove(line.begin(), line.end(), '\r'), line.end());
    line.erase(remove(line.begin(), line.end(), ' '), line.end());
    if(line == "") continue;
    else if(line == "Bar1" || line == "Bar2"){
      currentBar = line;
    }
    else{
      Customer* newCustomer = new Customer(decodeInput(line));
      if(currentBar == "Bar1"){
        bar1.addCustomer(newCustomer);
      }
      else if(currentBar == "Bar2"){
        bar2.addCustomer(newCustomer);
      }
    }
  }

  Customer* b1 = bar1.getHead();
  Customer* b2 = bar2.getHead();

  vector<string> guiltyList;
  vector<string> innocentList;
  
  //check IDs in both bar1 and bar2
  while(b1){
    b2 = bar2.getHead();
    while(b2){
      if(b1->id == b2->id){
        b1->guilty = true;
        b2->guilty = true;
        break;
      }
      b2 = b2->next;
    }
    if(b1->guilty){
      guiltyList.push_back(b1->id);
    }
    else{
      innocentList.push_back(b1->id);
    }
    b1 = b1->next;
  }

  //put remaininng IDs of bar2 into innocentList
  b2 = bar2.getHead();
  while(b2){
    //if id is not in guitly list => innocent
    if(find(guiltyList.begin(), guiltyList.end(), b2->id) == guiltyList.end()){
      if(b2->guilty){
        guiltyList.push_back(b2->id);
      }
      else{
        innocentList.push_back(b2->id);
      }
    }
    b2 = b2->next;
  }

  //sort
  sort(guiltyList);
  sort(innocentList);

  if(guiltyList.size() != 0){
    ofs << "Guilty:" << endl;
    for(int i =0; i < guiltyList.size(); i++){
        ofs << guiltyList[i] << endl;
    } 
  }
  if(innocentList.size() != 0){
    ofs << "Innocent:" << endl;
    for(int i =0; i < innocentList.size(); i++){
      ofs << innocentList[i] << endl;
    }
  }
  
  return 0;
}
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
      while(curr->next){
        if(curr->id == newCustomer->id){
          curr->guilty = true;
        }
        curr = curr->next;
      }
      curr->next = newCustomer;
      size++;
    }

    //insertion sort
    void sortCustomer(){
      if(head == nullptr || head->next == nullptr) return;

      for(int i = 0; i < size; i++){
        bool isSorted = true;
        Customer* curr = head;

        for(int j = 0; j < size - 1 - i;j++){
          if(stoi(curr->id) > stoi(curr->next->id)){
            string tempID = curr->id;
            curr->id = curr->next->id;
            curr->next->id = tempID;
            isSorted = false;
          }
          curr  = curr->next;
        }
        if(isSorted) return;
      }
    }
    void printCustomer(){
      Customer* curr = head;
      cout << name << ":" << endl;
      while(curr){
        cout << curr->id << endl;
        curr = curr->next;
      }
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
        position++;
    }
    int stopIndex = str.find(')');
    string reversedStr = reverseStr(str.substr(startIndex + 1, stopIndex - startIndex - 1));
    string replacedStr = str.replace(startIndex, stopIndex - startIndex + 1, reversedStr);
    // cout << replacedStr << endl;
    return decodeInput(replacedStr);
}

int main(int agrc, char* argv[]){
  ArgumentManager am(agrc, argv);
  ifstream ifs(am.get("input"));
  ofstream ofs(am.get("output"));

  Bar bar1("bar1");
  Bar bar2("bar2");
  string currentBar = "";
  string line;
  while(getline(ifs,line)){
    if(line == "") continue;
    if(line == "Bar1" || line == "Bar2"){
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
  bar1.sortCustomer();
  bar2.sortCustomer();
  // bar1.printCustomer();
  // cout << endl;
  // bar2.printCustomer();

  Customer* b1 = bar1.getHead();
  Customer* b2 = bar2.getHead();

  vector<string> guiltyList;
  vector<string> innocentList;

  while(b1){
    bool guilty = false;
    b2 = bar2.getHead();
    while(b2){
      if(b1->id == b2->id){
        guilty = true;
        b1->guilty = true;
        b2->guilty = true;
      }
      b2 = b2->next;
    }
    if(guilty){
      guiltyList.push_back(b1->id);
    }
    else{
      innocentList.push_back(b1->id);
    }
    b1 = b1->next;
  }

  b1 = bar1.getHead();
  while(b1){
    if(find(guiltyList.begin(), guiltyList.end(), b1->id) == guiltyList.end()){
      if(b1->guilty){
        guiltyList.push_back(b1->id);
      }
      else if(count(innocentList.begin(), innocentList.end(), b1->id) == 0){
        innocentList.push_back(b1->id);
      }
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
      else if(count(innocentList.begin(), innocentList.end(), b2->id) == 0){
        innocentList.push_back(b2->id);
      }
    }
    b2 = b2->next;
  }

  //sort guityList
  for(int i = 0; i < guiltyList.size(); i++){
    for(int j = 0; j < guiltyList.size() - 1 - i; j++){
      if(stoi(guiltyList[j]) > stoi(guiltyList[j+1])){
        string temp = guiltyList[j];
        guiltyList[j] = guiltyList[j+1];
        guiltyList[j+1] = temp;
      }
    }
  }

  //sort innocentList 
  for(int i = 0; i < innocentList.size(); i++){
    for(int j = 0; j < innocentList.size() - 1 - i; j++){
      if(stoi(innocentList[j]) > stoi(innocentList[j+1])){
        string temp = innocentList[j];
        innocentList[j] = innocentList[j+1];
        innocentList[j+1] = temp;
      }
    }
  }

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
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <sstream>
using namespace std;

class Node{
  public:
    Node(){};
    virtual void Evaluate(){return ;};
    virtual variant<string,int> Value(){return variant<string,int>();};
    virtual void Print(ostream& os){};
    virtual  bool IsZero(){return true;};
    virtual  bool IsScalar(){return true;};
    virtual ~Node(){};
};
class MultiplicationNode:public Node{
  public:
    char op;
    Node* left;
    Node* right;
    variant<string,int> value;
  public:
    MultiplicationNode(string s):value(s),op('*'),left(nullptr),right(nullptr){}
    MultiplicationNode(int n):value(n),op('*'),left(nullptr),right(nullptr){}
    MultiplicationNode(Node* left,Node* right):left(left),op('*'),right(right){
    }
    variant<string,int> Value(){
      return value;
    }
    void Evaluate(){
      if (left&&right){
          left->Evaluate();
          right->Evaluate();
          if(left->IsZero()||right->IsZero()){
            value = 0;
          }
          else if(left->IsScalar()&&right->IsScalar()){
            int val = get<int>(left->Value())*get<int>(right->Value()); 
          }
        }
      }
    void  Print(ostream& os){
      if (left&&right)
      {
        if (holds_alternative<int>(left->Value())&&(get<int>(left->Value())==1)){
          right->Print(os);
        }
        else if(holds_alternative<int>(right->Value())&&(get<int>(right->Value())==1)){
          left->Print(os);
        }
        else{
          os<<"(* ";
          left->Print(os);
          os<<" ";
          right->Print(os);
          os<<")";
        }
      }
      else {
        if (holds_alternative<string>(value)) os<<get<string>(value);
        else os<<get<int>(value);
      }
    }
    bool IsZero(){
      if (holds_alternative<int>(value)){
        if (get<int>(value)==0) return true;
      }
      return false;
    }
    bool IsScalar(){
      return holds_alternative<int>(value);
    }
};
class LinearNode:public Node{
  public:
    char op;
    Node* left;
    Node* right;
    variant<string,int> value;
  public:
    LinearNode(string s):value(s),op('*'),left(nullptr),right(nullptr){}
    LinearNode(int n):value(n),op('*'),left(nullptr),right(nullptr){}
    LinearNode(Node* left,Node* right):left(left),op('*'),right(right){
    }
    variant<string,int> Value(){
      return value;
    }
    void Evaluate(){
      if (left&&right){
          left->Evaluate();
          right->Evaluate();
          if(left->IsZero()||right->IsZero()){
            value = 0;
          }
          else if(left->IsScalar()&&right->IsScalar()){
            int val = get<int>(left->Value())*get<int>(right->Value()); 
          }
        }
      }
    void  Print(ostream& os){
      if (left&&right)
      {
        if (holds_alternative<int>(left->Value())&&(get<int>(left->Value())==1)){
          right->Print(os);
        }
        else if(holds_alternative<int>(right->Value())&&(get<int>(right->Value())==1)){
          left->Print(os);
        }
        else{
          os<<"(* ";
          left->Print(os);
          os<<" ";
          right->Print(os);
          os<<")";
        }
      }
      else {
        if (holds_alternative<string>(value)) os<<get<string>(value);
        else os<<get<int>(value);
      }
    }
    bool IsZero(){
      if (holds_alternative<int>(value)){
        if (get<int>(value)==0) return true;
      }
      return false;
    }
    bool IsScalar(){
      return holds_alternative<int>(value);
    }
}
Node* Parse(string s){
  return nullptr;
}
int main(){
  MultiplicationNode m1("x");
  MultiplicationNode m2(1);
  MultiplicationNode m3(static_cast<Node*>(&m1),static_cast<Node*>(&m2));
  MultiplicationNode m4(static_cast<Node*>(&m1),static_cast<Node*>(&m3));
  m3.Print(cout);
  cout<<endl;
  m4.Print(cout);
  return 0;
}

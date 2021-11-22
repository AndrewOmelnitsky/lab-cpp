#include <iostream>
#include <memory>
#include <string>
#include <vector>
// #include "ssteam"

using namespace std;

class ArithmeticExpression
{
private:
public:
  // float get_value(){
  //   return -1;
  // }
  virtual float get_value() = 0;
};

class ValueExpression : public ArithmeticExpression
{
private:
  float value;

public:
  ValueExpression(float new_value){
    value = new_value;
  }
  float get_value() override{
    // cout << "value: " << value << endl;
    return value;
  }
};

class BinaryExpression : public ArithmeticExpression
{
private:
public:
  shared_ptr<ArithmeticExpression> left;
  shared_ptr<ArithmeticExpression> right;
  BinaryExpression(shared_ptr<ArithmeticExpression> new_left, shared_ptr<ArithmeticExpression> new_right){
    left = new_left;
    right = new_right;
  }
  BinaryExpression(){};
  BinaryExpression(const BinaryExpression *be){
    left = be->left;
    right = be->right;
  }
  ~BinaryExpression(){
    left.reset();
    right.reset();
  }
};

class ProductExpression : public BinaryExpression
{
public:
  ProductExpression(
    shared_ptr<ArithmeticExpression> new_left,
    shared_ptr<ArithmeticExpression> new_right) :BinaryExpression(new_left, new_right){};

  ~ProductExpression(){};

  float get_value() override{
    return left->get_value() * right->get_value();
  }
};


class DivisionExpression : public BinaryExpression
{
public:
  DivisionExpression(
    shared_ptr<ArithmeticExpression> new_left,
    shared_ptr<ArithmeticExpression> new_right) :BinaryExpression(new_left, new_right){};

  ~DivisionExpression(){};

  float get_value() override{
    return left->get_value() / right->get_value();
  }
};


class SumExpression : public BinaryExpression
{
public:
  SumExpression(
    shared_ptr<ArithmeticExpression> new_left,
    shared_ptr<ArithmeticExpression> new_right) :BinaryExpression(new_left, new_right){};

  ~SumExpression(){};

  float get_value() override{

    return left->get_value() + right->get_value();
  }
};


class DifferenceExpression : public BinaryExpression
{
public:
  DifferenceExpression(
    shared_ptr<ArithmeticExpression> new_left,
    shared_ptr<ArithmeticExpression> new_right) :BinaryExpression(new_left, new_right){};

  ~DifferenceExpression(){};

  float get_value() override{
    return left->get_value() - right->get_value();
  }
};


class instream {
private:
  vector <char> stream_vect;

public:
  instream(){
    append_sub_symbol();
  };
  instream(string new_str){
    for (auto c : new_str) {
      stream_vect.push_back(c);
    }
    append_sub_symbol();
  }
  instream(const instream &stream){
    stream_vect = stream.stream_vect;
  }

  void putback(char c){
    auto iter = stream_vect.cbegin();
    stream_vect.emplace(iter, c);
  }

  void append_sub_symbol(){
    stream_vect.push_back(';');
  }

  char get(){
    char c = stream_vect[0];
    auto iter = stream_vect.cbegin();
    stream_vect.erase(iter);

    return c;
  }

  instream& operator<< (string str){
    stream_vect.pop_back();
    for (auto c : str) {
      stream_vect.push_back(c);
    }
    append_sub_symbol();
    return *this;
  }

  string str(){
    string result;
    for (auto c : stream_vect) {
      result += c;
    }
    return result;
  }
};

class ExpressionConvertor {
private:
  instream stream;

  shared_ptr<ArithmeticExpression> number(){
    int result = 0;
    while (true) {
      char c = stream.get();
      if ('0' <= c && c <= '9') {
        result = result * 10 + c - '0';
      }
      else{
        stream.putback(c);
        break;
      }
    }

    auto res = make_shared<ValueExpression>(result);
    return res;
  }

  shared_ptr<ArithmeticExpression> product(){
    shared_ptr<ArithmeticExpression> num;
    char op = stream.get();
    if (op == '(') {
      num = this->sum();
      stream.get();
    }
    else{
      stream.putback(op);
      num = number();
    }
    op = stream.get();
    shared_ptr<ArithmeticExpression> next_result;
    switch (op) {
      case '*':
        next_result = this->product();
        return make_shared<ProductExpression>(num, next_result);
        break;
      case '/':
        next_result = this->product();
        return make_shared<DivisionExpression>(num, next_result);
        break;
      default:
        stream.putback(op);
        return num;
        break;
    }
  }

  shared_ptr<ArithmeticExpression> sum(){
    shared_ptr<ArithmeticExpression> num = this->product();
    char op = stream.get();
    shared_ptr<ArithmeticExpression> next_result;
    switch (op) {
      case '+':
        next_result = this->sum();
        return make_shared<SumExpression>(num, next_result);;
        break;
      case '-':
        next_result = this->sum();
        return make_shared<DifferenceExpression>(num, next_result);;
        break;
      default:
        stream.putback(op);
        return num;
        break;
    }
  }

public:
  ExpressionConvertor(instream new_stream){
    stream = new_stream;
  }
  shared_ptr<ArithmeticExpression> convert(){
    shared_ptr<ArithmeticExpression> result;
    result = this->sum();
    return result;
  }
};

// float number(){
//   int result = 0;
//   while (true) {
//     char c = cin.get();
//     if ('0' <= c && c <= '9') {
//       result = result * 10 + c - '0';
//     }
//     else{
//       cin.putback(c);
//       break;
//     }
//   }
//   return result;
// }
//
// float sum();
//
// float mull(){
//   float num;
//   char op = cin.get();
//   if (op == '(') {
//     num = sum();
//     cin.get();
//   }
//   else{
//     cin.putback(op);
//     num = number();
//   }
//   op = cin.get();
//   switch (op) {
//     case '*':
//       return num*mull();
//       break;
//     case '/':
//       return num/mull();
//       break;
//     default:
//       cin.putback(op);
//       return num;
//       break;
//   }
// }
//
// float sum(){
//   float num = mull();
//   char op = cin.get();
//   switch (op) {
//     case '+':
//       return num+sum();
//       break;
//     case '-':
//       return num-sum();
//       break;
//     default:
//       cin.putback(op);
//       return num;
//       break;
//   }
// }

int main(){
  string string_expression;
  cin >> string_expression;
  instream stream_expression;
  stream_expression << string_expression;
  ExpressionConvertor conv_expression(stream_expression);
  shared_ptr<ArithmeticExpression> expression = conv_expression.convert();
  cout << expression->get_value() << endl;
}


// (2+2)*2/(2*3-5)

// #include <stdafx.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <exception>
#include <float.h>
#include <memory>
#include <vector>
#include <map>
#include <utility>
#include <fstream>


using namespace std;

class ComplexNumberException : public exception
{
private:

public:
    string m_error;

    ComplexNumberException()
    {
        m_error = "complex number exception";
    }
    ComplexNumberException(string error)
    {
        m_error = error.c_str();
    }

    const char* what() const noexcept { return m_error.c_str(); }
};


class ComplexNumberStringConstructorException : public ComplexNumberException
{
private:

public:
    char wrong_character;

    ComplexNumberStringConstructorException()
    {
        m_error = "it is not possible to convert this string to a complex number";
    }
    ComplexNumberStringConstructorException(char exception_char) : wrong_character(exception_char)
    {
        m_error = "it is not possible to convert this char string to a complex number\n";
        m_error += "exception when converting this character '";
        m_error += exception_char;
        m_error += "' to a number";
    }

    char get_wrong_character() {
        return wrong_character;
    }
};


class ComplexNumberDividingByZero : public ComplexNumberException
{
private:

public:
    ComplexNumberDividingByZero()
    {
        m_error = "complex number cannot divide by zero\n";
    }
};


class ComplexNumberWrongIndex : public ComplexNumberException
{
private:

public:
    int wrong_index;

    ComplexNumberWrongIndex()
    {
        m_error = "you cannot take data by this index\n";
        m_error += get_help_info();
    }

    ComplexNumberWrongIndex(const int index) : wrong_index(index)
    {
        m_error = "you cannot take date by index ";
        m_error += to_string(index);
        m_error += "\n";
        m_error += get_help_info();
    }

    string get_help_info() {
        string help_info = "You can take data by 0 or 1 index\
    \n0 - real part of complex number\
    \n1 - imagine part of complex number\n";
        return help_info;
    }

    char get_wrong_character() {
        return wrong_index;
    }
};


class ComplexNumber
{
private:
    void string_init(string num_str){
      float new_re = 0;
      float new_im = 0;
      int temp_num = 0;
      bool is_swap = 0;
      bool all_im = 0;
      int i = 0, sign_re = 1, sign_im = 1;
      // cout << "input str: " << num_str << endl;
      if (num_str[0] == '-')
      {
          sign_re = -1;
          i++;
      }
      for (; i < num_str.length(); i++)
      {
          if (num_str[i] == 'i') {
            if (!is_swap) {
              all_im = 1;
            }
            break;
          }
          if (num_str[i] == '+' || num_str[i] == '-') {
            is_swap = 1;
            if (num_str[i] == '-') {
                sign_im = -1;
            }
            continue;
          }
          if (!is_swap) {
            new_re *= 10;
            if (num_str[i] > '9' || num_str[i] < '0')
            {
              throw ComplexNumberStringConstructorException(num_str[i]);
            }
            new_re += (num_str[i] - '0');
          }
          else{
            new_im *= 10;
            if (num_str[i] > '9' || num_str[i] < '0')
            {
              throw ComplexNumberStringConstructorException(num_str[i]);
            }
            new_im += (num_str[i] - '0');
          }
      }
      if (!all_im) {
        *re = sign_re * new_re;
        *im = sign_im * new_im;
      }
      else{
        *re = 0;
        *im = sign_re * new_re;
      }
    }

public:
    float* re = new float;
    float* im = new float;


    ComplexNumber(float real = 0, float imagine = 0) {

        *re = real;
        *im = imagine;
    };
    ComplexNumber(int real) {
        *re = real;
        *im = 0;
    }

    ComplexNumber(string num_s) {
      string_init(num_s);
    };

    ComplexNumber(const char* num_s) {
        string_init(string(num_s));
    };

    ComplexNumber(const ComplexNumber& CopiedComplexNumber) {
        *re = *(CopiedComplexNumber.re);
        *im = *(CopiedComplexNumber.im);
    }

    ~ComplexNumber() {
        //cout << "delete re\t" << this->re << "\t" << *this->re << '\n';
        //cout << "delete im\t" << this->im << "\t" << *this->im << '\n';
        delete this->re;
        delete this->im;
    };

    float module() {
        float result;
        result = sqrt((*this->re) * (*this->re) + (*this->im) * (*this->im));
        return result;
    }

    ComplexNumber operator[](const int index)
    {
        if (index == 0) {
            return *re;
        }
        else if (index == 1) {
            return *im;
        }
        else {
            throw ComplexNumberWrongIndex(index);
        }

        return 0;
    }

    ComplexNumber operator + (ComplexNumber num2)
    {
        ComplexNumber result_num = ComplexNumber();

        if (((*(num2.re) > 0) && (*(this->re) > (FLT_MAX - *(num2.re)))) ||
            ((*(num2.re) < 0) && (*(this->re) < (-FLT_MAX - *(num2.re)))))
        {
            return result_num;
        }
        if (((*(num2.im) > 0) && (*(this->im) > (FLT_MAX - *(num2.im)))) ||
            ((*(num2.im) < 0) && (*(this->im) < (-FLT_MAX - *(num2.im)))))
        {
            return result_num;
        }

        *(result_num.re) = *(this->re) + *(num2.re);
        *(result_num.im) = *(this->im) + *(num2.im);

        return result_num;
    }

    ComplexNumber operator - (ComplexNumber num2)
    {
        ComplexNumber* num1 = this;
        ComplexNumber result_num = ComplexNumber();
        if ((*(num2.re) > 0 && *(num1->re) < -FLT_MAX + *(num2.re)) ||
            (*(num2.re) < 0 && *(num1->re) > FLT_MAX + *(num2.re)))
        {
            return result_num;
        }
        if ((*(num2.im) > 0 && *(num1->im) < -FLT_MAX + *(num2.im)) ||
            (*(num2.im) < 0 && *(num1->im) > FLT_MAX + *(num2.im)))
        {
            return result_num;
        }

        *(result_num.re) = *(num1->re) - *(num2.re);
        *(result_num.im) = *(num1->im) - *(num2.im);

        return result_num;
    }

    ComplexNumber operator * (ComplexNumber num2)
    {
        ComplexNumber* num1 = this;
        ComplexNumber result_num = ComplexNumber();
        *(result_num.re) = (*(num1->re) * *(num2.re)) - (*(num1->im) * *(num2.im));
        *(result_num.im) = (*(num1->re) * *(num2.im)) + (*(num1->im) * *(num2.re));

        return result_num;
    }

    ComplexNumber operator / (ComplexNumber num2)
    {
        ComplexNumber* num1 = this;
        float divider = (*(num2.re) * *(num2.re) + *(num2.im) * *(num2.im));
        ComplexNumber result_num = ComplexNumber();
        if (divider)
        {
            *(result_num.re) = ((*(num1->re) * *(num2.re)) + (*(num1->im) * *(num2.im))) / divider;
            *(result_num.im) = ((*(num2.re) * *(num1->im)) - (*(num2.im) * *(num1->re))) / divider;
        }
        else {
            throw ComplexNumberDividingByZero();
        }


        return result_num;
    }

    ComplexNumber& operator += (ComplexNumber num) {
        *this = (*this + num);
        return *this;
    };
    ComplexNumber& operator -= (ComplexNumber num) {
        *this = (*this - num);
        return *this;
    };
    ComplexNumber& operator *= (ComplexNumber num) {
        *this = (*this * num);
        return *this;
    };
    ComplexNumber& operator /= (ComplexNumber num) {
        *this = (*this / num);
        return *this;
    };


    ComplexNumber& operator= (const float& num)
    {
        *re = *&num;
        *im = 0;

        return *this;
    }

    ComplexNumber& operator= (const int& num)
    {
        *re = *&num;
        *im = 0;

        return *this;
    }

    ComplexNumber& operator= (const ComplexNumber& num)
    {
        *re = *(num.re);
        *im = *(num.im);

        return *this;
    }

    ComplexNumber& operator= (const char* num_s)
    {
        *this = ComplexNumber(num_s);

        return *this;
    }

    operator string() const {
        ostringstream ss_re, ss_im;
        ss_re << *re;
        if (!*re && !*im)
        {
            return to_string(0);
        }
        if (!*re)
        {
            ss_im << *im;
            return ss_im.str() + "i";
        }
        if (!*im)
        {
            return ss_re.str();
        }

        ss_im << abs(*im);
        return ss_re.str() + (*im > 0 ? "+" : "-") + ss_im.str() + "i";
    }

    friend ostream& operator<< (std::ostream& out, ComplexNumber num) {
        out << string(num);
        return out;
    };
};

bool operator == (ComplexNumber num1, ComplexNumber num2)
{
    return num1.module() == num2.module();
}
bool operator != (ComplexNumber num1, ComplexNumber num2)
{
    return num1.module() != num2.module();
}
bool operator > (ComplexNumber num1, ComplexNumber num2)
{
    return num1.module() > num2.module();
}
bool operator < (ComplexNumber num1, ComplexNumber num2)
{
    return num1.module() < num2.module();
}
bool operator >= (ComplexNumber num1, ComplexNumber num2)
{
    return num1.module() >= num2.module();
}
bool operator <= (ComplexNumber num1, ComplexNumber num2)
{
    return num1.module() <= num2.module();
}



class BaseExpression {
private:
  // shared_ptr<BaseExpression> shared_this;
public:
    virtual ComplexNumber count_value() = 0;
    virtual void print_expression() = 0;
    virtual void print_structure(string prefix = "") = 0;
    virtual bool find(ComplexNumber num) = 0;
    virtual int size() = 0;
    virtual pair<int, shared_ptr<BaseExpression>> get_by_id(int n) = 0;

    BaseExpression(){
      // shared_this = shared_ptr<BaseExpression>(this);
    };
};


class ValueExpression : public BaseExpression
{
private:
    ComplexNumber value;

public:
    ValueExpression(ComplexNumber new_value) :BaseExpression() {
        value = new_value;
    }

    // ValueExpression(const ValueExpression& new_exp) {
    //     value = new_value;
    //     new_exp;
    // }

    ComplexNumber count_value() override {
        // cout << "value: " << value << endl;
        return value;
    }

    void print_expression() override {
      cout << "[" << value << "]";
    }

    bool find(ComplexNumber num) override {
      return (value == num);
    }

    void print_structure(string prefix = "") override {
      prefix += "|_";
      cout << prefix << value << endl;
    }

    int size() override {
      return 1;
    }

    pair<int, shared_ptr<BaseExpression>> get_by_id(int n) override {
      // cout << n-1 << endl;
      shared_ptr<BaseExpression> res(this);
      return pair<int, shared_ptr<BaseExpression>>{n-1, res};
    }
};


class BracketExpression :public BaseExpression {
private:
  shared_ptr<BaseExpression> expression;
  char start_bracket = '(';
  char end_bracket = ')';

public:
  BracketExpression() :BaseExpression() {};
  BracketExpression(shared_ptr<BaseExpression> be) :BaseExpression() {
      expression = be;
  }
  BracketExpression(const BracketExpression* be) :BaseExpression() {
      expression = be->expression;
  }
  ~BracketExpression() {
      // expression.reset();
  }

  void print_expression() override {
    cout << start_bracket;
    expression->print_expression();
    cout << end_bracket;
  }

  ComplexNumber count_value() override {
      return expression->count_value();
  }

  bool find(ComplexNumber num) override {
    return expression->find(num);
  }

  void print_structure(string prefix = "") override {
    prefix += "|";
    cout << prefix << "_" << start_bracket << end_bracket << endl;
    prefix += " ";
    expression->print_structure(prefix);
  }

  int size() override {
    return expression->size()+1;
  }

  pair<int, shared_ptr<BaseExpression>> get_by_id(int n) override {
    // cout << "BracketExpression " << n << endl;
    if (n == 0) {
      shared_ptr<BaseExpression> res(this);
      return pair<int, shared_ptr<BaseExpression>>{n, res};
    }
    n -= 1;
    return expression->get_by_id(n);
  }
};


class BinaryExpression : public BaseExpression
{
private:
    // virtual void op_init(){
    //   op = '\0';
    // }
    virtual char get_op(){
      return op;
      // return static_cast<BinaryExpression>(this)->op;
    }
public:
    char op;
    shared_ptr<BaseExpression> left;
    shared_ptr<BaseExpression> right;


    BinaryExpression(shared_ptr<BaseExpression> new_left, shared_ptr<BaseExpression> new_right) :BaseExpression() {
        left = new_left;
        right = new_right;
    }
    BinaryExpression():BaseExpression() {
        // op_init();
    };
    BinaryExpression(const BinaryExpression* be) :BaseExpression() {
        left = be->left;
        right = be->right;
        // op_init();
    }
    ~BinaryExpression() {
        // left.reset();
        // right.reset();
    }

    // template <class T>
    // void set_op(){
    //   cout << "op: " << static_cast<T*>(this)->T::op << endl;
    //   static_cast<T*>(this)->BinaryExpression::op = static_cast<T*>(this)->T::op;
    // }

    void print_expression() override {
      left->print_expression();
      cout << get_op();
      right->print_expression();
    }

    bool find(ComplexNumber num) override {
      return (left->find(num) || right->find(num));
    }

    void print_structure(string prefix = "") override {
      prefix += "|";
      cout << prefix << "_" << get_op() << endl;
      prefix += " ";
      left->print_structure(prefix);
      right->print_structure(prefix);
    }

    int size() override {
      return left->size()+right->size()+1;
    }

    pair<int, shared_ptr<BaseExpression>> get_by_id(int n) override {
      // cout << "BinaryExpression " << n << endl;
      if (n == 0) {
        shared_ptr<BaseExpression> res(this);
        return pair<int, shared_ptr<BaseExpression>>{n, res};
      }
      n -= 1;
      auto temp_l = left->get_by_id(n);
      n = temp_l.first;
      if (n == 0) {
        return temp_l;
      }
      return right->get_by_id(n);
    }
};


class ProductExpression : public BinaryExpression
{
private:
public:
    char op = '*';
    char get_op() override{
      return op;
    }
    // void op_init() override{
    //   set_op<ProductExpression>();
    // }
    ProductExpression(
        shared_ptr<BaseExpression> new_left,
        shared_ptr<BaseExpression> new_right)
          :BinaryExpression(new_left, new_right)
    {};

    ~ProductExpression(){};

    ComplexNumber count_value() override {
        return left->count_value() * right->count_value();
    }
};


class DivisionExpression : public BinaryExpression
{
private:
public:
    char op = '/';
    char get_op() override{
      return op;
    }

    DivisionExpression(
        shared_ptr<BaseExpression> new_left,
        shared_ptr<BaseExpression> new_right) :BinaryExpression(new_left, new_right) {};

    ~DivisionExpression() {};

    ComplexNumber count_value() override {
        return left->count_value() / right->count_value();
    }
};


class SumExpression : public BinaryExpression
{
private:
public:
    char op = '+';
    char get_op() override{
      return op;
    }

    SumExpression(
        shared_ptr<BaseExpression> new_left,
        shared_ptr<BaseExpression> new_right) :BinaryExpression(new_left, new_right) {};

    ~SumExpression() {};

    ComplexNumber count_value() override {
        return left->count_value() + right->count_value();
    }
};


class DifferenceExpression : public BinaryExpression
{
private:
public:
    char op = '-';
    char get_op() override{
      return op;
    }

    DifferenceExpression(
        shared_ptr<BaseExpression> new_left,
        shared_ptr<BaseExpression> new_right) :BinaryExpression(new_left, new_right) {};

    ~DifferenceExpression() {};

    ComplexNumber count_value() override {
        return left->count_value() - right->count_value();
    }
};


class ArithmeticExpression {
private:
  shared_ptr<BaseExpression> expression;
public:
  ArithmeticExpression(){};
  ArithmeticExpression(shared_ptr<BaseExpression> new_expression){
    expression = new_expression;
  }

  ~ArithmeticExpression(){
    // expression.reset();
  }

  void set_expression(shared_ptr<BaseExpression> new_expression){
    expression = new_expression;
  }

  shared_ptr<BaseExpression> get_expression(){
    return expression;
  }

  void print_expression(){
    expression->print_expression();
    cout << endl;
  }

  ComplexNumber count_value(){
    return expression->count_value();
  }

  void print_structure(){
    expression->print_structure();
  }

  bool find(ComplexNumber num){
    return expression->find(num);
  }

  int size(){
    return expression->size();
  }

  int use_count(){
    return expression.use_count();
  }

  shared_ptr<BaseExpression> get_by_id(int n) {
    cout << "strat getting id" << endl;
    auto ans = expression->get_by_id(n);
    if (ans.first > 0) {
      cout << "un find elm" << endl;
    }
    cout << "false print(";
    ans.second->print_expression();
    cout << ")" << endl;
    // cout << "end getting id" << endl;
    return ans.second;
  }

  class Iterator{
    shared_ptr<BaseExpression> it_expression;
    int i;
    ArithmeticExpression* it_obj;
  public:

    Iterator(ArithmeticExpression* new_it_obj) :it_obj(new_it_obj)
    {}

    Iterator(shared_ptr<BaseExpression>start,
      ArithmeticExpression* new_it_obj)
    {
      it_obj = new_it_obj;
      it_expression = start;
    }

    Iterator(int n, ArithmeticExpression* new_it_obj){
      it_obj = new_it_obj;
      auto ans = new_it_obj->get_by_id(i);
      it_expression = ans;
    }

    ~Iterator(){
      // it_expression.reset();
    }

    Iterator operator+ (int n) { return Iterator(i+n, it_obj); }
    Iterator operator- (int n) { return Iterator(i-n, it_obj); }

    Iterator operator++ (int) { return Iterator(i++, it_obj); }
    Iterator operator-- (int) { return Iterator(i--, it_obj); }
    Iterator operator++ () { return Iterator(--i, it_obj); }
    Iterator operator-- () { return Iterator(++i, it_obj); }

    bool operator== (const Iterator& it) { return it.i == i; }
    bool operator!= (const Iterator& it) { return it.i != i; }

    shared_ptr<BaseExpression> operator* () { return it_expression; }
  };

  Iterator begin(){
    return Iterator(0, this);
  }

  Iterator end(){
    Iterator it(expression->size()+1, this);
    return it;
  }
};



class instream {
private:
    vector <char> stream_vect;

public:
    instream() {
        append_sub_symbol();
    };
    instream(string new_str) {
        for (auto c : new_str) {
            stream_vect.push_back(c);
        }
        append_sub_symbol();
    }
    instream(const instream& stream) {
        stream_vect = stream.stream_vect;
    }

    void putback(char c) {
        auto iter = stream_vect.cbegin();
        stream_vect.emplace(iter, c);
    }

    void append_sub_symbol() {
        stream_vect.push_back(';');
    }

    char get() {
        char c = stream_vect[0];
        auto iter = stream_vect.cbegin();
        stream_vect.erase(iter);

        return c;
    }

    instream& operator<< (string str) {
        stream_vect.pop_back();
        for (auto c : str) {
            stream_vect.push_back(c);
        }
        append_sub_symbol();
        return *this;
    }

    bool empty(){
      return stream_vect.empty();
    }

    string str() {
        string result;
        for (auto c : stream_vect) {
            result += c;
        }
        return result;
    }
};


class ExpressionConverterUndefindVariable : public exception
{
private:

public:
  string m_error;

  ExpressionConverterUndefindVariable()
  {
    m_error = "expression convertor exception";
  }
  ExpressionConverterUndefindVariable(string error)
  {
    m_error = error;
  }

  const char* what() const noexcept { return m_error.c_str(); }
};


class VariableConverterVariableException : public exception
{
private:

public:
  string m_error;

  VariableConverterVariableException()
  {
    m_error = "expression convertor exception";
  }
  VariableConverterVariableException(string error)
  {
    m_error = error;
  }

  const char* what() const noexcept { return m_error.c_str(); }
};

class VariableConverter {
private:
  instream stream;
  char stop_char = ';';

  pair<string, ComplexNumber> pars_stream(){
    string variable_name;
    while(!stream.empty()){
      char c = stream.get();
      if (c == '=') {
        break;
      }
      else if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_') {
          variable_name += c;
      }
      else if ('0' <= c && c <= '9') {
          if (!variable_name.empty())
          {
              variable_name += c;
          }
          else{
              throw VariableConverterVariableException("number can not be used as variable name");
          }
      }
      else {
          throw VariableConverterVariableException((string)"this char csn not be used as variable name char="+c);
      }
    }
    string complex_num_str;
    while(!stream.empty()){
      char c = stream.get();
      if (c == stop_char) {
        break;
      }
      complex_num_str += c;
    }

    pair<string, ComplexNumber> result{variable_name, ComplexNumber(complex_num_str)};
    return result;
  }
public:
  VariableConverter(instream new_stream) {
      stream = new_stream;
  }
  VariableConverter(string expression_str) {
      stream << expression_str;
  }
  pair<string, ComplexNumber> convert(){
    instream temp_stream = stream;
    auto result = pars_stream();
    stream = temp_stream;
    return result;
  }
};


class ExpressionConverter {
private:
    instream stream;
    map<string, ComplexNumber> variables;

    shared_ptr<BaseExpression> number() {
        string var_name;
        while (1) {
            char c = stream.get();
            if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_') {
                var_name += c;
            }
            else if ('0' <= c && c <= '9') {
                if (!var_name.empty())
                {
                    var_name += c;
                }
                else{
                    throw ExpressionConverterUndefindVariable("number can not be used as variable name");
                }
            }
            else {
                stream.putback(c);
                break;
            }
        }
        if (!variables.count(var_name))
        {
            throw ExpressionConverterUndefindVariable("undefinded variable: " + var_name);
        }
        auto result = variables[var_name];
        auto res = make_shared<ValueExpression>(ComplexNumber(result));
        return res;
    }

    shared_ptr<BaseExpression> product() {
        shared_ptr<BaseExpression> num;
        char op = stream.get();
        if (op == '(') {
            num = make_shared<BracketExpression>(this->sum());
            char end_bracket = stream.get();
        }
        else {
            stream.putback(op);
            num = number();
        }
        op = stream.get();
        shared_ptr<BaseExpression> next_result;
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

    shared_ptr<BaseExpression> sum() {
        shared_ptr<BaseExpression> num = this->product();
        char op = stream.get();
        shared_ptr<BaseExpression> next_result;
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
    ExpressionConverter(instream new_stream, map<string, ComplexNumber> new_variables) {
        stream = new_stream;
        variables = new_variables;
    }
    ExpressionConverter(string expression_str, map<string, ComplexNumber> new_variables) {
        stream << expression_str;
        variables = new_variables;
    }
    shared_ptr<BaseExpression> convert() {
        instream temp_stream = stream;
        shared_ptr<BaseExpression> result;
        result = this->sum();
        stream = temp_stream;
        return result;
    }
};


class LoaderUndefindedCommand : public exception
{
private:

public:
  string m_error;

  LoaderUndefindedCommand()
  {
    m_error = "loader cndefinded command";
  }
  LoaderUndefindedCommand(string error)
  {
    m_error = error.c_str();
  }

  const char* what() const noexcept { return m_error.c_str(); }
};


class Loader {
private:
  virtual string get_line() = 0;
  map<string, ComplexNumber> variables;
  vector<ArithmeticExpression> expressions;
  string variables_command = "variables";
  string expressions_command = "expressions";
  string end_command = "end";

  bool is_letter(char c){
    return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
  };

public:
  int pars_command(string str){
    string command;
    bool command_start = 0;
    for (auto c : str) {
      if (!command_start) {
        if (is_letter(c)) {
          command_start = 1;
        }
        else{
          continue;
        }
      }
      if (is_letter(c)) {
        command += c;
      }
      else{
        break;
        // throw LoaderUndefindedCommand("character nust be a letter char="+c+"\nrise in string="+str);
      }

    }
    if (command.empty()) {
      return -1;
    }
    if (command == end_command) {
      return 0;
    }
    if (command == variables_command) {
      return 1;
    }
    if (command == expressions_command) {
      return 2;
    }

    // throw LoaderUndefindedCommand("unknown command: " + command + "\nrise in string=" + str);
    return -1;
  }

  void pars_variables(string str){
    VariableConverter var_conv(str);
    auto var = var_conv.convert();
    // cout << var.first << endl;
    variables[var.first] = var.second;
  }

  void pars_expressions(string str){
    ExpressionConverter exp_conv(str, variables);
    auto exp = exp_conv.convert();
    expressions.push_back(exp);
  }

  void pars_stream(){
    bool is_command_run = 0;
    int command_status = -1;
    while(1){
      string line = get_line();
      // cout << line << " " << command_status << endl;
      if (!is_command_run) {
        command_status = pars_command(line);
        // cout << "new " << line << " " << command_status << endl;
        if (command_status != -1) {
          if (command_status == 0) {
            // cout << "program finished" << endl;
            break;
          }
          is_command_run = 1;
        }
      }
      else{
        if (pars_command(line) == 0) {
          is_command_run = 0;
          continue;
        }
        if (command_status == 1){
          // cout << "command pars_variables" << endl;
          pars_variables(line);
        }
        else if (command_status == 2){
          // cout << "command pars_expressions" << endl;
          pars_expressions(line);
        }
      }
    }
  }
  map<string, ComplexNumber> get_variables(){
    return variables;
  }
  vector<ArithmeticExpression> get_expressions(){
    return expressions;
  }
};


class ConsoleLoader:public Loader {
private:
  string get_line() override{
    string temp;
    getline(cin, temp);
    return temp;
  }
public:
};


class FileLoaderException : public exception
{
private:

public:
  string m_error;

  FileLoaderException()
  {
    m_error = "file loader exception";
  }
  FileLoaderException(string error)
  {
    m_error = error;
  }

  const char* what() const noexcept { return m_error.c_str(); }
};

class FileLoader:public Loader {
private:
  ifstream file;
  string get_line() override{
    string temp;
    getline(file, temp);
    return temp;
  }
public:
  FileLoader(string file_path) {
    file.open(file_path);
    if (!file.is_open()) {
      throw FileLoaderException("could not open file whith path="+file_path);
    }
  }
  // FileLoader(ifstream new_file) {
  //   file = new_file;
  // }

  ~FileLoader(){
    file.close();
  }
};



int main() {
    string path = "lab5_tests.txt";
    // FileLoader *loader = new FileLoader(path);
    ConsoleLoader *loader = new ConsoleLoader();
    loader->pars_stream();
    auto variables = loader->get_variables();
    auto expressions = loader->get_expressions();
    delete loader;
    // cout << "use_count(): " << expressions[0].use_count() << endl;

    for (auto exp : expressions) {
      exp.print_expression();
      cout << "size: " << exp.size() << endl;
      cout << "use_count(): " << exp.use_count() << endl;
      exp.print_structure();
      cout << exp.count_value() << endl;
    }

    auto first_exp = expressions[0];
    cout << "it test" << endl;
    first_exp.print_expression();
    cout << "use_count(): " << first_exp.use_count() << endl;

    // auto sub_exp = first_exp.get_by_id(2);
    auto i = first_exp.begin();
    first_exp.print_expression();
    // cout << "strat" << endl;
    // cout << (i != first_exp.end()) << endl;
    // for (; i != first_exp.end(); i++) {
    //   first_exp.print_expression();
    //   (*i)->print_expression();
    // }
    // auto f_it = first_exp.begin();
    // (*f_it)->print_expression();

}

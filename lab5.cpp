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
    void string_init(string num_str) {
        float new_re = 0;
        float new_im = 0;
        int temp_num = 0;
        bool is_swap = 0;
        bool all_im = 0;
        bool is_fill_re = 0;
        bool is_fill_im = 0;
        int i = 0, sign_re = 1, sign_im = 1;
        if (num_str[0] == '-')
        {
            sign_re = -1;
            i++;
        }
        for (; i < num_str.length(); i++)
        {
            if (num_str[i] == 'i') {
                if (!is_swap) {
                    if (!is_fill_re)
                    {
                        new_re = 1;
                    }
                    all_im = 1;
                }
                if (!is_fill_im)
                {
                    new_im = 1;
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
                is_fill_re = 1;
            }
            else {
                new_im *= 10;
                if (num_str[i] > '9' || num_str[i] < '0')
                {
                    throw ComplexNumberStringConstructorException(num_str[i]);
                }
                new_im += (num_str[i] - '0');
                is_fill_im = 1;
            }
        }
        if (!all_im) {
            *re = sign_re * new_re;
            *im = sign_im * new_im;
        }
        else {
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
        return ss_re.str() + (*im > 0 ? "+" : "-") + ((*im != 1 && *im != -1) ? ss_im.str()+"i" : "i");
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
    virtual void print_structure(int depth = 0, bool is_right = false, bool* rec = new bool[10000]) = 0;
    virtual bool find(ComplexNumber num) = 0;
    virtual int size() = 0;
    virtual pair<int, BaseExpression*> get_by_id(int n) = 0;

    BaseExpression() {
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

    void print_structure(int depth = 0, bool is_right = false, bool* rec = new bool[10000]) override {
        for (int i = 0; i < depth; i++) {
            if (i == depth - 1 && is_right)
            {
                cout << " |";
            }
            else {
                if (rec[i])
                {
                    cout << " |";
                }
                else {
                    cout << "  ";
                }
            }
        }

        cout << "_" << value << " id: " << this << endl;
    }

    int size() override {
        return 1;
    }

    pair<int, BaseExpression*> get_by_id(int n) override {
        // cout << n-1 << endl;
        BaseExpression* res(this);
        if (n == 0)
        {
            return pair<int, BaseExpression*>{-1, res};
        }
        return pair<int, BaseExpression*>{n - 1, res};
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

    void print_structure(int depth = 0, bool is_right = false, bool* rec = new bool[10000]) override {
        for (int i = 0; i < depth; i++) {
            if (i == depth - 1 && is_right)
            {
                cout << " |";
            }
            else {
                if (rec[i])
                {
                    cout << " |";
                }
                else {
                    cout << "  ";
                }
            }
        }

        cout << "_" << start_bracket << end_bracket << endl;
        rec[depth] = 0;
        expression->print_structure(depth + 1, 1, rec);
    }

    int size() override {
        return expression->size() + 1;
    }

    pair<int, BaseExpression*> get_by_id(int n) override {
        // cout << "BracketExpression " << n << endl;
        if (n == 0) {
            BaseExpression* res(this);
            return pair<int, BaseExpression*>{-1, res};
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
    virtual char get_op() {
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
    BinaryExpression() :BaseExpression() {
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

    void print_structure(int depth = 0, bool is_right = false, bool* rec = new bool[10000]) override {
        for (int i = 0; i < depth; i++) {
            if (i == depth-1 && is_right)
            {
                cout << " |";
            }
            else {
                if (rec[i])
                {
                    cout << " |";
                }
                else {
                    cout << "  ";
                }
            }
        }

        cout << "_" << get_op() << endl;
        rec[depth] = 1;
        left->print_structure(depth + 1, 0, rec);
        rec[depth] = 0;
        right->print_structure(depth + 1, 1, rec);
    }

    int size() override {
        return left->size() + right->size() + 1;
    }

    pair<int, BaseExpression*> get_by_id(int n) override {
        // cout << "BinaryExpression " << n << endl;
        if (n == 0) {
            BaseExpression* res(this);
            return pair<int, BaseExpression*>{-1, res};
        }
        n -= 1;
        // cout << "binary st: " << n << endl;
        auto temp_l = left->get_by_id(n);
        n = temp_l.first;
        if (n == -1) {
            return temp_l;
        }
        // cout << "binary l: " << n << endl;
        // cout << "binary r: " << n << endl;
        return right->get_by_id(n);
    }
};


class ProductExpression : public BinaryExpression
{
private:
public:
    char op = '*';
    char get_op() override {
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

    ~ProductExpression() {};

    ComplexNumber count_value() override {
        return left->count_value() * right->count_value();
    }
};


class DivisionExpression : public BinaryExpression
{
private:
public:
    char op = '/';
    char get_op() override {
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
    char get_op() override {
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
    char get_op() override {
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
    ArithmeticExpression() {};
    ArithmeticExpression(shared_ptr<BaseExpression> new_expression) {
        expression = new_expression;
    }

    ~ArithmeticExpression() {
        // expression.reset();
    }

    void set_expression(shared_ptr<BaseExpression> new_expression) {
        expression = new_expression;
    }

    shared_ptr<BaseExpression> get_expression() {
        return expression;
    }

    void print_expression() {
        expression->print_expression();
        cout << endl;
    }

    ComplexNumber count_value() {
        return expression->count_value();
    }

    void print_structure() {
        expression->print_structure();
    }

    bool find(ComplexNumber num) {
        return expression->find(num);
    }

    int size() {
        return expression->size();
    }

    int use_count() {
        return expression.use_count();
    }

    BaseExpression* get_by_id(int n) {
        // cout << "strat getting id" << endl;
        if (n < 0 || n > expression->size())
        {
            cout << "index out of rangne" << endl;
        }
        auto ans = expression->get_by_id(n);
        return ans.second;
    }

    BaseExpression* operator[] (int n) {
        return this->get_by_id(n);
    }

    class Iterator {
        int i = 0;
        ArithmeticExpression* it_obj;
    public:

        Iterator(ArithmeticExpression* new_it_obj) :it_obj(new_it_obj)
        {}

        Iterator(int n, ArithmeticExpression* new_it_obj) {
            it_obj = new_it_obj;
            i = n;
        }

        ~Iterator() {};

        Iterator operator+ (int n) { return Iterator(i + n, it_obj); }
        Iterator operator- (int n) { return Iterator(i - n, it_obj); }

        Iterator operator++ (int) { return Iterator(i++, it_obj); }
        Iterator operator-- (int) { return Iterator(i--, it_obj); }
        Iterator operator++ () { return Iterator(--i, it_obj); }
        Iterator operator-- () { return Iterator(++i, it_obj); }

        bool operator== (const Iterator& it) { return it.i == i; }
        bool operator!= (const Iterator& it) { return it.i != i; }

        BaseExpression* operator* () { return it_obj->get_by_id(i); }

        int get_id(){
          return i;
        }
    };

    Iterator begin() {
        return Iterator(0, this);
    }

    Iterator end() {
        int exp_size = expression->size();
        Iterator it(exp_size, this);
        return it;
    }
};


template< typename T >
class ICompare {
public:
    virtual bool compare(T obj1, T obj2) = 0;
};


class ICompareArithmeticExp : public ICompare<ArithmeticExpression>
{ };


class ICompareArithmeticExpB : public ICompareArithmeticExp
{
public:
    bool compare(ArithmeticExpression obj1, ArithmeticExpression obj2) override {
        return obj1.count_value() > obj2.count_value();
    }
};


class ICompareArithmeticExpL : public ICompareArithmeticExp
{
public:
    bool compare(ArithmeticExpression obj1, ArithmeticExpression obj2) override {
        return obj1.count_value() < obj2.count_value();
    }
};


class ICompareArithmeticExpNE : public ICompareArithmeticExp
{
public:
    bool compare(ArithmeticExpression obj1, ArithmeticExpression obj2) override {
        return obj1.count_value() != obj2.count_value();
    }
};


class ICompareArithmeticExpE : public ICompareArithmeticExp
{
public:
    bool compare(ArithmeticExpression obj1, ArithmeticExpression obj2) override {
        return obj1.count_value() == obj2.count_value();
    }
};


class ICompareArithmeticExpBE : public ICompareArithmeticExp
{
public:
    bool compare(ArithmeticExpression obj1, ArithmeticExpression obj2) override {
        return obj1.count_value() >= obj2.count_value();
    }
};


class ICompareArithmeticExpLE : public ICompareArithmeticExp
{
public:
    bool compare(ArithmeticExpression obj1, ArithmeticExpression obj2) override {
        return obj1.count_value() <= obj2.count_value();
    }
};


template< typename T >
class VectorAnalog {
private:
    struct Node {
        Node() : next(NULL), last(NULL)
        { }

        Node(const T& t) : value(t), next(NULL), last(NULL)
        { }

        T value;

        Node* next;
        Node* last;
    };

    Node* head;

public:
    class Iterator {
    public:
        Iterator(Node* node) : value_node(node)
        { };
        Iterator(const Iterator& it)
        {
            value_node = it.value_node;
        };

        bool operator==(const Iterator& iter) const {
            if (this == &iter) {
                return true;
            }
            return value_node == iter.value_node;
        }

        bool operator!=(const Iterator& iter) const {
            return !operator==(iter);
        }

        T operator*() const {
            if (value_node) {
                return value_node->value;
            }
            return T();
        }

        Iterator operator++ () {
            if (value_node) {
                value_node = value_node->next;
                return *this;
            }
        }
        Iterator operator-- () {
            if (value_node) {
                value_node = value_node->last;
                return *this;
            }
        }
        Iterator operator++ (int) {
            if (value_node) {
                Iterator prev = *this;
                ++(*this);
                return prev;
            }
        }
        Iterator operator-- (int) {
            if (value_node) {
                Iterator prev = *this;
                --(*this);
                return prev;
            }
        }

        Iterator operator+ (int n) {
            Iterator iter(this->value_node);
            for (int i = 0; i < n; i++)
            {
                iter++;
            }
            return iter;
        }

        Iterator operator- (int n) {
            Iterator iter(this->value_node);
            for (int i = 0; i < n; i++)
            {
                iter--;
            }
            return iter;
        }

        Iterator operator-= (int n) {
            for (int i = 0; i < n; i++)
            {
                if (value_node)
                {
                    value_node = value_node->last;
                }
                else {
                    cout << "index out of range n: " << n << endl;
                }
            }

            return *this;
        }

        Iterator operator+= (int n) {
            for (int i = 0; i < n; i++)
            {
                if (value_node)
                {
                    value_node = value_node->next;
                }
                else {
                    cout << "index out of range n: " << n << endl;
                }
            }

            return *this;
        }

        Node* get_node() {
            return value_node;
        }

    private:
      Node* value_node;
    };


    VectorAnalog() :head(NULL) {};

    ~VectorAnalog() {
        while (head) {
            pop();
        }
    };

    void append(const T& new_node) {
        Node* node = new Node(new_node);
        if (head)
        {
            head->last = node;
            node->next = head;
        }
        else
        {
            node->next = head;
        }


        head = node;
    }

    void pop(Iterator iter) {
        auto del_item = iter.get_node();
        if (del_item)
        {
            if (iter == begin())
            {
                head = del_item->next;
            }
            else if (iter == end()) {

            }
            else {

            }
            del_item->last->next = del_item->next;
            del_item->next->last = del_item->last;
            delete del_item;
        }
    }

    void pop(int n = -1) {
        Iterator iter = begin();
        Node* del_item;
        if (n == -1)
        {
            iter = end();
        }
        else {
            iter += n;
        }
        del_item = iter.get_node();
        if (del_item)
        {
            if (iter == begin())
            {
                head = del_item->next;
            }
            else if (iter == end()) {

            }
            else {

            }
            del_item->last->next = del_item->next;
            del_item->next->last = del_item->last;
            delete del_item;
        }

        for (; iter != end(); iter++)
        {

        }
        if (head) {
            Node* new_head = head->next;

            delete head;

            head = new_head;
        }
    };

    T get_head() {
        return head;
    }

    T get_tail() {
        return *(last());
    }

    Iterator begin() {
        return Iterator(head);
    }

    Iterator end() {
        return Iterator(NULL);
    }

    Iterator last() {
        Iterator result = begin();
        Iterator temp = begin();
        for (Iterator i = begin(); i != end(); i++)
        {
            temp = i;
            result = temp;
        }

        return result;
    }

    int size() {
        int s = 0;
        for (Iterator it = begin(); it != end(); it++) {
            s++;
        }

        return s;
    }

    bool empty() {
        return head == NULL;
    }

    void swap_nodes(Iterator it1, Iterator it2) {
      auto n1 = it1.get_node();
      auto n2 = it2.get_node();
      if (head == n1){
        head = n2;
      }
      else if (head == n2) {
        head = n1;
      }
      if (n1->next == n2) {
        if (n1->last == NULL) {
          n1->next = n2->next;
          n1->last = n2;
          n2->next = n1;
          n2->last = NULL;
          if (n1->next != NULL) {
            n1->next->last = n1;
          }
        }
        else {
          auto temp_n1_last = n1->last;
          n1->next = n2->next;
          n1->last = n2;
          n2->next = n1;
          n2->last = temp_n1_last;
          if (n1->next != NULL) {
            n1->next->last = n1;
          }
          n2->last->next = n2;
        }
      }
      else if(n1->last == n2){
        if (n2->last == NULL) {
          n2->next = n1->next;
          n2->last = n1;
          n1->next = n2;
          n1->last = NULL;
          if (n2->next != NULL) {
            n2->next->last = n2;
          }
        }
        else {
          auto temp_n2_last = n2->last;
          n2->next = n1->next;
          n2->last = n1;
          n1->next = n2;
          n1->last = temp_n2_last;
          if (n2->next != NULL) {
            n2->next->last = n2;
          }
          n1->last->next = n1;
        }
      }
      else{
        swap(n1->next, n2->next);
        swap(n1->last, n2->last);
        if (n1->next != NULL) {
          n1->next->last = n1;
        }
        if (n1->last != NULL) {
          n1->last->next = n1;
        }
        if (n2->next != NULL) {
          n2->next->last = n2;
        }
        if (n2->last != NULL) {
          n2->last->next = n2;
        }
      }

    }

    void sort(shared_ptr<ICompare<T>> compare){
      int n = size();
      for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
          auto iter_i = begin()+i;
          auto iter_j = begin()+j;
          if (compare->compare(*iter_i, *iter_j)) {
            swap_nodes(iter_i, iter_j);
          };
        }
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

    bool empty() {
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

    pair<string, ComplexNumber> pars_stream() {
        string variable_name;
        while (!stream.empty()) {
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
                else {
                    throw VariableConverterVariableException("number can not be used as variable name");
                }
            }
            else {
                throw VariableConverterVariableException((string)"this char csn not be used as variable name char=" + c);
            }
        }
        string complex_num_str;
        while (!stream.empty()) {
            char c = stream.get();
            if (c == stop_char) {
                break;
            }
            complex_num_str += c;
        }
        auto num = ComplexNumber(complex_num_str);
        pair<string, ComplexNumber> result{ variable_name, num };
        return result;
    }
public:
    VariableConverter(instream new_stream) {
        stream = new_stream;
    }
    VariableConverter(string expression_str) {
        stream << expression_str;
    }
    ~VariableConverter(){

    }
    pair<string, ComplexNumber> convert() {
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
        bool start_create_new_number = 0;
        char num_start_bracket = '[';
        char num_end_bracket = ']';
        string new_num;
        while (1) {
            char c = stream.get();
            if (!start_create_new_number)
            {
                if (var_name.empty() && c == num_start_bracket)
                {
                    start_create_new_number = 1;
                    continue;
                }
            }
            else {
                if (c == num_end_bracket)
                {
                    break;
                }
                new_num += c;
                continue;
            }
            if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_') {
                var_name += c;
            }
            else if ('0' <= c && c <= '9') {
                if (!var_name.empty())
                {
                    var_name += c;
                }
                else {
                    throw ExpressionConverterUndefindVariable("number can not be used as variable name");
                }
            }
            else {
                stream.putback(c);
                break;
            }
        }
        if (!start_create_new_number)
        {
            if (!variables.count(var_name))
            {
                throw ExpressionConverterUndefindVariable("undefinded variable: " + var_name);
            }
            auto result = variables[var_name];
            // cout << "exp var_name: " << var_name << " num: " << result
            return shared_ptr<ValueExpression>(new ValueExpression(ComplexNumber(result)));
        }
        return shared_ptr<ValueExpression>(new ValueExpression(ComplexNumber(new_num)));
    }

    shared_ptr<BaseExpression> product() {
        shared_ptr<BaseExpression> num;
        char op = stream.get();
        if (op == '(') {
            num = shared_ptr<BaseExpression>(new BracketExpression(this->sum()));
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
            return shared_ptr<ProductExpression>(new ProductExpression(num, next_result));
            break;
        case '/':
            next_result = this->product();
            return shared_ptr<DivisionExpression>(new DivisionExpression(num, next_result));
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
            return shared_ptr<SumExpression>(new SumExpression(num, next_result));
            break;
        case '-':
            next_result = this->sum();
            return shared_ptr<DifferenceExpression>(new DifferenceExpression(num, next_result));
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
    map<string, ArithmeticExpression> expression_variables;
    vector<ArithmeticExpression> expressions;
    string variables_command = "variables";
    string expressions_command = "expressions";
    string end_command = "end";

    bool is_letter(char c) {
        return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
    };

public:
    int pars_commands(string str) {
        string command;
        bool command_start = 0;
        for (auto c : str) {
            if (!command_start) {
                if (is_letter(c)) {
                    command_start = 1;
                }
                else {
                    continue;
                }
            }
            if (is_letter(c)) {
                command += c;
            }
            else {
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

    void pars_variables(string str) {
        VariableConverter var_conv(str);
        auto var = var_conv.convert();
        variables[var.first] = var.second;
    }

    void pars_expressions(string str) {
      auto temp_stream = instream(str);
      bool is_variable_expression = 0;
      while (!temp_stream.empty()){
        char c = temp_stream.get();
        if (c == '=') {
            is_variable_expression = 1;
            break;
        }
      }
      if (is_variable_expression) {
        temp_stream = instream(str);
        string variable_name;
        while (!temp_stream.empty()) {
            char c = temp_stream.get();
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
                else {
                    throw VariableConverterVariableException("number can not be used as variable name");
                }
            }
            else {
                throw VariableConverterVariableException((string)"this char csn not be used as variable name char=" + c);
            }
          }
          ExpressionConverter exp_conv(temp_stream, variables);
          auto exp = exp_conv.convert();
          expression_variables[variable_name] = exp;
          variables[variable_name] = exp->count_value();
          expressions.push_back(exp);
      }
      else{
        ExpressionConverter exp_conv(str, variables);
        auto exp = exp_conv.convert();
        expressions.push_back(exp);
      }


    }

    void pars_stream() {
        bool is_command_run = 0;
        int command_status = -1;
        while (1) {
            string line = get_line();
            // cout << line << " " << command_status << endl;
            if (!is_command_run) {
                command_status = pars_commands(line);
                // cout << "new " << line << " " << command_status << endl;
                if (command_status != -1) {
                    if (command_status == 0) {
                        // cout << "program finished" << endl;
                        break;
                    }
                    is_command_run = 1;
                }
            }
            else {
                if (pars_commands(line) == 0) {
                    is_command_run = 0;
                    continue;
                }
                if (command_status == 1) {
                    // cout << "command pars_variables" << endl;
                    pars_variables(line);
                }
                else if (command_status == 2) {
                    // cout << "command pars_expressions" << endl;
                    pars_expressions(line);
                }
            }
        }
    }
    void set_variables(map<string, ComplexNumber> new_vars) {
        variables = new_vars;
    }
    void set_expression_variables(map<string, ArithmeticExpression> new_exp_vars) {
        expression_variables = new_exp_vars;
    }
    void set_expressions(vector<ArithmeticExpression> new_exp) {
        expressions = new_exp;
    }
    map<string, ComplexNumber> get_variables() {
        return variables;
    }
    map<string, ArithmeticExpression> get_expression_variables() {
        return expression_variables;
    }
    vector<ArithmeticExpression> get_expressions() {
        return expressions;
    }
};


class ConsoleLoader :public Loader {
private:
    string get_line() override {
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


class FileLoader :public Loader {
private:
    ifstream file;
    string get_line() override {
        string temp;
        getline(file, temp);
        return temp;
    }
public:
    FileLoader(string file_path) {
        file.open(file_path);
        if (!file.is_open()) {
            throw FileLoaderException("could not open file whith path=" + file_path);
        }
    }
    // FileLoader(ifstream new_file) {
    //   file = new_file;
    // }

    ~FileLoader() {
        file.close();
    }
};



int main() {
    string path = "lab5_tests.txt";
    FileLoader* loader1 = new FileLoader(path);
    ConsoleLoader* loader2 = new ConsoleLoader();
    loader1->pars_stream();
    auto l1_variables = loader1->get_variables();
    auto l1_expression_variables = loader1->get_expression_variables();
    auto l1_expressions = loader1->get_expressions();
    delete loader1;

    loader2->set_variables(l1_variables);
    loader2->set_expressions(l1_expressions);
    loader2->pars_stream();
    auto l2_variables = loader2->get_variables();
    auto l2_expression_variables = loader2->get_expression_variables();
    auto l2_expressions = loader2->get_expressions();

    VectorAnalog<ArithmeticExpression> my_vect;
    for (auto exp : l2_expressions) {
        exp.print_expression();
        cout << "size: " << exp.size() << endl;
        exp.print_structure();
        cout << "value: " << exp.count_value() << endl;
        my_vect.append(exp);
    }

    // auto iter = my_vect.begin();
    // for (; iter != my_vect.end(); iter++)
    // {
    //     (*iter).print_expression();
    // }

    shared_ptr<ICompareArithmeticExp> simple_compare;

    simple_compare = shared_ptr<ICompareArithmeticExp>(new ICompareArithmeticExpB());
    my_vect.sort(simple_compare);
    cout << "-----------------" << endl;
    cout << "sort <-: " << endl;
    for (auto item : my_vect)
    {
        cout << item.count_value() << " ";
        item.print_expression();
    }
    cout << "-----------------" << endl;

    cout << "-----------------" << endl;
    simple_compare = shared_ptr<ICompareArithmeticExp>(new ICompareArithmeticExpL());
    my_vect.sort(simple_compare);
    cout << "sort ->: " << endl;
    for (auto item : my_vect)
    {
        cout << item.count_value() << " ";
        item.print_expression();
    }
    cout << "-----------------" << endl;


    auto first_exp = l2_expressions[0];
    cout << "iterated expression: ";
    first_exp.print_expression();  cout << endl;
    for (auto i = first_exp.begin(); i != first_exp.end(); i++) {
      cout << "inner value: "  << i.get_id() << " " << (*i)->count_value() << endl;
      (*i)->print_expression(); cout << endl;
      (*i)->print_structure();
    }
    // for (auto inner_expression : first_exp) {
    //   cout << "inner value: " << (*i)->count_value() << endl;
    //   cout << (*i)->print_structure() << endl;
    // }
}

/*
base test:
expressions
a=a+b
a=a+[2]*[2]
end
end



test1:
variables
a=1
b=1-i
c=i
end

expressions
a*b+c
((a+b))+c
a+b+c+[1+i]
(a+b)*c
end

end



test2:
variables
a=123
b=321
end
expressions
a=a+b
a=a+[2]
end
end



test3:
variables
a=1;
end

variables
b=1+1i;
c=1i;
end

expressions
(a+b)*c;
a+b+c
end

expressions
a*(b*c)
end

variables
a=0;
end

expressions
a;
end

end

*/

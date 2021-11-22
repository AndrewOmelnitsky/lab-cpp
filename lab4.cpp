#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <exception>
#include <float.h>

using namespace std;

class ComplexNumberException: public exception
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

class ComplexNumberStringConstructorException: public ComplexNumberException
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

  char get_wrong_character(){
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

  string get_help_info(){
    string help_info = "You can take data by 0 or 1 index\
    \n0 - real part of complex number\
    \n1 - imagine part of complex number\n";
    return help_info;
  }

  char get_wrong_character(){
    return wrong_index;
  }
};


class ComplexNumber
{
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
	};
	ComplexNumber(const char* num_s) {
		*re = 0;
		*im = 0;
		int i = 0, sign = 1;
		if (num_s[0] == '-')
		{
			sign = -1;
			i = 1;
		}
		for (; num_s[i] != '\0'; i++)
		{
			*re *= 10;
			if (num_s[i] > '9' || num_s[i] < '0')
			{
        throw ComplexNumberStringConstructorException(num_s[i]);
				//*re = 0;
				//break;
			}
			*re += (num_s[i] - '0');
		}
		*re = sign * *re;
	};

  ComplexNumber(const ComplexNumber &CopiedComplexNumber) {
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
    else if (index == 1){
      return *im;
    }
    else{
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
private:

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




int main()
{
	ComplexNumber num1 = ComplexNumber();
	ComplexNumber num2 = ComplexNumber(1);
	ComplexNumber num3 = ComplexNumber(0, 1);
	ComplexNumber num4 = ComplexNumber(1, 1);

	cout << num1 << endl;
	cout << num2 << endl;
	cout << num3 << endl;
	cout << num4 << endl;

	cout << (num3 + num4) << endl;
	cout << (num2 - num4 + "-222") << endl;
	cout << (ComplexNumber)"222" << endl;
	cout << ComplexNumber(2, 3) * ComplexNumber(-1, 1) << endl;
	cout << ComplexNumber(-2, 1) / ComplexNumber(1, -1) << endl;

	ComplexNumber num = ComplexNumber(2, 3);
	num *= ComplexNumber(-1, 1);
	cout << endl << num << endl;
	num = ComplexNumber(-2, 1);
	num /= ComplexNumber(1, -1);
	cout << num << endl;

  std::cout << "<-------------------------errors test------------------------->" << '\n';
  std::cout << num[0] << '\n';
  std::cout << num[1] << '\n';

  try {
    std::cout << num[2] << '\n';
  }
  catch (ComplexNumberWrongIndex &exception)
  {
    std::cerr << exception.what();
  }
  catch (ComplexNumberException &exception)
  {
    std::cerr << exception.what();
  }

  try {
    cout << ComplexNumber(-2, 1) / ComplexNumber(0, 0) << endl;
  }
  catch (ComplexNumberDividingByZero &exception)
  {
    std::cerr << exception.what();
  }
  catch (ComplexNumberException &exception)
  {
    std::cerr << exception.what();
  }


  try {
    cout << (ComplexNumber)"+222" << endl;
  }
  catch (ComplexNumberStringConstructorException &exception)
  {
    std::cerr << exception.what();
  }
  catch (ComplexNumberException &exception)
  {
    std::cerr << exception.what();
  }

	return 0;
}

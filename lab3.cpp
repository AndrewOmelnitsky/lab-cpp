#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

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
				*re = 0;
				break;
			}
			*re += (num_s[i] - '0');
		}
		*re = sign * *re;
	};
	~ComplexNumber() {
		//delete this->re;
		//delete this->im;
	};

	float module() {
		float result;
		result = sqrt((*this->re) * (*this->re) + (*this->im) * (*this->im));
		return result;
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
	cout << ComplexNumber(-2, 1) / 0 << endl;

	ComplexNumber num = ComplexNumber(2, 3);
	num *= ComplexNumber(-1, 1);
	cout << endl << num << endl;
	num = ComplexNumber(-2, 1);
	num /= ComplexNumber(1, -1);
	cout << num << endl;

	return 0;
}

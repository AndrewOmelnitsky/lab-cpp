#include <iostream>
#include <string>
#include <vector>


using namespace std;

template<class T>
class Counter
{
private:
	static int count;
public:
	Counter() {
		count++;
	}
	Counter(const Counter& c) {
		count++;
	}
	~Counter() {
		count--;
	}
	static int getCount() {
		return count;
	}
};

template<class T>
int Counter<T>::count = 0;



class Hemisphere
{
	string name;
	int id;

public:
	Hemisphere() {
		name = "";
		id = 0;
	};
	Hemisphere(string new_name, int new_id) {
		name = new_name;
		id = new_id;
	};

	string getName() {
		return name;
	};

	string getName() const {
		return name;
	};

	int getId() {
		return id;
	};

	operator string() const {
		return getName();
	}

private:

};

class Country : private Counter<Country>
{
	string name, language;
	float area;
	int population;

public:
	Country() {
		name = "";
		language = "";
		area = 0;
		population = 0;
	};
	Country(string name, float area = 0, string language = "", int population = 0) {
		this->name = name;
		this->population = population;
		this->area = area;
		this->language = language;
	};
	~Country() {
		cout << "Country " << name << " removed" << endl;
	};

	using Counter<Country>::getCount;

	
	void setPopulation(float population) {
		this->population = population;
	}
	float getPopulation() {
		return population;
	}
	void setArea(float area) {
		this->area = area;
	}
	float getArea() {
		return area;
	}

	void setName(string name) {
		this->name = name;
	}
	string getName() {
		return name;
	}

	void setLanguage(string language) {
		this->language = language;
	}
	string getLanguage() {
		return language;
	}

	bool operator== (Country other) {
		return name == other.getName();
	}

private:

};

class Continent : private Counter<Continent>
{
	string name;
	float area;
	vector <Country> countrys;
	Hemisphere hemisphere;

public:
	Continent() {
		name = "";
		area = 0;
		Hemisphere default_hemisphere;
		hemisphere = default_hemisphere;
	};
	Continent(string name, float area = 0, Hemisphere hemisphere = *new Hemisphere, vector<Country> countrys = *new vector<Country>) {
		this->name = name;
		this->area = area;
		this->hemisphere = hemisphere;
		this->countrys = countrys;
	};
	Continent(string name, Country country, float area = 0, Hemisphere hemisphere = *new Hemisphere) {
		this->name = name;
		this->area = area;
		this->hemisphere = hemisphere;
		this->countrys.push_back(country);
	};
	~Continent() {
		cout << "Continent " << name << " removed" << endl;
		cout << "All countries linked to continent removed" << endl;
		//delete[] &countrys;
	}

	using Counter<Continent>::getCount;

	void setArea(float new_area) {
		area = new_area;
	}
	float getArea() {
		return area;
	}

	void setName(string new_name) {
		name = new_name;
	}
	string getName() {
		return name;
	}

	string getHemisphere() {
		return hemisphere;
	}

	void addCountry(Country *new_country) {
		countrys.push_back(*new_country);
	}
	void removeCountryByName(string name) {
		for (size_t i = 0; i < countrys.size(); i++)
		{
			if ((&countrys[i])->getName() == name)
			{
				//cout << "erase counry: " << countrys[i].getName() << endl;
				countrys.erase(countrys.begin() + i);
				break;
			}
		}
	}
	vector<Country> getCountrysList() {
		return countrys;
	}
	Country getCountryByName(string name) {
		for (size_t i = 0; i < countrys.size(); i++)
		{
			if (countrys[i] == name)
			{
				return countrys[i];
			}
		}
		cout << "country not found" << endl;
		return *(new Country);
	}

	bool doseCountryInCountrysByName(string name) {
		for (size_t i = 0; i < countrys.size(); i++)
		{
			if (countrys[i] == name)
			{
				return true;
			}
		}
		return false;
	}

	void printCountrys() {
		cout << "Continent " << this->getName() << endl;
		cout << "Countrys:" << endl;
		for (size_t i = 0; i < countrys.size(); i++)
		{
			cout << "	" << countrys[i].getName() << endl;
		}
	}
private:

};




int main()
{
	Country country1, country2, country3("country3");
	Country country4("country4", 200.5);

	country1.setName("country1");
	country2.setName("country2");


	Continent *continent1 = new Continent;
	Continent continent2("continent2");
	Continent continent3("continent3", country4);

	continent1->setName("continent1");
	continent1->addCountry(&country1);
	continent1->addCountry(&country2);
	continent2.addCountry(&country3);
	
	cout << "Country counter: " << Counter<Country>::getCount() << endl;
	cout << "Country continent: " << Counter<Continent>::getCount() << endl;


	continent1->printCountrys();
	
	cout << "some info: " << continent1->getCountryByName("country1").getName() << endl;
	continent1->removeCountryByName("country1");

	if (continent1->doseCountryInCountrysByName("country1"))
	{
		cout << "some info: " << continent1->getCountryByName("country1").getName() << endl;
	}
	//cout << continent1.getCountryByName("country1").getName() << endl;

	continent1->printCountrys();


	delete continent1;

    cout << "Country counter: " << Counter<Country>::getCount() << endl;
	cout << "Country continent: " << Counter<Continent>::getCount() << endl;
	cin.get();
	return 0;
}

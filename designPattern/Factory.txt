#include <iostream>
#include <string>
using namespace std;

class AbstractFactory {
public:
	virtual const string type() = 0;
};
class FactoryCat:public  AbstractFactory {
public:
	FactoryCat() :m_name("cat"){};
	const string type() override {
		cout << "FactoryCat" << m_name << endl;
		return m_name;
	}
private:
	string m_name;
};
class FactoryDog:public AbstractFactory {
public:
	FactoryDog( ) :m_name("Dog") {};
	const string type() {
		cout << "FactoryDog" << m_name << endl;
		return m_name;
	}
private:
	string m_name;
};
class Factory {
public:
	virtual AbstractFactory * create(string type) = 0;
};
class TankFactory:public Factory{
public:
	AbstractFactory * create(string type) override{
		if (type == "dog") {
			return new FactoryCat;
		}
		else if (type == "cat") {
			return new FactoryDog;
		}
		return nullptr;
	}
};

int main()
{
	TankFactory tank;
	tank.create("dog")->type();
	tank.create("cat")->type();
    return 0;
}
#include <iostream>
#include <list>
#include <string>
using namespace std;

class IObserver {
public:
	virtual void update(float price) = 0;
};
class ConcreteObserver :public IObserver {
public:
	ConcreteObserver(string name) :m_name(name){}
	void update(float price) override {
		cout << m_name << "--" << price << endl;
	}
private:
	string m_name;
};
class ISubject {
public:
	virtual void attach__(IObserver *)=0;
	virtual void delete__(IObserver *) = 0;
	virtual void notify__() = 0;
};
class ConcreteSubject :public ISubject {
public:
	ConcreteSubject() :m_price(10.0){ }
	void setPrice(float price) { m_price = price;  }
	void attach__(IObserver *obj) {
		m_observe.push_back(obj);
	}
	virtual void delete__(IObserver *obj) {
		m_observe.remove(obj);
	}
	virtual void notify__() {
		for (auto obj : m_observe) {
			obj->update(m_price);
		}
	}
private:
	float m_price;
	list<IObserver *> m_observe;
};
int main()
{
	ConcreteSubject *pSub = new ConcreteSubject;


	list<string> content = {"apple","pear","peach","batterey","watermelon"};
	list<ConcreteObserver *> objContent;
	for (auto c : content) {
		ConcreteObserver *pObj1 = new ConcreteObserver(c);
		pSub->attach__(pObj1);
		objContent.push_back(pObj1);
	}
	pSub->notify__();
	pSub->setPrice(50.0);
	pSub->notify__();
    return 0;
}

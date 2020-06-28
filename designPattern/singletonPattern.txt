/*
	*vs2015 x86
*/
#include <iostream>
#include <memory>
using namespace std;
#include <mutex>
#define  CONSTRACT(x) cout <<(x)<<"构造函数"<<endl 
#define  DECONSTRACT(x) cout <<(x)<<"析构函数"<<endl 

// 饿汉式
class Singleton1 {
private:
	Singleton1() { CONSTRACT("饿汉式：Singleton1"); }
	~Singleton1() {}
	Singleton1(Singleton1 &) = delete;
	Singleton1& operator=(Singleton1 &) = delete;
	static Singleton1* m_pInstance1;
public:
	static Singleton1* getInstance() {
		return m_pInstance1;
	}
	static void deleteThis() {
		delete m_pInstance1;
		DECONSTRACT("Singleton1");
	}
};
Singleton1*  Singleton1::m_pInstance1 = new Singleton1;

// 懒汉式线程不安全
class Singleton2 {
	Singleton2() { CONSTRACT("懒汉式线程不安全：Singleton2"); }
	~Singleton2() {}
	Singleton2(Singleton2 &) = delete;
	Singleton2& operator=(Singleton2 &) = delete;
	static Singleton2* m_pInstance2;
public:
	static Singleton2* getInstance() {
		if (m_pInstance2 == nullptr) {
			m_pInstance2 = new Singleton2;
		}
		return m_pInstance2;
	}
	static void release() {
		delete m_pInstance2;
		DECONSTRACT("Singleton2");
	}
};

// 饿汉式线程安全
class Singleton3 {
	typedef std::shared_ptr<Singleton3> Ptr;
private:
	Singleton3() { CONSTRACT("懒汉式线程安全：Singleton3"); }
	Singleton3(Singleton3 &) = delete;
	Singleton3& operator=(Singleton3 &) = delete;
	static Singleton3* m_pInstance3;
	static mutex* m_pMutex;
public:
	static Singleton3* Singleton3::getInstance() {
		if (m_pInstance3 == nullptr) {
			std::lock_guard<std::mutex> lg(*m_pMutex);
			if (m_pInstance3 == nullptr) {
				m_pInstance3 = new Singleton3();
			}
		}
		return m_pInstance3;
	}
	~Singleton3() {}
};
Singleton3* Singleton3::m_pInstance3 = nullptr;
mutex* Singleton3::m_pMutex = nullptr;

int main()
{
	Singleton1::getInstance();
	Singleton1::deleteThis();
	Singleton2::getInstance();
	Singleton2::release();
	Singleton3::getInstance(); // 62行有点问题
	
	return 0;
}

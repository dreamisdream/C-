#include <thread>
#include <iostream>

using namespace std;

int main(){
    cout<<"Ӳ��֧�ֵ�����߳���:";
    cout<<thread::hardware_concurrency()<<endl;
    return 0;
}
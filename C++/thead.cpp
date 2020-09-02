#include <thread>
#include <iostream>

using namespace std;

int main(){
    cout<<"硬件支持的最大线程数:";
    cout<<thread::hardware_concurrency()<<endl;
    return 0;
}
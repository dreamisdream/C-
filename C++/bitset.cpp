#include <bitset>
#include <iostream>
#include <string>

using namespace std;

int main(){

    if(0)
    while(1){
    bitset<8> foo;
    cout<<"please enter an 8-bit binary number"<<endl;
    cin>>foo;
    cout<<boolalpha;
    cout<<"all:"<<foo.all()<<endl;
    cout<<"any:"<<foo.any()<<endl;
    cout<<"none:"<<foo.none()<<endl;
    cout<<"count:"<<foo.count()<<endl;
    cout<<"test:"<<foo.test(1)<<endl;
    cout<<"flip: "<<foo.flip()<<endl;
    cout<<"flip1: "<<foo.flip(1)<<endl;
    cout<<"flip2: "<<foo.flip(2)<<endl;
    cout<<"flip3: "<<foo.flip(3)<<endl;
    }

    return 0;

}
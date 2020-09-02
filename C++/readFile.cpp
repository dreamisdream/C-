#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>

using namespace std;


struct Stu{
    char name[64];
    char sex[8];
    int age;
    Stu(char *_name,char *_sex,int _age){
        strcpy(name,_name);
        strcpy(sex,_sex);
        age = _age;
    }
};
void splitString(const string &str,vector<string> &arr,string &suffix)
{
    size_t left = 0;
    size_t right = 0;
    while(pos = str.find(suffix,left) != string::npos){
        arr.push_back(str.substr(left,pos-left));
        left = pos+1;
    }
}
void readFile(string &filename , vector<Stu> &vStu){
    ifstream f;
    f.open(filename,ios::in);
    while(f.good() && !f.eof()){
        string str;
        getline(f,str);
        char name[64];
        char sex[8];
        int age;
        if(!str.empty()){
            sscanf(str.c_str(),"%s %s %d\n",name,sex,&age);
            Stu temp(name,sex,age);
            vStu.push_back(temp);
        }
    }   
}


int main(){
    string name("1.txt");
    vector<Stu> stuVector;
    readFile(name,stuVector);
    cout<<stuVector.size()<<endl;
    for(auto i:stuVector){
        cout<<"name:" <<i.name<<"\tsex:"<<i.sex<<"\tage:"<<i.age<<endl;
    }
    
    return 0;
}
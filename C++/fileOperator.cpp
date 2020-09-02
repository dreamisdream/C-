#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#pragma region 文件
string readFile(string &filename){
    ifstream f(filename);
    string ret;
    while(f.good() && !f.eof()){
        string temp;
        getline(f,temp); 
        if(temp.find(',') == string::npos)
            ret.append(temp);
    }
    return ret;
}
// 读文件
void readTxt(string &name){
    ifstream in;
    in.open(name);
    if(!in){
        cout<<"open file failed!"<<endl;
        return ;
    }
    char x;
    while(in>>x){
        cout<<x;
    }
    cout<<endl;
}
// 写文件
void writeTxt(string &name){
    ofstream out;
    // 打开方式 in out binary app trunk nocreate noreplace
    out.open(name,ios::app);
    if(!out){
        cout<<"open file failed!"<<endl;
        return ;
    }
    out.seekp(0,ios::beg);
    out<<"11111111222222222233333333";
    return ;
}

#pragma endregion


void fileRead(string &name){
    ifstream f;
    f.open(name,ios::in);
    while(f.good() && !f.eof()){
        string temp;
        getline(f,temp);
        cout<<temp<<endl;
    }
    return ;
}

void fileWrite(string &name){
    ofstream f(name,ios::app);
    if(!f.eof()){
        
        //f.write("this is a test text?????????????",30);
      
    }
       // f<<"this is a test text!";
    return ;
}
streampos calcFileSize(string &name)  {
    ifstream f(name);
    f.seekg(0,ios::end);
    streampos sp = f.tellg();
    return sp; 
}
void moveFilePointer(string &name){
    ifstream f(name);
    f.seekg(-10,ios::end);  // 移动到文件结尾前
     while(f.good() && !f.eof()){
        string temp;
        getline(f,temp);
        cout<<temp<<endl;
    }
    f.seekg(0,ios::beg); // 移动到文件开头
         while(f.good() && !f.eof()){
        string temp;
        getline(f,temp);
        cout<<temp<<endl;
    }
    f.close();
    return ;
}
void getTellg(string &name){
    ifstream f(name);
    //f.seekg(-100,ios::end);  // 移动到文件结尾前
     while(f.good() && !f.eof()){
        string temp;
        cout<<f.tellg()<<" : ";
        getline(f,temp);
    }
}

int main(){
    string str("1.txt");
    
   // fileWrite(str);
   //cout <<calcFileSize(str)<<endl;
    //getTellg(str);
    

   // readTxt(str);
   // writeTxt(str);
    //cout<<readFile(str)<<endl;
    return 0;
}
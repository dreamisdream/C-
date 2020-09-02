#include <complex>
#include <iostream>
#include <queue>
#include <string>

using namespace std;
string addString(string &str1,string &str2){
    if(str1.size() > str2.size())
        swap(str1,str2);    
    int i=0;
    int jinwei=0;
    int str1Len =str1.size();
    int str2Len =str2.size();
    for(;i<str1.size();++i){
        int temp = str1[str1Len-1-i] - 48 + str2[str2Len-1-i] -48 +jinwei;
        jinwei =temp/10;
        str2[str2Len-1-i] = temp%10 +48;
    }
    for(;i<str2.size();++i){
        if(jinwei){
            int temp = str2[str2Len-1-i] -48 +jinwei;
            jinwei =temp/10;
            str2[str2Len-1-i] = temp%10 +48;
        }
        else
        {
            break;
        }
        
    }
    if(jinwei){
        str2 = '1'+str2;
    }
    return str2;
}
 int fib(int n) {
        n++;
        vector<int> d(n,0);
        d[0]=0;
        d[1] =1;
        for(int i=2;i<n;++i){
            d[i] = d[i-1]+d[i-2];
        }
        return d[n-1];
    }
int main(){
    
    string s1("12313554");
    string s2("456");
   // cout<<addString(s1,s2)<<endl;
    
    cout<<fib(5)<<endl;
    return 0;

}
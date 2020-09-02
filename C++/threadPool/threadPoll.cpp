#include <iostream>
#include <queue>
#include <string>
#include <map>
#include <stack>

using namespace std;
using INTTER = int;

bool isValid(string s) {
    stack <char> t;
    for(char n : s)
    {
        if (n == '(')                t.push(')');
        else if (n == '{')           t.push('}');
        else if (n == '[')           t.push(']');
        else 
        {
            if(t.empty() || t.top() != n)      return false; //��ʱnΪ����������֮һ����ջ��Ҳ������������֮һ
            else        t.pop(); 
        }        
    }
        return t.empty();      
}

bool isValidQueue(string s){
    queue<char > q;
    for (char i:s){
        if(i == '('){
            q.push(')');
        }
        else if(i=='['){
            q.push(']');
        }
        else if(i=='{'){
            q.push('}');
        }else{
            if(q.empty() || q.back() !=i ){
                return false;
            }else
            {
                q.pop();
            }
            
        }
    }
    return q.empty();
}
int main(){
    
    string s("{(())}");
    cout<<boolalpha<<isValid(s)<<endl;
    cout<<boolalpha<<isValidQueue(s)<<endl;
    return 0;

}
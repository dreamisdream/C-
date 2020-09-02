// #include <vector>
// #include <iostream>
// #include <algorithm>

// using namespace std;

// template<typename InputIterator, typename Function>
// Function foreach(InputIterator beg, InputIterator end, Function f) {
//   while(beg != end) 
//     f(*beg++);
// }

// void add(const int &i){
//     cout<< i<<endl;
// }

// int main(){
    
//     std::vector<int> arr;
//     arr.push_back(1);
//     arr.push_back(2);

//     std::for_each(arr.begin(), arr.end(), add);
// }

#include <vector>
#include <iostream>

using namespace std;


#ifdef __cplusplus                      // 定义了这个宏就是在用C++编译
    extern "C" {                        // 函数按照C的方式去处理
#endif
    void a_c_function(int a);
#ifdef __cplusplus                      // 检查是否是C++编译
    }                                   // extern "C" 结束
#endif

#if __cplusplus >= 201402                // 检查C++标准的版本号
#    define C++ "c14"    // 201402就是C++14
#elif __cplusplus >= 201103              // 检查C++标准的版本号
#    define C++ "c11"    // 201103是C++11
#else   // __cplusplus < 201103          // 199711是C++98
#   error "c++ is too old"               // 太低则预处理报错
#endif  // __cplusplus >= 201402         // 预处理语句结束

bool containsPattern(vector<int>& arr, int m, int k) {
        int count=1;
            
        for(int i=m;i<arr.size();i++){
            for(int j=0;j<m;++j){
                if(arr[i+j] != arr[j])
                    break;
                else
                    count++;
            }
            
            if(count>=k)
                return true;
        }
        return false;
}

int main(void){
    // vector<int> arr{1,2,1,2,1,1,1,3};
    // cout<<containsPattern(arr,1,3)<<endl;
    // vector<int> arr2{1,2,1,2,1,1,1,3};
    // cout<<containsPattern(arr2,2,2)<<endl;
    cout<<C++<<endl;
    return 0;
}
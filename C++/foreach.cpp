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


#ifdef __cplusplus                      // ������������������C++����
    extern "C" {                        // ��������C�ķ�ʽȥ����
#endif
    void a_c_function(int a);
#ifdef __cplusplus                      // ����Ƿ���C++����
    }                                   // extern "C" ����
#endif

#if __cplusplus >= 201402                // ���C++��׼�İ汾��
#    define C++ "c14"    // 201402����C++14
#elif __cplusplus >= 201103              // ���C++��׼�İ汾��
#    define C++ "c11"    // 201103��C++11
#else   // __cplusplus < 201103          // 199711��C++98
#   error "c++ is too old"               // ̫����Ԥ������
#endif  // __cplusplus >= 201402         // Ԥ����������

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
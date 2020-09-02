#include <iostream>
#include <vector>
#include <array>
#include <cassert>


using namespace std;
constexpr int foo(int i){
    return i+10;
}
[[noreturn]]
void getNum(){
     ;
}
int main(){
    int i=10;
    array<int,foo(2)> arr;
    assert(arr.size() != foo(10));
    // static_assert(sizeof(long)>=8,"64bit");
    static_assert(sizeof(long)>=4,"32bit");
    // foo(i);
    // array<int,foo(i)> arr1;
        
    return 0;
}

//

auto hasher = [](const auto& p)    // 定义一个lambda表达式
{
    return std::hash<int>()(p.x);  // 调用标准hash函数对象计算
};


// 算法
要求排序后仍然保持元素的相对顺序，应该用 stable_sort，它是稳定的；选出前几名（TopN），应该用 partial_sort；选出前几名，但不要求再排出名次（BestN），应该用 nth_element；中位数（Median）、百分位数（Percentile），还是用 nth_element；按照某种规则把元素划分成两组，用 partition；第一名和最后一名，用 minmax_element。


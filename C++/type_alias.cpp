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

auto hasher = [](const auto& p)    // ����һ��lambda���ʽ
{
    return std::hash<int>()(p.x);  // ���ñ�׼hash�����������
};


// �㷨
Ҫ���������Ȼ����Ԫ�ص����˳��Ӧ���� stable_sort�������ȶ��ģ�ѡ��ǰ������TopN����Ӧ���� partial_sort��ѡ��ǰ����������Ҫ�����ų����Σ�BestN����Ӧ���� nth_element����λ����Median�����ٷ�λ����Percentile���������� nth_element������ĳ�ֹ����Ԫ�ػ��ֳ����飬�� partition����һ�������һ������ minmax_element��


/*
    *预编译
*/
#                           //预处理空行
#if  __linux__              //预处理检查宏是否存在
#   define HAS_LINUX    1   //宏定义有缩进
#endif                      //预处理条件语句结束
#

#ifndefine CODESTYLE_H
#define    CODESTYLE_H


...     // 头文件内容

#endif

#pragma region 源码级抽象
static uint32_t calc_table[]={
#   include "calc_values.inc" //非常大的一个数组,细节被隐藏
};
include "calc_values.inc"
内容如下 :

static uint32_t  calc_table[] = {  // 非常大的一个数组，有几十行
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
    0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    ...                          
};
#pragma endregion


#define ngx_tolower(c)      ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ngx_toupper(c)      ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)



#define CUBE(a) (a) * (a) * (a)  // 定义一个简单的求立方的宏

cout << CUBE(10) << endl;        // 使用宏简化代码
cout << CUBE(15) << endl;        // 使用宏简化代码

#undef CUBE                      // 使用完毕后立即取消定义


#ifdef AUTH_PWD                  // 检查是否已经有宏定义
#  undef AUTH_PWD                // 取消宏定义
#endif                           // 宏定义检查结束
#define AUTH_PWD "xxx"           // 重新宏定义


#define BEGIN_NAMESPACE(x)  namespace x {
#define END_NAMESPACE(x)    }

BEGIN_NAMESPACE(my_own)

...      // functions and classes

END_NAMESPACE(my_own)


#ifdef __cplusplus                      // 定义了这个宏就是在用C++编译
    extern "C" {                        // 函数按照C的方式去处理
#endif
    void a_c_function(int a);
#ifdef __cplusplus                      // 检查是否是C++编译
    }                                   // extern "C" 结束
#endif

#if __cplusplus >= 201402                // 检查C++标准的版本号
    cout << "c++14 or later" << endl;    // 201402就是C++14
#elif __cplusplus >= 201103              // 检查C++标准的版本号
    cout << "c++11 or before" << endl;   // 201103是C++11
#else   // __cplusplus < 201103          // 199711是C++98
#   error "c++ is too old"               // 太低则预处理报错
#endif  // __cplusplus >= 201402         // 预处理语句结束

使用宏定义包含头文件
#if (NGX_FREEBSD)
#  include <ngx_freebsd.h>

#elif (NGX_LINUX)
#  include <ngx_linux.h>

#elif (NGX_SOLARIS)
#  include <ngx_solaris.h>

#elif (NGX_DARWIN)
#  include <ngx_darwin.h>
#endif


#if 0          // 0即禁用下面的代码，1则是启用
  ...          // 任意的代码
#endif         // 预处理结束

#if 1          // 1启用代码，用来强调下面代码的必要性
  ...          // 任意的代码
#endif         // 预处理结束

// 属性和静态断言
// 智能指针
    auto ptr = make_unique<int>(42);
    assert(ptr && *ptr==42);
    auto ptr2=make_unique<string>("god of war");


template<class T, class... Args>              // 可变参数模板
std::unique_ptr<T>                            // 返回智能指针
my_make_unique(Args&&... args)                // 可变参数模板的入口参数
{
    return std::unique_ptr<T>(                // 构造智能指针
            new T(std::forward<Args>(args)...));    // 完美转发
}



using namespace std::literals::string_literals;  //必须打开名字空间

auto str = "std string"s;      // 后缀s，表示是标准字符串，直接类型推导

assert("time"s.size() == 4);   // 标准字符串可以直接调用成员函数


auto str = R"(nier:automata)";    // 原始字符串：nier:automata



auto str5 = R"==(R"(xxx)")==";// 原样输出：R"(xxx)"


//regex

auto make_regex = [](const auto& txt)    // 生产正则表达式
{
    return std::regex(txt);
};

auto make_match = []()                  // 生产正则匹配结果
{
    return std::smatch();
};

auto str = "neir:automata"s;          // 待匹配的字符串
auto reg = 
    make_regex(R"(^(\w+)\:(\w+)$)");  // 原始字符串定义正则表达式
auto what = make_match();             // 准备获取匹配的结果


/*
    *Ԥ����
*/
#                           //Ԥ�������
#if  __linux__              //Ԥ��������Ƿ����
#   define HAS_LINUX    1   //�궨��������
#endif                      //Ԥ��������������
#

#ifndefine CODESTYLE_H
#define    CODESTYLE_H


...     // ͷ�ļ�����

#endif

#pragma region Դ�뼶����
static uint32_t calc_table[]={
#   include "calc_values.inc" //�ǳ����һ������,ϸ�ڱ�����
};
include "calc_values.inc"
�������� :

static uint32_t  calc_table[] = {  // �ǳ����һ�����飬�м�ʮ��
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



#define CUBE(a) (a) * (a) * (a)  // ����һ���򵥵��������ĺ�

cout << CUBE(10) << endl;        // ʹ�ú�򻯴���
cout << CUBE(15) << endl;        // ʹ�ú�򻯴���

#undef CUBE                      // ʹ����Ϻ�����ȡ������


#ifdef AUTH_PWD                  // ����Ƿ��Ѿ��к궨��
#  undef AUTH_PWD                // ȡ���궨��
#endif                           // �궨�������
#define AUTH_PWD "xxx"           // ���º궨��


#define BEGIN_NAMESPACE(x)  namespace x {
#define END_NAMESPACE(x)    }

BEGIN_NAMESPACE(my_own)

...      // functions and classes

END_NAMESPACE(my_own)


#ifdef __cplusplus                      // ������������������C++����
    extern "C" {                        // ��������C�ķ�ʽȥ����
#endif
    void a_c_function(int a);
#ifdef __cplusplus                      // ����Ƿ���C++����
    }                                   // extern "C" ����
#endif

#if __cplusplus >= 201402                // ���C++��׼�İ汾��
    cout << "c++14 or later" << endl;    // 201402����C++14
#elif __cplusplus >= 201103              // ���C++��׼�İ汾��
    cout << "c++11 or before" << endl;   // 201103��C++11
#else   // __cplusplus < 201103          // 199711��C++98
#   error "c++ is too old"               // ̫����Ԥ������
#endif  // __cplusplus >= 201402         // Ԥ����������

ʹ�ú궨�����ͷ�ļ�
#if (NGX_FREEBSD)
#  include <ngx_freebsd.h>

#elif (NGX_LINUX)
#  include <ngx_linux.h>

#elif (NGX_SOLARIS)
#  include <ngx_solaris.h>

#elif (NGX_DARWIN)
#  include <ngx_darwin.h>
#endif


#if 0          // 0����������Ĵ��룬1��������
  ...          // ����Ĵ���
#endif         // Ԥ�������

#if 1          // 1���ô��룬����ǿ���������ı�Ҫ��
  ...          // ����Ĵ���
#endif         // Ԥ�������

// ���Ժ;�̬����
// ����ָ��
    auto ptr = make_unique<int>(42);
    assert(ptr && *ptr==42);
    auto ptr2=make_unique<string>("god of war");


template<class T, class... Args>              // �ɱ����ģ��
std::unique_ptr<T>                            // ��������ָ��
my_make_unique(Args&&... args)                // �ɱ����ģ�����ڲ���
{
    return std::unique_ptr<T>(                // ��������ָ��
            new T(std::forward<Args>(args)...));    // ����ת��
}



using namespace std::literals::string_literals;  //��������ֿռ�

auto str = "std string"s;      // ��׺s����ʾ�Ǳ�׼�ַ�����ֱ�������Ƶ�

assert("time"s.size() == 4);   // ��׼�ַ�������ֱ�ӵ��ó�Ա����


auto str = R"(nier:automata)";    // ԭʼ�ַ�����nier:automata



auto str5 = R"==(R"(xxx)")==";// ԭ�������R"(xxx)"


//regex

auto make_regex = [](const auto& txt)    // ����������ʽ
{
    return std::regex(txt);
};

auto make_match = []()                  // ��������ƥ����
{
    return std::smatch();
};

auto str = "neir:automata"s;          // ��ƥ����ַ���
auto reg = 
    make_regex(R"(^(\w+)\:(\w+)$)");  // ԭʼ�ַ�������������ʽ
auto what = make_match();             // ׼����ȡƥ��Ľ��


// String 验收测试：每个用例单独进程运行
//   ./test_string <case>
//   case 1  析构（new[] / delete[] 匹配）
//   case 2  拷贝构造深拷贝
//   case 3  移动构造：源对象可安全析构、可重新赋值
//   case 4  自拷贝赋值  a = a
//   case 5  拷贝赋值：旧资源必须释放（不泄漏）
//   case 6  移动赋值：旧资源释放 + 源被置空
//   case 7  自移动赋值  a = std::move(a)
//   case 8  移动后源对象可复用（valid but unspecified）
//   case 9  从 moved-from 对象拷贝构造
//   case 10 从 moved-from 对象拷贝赋值
//   case 11 赋值来源是空串时，目标必须变空
//   case 12 空串的 c_str() 不能是 nullptr

#include <cppself/string.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <utility>

using cppself::String;

static int fail(const char* what) {
    std::printf("FAIL: %s\n", what);
    return 1;
}

int main(int argc, char** argv) {
    const int c = (argc > 1) ? std::atoi(argv[1]) : 0;

    switch (c) {
    case 1: {
        {
            String s("hello");
        }
        std::printf("PASS: case1 dtor\n");
        return 0;
    }
    case 2: {
        String a("hello");
        String b(a);
        if (b.size() != 5 || std::strcmp(b.c_str(), "hello") != 0) return fail("copy content");
        if (a.c_str() == b.c_str()) return fail("copy shares buffer (shallow)");
        std::printf("PASS: case2 copy ctor\n");
        return 0;
    }
    case 3: {
        String a("hello");
        String b(std::move(a));
        if (b.size() != 5 || std::strcmp(b.c_str(), "hello") != 0) return fail("move content");
        a = String("world");
        if (a.size() != 5 || std::strcmp(a.c_str(), "world") != 0) return fail("moved-from reuse");
        std::printf("PASS: case3 move ctor\n");
        return 0;
    }
    case 4: {
        String a("hello");
        a = a;
        if (a.size() != 5 || std::strcmp(a.c_str(), "hello") != 0) return fail("self copy assign content");
        std::printf("PASS: case4 self copy assign\n");
        return 0;
    }
    case 5: {
        {
            String a("this is a long string that must be freed");
            String b("short");
            a = b;
            if (a.size() != 5 || std::strcmp(a.c_str(), "short") != 0) return fail("copy assign content");
        }
        std::printf("PASS: case5 copy assign\n");
        return 0;
    }
    case 6: {
        {
            String a("this is a long string that must be freed");
            String b("short");
            a = std::move(b);
            if (a.size() != 5 || std::strcmp(a.c_str(), "short") != 0) return fail("move assign content");
            if (b.size() != 0) return fail("move assign: source not reset (size != 0)");
            if (b.c_str() == nullptr || b.c_str()[0] != '\0') return fail("move assign: source not empty");
            b = String("again");
            if (b.size() != 5 || std::strcmp(b.c_str(), "again") != 0) return fail("move assign: source reuse");
        }
        std::printf("PASS: case6 move assign\n");
        return 0;
    }
    case 7: {
        String a("hello");
        a = std::move(a);
        if (a.size() != 0 || a.c_str() != nullptr) {
            // 标准不要求具体状态，但必须不崩
        }
        std::printf("PASS: case7 self move assign\n");
        return 0;
    }
    case 8: {
        String a("hello");
        String b(std::move(a));
        (void)a.size();
        a = String("x");
        std::printf("PASS: case8 moved-from valid\n");
        return 0;
    }
    case 9: {
        String a("hello");
        String b(std::move(a));
        String c(a);            // a 的 data_ 是 nullptr —— 拷贝必须仍然合法
        (void)c.size();
        std::printf("PASS: case9 copy from moved-from\n");
        return 0;
    }
    case 10: {
        String a("hello");
        String b(std::move(a));
        String c("xyz");
        c = a;                  // a 的 data_ 是 nullptr
        (void)c.size();
        std::printf("PASS: case10 copy-assign from moved-from\n");
        return 0;
    }
    case 11: {
        String a("hello");
        String e("");
        a = e;
        if (a.size() != 0) return fail("assign from empty: target not cleared");
        std::printf("PASS: case11 assign from empty\n");
        return 0;
    }
    case 12: {
        String s("");
        String t(s);
        if (t.c_str() == nullptr) return fail("c_str() == nullptr for empty copy");
        if (std::strlen(t.c_str()) != t.size()) return fail("c_str()/size() inconsistent");
        std::printf("PASS: case12 empty c_str\n");
        return 0;
    }
    default:
        std::printf("usage: test_string <1-12>\n");
        return 2;
    }
}

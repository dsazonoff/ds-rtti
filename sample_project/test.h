#pragma once

class Test final
{
public:
    Test() = default;
    ~Test() = default;

    void foo();

private:
    int m_test{42}; // [[ignore]]
};

template<typename T>
class TemplatedTest final
{
public:
    auto get_some() -> T;

private:
    T m_member{};
};

template<typename T> auto TemplatedTest<T>::get_some() -> T
{
    return m_member;
}

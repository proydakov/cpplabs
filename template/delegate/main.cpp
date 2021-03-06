/*
 *  Copyright (c) 2019 Evgeny Proydakov <lord.tiran@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include <utility>

// delegate code

template<typename T>
class delegate;

template<typename RETURN, typename ... INPUTS>
class delegate<RETURN(INPUTS ...)>
{
public:
    template<RETURN(*func)(INPUTS...)>
    static delegate function()
    {
        return delegate(&call_function<func>);
    }

    template<class T, RETURN(T::*meth)(INPUTS...)>
    static delegate method(T* obj)
    {
        return delegate(obj, &call_method<T, meth>);
    }

    template<class T, RETURN(T::*meth)(INPUTS...) const>
    static delegate const_method(T* obj)
    {
        return delegate(obj, &call_const_method<T, meth>);
    }

    template<class T, RETURN(T::*meth)(INPUTS...) const>
    static delegate const_method(T const* obj)
    {
        return delegate((void*)(obj), &call_const_method<T, meth>);
    }

    template<class T>
    static delegate functor(T* obj)
    {
        return delegate(obj, &call_method<T, &T::operator()>);
    }

    template<class T>
    static delegate const_functor(T* obj)
    {
        return delegate(obj, &call_const_method<T, &T::operator()>);
    }

    template<class T>
    static delegate const_functor(T const* obj)
    {
        return delegate((void*)(obj), &call_const_method<T, &T::operator()>);
    }

    RETURN operator()(INPUTS ... params) const
    {
        return (*m_func)(m_obj, std::forward<INPUTS>(params) ...);
    }

    operator bool() const noexcept
    {
        return m_func != nullptr;
    }

    bool operator !() const noexcept
    {
        return !(operator bool());
    }

    constexpr delegate() noexcept : m_func(nullptr), m_obj(nullptr)
    {
    }

    ~delegate() noexcept
    {
        m_func = nullptr;
        m_obj = nullptr;
    }

    constexpr delegate(delegate const&) noexcept = default;
    delegate& operator=(delegate const&) noexcept = default;

    constexpr delegate(delegate&& other) noexcept
        : m_obj(other.m_obj)
        , m_func(other.m_func)
    {
        other.m_obj = nullptr;
        other.m_func = nullptr;
    }

    delegate& operator=(delegate&& other) noexcept
    {
        if (&other != this)
        {
            m_obj = other.m_obj;
            m_func = other.m_func;

            other.m_obj = nullptr;
            other.m_func = nullptr;
        }
        return *this;
    }

private:
    using FuncPtr = RETURN(*)(void * obj, INPUTS...);

    void* m_obj;
    FuncPtr m_func;

    constexpr delegate(FuncPtr func) noexcept
        : m_obj(nullptr)
        , m_func(func)
    {
    }

    constexpr delegate(void* obj, FuncPtr func) noexcept
        : m_obj(obj)
        , m_func(func)
    {
    }

    template<RETURN(*function)(INPUTS...)>
    static RETURN call_function(void*, INPUTS... params)
    {
        return (*function)(std::forward<INPUTS>(params)...);
    }

    template<class T, RETURN(T::*method)(INPUTS...)>
    static RETURN call_method(void* obj, INPUTS... params)
    {
        T * p = static_cast<T*>(obj);
        return (p->*method)(std::forward<INPUTS>(params)...);
    }

    template<class T, RETURN(T::*method)(INPUTS...) const>
    static RETURN call_const_method(void* obj, INPUTS... params)
    {
        T const * p = static_cast<T*>(obj);
        return (p->*method)(std::forward<INPUTS>(params)...);
    }
};

// test code

#include <iostream>

// function test

void function(int val)
{
    std::cout << "function: " << val << std::endl;
}

void function_test()
{
    using Delegate = delegate<void(int)>;

    auto delayed_call = Delegate::function<&function>();
    for (int i = 1; i <= 1024; i *= 2)
    {
        delayed_call(i);
    }
}

// method

struct IObjectCall
{
    void call(int val)
    {
        std::cout << "IObjectCall: " << val << std::endl;
    }
};

void method_test()
{
    using Delegate = delegate<void(int)>;

    IObjectCall object;

    auto delayed_call = Delegate::method<IObjectCall, &IObjectCall::call>(&object);
    for (int i = 1; i <= 1024; i *= 2)
    {
        delayed_call(i);
    }
}

// const method

void const_method_test()
{
    using Delegate = delegate<void(int)>;

    {
        struct IObjectConstCall
        {
            void call(int val) const
            {
                std::cout << "IObjectConstCall ptr: " << val << std::endl;
            }
        };

        IObjectConstCall object;

        auto delayed_call = Delegate::const_method<IObjectConstCall, &IObjectConstCall::call>(&object);
        for (int i = 1; i <= 1024; i *= 2)
        {
            delayed_call(i);
        }
    }

    {
        struct IObjectConstCall
        {
            void call(int val) const
            {
                std::cout << "IObjectConstCall ptr const: " << val << std::endl;
            }
        };

        IObjectConstCall const object2;

        auto delayed_call = Delegate::const_method<IObjectConstCall, &IObjectConstCall::call>(&object2);
        for (int i = 1; i <= 1024; i *= 2)
        {
            delayed_call(i);
        }
    }
}

// functor test

void functor_test()
{
    using Delegate = delegate<void(int)>;

    auto functor = [](int val) mutable
    {
        std::cout << "mutable functor: " << val << std::endl;
    };

    auto delayed_call = Delegate::functor(&functor);
    for (int i = 1; i <= 1024; i *= 2)
    {
        delayed_call(i);
    }
}

// const functor test

void const_functor_test()
{
    using Delegate = delegate<void(int)>;

    {
        auto functor = [](int val)
        {
            std::cout << "functor ptr: " << val << std::endl;
        };

        auto delayed_call = Delegate::const_functor(&functor);
        for (int i = 1; i <= 1024; i *= 2)
        {
            delayed_call(i);
        }
    }

    {
        auto const functor = [](int val)
        {
            std::cout << "functor const ptr: " << val << std::endl;
        };

        auto delayed_call = Delegate::const_functor(&functor);
        for (int i = 1; i <= 1024; i *= 2)
        {
            delayed_call(i);
        }
    }

}

// perf test

#include <ratio>
#include <chrono>

std::size_t g_counter = 0;

void perf_function(std::size_t val)
{
    g_counter += val;
}

void function_perf_test()
{
    using Delegate = delegate<void(std::size_t)>;

    auto delayed_call = Delegate::function<&perf_function>();

    std::chrono::high_resolution_clock clock;
    auto start = clock.now();
    {
        for (std::size_t i = 0; i < std::giga::num; i++)
        {
            delayed_call(i);
        }
    }
    auto end = clock.now();

    std::cout << "BENCHMARK: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << std::endl;
}

int main()
{
    function_test();

    method_test();
    const_method_test();

    functor_test();
    const_functor_test();

    //function_perf_test();

    return 0;
}

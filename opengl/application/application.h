#pragma once

#include <ostream>
#include <chrono>
#include <cstdint>
#include <functional>

#ifdef _MSC_VER
#include <windows.h>
#endif // _MSC_VER

#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class keyboard_press_guard
{
public:
    keyboard_press_guard(SDL_Keycode code, std::function<void()>&& lambda);
    void operator()(SDL_Event const& Event);

private:
    SDL_Keycode const m_code;
    bool m_lock;
    std::function<void()> m_lambda;
};

class application
{
public:
    application(int argc, char* argv[], std::size_t width, std::size_t height);
    virtual ~application();

    virtual void init() = 0;
    virtual void resize(std::size_t width, std::size_t height) = 0;
    virtual void update(std::chrono::microseconds delta) = 0;
    virtual void draw() = 0;

    virtual void info(std::ostream&);
    virtual void on_event(SDL_Event const&);

    int run();

    // unused
    application(application const&) = delete;
    application(application &&) = delete;
    application& operator=(application const&) = delete;
    application& operator=(application &&) = delete;

private:
    void default_info();

private:
    SDL_Window*   m_window;
    SDL_GLContext m_context;

    std::uint32_t m_flags;
    std::size_t   m_width;
    std::size_t   m_height;
    bool          m_fullscreen;

    keyboard_press_guard m_fs_functor;
};

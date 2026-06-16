
#pragma once
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>

    #include <libloaderapi.h>
    #undef WIN32_LEAN_AND_MEAN
    #undef NOMINMAX

    #define load_library(path) LoadLibraryExA(path, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
    #define free_library(handle) FreeLibrary(handle)
    #define get_symbol(handle, symbol) GetProcAddress(handle, symbol)
using library_handle_t = HMODULE;
constexpr library_handle_t invalid_library_handle = nullptr;
#else
    #include <dlfcn.h>
    #define load_library(path) dlopen(path.c_str(), RTLD_LAZY)
    #define free_library(handle) dlclose(handle)
    #define get_symbol(handle, symbol) dlsym(handle, symbol)
using library_handle_t = void*;
constexpr library_handle_t invalid_library_handle = nullptr;
#endif

template <auto Api>
    requires(std::is_pointer_v<decltype(Api)> && std::is_function_v<std::remove_pointer_t<decltype(Api)>>)
struct function_traits
{
    static constexpr auto name = []() constexpr {
#if defined(_MSC_VER)
        auto s = []() constexpr {
            constexpr std::string_view s = __FUNCSIG__;
            constexpr std::string_view begin_key = "function_traits<";
            constexpr std::string_view end_key = "::operator ()(void) const";
            auto pos = s.find(begin_key) + begin_key.size();
            auto end = s.find(end_key);
            if (pos == std::string_view::npos || end == std::string_view::npos)
                return std::string_view{};
            auto sub = s.substr(pos, end - pos);
            end = sub.rfind(">::");
            return sub.substr(0, end);
        }();

        constexpr std::string_view key = "__cdecl ";
        auto pos = s.find(key) + key.size();
        if (pos == std::string_view::npos)
            return std::string_view{};
        auto sub = s.substr(pos);
        auto end = sub.find("(");
        if (end == std::string_view::npos)
            return std::string_view{};
        sub = sub.substr(0, end);
        return sub;
#else
        return std::string_view{};
#endif
    }();
    using type = decltype(Api);
};

template <auto... Apis> struct dynamic_loader
{
    library_handle_t handle;
    std::tuple<typename function_traits<Apis>::type...> func_ptrs;
    dynamic_loader() : handle(invalid_library_handle) {}
    dynamic_loader(const std::string& library_path) : handle(invalid_library_handle) { load(library_path); }
    ~dynamic_loader() { unload(); }
    dynamic_loader(const dynamic_loader&) = delete;
    dynamic_loader& operator=(const dynamic_loader&) = delete;
    dynamic_loader(dynamic_loader&& other) noexcept : handle(other.handle), func_ptrs(std::move(other.func_ptrs)) { other.handle = invalid_library_handle; }
    dynamic_loader& operator=(dynamic_loader&& other) noexcept
    {
        if (this != &other)
        {
            if (handle && handle != invalid_library_handle)
                free_library(handle);
            handle = other.handle;
            func_ptrs = std::move(other.func_ptrs);
            other.handle = invalid_library_handle;
        }
        return *this;
    }

    bool load(const std::string& library_path)
    {
        handle = load_library(library_path.c_str());
        if (!handle)
            return false;
        load_all();
        return true;
    }
    void unload()
    {
        if (handle && handle != invalid_library_handle)
        {
            free_library(handle);
            handle = invalid_library_handle;
        }
    }
    bool is_loaded() const { return handle && handle != invalid_library_handle; }

    template <auto Api> auto get() -> typename function_traits<Api>::type
    {
        constexpr std::size_t idx = index_of<Api>;
        return std::get<idx>(func_ptrs);
    }
    template <auto Api, typename... Args> auto call(Args&&... args) -> decltype(auto)
    {
        auto func = get<Api>();
        if (!func)
            throw std::runtime_error("Function pointer is null");
        return func(std::forward<Args>(args)...);
    }

private:
    template <auto> struct always_false : std::false_type
    {
    };

    template <auto Target, std::size_t I = 0, auto... List> struct index_of_helper;

    template <auto Target, std::size_t I, auto First, auto... Rest> struct index_of_helper<Target, I, First, Rest...> : index_of_helper<Target, I + 1, Rest...>
    {
    };

    template <auto Target, std::size_t I, auto... Rest> struct index_of_helper<Target, I, Target, Rest...>
    {
        static constexpr std::size_t value = I;
    };

    template <auto Target, std::size_t I> struct index_of_helper<Target, I>
    {
        static_assert(always_false<Target>::value, "API not found in Apis pack");
    };

    template <auto Target> static constexpr std::size_t index_of = index_of_helper<Target, 0, Apis...>::value;

    template <auto Api> void load_function()
    {
        constexpr std::size_t idx = index_of<Api>;
        auto symbol = std::string(function_traits<Api>::name);
        std::get<idx>(func_ptrs) = reinterpret_cast<typename function_traits<Api>::type>(get_symbol(handle, symbol.c_str()));

        if (!std::get<idx>(func_ptrs))
            throw std::runtime_error(std::string("Failed to load function: ") + symbol);
    }
    void load_all() { (load_function<Apis>(), ...); }
};
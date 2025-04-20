#pragma once

#include <iostream>
#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>


// Constants
inline constexpr const char *APPLICATION_NAME = "analyzer";
inline constexpr uint32_t APPLICATION_MAJOR_VERSION = 1;
inline constexpr uint32_t APPLICATION_MINOR_VERSION = 0;
inline constexpr uint32_t APPLICATION_PATCH_VERSION = 0;

// Enum
enum class AnalyzerStatus {
    OK                  =  1,
    GLFW_INIT_ERROR     = -1,
    WAITING             =  2,
    DONE                =  3,
    UNDEFINED_ERROR     =  -2,
    FUNCTION_DOESNT_DEFINED = -3,
    TASK_NOT_FOUND          = -4,
};

template<typename T = int, typename S = AnalyzerStatus>
struct Ok {
    T data;
    S status;

    const T& unwrap() const& noexcept { return data; }
    T& unwrap() & noexcept { return data; }
    T&& unwrap() && noexcept { return std::move(data); }
    S what() const { return status; }
    bool operator==(const Ok& other) const {
        return data == other.data && status == other.status;
    }
};

using GlfwWindowCreateResult = Ok<GLFWwindow, AnalyzerStatus>;
using GlfwInitResult = Ok<uint8_t, AnalyzerStatus>;
using GladInitResult = Ok<uint8_t, AnalyzerStatus>;

// Factory function
template<typename T = int, typename S = AnalyzerStatus>
Ok<T, S> make_ok(T&& data, AnalyzerStatus status = AnalyzerStatus::OK) {
    return Ok<T>{std::forward<T>(data), status};
}

template <typename S = AnalyzerStatus>
struct Task
{
    std::string name;
    S status;
    std::function<S()> func;

    Task(const std::string& name, std::function<S()> func, S status = S::WAITING)
        : name(name), status(status), func(func) {}

    S execute()
    {
        if(func) {
            status = func();
            return status;
        }
        return S::FUNCTION_DOESNT_EXIST;
    }
};

template <typename S = AnalyzerStatus>
struct Pipeline
{
    std::string name;
    std::vector<Task<S>> tasks;
    std::unordered_map<std::string, size_t> task_index_map;

    Pipeline(const std::string& name) : name(name) {}
    ~Pipeline() = default;

    S create_task(const std::string& task_name, std::function<S()> func)
    {
        if(task_index_map.find(task_name) != task_index_map.end()) {
            return S::TASK_ALREADY_EXISTS;
        }
        tasks.emplace_back(task_name, func, S::WAITING);
        task_index_map[task_name] = tasks.size() - 1;
        return S::SUCCESS;
    }

    S remove_task(const std::string& task_name)
    {
        auto it = task_index_map.find(task_name);
        if(it == task_index_map.end()) {
            return S::TASK_NOT_FOUND;
        }

        size_t index = it->second;
        tasks.erase(tasks.begin() + index);
        task_index_map.erase(it);

        for(auto& pair : task_index_map) {
            if(pair.second > index) {
                pair.second--;
            }
        }
        return S::SUCCESS;
    }

    S execute_task_by_name(const std::string& task_name)
    {
        auto it = task_index_map.find(task_name);
        if(it == task_index_map.end()) {
            return S::TASK_NOT_FOUND;
        }
        return tasks[it->second].execute();
    }

    S execute_all()
    {
        S overall_status = S::SUCCESS;
        for(auto& task : tasks) {
            S task_status = task.execute();
            if(task_status != S::SUCCESS) {
                overall_status = task_status;
            }
        }
        return overall_status;
    }

    size_t task_count() const
    {
        return tasks.size();
    }

    bool has_task(const std::string& task_name) const
    {
        return task_index_map.find(task_name) != task_index_map.end();
    }

    void clear()
    {
        tasks.clear();
        task_index_map.clear();
    }
};
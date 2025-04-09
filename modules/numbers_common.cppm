export module numbers_common;

#include <iostream>
#include <cstdint>

// Constants
export inline constexpr const char *APPLICATION_NAME = "analyzer";
export inline constexpr uint32_t APPLICATION_MAJOR_VERSION = 1;
export inline constexpr uint32_t APPLICATION_MINOR_VERSION = 0;
export inline constexpr uint32_t APPLICATION_PATCH_VERSION = 0;

// Enum
export enum class AnalyzerStatus {
    OK                  =  1,
    GLFW_INIT_ERROR     = -1,
};

// Template struct
export template<typename T>
struct Ok {
    T data;
    AnalyzerStatus status;

    const T& unwrap() const& noexcept { return data; }
    T& unwrap() & noexcept { return data; }
    T&& unwrap() && noexcept { return std::move(data); }
    AnalyzerStatus what() const { return status; }
    bool operator==(const Ok& other) const {
        return data == other.data && status == other.status;
    }
};

// Factory function
export template<typename T>
Ok<T> make_ok(T&& data, AnalyzerStatus status = AnalyzerStatus::OK) {
    return Ok<T>{std::forward<T>(data), status};
}

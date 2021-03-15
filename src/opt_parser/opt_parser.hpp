#pragma once

#include <exception>
#include <string>

#include <spdlog/fmt/fmt.h>

namespace utils {
struct Opt_parser_error : std::exception {
    explicit Opt_parser_error(std::string msg = "command line parser error") : msg_(std::move(msg)) {}
    char const* what() const noexcept override { return msg_.c_str(); }

protected:
    void set_msg(std::string&& msg) { msg_ = msg; }

private:
    std::string msg_;
};

struct Missing_mandatory_arg : Opt_parser_error {
    explicit Missing_mandatory_arg(std::string arg) : arg_(std::move(arg)) {
        set_msg(fmt::format(FMT_STRING("argument '{}' is mandatory"), arg_));
    }
    std::string const& arg() const noexcept { return arg_; }

private:
    std::string arg_;
};

struct Duplicated_arg : Opt_parser_error {
    explicit Duplicated_arg(std::string arg) : arg_(std::move(arg)) {
        set_msg(fmt::format(FMT_STRING("duplicated argument '{}'"), arg_));
    }
    std::string const& arg() const noexcept { return arg_; }

private:
    std::string arg_;
};

struct Unknown_arg : Opt_parser_error {
    explicit Unknown_arg(std::string arg) : arg_(std::move(arg)) {
        set_msg(fmt::format(FMT_STRING("unknown argument '{}'"), arg_));
    }
    std::string const& arg() const noexcept { return arg_; }

private:
    std::string arg_;
};

struct Options {
    char const* model_file = nullptr;
    char const* class_file = nullptr;
    char const* input = nullptr;
    char const* output = nullptr;
};

Options parse_cmd_opts(int argc, char const* const* argv);
} // namespace utils

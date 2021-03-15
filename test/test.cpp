#include <array>

#include <spdlog/fmt/fmt.h>
#include <catch2/catch.hpp>

#include <opt_parser/opt_parser.hpp>

TEST_CASE("command line parser") {
    constexpr auto prog = "prog";
    constexpr auto model_file_a = "--model_file";
    constexpr auto model_file_v = "model_file_v";
    constexpr auto class_file_a = "--class_file";
    constexpr auto class_file_v = "class_file_v";
    constexpr auto input_a = "--input";
    constexpr auto input_v = "input_v";
    constexpr auto output_a = "--output";
    constexpr auto output_v = "output_v";

    auto const fmt_arg = [](char const* k, char const* v) noexcept -> std::string {
        return fmt::format(FMT_STRING("{}={}"), k, v);
    };
    auto const model_file = fmt_arg(model_file_a, model_file_v);
    auto const class_file = fmt_arg(class_file_a, class_file_v);
    auto const input = fmt_arg(input_a, input_v);
    auto const output = fmt_arg(output_a, output_v);

    SECTION("output, input, model_file and class_file are mandatory") {
        auto const missing_arg_tests_data = std::array<std::array<char const*, 5>, 4>{
            std::array<char const*, 5>{output_a, prog, model_file.c_str(), input.c_str(), class_file.c_str()},
            std::array<char const*, 5>{input_a, prog, output.c_str(), class_file.c_str(), model_file.c_str()},
            std::array<char const*, 5>{model_file_a, prog, class_file.c_str(), output.c_str(), input.c_str()},
            std::array<char const*, 5>{class_file_a, prog, output.c_str(), input.c_str(), model_file.c_str()},
        };

        for (auto const& m : missing_arg_tests_data) {
            auto const* missing_arg = m[0];
            auto const* argv = &m[1];
            auto const argc = static_cast<int>(m.size()) - 1;
            REQUIRE_THROWS_MATCHES(utils::parse_cmd_opts(argc, argv), utils::Missing_mandatory_arg,
                                   Catch::Matchers::Predicate<utils::Missing_mandatory_arg>(
                                       [missing_arg](auto const& e) {
                                           return e.arg() == missing_arg &&
                                                  std::string(e.what()).find(missing_arg) != std::string::npos;
                                       },
                                       fmt::format(FMT_STRING("should report '{}' missing"), missing_arg)));
        }
    }

    SECTION("having duplicated argument is ill formed") {
        auto const argv = std::array<char const*, 3>{prog, model_file.c_str(), model_file.c_str()};
        REQUIRE_THROWS_MATCHES(utils::parse_cmd_opts(static_cast<int>(argv.size()), argv.data()), utils::Duplicated_arg,
                               Catch::Matchers::Predicate<utils::Duplicated_arg>(
                                   [model_file_a](auto const& e) noexcept {
                                       return e.arg() == model_file_a &&
                                              std::string(e.what()).find(model_file_a) != std::string::npos;
                                   },
                                   fmt::format(FMT_STRING("should report '{}' duplicated"), model_file_a)));
    }

    SECTION("having unknown argument is ill formed") {
        auto const* arg_a = "--unknown";
        auto const arg = fmt_arg(arg_a, "unknown_v");
        auto const argv = std::array<char const*, 2>{prog, arg.c_str()};
        REQUIRE_THROWS_MATCHES(utils::parse_cmd_opts(static_cast<int>(argv.size()), argv.data()), utils::Unknown_arg,
                               Catch::Matchers::Predicate<utils::Unknown_arg>(
                                   [&arg](auto const& e) noexcept {
                                       return e.arg() == arg && std::string(e.what()).find(arg) != std::string::npos;
                                   },
                                   fmt::format(FMT_STRING("should report '{}' unknown"), arg_a)));
    }

    SECTION("empty arguments should report argument missing") {
        REQUIRE_THROWS_AS(utils::parse_cmd_opts(1, std::array<char const*, 1>{prog}.data()),
                          utils::Missing_mandatory_arg);
    }

    SECTION("normal") {
        auto const argv =
            std::array<char const*, 5>{prog, model_file.c_str(), class_file.c_str(), input.c_str(), output.c_str()};
        auto const opt = utils::parse_cmd_opts(static_cast<int>(argv.size()), argv.data());
        REQUIRE(strcmp(opt.output, output_v) == 0);
        REQUIRE(strcmp(opt.input, input_v) == 0);
        REQUIRE(strcmp(opt.model_file, model_file_v) == 0);
        REQUIRE(strcmp(opt.class_file, class_file_v) == 0);
    }

    SECTION("argument like empty value should be considered normal") {
        auto const argv =
            std::array<char const*, 5>{prog, model_file.c_str(), "--class_file=", input.c_str(), output.c_str()};
        auto const opt = utils::parse_cmd_opts(static_cast<int>(argv.size()), argv.data());
        REQUIRE(strcmp(opt.output, output_v) == 0);
        REQUIRE(strcmp(opt.input, input_v) == 0);
        REQUIRE(strcmp(opt.model_file, model_file_v) == 0);
        REQUIRE(strcmp(opt.class_file, "") == 0);
    }

    SECTION("arguments without equal sign should be considered unknown") {
        constexpr auto arg = "--class_file";
        auto const argv = std::array<char const*, 5>{prog, model_file.c_str(), arg, input.c_str(), output.c_str()};
        REQUIRE_THROWS_MATCHES(utils::parse_cmd_opts(static_cast<int>(argv.size()), argv.data()), utils::Unknown_arg,
                               Catch::Matchers::Predicate<utils::Unknown_arg>(
                                   [arg](auto const& e) noexcept {
                                       return e.arg() == arg && std::string(e.what()).find(arg) != std::string::npos;
                                   },
                                   fmt::format(FMT_STRING("should report '{}' unknown"), arg)));
    }
}

#include <array>
#include <cstring>

#include "opt_parser.hpp"

namespace utils {
Options parse_cmd_opts(int argc, char const* const* argv) {
    struct Info {
        char const* arg = nullptr;
        char const* arg_e = nullptr;
        char const** value = nullptr;
    };

    auto opt = Options();

    auto const info = std::array<Info, 4>{
        Info{.arg = "--output", .arg_e = "--output=", .value = &opt.output},
        Info{.arg = "--input", .arg_e = "--input=", .value = &opt.input},
        Info{.arg = "--model_file", .arg_e = "--model_file=", .value = &opt.model_file},
        Info{.arg = "--class_file", .arg_e = "--class_file=", .value = &opt.class_file},
    };

    for (auto n = 1; n < argc; ++n) {
        bool found = false;
        for (auto const& i : info) {
            if (strncmp(argv[n], i.arg_e, strlen(i.arg_e)) == 0) {
                if (*i.value != nullptr) {
                    throw Duplicated_arg(i.arg);
                }
                *i.value = argv[n] + strlen(i.arg_e);
                found = true;
                break;
            }
        }
        if (!found) {
            throw Unknown_arg(argv[n]);
        }
    }

    for (auto const& i : info) {
        if (*i.value == nullptr) {
            throw Missing_mandatory_arg(i.arg);
        }
    }

    return opt;
}
} // namespace utils

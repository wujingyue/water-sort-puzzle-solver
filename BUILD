cc_library(
    name = "state",
    srcs = ["state.cc"],
    hdrs = ["state.h"],
    deps = [
        "@com_google_absl//absl/log:check",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "idastar",
    srcs = ["idastar.cc"],
    hdrs = ["idastar.h"],
    deps = [
        ":state",
        "@com_google_absl//absl/log",
    ],
)

cc_library(
    name = "solver",
    srcs = ["solver.cc"],
    hdrs = ["solver.h"],
    deps = [
        ":idastar",
        ":state",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "solver_test",
    srcs = ["solver_test.cc"],
    deps = [
        ":solver",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        ":solver",
    ],
)

cc_library(
    name = "state",
    srcs = ["state.cc"],
    hdrs = ["state.h"],
)

cc_library(
    name = "solver",
    srcs = ["solver.cc"],
    hdrs = ["solver.h"],
    deps = [":state"],
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
    deps = [":solver"],
)

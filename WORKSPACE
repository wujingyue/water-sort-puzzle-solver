load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# https://github.com/google/googletest/releases/tag/v1.13.0
http_archive(
    name = "com_google_googletest",
    urls = ["https://github.com/google/googletest/archive/b796f7d44681514f58a683a3a71ff17c94edb0c1.zip"],
    strip_prefix = "googletest-b796f7d44681514f58a683a3a71ff17c94edb0c1",
)

# https://github.com/abseil/abseil-cpp/releases/tag/20220623.1
http_archive(
    name = "com_google_absl",
    urls = ["https://github.com/abseil/abseil-cpp/archive/8c0b94e793a66495e0b1f34a5eb26bd7dc672db0.zip"],
    strip_prefix = "abseil-cpp-8c0b94e793a66495e0b1f34a5eb26bd7dc672db0",
)

http_archive(
    name = "bazel_skylib",
    urls = ["https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz"],
    sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
)

from conan import ConanFile
from conan.tools.cmake import cmake_layout


class Conan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    default_options = {
        "gtest/*:shared": True,
        "fmt/*:shared": True,
        "spdlog/*:shared": True,
    }

    def requirements(self):
        self.requires("gtest/1.14.0")
        self.requires("fmt/10.2.1")
        self.requires("spdlog/1.14.1")

    def layout(self):
        cmake_layout(self)

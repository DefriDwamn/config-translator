from conan import ConanFile

class maven_deps(ConanFile):
    name = "config-translator"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    requires = [
        "boost/1.86.0",
    ]
    default_options = {
        "boost/*:shared": False,
    }
    generators = "CMakeDeps", "CMakeToolchain"
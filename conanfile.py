import os
from conans import ConanFile, CMake
from conans.errors import ConanException


class XmsinterpConan(ConanFile):
    name = "xmsinterp"
    version = None
    license = "XMSNG Software License"
    url = "https://github.com/Aquaveo/xmsinterp"
    description = "Interpolation library for XMS products"
    settings = "os", "compiler", "build_type", "arch"
    options = {"xms": [True, False]}
    default_options = "xms=False"
    generators = "cmake"
    build_requires = "cxxtest/4.4@aquaveo/stable"
    exports = "CMakeLists.txt", "LICENSE"
    exports_sources = "xmsinterp/*"

    def configure(self):
        # Set verion dynamically using XMS_VERSION env variable.
        self.version = self.env.get('XMS_VERSION', 'master')

        # Raise ConanExceptions for Unsupported Versions
        s_os = self.settings.os
        s_compiler = self.settings.compiler
        s_compiler_version = self.settings.compiler.version

        self.options['xmscore'].xms = self.options.xms

        if s_compiler == "clang" and s_os == 'Linux':
            raise ConanException("Clang on Linux is not supported.")

        if s_compiler == "clang" \
                and s_os == 'Darwin' \
                and s_compiler_version < "9.0":
            raise ConanException("Clang > 9.0 is required for Mac.")

    def requirements(self):

        if self.options.xms:
            self.requires("boost/1.60.0@aquaveo/testing")
            self.requires("xmscore/[>=1.0.13,<1.1.0]@aquaveo/stable")
        else:
            self.requires("boost/1.66.0@conan/stable")
            self.requires("xmscore/[>=1.0.13,<1.1.0]@aquaveo/stable")
            self.requires("pybind11/2.2.2@aquaveo/stable")

    def build(self):
        cmake = CMake(self)
        if self.settings.compiler == 'Visual Studio':
            cmake.definitions["XMS_BUILD"] = self.options.xms
        cmake.definitions["BUILD_TESTING"] = 1
        cmake.configure(source_folder=".")
        cmake.build()

        run_tests = self.env.get('XMS_RUN_TESTS', None)
        if run_tests is not None:
            print("***********(0.0)*************")
            try:
                cmake.test()
            except ConanException:
                raise
            finally:
                if os.path.isfile("TEST-cxxtest.xml"):
                    with open("TEST-cxxtest.xml", "r") as f:
                        for line in f.readlines():
                            no_newline = line.strip('\n')
                            print(no_newline)
                print("***********(0.0)*************")

    def package(self):
        self.copy("*.h", dst="include/xmsinterp", src="xmsinterp")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
        self.copy("license", dst="licenses", ignore_case=True, keep_path=False)

    def package_info(self):
        if self.settings.build_type == 'Debug':
            self.cpp_info.libs = ["xmsinterp_d"]
        else:
            self.cpp_info.libs = ["xmsinterp"]

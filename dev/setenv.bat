@echo off
set XMS_VERSION=99.99.99
set CONAN_VISUAL_VERSIONS=14
set CONAN_REFERENCE=xmsinterp/%XMS_VERSION%
set CONAN_CHANNEL=testing
set CONAN_GCC_VERSIONS=5
set CONAN_ARCHS=x86_64
set CONAN_BUILD_TYPES=Release
set CONAN_USERNAME=aquaveo
set CONAN_PASSWORD=aquaveo
set PYTHON_TARGET_VERSION=3.10
REM export CONAN_UPLOAD="<conanserver>"
set | findstr "XMS"
set | findstr "CONAN"
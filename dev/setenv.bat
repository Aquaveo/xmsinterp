@echo off
set XMS_VERSION=0.0.6
set XMS_RUN_TESTS=1
set CONAN_REFERENCE=xmsinterp/%XMS_VERSION%
set CONAN_USERNAME=aquaveo
set CONAN_CHANNEL=stable
set CONAN_GCC_VERSIONS=5
REM export CONAN_UPLOAD=1
set | findstr XMS
set | findstr CONAN
#!/bin/bash

set -e
set -x

if [[ "$(uname -s)" == 'Darwin' ]]; then
    export PYTHON_BUILD_CONFIGURE_WITH_OPENSSL=1
    brew update || brew update
    brew outdated pyenv || brew upgrade pyenv
    brew install pyenv-virtualenv
    brew install cmake || true

    if which pyenv > /dev/null; then
        eval "$(pyenv init -)"
    fi

    pyenv install 2.7.10
    pyenv virtualenv 2.7.10 conan
    pyenv rehash
    pyenv activate conan
fi

pip install conan --upgrade
pip install conan_package_tools

conan user

export XMSCORE_VERSION="0.0.6"
export XMSCORE_RUN_TESTS="1"
export CONAN_REFERENCE="xmsinterp/${XMSCORE_VERSION}"
export CONAN_USERNAME="aquaveo"
export CONAN_CHANNEL="stable"
export CONAN_GCC_VERSIONS="5"
# export CONAN_UPLOAD="1"
# export CONAN_DOCKER_IMAGE="lasote/conangcc${CONAN_GCC_VERSIONS}"
printenv | grep XMS
printenv | grep CONAN

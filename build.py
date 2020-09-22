import os
from conan.packager import ConanMultiPackager
import time


if __name__ == "__main__":
    # ConanPackageTools
    # See: https://github.com/conan-io/conan-package-tools/blob/develop/README.md
    builder = ConanMultiPackager()
    builder.add_common_builds()

    # Add environment variables to build definitions
    XMS_VERSION = os.environ.get('XMS_VERSION', None)
    python_target_version = os.environ.get('PYTHON_TARGET_VERSION', "3.6")
    release_python = os.environ.get('RELEASE_PYTHON', 'False')
    aquapi_username = os.environ.get('AQUAPI_USERNAME', None)
    aquapi_password = os.environ.get('AQUAPI_PASSWORD', None)
    aquapi_url = os.environ.get('AQUAPI_URL', None)

    for settings, options, env_vars, build_requires, reference in builder.items:
        # General Options
        env_vars.update({
            'XMS_VERSION': XMS_VERSION,
            'VERBOSE': 1,
            'PYTHON_TARGET_VERSION': python_target_version,
            'RELEASE_PYTHON': release_python,
            'AQUAPI_USERNAME': aquapi_username,
            'AQUAPI_PASSWORD': aquapi_password,
            'AQUAPI_URL': aquapi_url,
        })

        # Require c++11 compatibility
        if settings['compiler'] == 'gcc':
            settings.update({
                'compiler.libcxx': 'libstdc++11'
            })
            compiler_version = int(settings['compiler.version'])
            if compiler_version in [5, 6]:
                settings.update({'cppstd': '14'})
            elif compiler_version == 7:
                settings.update({'cppstd': '17'})
        elif settings['compiler'] == 'apple-clang':
            settings.update({'cppstd': 'gnu17'})
        elif settings['compiler'] == 'Visual Studio':
            settings.update({'cppstd': '17'})

    pybind_updated_builds = []
    for settings, options, env_vars, build_requires, reference in builder.items:
        # pybind option
        if (not settings['compiler'] == "Visual Studio" \
                     or int(settings['compiler.version']) > 12) \
                and settings['arch'] == "x86_64" and settings['build_type'] != 'Debug':
            pybind_options = dict(options)
            pybind_options.update({'xmsinterp:pybind': True})
            pybind_updated_builds.append([settings, pybind_options, env_vars, build_requires])

        pybind_updated_builds.append([settings, options, env_vars, build_requires])
    builder.builds = pybind_updated_builds

    testing_updated_builds = []
    for settings, options, env_vars, build_requires, reference in builder.items:
        # xms option
        if not options.get('xmsinterp:xms', False) and not options.get('xmsinterp:pybind', False):
            testing_options = dict(options)
            testing_options.update({'xmsinterp:testing': True})
            testing_updated_builds.append([settings, testing_options, env_vars, build_requires])
        testing_updated_builds.append([settings, options, env_vars, build_requires])
    builder.builds = testing_updated_builds

    builder.run()

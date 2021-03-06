"""Install the xmsinterp package."""
import os

from setuptools import setup

from xms.interp import __version__


# allow setup.py to be run from any path
os.chdir(os.path.normpath(os.path.join(os.path.abspath(__file__), os.pardir)))

requires = [
    'numpy',
    'xmscore>=4.0.2',
    'xmsgrid>=5.0.5',
]

version = __version__

setup(
    python_requires='>=3.6',
    name='xmsinterp',
    version=version,
    packages=['xms.interp', 'xms.interp.interpolate', 'xms.interp.api'],
    include_package_data=True,
    license='BSD 2-Clause License',
    description='',
    author='Aquaveo',
    install_requires=requires,
    package_data={'': ['*.pyd', '*.so']},
    test_suite="tests",
)

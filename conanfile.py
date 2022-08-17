"""
conanfile.py for the xmsinterp project.
"""
from xmsconan.xms_conan_file import XmsConanFile


class XmsinterpConanFile(XmsConanFile):
    """
    XmsinterpConan class used for defining the conan info.
    """
    name = 'xmsinterp'
    url = 'https://github.com/Aquaveo/xmsinterp'
    description = 'Interpolation library for XMS products'
    xms_dependencies = [
        "xmscore/[>=6.0.1 <7.0.0]@aquaveo/stable",
        "xmsgrid/[>=7.0.1 <9.0.0]@aquaveo/stable",
    ]

LIBRARY_NAME = XmsinterpConanFile.name

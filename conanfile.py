"""
conanfile.py for the xmsinterp project.
"""
from xmsconan.xms_conan_file import XmsConanFile


class XmscoreConanFile(XmsConanFile):
    """
    XmscoreConan class used for defining the conan info.
    """
    name = 'xmscore'
    url = 'https://github.com/Aquaveo/xmscore'
    description = 'Support library for XMS products'
    xms_dependencies = [
        "xmscore/[>=6.0.1 <7.0.0]@aquaveo/stable",
        "xmsgrid/[>=7.0.1 <8.0.0]@aquaveo/stable",
    ]

LIBRARY_NAME = XmscoreConanFile.name

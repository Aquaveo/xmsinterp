"""
conanfile.py for the xmsinterp project.
"""
from xmsconan.xms_conan2_file import XmsConan2File


class XmsinterpConanFile(XmsConan2File):
    """
    XmsinterpConanFile class used for defining the conan info.
    """
    name = "xmsinterp"
    url = "https://github.com/Aquaveo/xmsinterp"
    description = "Interpolation library for XMS products"
    xms_dependencies = [
        "xmscore/6.2.5-8-gfabf5914",
        "xmsgrid/5.5.3-56-ga088ecb",
    ]
    extra_export_sources = ['test_files']

LIBRARY_NAME = XmsinterpConanFile.name

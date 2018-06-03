# -*- coding: utf-8 -*-
#
# Copyright (C) 2009-2017 Wander Lairson Costa
# Copyright (C) 2017-2018 Robert Wlodarczyk
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import ctypes
import ctypes.util
import logging
import sys

__all__ = [
            'LibraryException',
            'LibraryNotFoundException',
            'NoLibraryCandidatesException',
            'LibraryNotLoadedException',
            'LibraryMissingSymbolsException',
            'locate_library',
            'load_library',
            'load_locate_library'
]


_LOGGER = logging.getLogger('usb.libloader')


class LibraryException(OSError):
    pass

class LibraryNotFoundException(LibraryException):
    pass

class NoLibraryCandidatesException(LibraryNotFoundException):
    pass

class LibraryNotLoadedException(LibraryException):
    pass

class LibraryMissingSymbolsException(LibraryException):
    pass


def locate_library (candidates, find_library=ctypes.util.find_library):
    """Tries to locate a library listed in candidates using the given
    find_library() function (or ctypes.util.find_library).
    Returns the first library found, which can be the library's name
    or the path to the library file, depending on find_library().
    Returns None if no library is found.

    arguments:
    * candidates   -- iterable with library names
    * find_library -- function that takes one positional arg (candidate)
                      and returns a non-empty str if a library has been found.
                      Any "false" value (None,False,empty str) is interpreted
                      as "library not found".
                      Defaults to ctypes.util.find_library if not given or
                      None.
    """
    if find_library is None:
        find_library = ctypes.util.find_library

    use_dll_workaround = (
        sys.platform == 'win32' and find_library is ctypes.util.find_library
    )

    for candidate in candidates:
        # Workaround for CPython 3.3 issue#16283 / pyusb #14
        if use_dll_workaround:
            candidate += '.dll'

        libname = find_library(candidate)
        if libname:
            return libname
    # -- end for
    return None

def load_library(lib, name=None, lib_cls=None):
    """Loads a library. Catches and logs exceptions.

    Returns: the loaded library or None

    arguments:
    * lib        -- path to/name of the library to be loaded
    * name       -- the library's identifier (for logging)
                    Defaults to None.
    * lib_cls    -- library class. Defaults to None (-> ctypes.CDLL).
    """
    try:
        if lib_cls:
            return lib_cls(lib)
        else:
            return ctypes.CDLL(lib)
    except Exception:
        if name:
            lib_msg = '%s (%s)' % (name, lib)
        else:
            lib_msg = lib

        lib_msg += ' could not be loaded'

        if sys.platform == 'cygwin':
            lib_msg += ' in cygwin'
        _LOGGER.error(lib_msg, exc_info=True)
        return None

def load_locate_library(candidates, cygwin_lib, name,
                        win_cls=None, cygwin_cls=None, others_cls=None,
                        find_library=None, check_symbols=None):
    """Locates and loads a library.

    Returns: the loaded library

    arguments:
    * candidates    -- candidates list for locate_library()
    * cygwin_lib    -- name of the cygwin library
    * name          -- lib identifier (for logging). Defaults to None.
    * win_cls       -- class that is used to instantiate the library on
                       win32 platforms. Defaults to None (-> ctypes.CDLL).
    * cygwin_cls    -- library class for cygwin platforms.
                       Defaults to None (-> ctypes.CDLL).
    * others_cls    -- library class for all other platforms.
                       Defaults to None (-> ctypes.CDLL).
    * find_library  -- see locate_library(). Defaults to None.
    * check_symbols -- either None or a list of symbols that the loaded lib
                       must provide (hasattr(<>)) in order to be considered
                       valid. LibraryMissingSymbolsException is raised if
                       any symbol is missing.

    raises:
    * NoLibraryCandidatesException
    * LibraryNotFoundException
    * LibraryNotLoadedException
    * LibraryMissingSymbolsException
    """
    if sys.platform == 'cygwin':
        if cygwin_lib:
            loaded_lib = load_library(cygwin_lib, name, cygwin_cls)
        else:
            raise NoLibraryCandidatesException(name)
    elif candidates:
        lib = locate_library(candidates, find_library)
        if lib:
            if sys.platform == 'win32':
                loaded_lib = load_library(lib, name, win_cls)
            else:
                loaded_lib = load_library(lib, name, others_cls)
        else:
            _LOGGER.error('%r could not be found', (name or candidates))
            raise LibraryNotFoundException(name)
    else:
        raise NoLibraryCandidatesException(name)

    if loaded_lib is None:
        raise LibraryNotLoadedException(name)
    elif check_symbols:
        symbols_missing = [
                    s for s in check_symbols if not hasattr(loaded_lib, s)
        ]
        if symbols_missing:
            msg = ('%r, missing symbols: %r', lib, symbols_missing )
            _LOGGER.error(msg)
            raise LibraryMissingSymbolsException(lib)
        else:
            return loaded_lib
    else:
        return loaded_lib

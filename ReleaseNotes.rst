==========
PyUSB News
==========

What's new in PyUSB 1.0.0 (beta 2)?
===================================

- You can now customize the search path for USB libraries (by André Erdmann).
- API improvements (more on that below).
- legacy module fully functional.
- Regressions tests for Python 2.4-3.4.

WARNING: API breakage ahead!!!!!
--------------------------------

- `util.get_string` does not receive the length parameter anymore (by
- the `find` and `find_descriptor` functions now return an iterator when
  find_all is true.
- Added the property `extra` for extra descriptors (by Prathmesh Prabhu).
- New function `util.create_buffer`.
- Now `read` and `ctrl_transfer` functions allow inplace reads.
- New method `clear_halt`.
- The functions `is_kernel_driver_active`, `detach_kernel_driver` and
  `attach_kernel_driver` does not accept an Interface object anymore.
- `write` and `read` does not receive the interface number anymore.
- added the properties `vendor`, `product` and `serial_number` to the
  Device class.
- Support for `str` and `repr` conversions for descriptors (by Walker Inman).

What's new in PyUSB 1.0.0 (beta 1)?
===================================

- Isochronous transfer for libusb 1.0 (by David Halter).
- Experimental OpenUSB support.
- Documentation update.
- ``PYUSB_DEBUG_LEVEL`` environment variable is now called ``PYUSB_DEBUG``.
- Legacy module nwo groups according to their *bus*.
- Version information available for apps (by Chris Clark).
- Faster read operation (by themperek).
- Tox support (by ponty).
- Support for port number info (by Stefano Di Martino).
- Several bug fixes (please, check the Changelog file).

Known issues
============

- OpenUSB backend hangs on some control transfers.

TODO
====

- More tests with legacy module.
- Isochronous transfers for libusb-win32.

What's new in PyUSB 1.0.0 (alpha 3)?
====================================

**WARNING**: this release renames the libusb 1.0 and libusb 0.1 backends. If
your code makes direct access to this backends, you will have to change it.

- Fixed several legacy module bugs (by Tormod Volden).
- Fixed libusb0 backend for BSDs and Mac OSX.
- Fixed data loss when less the requested number of bytes were read (by
  Braiden Kindt).
- Documentation fixes.

What's new in PyUSB 1.0.0 (alpha 2)?
====================================

- Test firmware now lives in its own respository (https://github.com/walac/bmfw).
- ``USBError`` now has the property ``backend_error_code`` that tells the
  backend specific error.
- ``errno`` value in ``USBError`` is translated according to the backend error.
- Now ``Device`` class has the ``bus`` and ``address`` attributes to
  differentiate identical devices.
- Optimization when log is disabled (by Emmanuel Blot).
- Several other minor fixes and improvaments (check ChangeLog file).

Features not implemented
------------------------

- OpenUSB support.
- Isochronous transfer.

What's new in PyUSB 1.0.0 (alpha 1)?
====================================

This release implements more PyUSB 1.0 features towards beta stage. The new
features implemented include:

- Standard control requests through usb.control module.
- Request current configuration from device when you do not call
  set_configuration.
- get_string function in the usb.util module to get string descriptors.
- Full 0.4 API emulation.
- Device is not reset anymore in test cases to avoid problems in systems
  where it does not work.

Features not implemented
------------------------

- OpenUSB support.
- Isochronous transfer.

What's new in PyUSB 1.0.0 (alpha 0)?
====================================

This is the first PyUSB 1.0 series public release. This is an alpha release,
which means that most of the features described in the README file and on the
website are not yet stable or even implemented.

Features not implemented
------------------------

- Full support for legacy 0.4 legacy code (although partial support is provided).
- OpenUSB backend.
- libusb 1.0 windows backend stability (although it is reasonable usable).
- Support for several standard control requests (including GET_STRING).
- Python < 2.6 and Python 3 not yet fully tested.

Known issues
------------

- ``reset`` method fails under FreeUSB (libusb 1.0 backend).
- ``reset`` method hangs under Windows (libusb 1.0 backend).
- Sometimes occurs `read` timeout on Windows (libusb 1.0 backend).
- Test cases fail to run under cygwin.

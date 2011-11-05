==========
PyUSB News
==========

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

This is the first PyUSB 1.0 series public release. This is an alpha release, which
means that most of the features described in the README file and on the website are
not yet stable or even implemented.

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

#!/usr/bin/env python
"""\
Run in Termux on Android. Prints device infos for selected device.

See: https://wiki.termux.com/wiki/Termux-usb

Call script in Termux like this:
    termux-usb -r -e ./tools/termux_device_info.py /dev/bus/usb/001/002

With /dev/bus/usb/001/002 being the correct device path as returned
by `termux-usb -l`.

If using a virtual environment then call the python script from
inside a bash script wrapper:

    #!/bin/bash
    source /path/to/venv/bin/activate
    python script.py "$@"
    deactivate

"""

from __future__ import print_function

import usb.core


def main(fd):
    device = usb.core.device_from_fd(fd)
    print(device)


if __name__ == "__main__":
    # grab fd number from args
    #   (from termux wrapper)
    import sys

    fd = int(sys.argv[1])
    main(fd)

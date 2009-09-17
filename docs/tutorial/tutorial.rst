==========================
Programming with PyUSB 1.0
==========================

Introduction
============

PyUSB 1.0 is a Python_ library allowing easy USB_ access. It has the following features:

100% written in Python:
    Unlike the previous version, which is written in C, 1.0 version is written in Python.
    This allows Python programmers with no background in C to understand better how PyUSB
    works.
Backend neutrality:
    1.0 version implements a frontend-backend scheme. This isolates the API from system
    specific implementation details. The glue between the two layers is the ``IBackend``
    interface. PyUSB comes with bultins backends for libusb 0.1, libusb 1.0 and OpenUSB.
    You can write your own backend if you desire to.
Portability:
    PyUSB should run on any platform with Python >= 2.3, ctypes_ and at least one of the
    suppoted USB libraries.
Easiness:
    Communicating with an USB_ device has never been so easy! USB is a complex protocol,
    but PyUSB has good defaults for most common devices.
Support for isochronous transfers:
    PyUSB supports isochronous transfer if the underline backend supports it.

Although PyUSB makes USB programming less painful, it is assumed in this tutorial that
you have a minimal USB protocol background. If you don't know anything about USB, I
recommend you the excellent Jan Axelson's book **USB Complete**.

Enough talk, let's code!
========================

Who's who
---------

First of all, let's do an overview on the PyUSB modules. PyUSB modules are under
the usb package. This package has the following contents:

======= ===========
Content Description
------- -----------
core    The main USB module.
util    Utility functions
legacy  The 0.x compatibility layer
backend A subpackage containing the builtin backends
======= ===========

For example, to import the ``core`` module, you do as so::

    import usb.core
    dev = usb.core.find()

Where are you?
--------------

The ``find()`` function present in the ``core`` module is used to
find and enumerate devices present in the system. For example, let's
say that our device has a vendor id equals to 0xfffe and product id
equals to 0x0001. If we would like to find it, we would do so::

    import usb.core

    dev = usb.core.find(idVendor=0xfffe, idProduct=0x0001)
    if dev is None:
        raise ValueError('Our device is not connected')

Just it, the function will return a ``usb.core.Device`` object representing
our device. It the device is not found, it returns ``None``. Actually, you
can use any field of the Device Descriptor_ you desire. For example, what
if we would like to discover if we find an USB printer connected? This
is far easy::

    # actually this is not the whole history, keep reading
    if usb.core.find(bDeviceClass=7) is None:
        raise ValueError('No printer found')

This 7 is the code for the printer class according to the USB specification.
Hey, wait, what if I want to enumerate all printers present? No problem::

    # this is not the whole history yet...
    printers = usb.core.find(find_all=True, bDeviceClass=7)

    # Python 2, Python 3, to be or not to be
    import sys
    sys.stdout.write('There are ' + len(printers) + ' in the system\n.')

What happend? Well, it is time for a little explanation... ``find``
has a parameter called ``find_all`` that defaults to False. When it is
false [#]_, ``find`` will return the first device found that matches the
specified criteria (more on it soon). If you give it the a true value,
``find`` instead will return a list with all device matching the criteria.
That's it! Simple.

Finished? No! I have not told you the whole history: many devices actually
put its class information in the Interface Descriptor_ instead of the
Device Descriptor_. So, to really find all printers connected to the
system, we would need to transverse all configurations, and then
all interfaces and check if one of the interfaces has its bInterfaceClass
field equals to 7. "I got tired reading this, image implementing it?!?!?!
I am a `programmer <http://en.wikipedia.org/wiki/Laziness>`_", you say.
Yes, I am one two, that's because I have implemented some
stuff to make our lives easier. First, let's give a look on the final
code to find all printers connected::

    import usb.core
    import usb.util
    import sys

    class find_class(object):
        def __init__(self, class_):
            self._class = class_
        def __call__(self, device):
            # first, let's check the device
            if device.bDeviceClass == self._class:
                return True
            # ok, transverse all devices to find an
            # interface that matches our class
            for cfg in device:
                # find_descriptor, what's it?
                intf = usb.util.find_descriptor(
                                            cfg,
                                            bInterfaceClass=self._class
                                    )
                if intf is not None:
                    return True

    printers = usb.core.find(find_all=1, custom_match=find_all(7))

The ``custom_match`` accepts any callable object that receives the device
object. It must return true for a matching device, and false for a non-match
device. You can also combine ``custom_match`` with device fields::

    # find all printers that belongs to our vendor:
    printers = usb.core.find(find_all=1, custom_match=find_all(7), idVendor=0xfffe)

Describe yourself
-----------------

Ok, we've found our device, but before talking to it, we would like
to know more about it, you know, configurations, interfaces, endpoints,
transfer types...

If you have a device, you can access any device descriptor fields as object
properties::

    >>> dev.bLength
    >>> dev.bNumConfigurations
    >>> dev.bDeviceClass
    >>> # ...

To access the configurations present in the device, you can iterate over the
device::

    for cfg in dev:
        sys.stdout.write(str(cfg.bConfigurationValue) + '\n')

In the same way, you can iterate over a configuration to access the interfaces,
and iterator over the interface to access its endpoints. Each kind of object has
as attributes the fields of the respective descriptor. Let's see an example::

    for cfg in dev:
        sys.stdout.write(str(cfg.bConfigurationValue) + '\n')
        for intf in cfg:
            sys.stdout.write('\t' + \
                             str(intf.bInterfaceNumber) + \
                             ',' + \
                             str(intf.bAlternateSetting) + \
                             '\n')
            for ep in intf:
                sys.stdout.write('\t\t' + \
                                 str(ep.bEndpointAddress) + \
                                 '\n')

You can also use the subscript operator to access the descriptors randomly, like that::

    >>> # access the second configuration
    >>> cfg = dev[1]
    >>> # access the first interface
    >>> intf = cfg[(0,0)]
    >>> # third endpoint
    >>> ep = intf[2]

As you can see, the index is zero based. But wait! There is something weird in way
I access an interface. Yes, you are right, the subscript operator in the Configuration
accepts an tuple of two item, with the first one being the index of the Interface and
the second one, the alternate setting. So, access the first interface, but its second
alternate setting, we write ``cfg[(0,1)]``.

Now it's time to we learn a powerfull way to find descriptor, the ``find_descriptor``
utility function. We have already seem it in the printer finding example.
``find_descriptor`` works in almost the same way as ``find``, with two exceptions:

* ``find_descriptor`` receives as its first parameter the parent descriptor that you
  will walk on.
* There is no ``backend`` [#]_ parameter.

For example, if we have a configuration descriptor ``cfg`` and want to find all
alternate setttings of the interface 1, we do so::

    import usb.util
    alt = usb.util.find_descriptor(find_all=True, bInterfaceNumber=1)

Repair that ``find_descriptor`` is in the ``usb.util`` module. It also
accepts the early described ``custom_match`` parameter.

How am I supposed to work?
--------------------------

USB devices after connected must be configured through a few requests. When
I first started to study USB_ spec, I found myself confused with descriptors,
configurations, interfaces, alternate settings, transfer types and all this
stuff... And worst, you cannot simply ignore them, a device does not work
without setting a configuration, even if it has just one! PyUSB tries to
make your life as easy as possible. For example, after getting your device
object, one of the first things you need to do before communicating with it
is issueing a ``set_configuration`` request. The parameter for this request
is the ``bConfigurationValue`` of the configuration you are interested in.
Most devices has no more than one configuration, and tracking the configuration
value to use is annoying (although most code I have seem simply hardcode it).
Therefore, in PyUSB, you can just issue a ``set_configuration`` call with no
parameters. In this case, I will set the first configuration found (if your
device has just one, you don't need to worry about the configuration value
at all). For example, let's you have a device with one configuration descriptor
with its bConfigurationValue field equals to 5 [#]_, the following ways bellow will
work equally::

    >>> dev.set_configuration(5)
    >>> dev.set_configuration() # we assume the configuration 5 is the first one
    >>> cfg = util.find_descriptor(dev, bConfiguration=5)
    >>> cfg.set()
    >>> dev.set_configuration(cfg)

Wow! You can use a ``Configuration`` object as a parameter to ``set_configuration``!
Yes, and also it has a ``set`` method to configure itself as the current configuration.

The other configuration you may have or may not have to do the interface alternate
setting. Ok, a crash course on USB: each device can have only one activated configuration
at a time, and each configuration may have more than one interface, and you can use
all interfaces at the same time. You better understand this concept if you think
of an interface as a logical device. For example, let's imagine a multifunction
printer, which is at the same time a printer and a scanner. To keep things simple
(or at least as simple as we can), let's consider it has just one configuration.
As we have a printer and a scanner, the configuration has two interfaces, one for
the printer and one for the scanner. A device with more than one interface is called
a composite device. When you connect your multifunction printer to your computer,
the Operating System would load two different drivers: one for each "logical"
peripheral you have [#]_.

And about the alternate setting? Good you have asked. An interface have one or
more alternate settings. An interface with just one alternate setting is considered
to not having an alternate settting [#]_. Alternate settings are for interfaces which
configurations are for devices, i.e, for each interface, you can have only one alternate
setting active for each interface. For example, USB_ spec says that a device cannot
have a isochronous endpoint in its primary alternate setting [#]_, so a streaming device
has to have at least two alternate settting, with the second one having the isochronous
endpoint(s). But as opposed to configurations, interfaces with just one alternate
setting don't need to be set [#]_. You select an interface alternate setting
through the ``set_interface_altsetting`` function::

    >>> dev.set_interface_altsetting(interface = 0, alternate_setting = 0)

.. warning::
    The USB spec says that a device is allowed to return an error in case it
    receives a SET_INTERFACE request for an interface that has no additional
    alternate settings. So, if you are not sure if the interface has more
    than one alternate setting or not, the safesty way is wrap the
    ``set_interface_altsetting`` call with a try-except block, like so::

        try:
            dev.set_interface_altsetting(...)
        except USBError:
            pass

You can also use an ``Interface`` object as parameter to the function, the
``interface`` and ``alternate_setting`` parameters are automatically infered
from ``bInterfaceNumber`` and ``bAlternateSetting`` fields. Example::

    >>> intf = find_descriptor(...)
    >>> dev.set_interface_altsetting(intf)
    >>> intf.set_altsetting() # ops, Interface also has a method for it

.. warning::
    The ``Interface`` object must belong to the active configuration descriptor.

.. [#] When I say True or False (capitalized), I mean the respectivelly values of the
       Python language. And when I say true and false, I mean any expression in Python
       which evals to true and false.

.. [#] See backend specific documentation.

.. [#] USB spec does not impose any sequential value to the configuration value. The same
       is true for interface and alternate setting numbers.

.. [#] Actually things are a little more complex, but this simple explanation is enough
       for us.

.. [#] I know it is weird.

.. [#] This is because if there is no bandwidth for isochronous transfer at the device
       configuration time, the device can be successfully enumerated.

.. [#] This does not happen for configurations because a device is allowed to be in a
       unconfigured state.

.. _libusb: http://www.libusb.org
.. _OpenUSB: http://openusb.wiki.sourceforge.net
.. _USB: http://www.usb.org
.. _PyUSB: http://pyusb.wiki.sourceforge.net
.. _Python: http://www.python.org
.. _ctypes: http://docs.python.org/library/ctypes.html
.. _Descriptor: http://www.beyondlogic.org/usbnutshell/usb5.htm

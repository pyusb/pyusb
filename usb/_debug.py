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

__author__ = 'Wander Lairson Costa'

__all__ = ['methodtrace', 'functiontrace']

import logging
import usb._interop as _interop

_enable_tracing = False

def enable_tracing(enable):
    global _enable_tracing
    _enable_tracing = enable

def _trace_function_call(logger, fname, *args, **named_args):
    logger.debug(
                # TODO: check if 'f' is a method or a free function
                fname + '(' + \
                ', '.join((str(val) for val in args)) + \
                ', '.join((name + '=' + str(val) for name, val in named_args.items())) + ')'
            )

# decorator for methods calls tracing
def methodtrace(logger):
    def decorator_logging(f):
        if not _enable_tracing:
            return f
        def do_trace(*args, **named_args):
            # this if is just a optimization to avoid unecessary string formatting
            if logging.DEBUG >= logger.getEffectiveLevel():
                fn = type(args[0]).__name__ + '.' + f.__name__
                _trace_function_call(logger, fn, *args[1:], **named_args)
            return f(*args, **named_args)
        _interop._update_wrapper(do_trace, f)
        return do_trace
    return decorator_logging

# decorator for methods calls tracing
def functiontrace(logger):
    def decorator_logging(f):
        if not _enable_tracing:
            return f
        def do_trace(*args, **named_args):
            # this if is just a optimization to avoid unecessary string formatting
            if logging.DEBUG >= logger.getEffectiveLevel():
                _trace_function_call(logger, f.__name__, *args, **named_args)
            return f(*args, **named_args)
        _interop._update_wrapper(do_trace, f)
        return do_trace
    return decorator_logging

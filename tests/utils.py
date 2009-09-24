import array
import usb.core
import devinfo

# data generation functions
def get_array_data1(length = 10):
    return array.array('B', range(length))
def get_array_data2(length = 10):
    data = range(length)
    data.reverse()
    return array.array('B', data)
def get_list_data1(length = 10):
    return range(length)
def get_list_data2(length = 10):
    data = range(length)
    data.reverse()
    return data
def get_str_data1(length = 10):
    return ''.join([chr(x) for x in range(length)])
def get_str_data2(length = 10):
    data = range(length)
    data.reverse()
    return ''.join([chr(x) for x in data])
def to_array(data):
    return array.array('B', data)

# check if our test hardware is present
def is_test_hw_present():
    try:
        return usb.core.find(idVendor=devinfo.ID_VENDOR,
                             idProduct=devinfo.ID_PRODUCT) is not None
    except:
        return False

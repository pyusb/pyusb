import array

# data generation functions
def get_array_data1(length = 10):
    return array.array('B', range(length))
def get_array_data2(length = 10):
    return array.array('B', reversed(range(length)))
def get_list_data1(length = 10):
    return range(length)
def get_list_data2(length = 10):
    return [x for x in reversed(range(length))]
def get_str_data1(length = 10):
    return ''.join([chr(x) for x in range(length)])
def get_str_data2(length = 10):
    return ''.join([chr(x) for x in reversed(range(length))])
def to_array(data):
    return array.array('B', data)

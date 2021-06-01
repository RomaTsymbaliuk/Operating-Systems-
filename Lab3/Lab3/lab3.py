import os
index_value = 5
#path = '/media/roma/01D5F93CEC804180/DATA'
#path = '/media/roma/01D5F93CEC804180/TESTDIR'
def max_search(dict):
    max = 0
    for object in dict:
        if dict[object]>max:
            max = dict[object]
    return max
start_path = '/media/roma/01D5F93CEC804180/TESTDIR'
count_and_sizes = {}
def processing_directory(path, interval):
    objects = os.listdir(path)
    for object in objects:
        name = path + '/' + object
        if os.path.isfile(name):
            file_size = os.stat(name).st_size
            count = int(file_size / interval)
            if count in count_and_sizes.keys():
                count_and_sizes[count] = count_and_sizes[count] + 1
            else:
                count_and_sizes[count] = 1
        else:
            processing_directory(name, interval)
def graph_draw(interval):
    max = max_search(count_and_sizes)
    scale_value = int(max / index_value)
    dictionary_items = count_and_sizes.items()
    sorted_items = sorted(dictionary_items)
    for i in range(len(list(count_and_sizes.keys()))):
        obj = list(sorted_items[i])
        num = int(obj[1] / scale_value) + 1
        print("Interval (", obj[0]*interval, "    ", (obj[0] + 1)*interval, ")", "*"*num)
        i = i + 1
interval = 1024
processing_directory(start_path, interval)
graph_draw(interval)

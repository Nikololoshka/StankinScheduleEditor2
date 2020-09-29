import os
import json

first_dir = "1 - Original"
second_dir = "3 -Schedules-json"

first_lst = set()
second_lst = set()

def find_files(dir, lst):
    for dirpath, dirnames, filenames in os.walk(dir):
        for filename in filenames:
            if filename.endswith(".json"):
                lst.add(dirpath + os.sep + filename)

def ordered(obj):
    if isinstance(obj, dict):
        return sorted((k, ordered(v)) for k, v in obj.items())
    if isinstance(obj, list):
        return sorted(ordered(x) for x in obj)
    else:
        return obj


find_files(first_dir, first_lst)
find_files(second_dir, second_lst)

first_lst = sorted(first_lst)
second_lst = sorted(second_lst)


for first, second in zip(first_lst, second_lst):
    with open(first, 'r', encoding='utf-8') as first_file, open(second, 'r', encoding='utf-8') as second_file:
        first_json = json.load(first_file)
        second_json = json.load(second_file)

        is_right = ordered(first_json) == ordered(second_json)
        if not is_right:
            print(first, 'and' , second)

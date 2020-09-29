import json    
import os

def resaver_dates(dates):
    new_dates = []
    for old_date in dates:
        new_date = {
            'frequency': old_date['frequency'],
            'date': old_date['date']
        }
        new_dates.append(new_date)

    return new_dates


def resaver(old_path, new_path):
    with open(old_path, 'r', encoding='utf-8') as input_file, open(new_path, 'w', encoding='utf-8') as output_file:
        js = json.load(input_file)
        new_pairs = []

        for pair in js:
            new_pair = {
                'title': pair['title'],
                'lecturer': pair['lecturer'],
                'type': pair['type'],
                'subgroup': pair['subgroup'],
                'classroom': pair['classroom'],
                'time': {
                    'start': pair['time']['start'],
                    'end': pair['time']['end']
                },
                'dates': resaver_dates(pair['dates'])
            }
            new_pairs.append(new_pair)
        
        json.dump(new_pairs, output_file, indent=4, ensure_ascii=False)


postfix = ' (fix)'
input_dir = './Json (11.09.20)'
output_dir = input_dir + postfix


for dirpath, dirnames, filenames in os.walk(input_dir):
    full_dir = output_dir + dirpath.replace(input_dir, '')
    if not os.path.exists(full_dir):
        os.makedirs(full_dir)
        print('Create folders:', full_dir)

    for filename in filenames:
        if str(filename).endswith(".json"):
            resaver(dirpath + os.sep + filename, full_dir + os.sep + filename)
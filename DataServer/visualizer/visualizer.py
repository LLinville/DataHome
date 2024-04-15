from matplotlib import pyplot as plt
import os, glob
import json

lines = []
for filename in glob.glob('*.log'):
    with open(os.path.join(os.getcwd(), filename), 'r') as logfile: # open in readonly mode
        file_lines = logfile.readlines()
        file_lines = [l for l in file_lines if l.strip()]
        file_lines = [line.replace("\'","\"") for line in file_lines]

        print(f"File {filename} contained {len(file_lines)} lines")
        for i,line in enumerate(file_lines):
            try:
                json.loads(''.join(line.split()[1:]))
            except Exception as ex:
                print(f"Failed for file {filename} line {i}:" + line)

        lines += file_lines

#with open(".\\DataServer\\saved_logs\\log1710949805.log") as logfile:
#with open(".\\DataServer\\saved_logs\\log1711037746.log") as logfile:
# with open(".\\DataServer\\saved_logs\\log1711317360.log") as logfile:
# with open(".\\DataServer\\saved_logs\\log1711758499.log") as logfile:
# with open(".\\DataServer\\saved_logs\\log1712941726.log") as logfile:
#     lines = logfile.readlines()

#print(lines)

lines_json = [
    json.loads(''.join(line.split()[1:]))
    for line in lines
]

for line, line_json in zip(lines, lines_json):
    line_json['time'] = int(line.split()[0])

#print([line.split(":")[1][:5] for line in lines])


earliest_time_to_graph = 1712941726

for board_id in [3,2,1]:
    times = [
        float(line_json['time'])
        for line_json in lines_json if line_json['boardId'] == board_id
    ]
    temperatures = [
        float(line_json['temperatureC'])
        for line_json in lines_json if line_json['boardId'] == board_id
    ]
    humidities = [
        float(line_json['humidity'])
        for line_json in lines_json if line_json['boardId'] == board_id
    ]
    # absolute_humidities = humidities = [
    #     float(line_json['absoluteHumidity'])
    #     for line_json in lines_json if line_json['boardId'] == board_id
    # ]

    x, y = [], []

    to_graph = temperatures
    for t, v in zip(times, to_graph):
        if t >= earliest_time_to_graph:
            x.append(t)
            y.append(v)
    plt.scatter(x,y)
plt.show()

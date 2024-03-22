from matplotlib import pyplot as plt

#with open(".\\DataServer\\saved_logs\\log1710949805.log") as logfile:
#with open(".\\DataServer\\saved_logs\\log1711037746.log") as logfile:
with open(".\\DataServer\\saved_logs\\log1711039371.log") as logfile:
    lines = logfile.readlines()

#print(lines)

times = [
    int(line.split()[0])
    for line in lines
]

#print([line.split(":")[1][:5] for line in lines])

temperatures = [
    float(line.split(":")[1][:5])
    for line in lines
]



plt.scatter(times, temperatures)
plt.show()

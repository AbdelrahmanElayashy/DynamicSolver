import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import csv
import numpy as np
import sys
import os

numLoops = int(sys.argv[1])
numEquations = int(sys.argv[2])
model = int(sys.argv[3])
step_size = int(sys.argv[4])
seq_time = float(sys.argv[5])
sch_time = float(sys.argv[6])

name_title = ""
name_graph = "system_error_graph_step" + str(step_size) + "_"
name_graph_speed = "system_speed_graph_step" + str(step_size) + "_"
name_min_file = "system_error_min_step" + str(step_size) + "_"
name_max_file = "system_error_max_step" + str(step_size) + "_"
name_avg_file = "system_error_avg_step" + str(step_size) + "_"
name_folder = "system_result_"


if model == 0:
    name_title = "single-track-model-7"
    name_graph = name_graph + "s7"
    name_folder = name_folder + "s7"
    name_graph_speed = name_graph_speed + "s7"
    name_min_file = name_min_file + "s7" + ".txt"
    name_max_file = name_max_file + "s7" + ".txt"
    name_avg_file = name_avg_file + "s7" + ".txt"
else:
    name_title = "lotka-volterra-" + str(numEquations)
    name_folder = name_folder + "l" + str(numEquations)
    name_graph = name_graph + "l" + str(numEquations)
    name_graph_speed = name_graph_speed + "l" + str(numEquations)
    name_min_file = name_min_file + "l" + str(numEquations) + ".txt"
    name_max_file = name_max_file + "l" + str(numEquations) + ".txt"
    name_avg_file = name_avg_file + "l" + str(numEquations) + ".txt"


current_directory = os.getcwd()
final_directory = os.path.join(current_directory, name_folder)
if not os.path.exists(final_directory):
   os.makedirs(final_directory)


height = [seq_time, sch_time]
bars = ('Scheduler', 'Seq_RK4')
y_pos = np.arange(len(bars))
 

count = 0
error_min = np.zeros(numLoops, dtype = float)
error_max = np.zeros(numLoops, dtype = float)
error_avg = np.zeros(numLoops, dtype = float)
dt = np.zeros(numLoops, dtype = float)

with open("resultScheduler.txt", "r") as scheduler, open("resultSequRK4.txt", "r") as rk4:
    rows1 = csv.reader(scheduler, delimiter=',')
    rows2 = csv.reader(rk4, delimiter= ',')
    for line1, line2, i in zip(rows1, rows2, range(numLoops)):
        dt[i] = line1[0]
        arr1 = np.array(line1[1:numEquations + 1], dtype = float)
        arr2 = np.array(line2[1:numEquations + 1], dtype = float)
        diff = np.abs(arr1 - arr2)
        error_min[i] = np.min(diff)
        error_max[i] = np.max(diff)
        error_avg[i] = np.average(diff)
        count = count + 1

_dir = name_folder + "/"
with open(_dir + name_min_file, "w") as min_file, open(_dir + name_max_file, "w") as max_file, open(_dir + name_avg_file, "w") as avg_file:
    for i in range(count):
        min_file.write( str(error_min[i]) + "\n")
        max_file.write( str(error_max[i]) + "\n")
        avg_file.write( str(error_avg[i]) + "\n")


outlier = numLoops - count

plt.plot(dt[:-outlier], error_min[:-outlier], label='error_min', color = 'green', linewidth=2)
plt.plot(dt[:-outlier], error_max[:-outlier], label='error_max', color = 'red', linewidth=2)
plt.plot(dt[:-outlier], error_avg[:-outlier], label='error_avg', color = 'blue', linewidth=2)

plt.xlabel('time')
plt.ylabel('error')
plt.title(name_title)
plt.legend()
plt.savefig(_dir + name_graph)

plt.clf()

plt.bar(y_pos, height, color = ['green', 'red'])
plt.xticks(y_pos, bars)
plt.ylabel('speed in minute')
plt.title(name_title)
plt.savefig(_dir + name_graph_speed)







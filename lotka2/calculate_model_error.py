import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import csv
import numpy as np
import time
import sys
import os


scheduler_file = str(sys.argv[1])
seq3_file = str(sys.argv[2])
seq5_file = str(sys.argv[3])
numLoops = int(sys.argv[4])
numEquations = int(sys.argv[5])
step_size = float(sys.argv[6])
model = float(sys.argv[7])



name_title = ""
name_graph = "system_error5_graph_step" + str(step_size) + "_"
name_min_file = "system_error5_min_step" + str(step_size) + "_"
name_max_file = "system_error5_max_step" + str(step_size) + "_"
name_avg_file = "system_error5_avg_step" + str(step_size) + "_"
name_folder = "system_result_error_"


if model == 0:
    name_title = "single-track-model-7 with a precision of " + str(step_size) 
    name_graph = name_graph + "s7"
    name_folder = name_folder + "s7"
    name_min_file = name_min_file + "s7" + ".txt"
    name_max_file = name_max_file + "s7" + ".txt"
    name_avg_file = name_avg_file + "s7" + ".txt"
else:
    name_title = "lotka-volterra-" + str(numEquations) + " with precision of " + str(step_size)
    name_folder = name_folder + "l" + str(numEquations)
    name_graph = name_graph + "l" + str(numEquations)
    name_min_file = name_min_file + "l" + str(numEquations) + ".txt"
    name_max_file = name_max_file + "l" + str(numEquations) + ".txt"
    name_avg_file = name_avg_file + "l" + str(numEquations) + ".txt"


current_directory = os.getcwd()
final_directory = os.path.join(current_directory, name_folder)
if not os.path.exists(final_directory):
   os.makedirs(final_directory)
   
jmp_st3 = int(0.001 / 0.00001)
jmp_st5 = int(step_size / 0.00001) if int(step_size / 0.00001) >= 1 else int(0.00001 / step_size)
check_side = 1 if int(step_size / 0.00001) >= 1 else 0

count5 = 0
error5_min = np.zeros(numLoops, dtype = float)
error5_max = np.zeros(numLoops, dtype = float)
error5_avg = np.zeros(numLoops, dtype = float)
dt5 = np.zeros(numLoops, dtype = float)
error3_min = np.zeros(numLoops, dtype = float)
error3_max = np.zeros(numLoops, dtype = float)
error3_avg = np.zeros(numLoops, dtype = float)

with open(scheduler_file, "r") as scheduler, open(seq5_file, "r") as rk4, open(seq3_file, "r") as rk43:
    rows1 = csv.reader(scheduler, delimiter=',')
    rows2 = csv.reader(rk4, delimiter= ',')
    rows3 = csv.reader(rk43, delimiter= ',')
    if check_side == 1 :
        auto = [line for i, line in enumerate(rows2) if i % jmp_st5 == 0] 
    else:
        auto = [lin for j, lin in enumerate(rows1) if j % jmp_st5 == 0]
    rk4.seek(0)
    auto3 = [lin1 for a, lin1 in enumerate(rows2) if a % jmp_st3 == 0]
    _zipped =  zip(rows1, auto, auto3, range(numLoops)) if check_side == 1 else zip(auto, rows2, auto3, range(numLoops))
    for line1, line2, line3, i in _zipped:
        dt5[i] = line1[0]
        arr1 = np.array(line1[1:numEquations + 1], dtype = float)
        arr2 = np.array(line2[1:numEquations + 1], dtype = float)
        arr3 = np.array(line3[1:numEquations + 1], dtype = float)
        diff_sch = np.abs(arr2 - arr1)
        diff_seq3 = np.abs(arr2 - arr3)
        ###############################################
        arr1.fill(-0.5)
        arr3.fill(-0.5)
        np.divide(diff_sch, arr2, out=arr1, where=arr2!=0)
        np.divide(diff_seq3, arr2, out=arr3, where=arr2!=0)
        ################################################
        error5_min[i] = np.min(arr1) #scheduler
        error5_max[i] = np.max(arr1)
        error5_avg[i] = np.average(arr1)
        error3_min[i] = np.min(arr3)
        error3_max[i] = np.max(arr3)
        error3_avg[i] = np.average(arr3)
        count5 = count5 + 1



_dir = name_folder + "/"
with open(_dir + name_min_file, "w") as min_file, open(_dir + name_max_file, "w") as max_file, open(_dir + name_avg_file, "w") as avg_file:
    for i in range(count5):
        min_file.write( str(error5_min[i]) + "\n")
        max_file.write( str(error5_max[i]) + "\n")
        avg_file.write( str(error5_avg[i]) + "\n")


outlier = numLoops - count5

plt.plot(dt5[:-outlier], error5_min[:-outlier], label='error_min Scheduler', color = 'limegreen', linewidth=1)
plt.plot(dt5[:-outlier], error5_max[:-outlier], label='error_max Scheduler', color = 'red', linewidth=1)
plt.plot(dt5[:-outlier], error5_avg[:-outlier], label='error_avg Scheduler', color = 'blue', linewidth=1)

plt.plot(dt5[:-outlier], error3_min[:-outlier], label='error_min 10^-3', color = 'darkgreen', linewidth=1)
plt.plot(dt5[:-outlier], error3_max[:-outlier], label='error_max 10^-3', color = 'darkred', linewidth=1)
plt.plot(dt5[:-outlier], error3_avg[:-outlier], label='error_avg 10^-3', color = 'darkblue', linewidth=1)

plt.xlabel('time')
plt.ylabel('error compTo 10^-5')
plt.title(name_title)
plt.legend()
plt.savefig(_dir + name_graph + ".png")
plt.clf()


##################################################################################################################################
######################################Solving with 10^-3##########################################################################


name_title = ""
name_graph = "system_error3_graph_step" + str(step_size) + "_"
name_min_file = "system_error3_min_step" + str(step_size) + "_"
name_max_file = "system_error3_max_step" + str(step_size) + "_"
name_avg_file = "system_error3_avg_step" + str(step_size) + "_"
name_folder = "system_result_error_"



if model == 0:
    name_title = "single-track-model-7 with a precision of " + str(step_size) 
    name_graph = name_graph + "s7"
    name_folder = name_folder + "s7"
    name_min_file = name_min_file + "s7" + ".txt"
    name_max_file = name_max_file + "s7" + ".txt"
    name_avg_file = name_avg_file + "s7" + ".txt"
else:
    name_title = "lotka-volterra-" + str(numEquations) + " with precision of " + str(step_size)
    name_folder = name_folder + "l" + str(numEquations)
    name_graph = name_graph + "l" + str(numEquations)
    name_min_file = name_min_file + "l" + str(numEquations) + ".txt"
    name_max_file = name_max_file + "l" + str(numEquations) + ".txt"
    name_avg_file = name_avg_file + "l" + str(numEquations) + ".txt"


   


_dir = name_folder + "/"
with open(_dir + name_min_file, "w") as min_file, open(_dir + name_max_file, "w") as max_file, open(_dir + name_avg_file, "w") as avg_file:
    for i in range(count5):
       min_file.write( str(error3_min[i]) + "\n")
       max_file.write( str(error3_max[i]) + "\n")
       avg_file.write( str(error3_avg[i]) + "\n")






















    
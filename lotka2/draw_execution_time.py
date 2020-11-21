import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import csv
import numpy as np
import time
import sys
import os

#python draw_execution_time.py numTest sch1..schn seq1 seq2 model step_size numEquations

numTest = int(sys.argv[1])
model = int(sys.argv[numTest + 4])
step_size = float(sys.argv[numTest + 5])
numEquations = int(sys.argv[numTest + 6])

name_title = ""
name_graph_speed = "system_speed_graph_" + str(step_size) + "_"
name_folder = "system_result_speed_"

if model == 0:
    name_title = "single-track-model-7 with a precision of " + str(step_size) 
    name_folder = name_folder + "s7"
    name_graph_speed = name_graph_speed + "s7"
else:
    name_title = "lotka-volterra-" + str(numEquations) + " with precision of " + str(step_size)
    name_folder = name_folder + "l" + str(numEquations)
    name_graph_speed = name_graph_speed + "l" + str(numEquations)


current_directory = os.getcwd()
final_directory = os.path.join(current_directory, name_folder)
if not os.path.exists(final_directory):
   os.makedirs(final_directory)

scheduler = []
seq1 = []
seq2 = []


for i in range(numTest):
    scheduler.append(float(sys.argv[i+2]))

seq1.append(float(sys.argv[numTest + 2]) -1)
seq1.append(float(sys.argv[numTest + 2]) +1)
seq1.append(float(sys.argv[numTest + 2]))
seq2.append(float(sys.argv[numTest + 3]) -1)
seq2.append(float(sys.argv[numTest + 3]) +1)
seq2.append(float(sys.argv[numTest + 3]))

_dir = name_folder + "/"

plt.title(name_title)
plt.ylabel('time in minute')

plt.gca().yaxis.grid(True)

box_plot_data=[scheduler, seq1, seq2]
colors = ['cyan', 'lightblue', 'lightgreen']
plt.boxplot(box_plot_data, patch_artist=True,labels=['Scheduler','SeqRK4 10^-3','SeqRK4 10^-5'])
plt.savefig(_dir + name_title + ".png")







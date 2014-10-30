__author__ = 'xelmirage'
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import string


datafile = open('converted.txt')
list_of_all_the_lines = datafile.readlines( )

fig = plt.figure()
fig.set_size_inches(8, 6)
ax = fig.add_subplot(111, projection='3d')


for line in list_of_all_the_lines:
    list=line.split()
    x=float(list[4])
    y=float(list[3])
    z=float(list[5])
    ax.scatter(x,y,z,s=1,marker='.',color='r')

ax.set_xlabel('LATITUDE')
ax.set_ylabel('LONGITUDE')
ax.set_zlabel('ALTITUDE')

plt.show()


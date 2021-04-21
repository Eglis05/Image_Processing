import matplotlib.pyplot as plt
import sys

with open(sys.argv[1], 'r') as f:
    lines = f.readlines()
    x = [i for i in range(len(lines))]
    y = [float(line) for line in lines]

plt.plot(x ,y)
plt.savefig(sys.argv[2])
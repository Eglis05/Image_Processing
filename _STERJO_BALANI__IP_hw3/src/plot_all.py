import matplotlib.pyplot as plt
import sys
import glob

for file in glob.glob("../output/onepiece*.txt"):
    with open(file, 'r') as f:
        lines = f.readlines()
        x = [i for i in range(len(lines))]
        y = [float(line) for line in lines]

    plt.plot(x ,y, label = file[file.rfind("/")+1:])

plt.legend()
plt.savefig(sys.argv[1])
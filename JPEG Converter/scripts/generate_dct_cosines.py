import math

for k in range(8):
    for n in range(8):
        val = math.cos((math.pi*(2*n+1)*k)/16)
        print(int(val * 256))  # Q8.8 format
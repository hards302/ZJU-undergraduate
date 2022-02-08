#%%
import numpy as np
import numpy.random as rd
import matplotlib.pyplot as plt
import subprocess
import time
# %%
def sample_gen(N,diff,filename="test.txt"):
    with open(filename,"w") as f:
        f.write("{} {}\n".format(N,diff))
        NUM_RANGE = int(1e5)
        f.write(" ".join(map(str,rd.randint(1,NUM_RANGE,size=N))))
    
#%%
def get_time(N,diff,filename="test.txt"):
    sample_gen(N,diff,filename)
    _begin_time = time.time()
    subprocess.run(["./script.sh",filename])
    _end_time = time.time()
    return (_end_time-_begin_time)*1e3  # using ms
# %%
get_time(2000,1000)

# %%
filename = "test.txt"
sample_gen(10,10)
# %%
subprocess.run(["./script.sh",filename])
# %%

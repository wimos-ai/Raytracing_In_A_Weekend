from PIL import Image
import glob
import os 

path = os.getcwd() + "\\*.ppm"
ppm_files = glob.glob(path)

for file in ppm_files:
    Image.open(file).show()

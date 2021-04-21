import numpy as np
import cv2
from skimage.util import random_noise

def noisy(image, type = "gauss", a = 5):
    if type == "gauss":
        blur = cv2.GaussianBlur(image, (a,a), 0) #a = 5
        return blur
    else: #type = "s&p"
        noise_img = random_noise(image, mode='s&p',amount=a) #a = 0.3
        noise_img = np.array(255*noise_img, dtype = 'uint8')
        return noise_img
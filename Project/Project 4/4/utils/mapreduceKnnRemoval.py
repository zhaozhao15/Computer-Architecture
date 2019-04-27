import numpy as np
import sys
import pdb
from pyspark import SparkContext

def knnRemoval(image, k, kernelSize, distance_thr):

    sc = SparkContext("local","knnRemoval")

    halfSize = int(kernelSize/2)
    angle_width_per_pixel = np.pi*2/image.shape[1]
    angle_height_per_pixel = np.pi*2/image.shape[0]
    x_grid, y_grid = np.meshgrid(list(range(kernelSize)), list(range(kernelSize)))
    cnt = np.zeros((1),dtype=np.uint32)
    def knnRemovalValue(image, y, x,  k, kernelSize, distance_thr):
        center_depth = image[y,x]   
        if (center_depth == 0):
            return (0.,0)
        depth_squared = center_depth**2
        width_per_pixel = np.sqrt( 2*depth_squared - (np.cos(angle_width_per_pixel)*2*depth_squared) )
        height_per_pixel = np.sqrt( 2*depth_squared - (np.cos(angle_height_per_pixel)*2*depth_squared) )
        kernel = np.abs(image[y-halfSize:y+halfSize+1, x-halfSize:x+halfSize+1] - center_depth)\
                 + np.abs(width_per_pixel * (x_grid-halfSize))\
                + np.abs(height_per_pixel * (y_grid-halfSize))
        sorted_kernel = np.sort(kernel.reshape(-1))
        sorted_kernel = sorted_kernel[np.where(sorted_kernel!=0)]
        mean = np.mean(sorted_kernel[:k])#a little bit different from cpp version here
        if(mean > distance_thr):
            cnt[0] += 1
            return (0.,1)
        return (image[y,x],0)
    
    filtered_image = np.zeros(image.shape, dtype=image.dtype)

    array = []
    for i in range(halfSize, image.shape[0] - halfSize):
        for j in range(halfSize, image.shape[1] - halfSize):
            array.append([i,j])

    def foo(pos):
        i = pos[0]
        j = pos[1]
        return knnRemovalValue(image, i, j, k, kernelSize, distance_thr)

    x = sc.parallelize(array)
    y = x.map(foo)
    z = np.array(y.collect())

    totalcount = 0
    mycount = 0;
    for i in range(halfSize,image.shape[0] - halfSize):
        for j in range(halfSize, image.shape[1] - halfSize):
            filtered_image[i,j] = z[mycount][0]
            totalcount = totalcount + z[mycount][1]
            mycount = mycount+1

    sc.stop()

    return filtered_image, totalcount

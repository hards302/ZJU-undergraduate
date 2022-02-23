import cv2
import numpy as np
from numpy import linalg as LA, uint64

# 2-dim Gauss weight
def gauss_weight(sigma):
    weight = []
    for x in range(-1, 2):
        for y in range(-1, 2):
            weight.append(1./(2*np.pi*(sigma**2))*np.e**(-(x**2+y**2)/2*(sigma**2)))
    return weight

# local maximum
def local_maximum(R, x, y, r, w, h):
    for i in range(max(0, x-r), min(w, x+r)):
        for j in range(max(0, y-r), min(h, y+r)):
            if R[i][j] > R[x][y]:
                return False
    return True

# harris corner
def harris_corner(img):
    H = img.shape[0]
    W = img.shape[1]

    # rgb to gray
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # get gradient Ix, Iy
    Ix = cv2.Sobel(gray, cv2.CV_64F, 1, 0, ksize=3)
    Iy = cv2.Sobel(gray, cv2.CV_64F, 0, 1, ksize=3)

    # init
    max_img = np.zeros((H-2, W-2),dtype=np.uint8)
    min_img = np.zeros((H-2, W-2),dtype=np.uint8)
    R_img = np.zeros((H-2, W-2),dtype=np.uint8)
    max_lambda_ = [[0 for x in range(H-2)] for y in range(W-2)]
    min_lambda_ = [[0 for x in range(H-2)] for y in range(W-2)]
    R = [[0 for x in range(H-2)] for y in range(W-2)]
    k = 0.05
    weight = gauss_weight(1.0)

    # calculate M
    for x in range(1, W-1):
        for y in range(1, H-1):
            M = np.array([[0., 0.], [0., 0.]])
            for i in range(x-1, x+2):
                for j in range(y-1, y+2):
                    M += weight[(i-(x-1))*(j-(y-1))+(j-(y-1))]*np.array([[(Ix[j][i])**2, Ix[j][i]*Iy[j][i]], [Ix[j][i]*Iy[j][i], (Iy[y][x])**2]])
            lambda_, _ = LA.eig(M)
            min_lambda_[x-1][y-1] = min(lambda_)
            max_lambda_[x-1][y-1] = max(lambda_)
            R[x-1][y-1] = min(lambda_)*max(lambda_)-k*(min(lambda_)+max(lambda_))

    # draw the image
    max1 = max(max(row) for row in min_lambda_)
    max2 = max(max(row) for row in max_lambda_)
    max3 = max(max(row) for row in R)
    for x in range(0, W-2):
        for y in range(0, H-2):
            v1 = int(min_lambda_[x][y]/max1 * 255)
            v2 = int(max_lambda_[x][y]/max2 * 255)
            v3 = int(R[x][y]/max3 * 255)
            min_img[y][x] = v1
            max_img[y][x] = v2
            R_img[y][x] = v3
            if R[x][y] > 0.005*max3 and local_maximum(R, x, y, 5, W-2, H-2):
                cv2.circle(img, (x+1,y+1), 2, (0,0,255), -1)
    # cv2.imshow("min_lambda", min_img)
    # cv2.imshow("max_lambda", max_img)
    cv2.imshow("R_image", R_img)

def readcamera():
    cap = cv2.VideoCapture(0)
    cap.set(3,240)
    cap.set(4,240)
    while cap.isOpened():
        flag, image = cap.read()
        harris_corner(image)
        cv2.imshow("harris_corner", image)
        k = cv2.waitKey(1)
        if k == ord(' '):
            break
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    readcamera()
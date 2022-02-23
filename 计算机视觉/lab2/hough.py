import cv2
import math

def rough(hough_space, width, height, x, y, space):
    thershold = hough_space[x][y]
    if x - space > 0 and x + space < width and y - space > 0 and y + space < height:
        tmp = []
        for i in range(x-space, x + space):
            tmp.append(max(hough_space[i][y-space:y+space]))
        thershold = max(tmp)
    return thershold

def tocanny(img):
    # gray
    gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    # gauss
    gauss = cv2.GaussianBlur(gray, (5,5), 0)
    # canny
    edge = cv2.Canny(gauss, 100, 400)       # if the picture is coin, choose 100,400, otherwise choose 200,400
    cv2.imshow("edge", edge)
    return edge

def hough_line(img, thershold, min_rho):
    # get edge image
    edge = tocanny(img)
    # hough space
    H = edge.shape[0]
    W = edge.shape[1]
    max_rho = int(math.hypot(H,W))
    max_theta = 360
    points = [[[] for i in range(max_theta)] for j in range(max_rho)]
    rho_theta = [[0 for i in range(max_theta)] for j in range(max_rho)]
    # vote
    for i in range(W):
        for j in range(H):
            if edge[j][i] == 0:
                continue
            for theta in range(max_theta):
                rho = int(i*math.cos(theta)+j*math.sin(theta))
                if min_rho < rho < max_rho:
                    rho_theta[rho][theta] += 1
                    points[rho][theta].append((i,j))

    # draw line that over thershold
    for rho in range(max_rho):
        for theta in range(max_theta):
            if rho_theta[rho][theta] >= thershold:
                points[rho][theta].sort()
                x0 = points[rho][theta][0][0]
                y0 = points[rho][theta][0][1]
                x1 = points[rho][theta][-1][0]
                y1 = points[rho][theta][-1][1]
                cv2.line(img, (x0,y0), (x1,y1), color=(0,0,255), thickness=2)
    cv2.imshow("hough_line", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def hough_circle(img, threshold, min_r, max_r):
    # get edge image
    edge = tocanny(img)
    # get G_mag
    dx = cv2.Sobel(edge, cv2.CV_64F, 1, 0, ksize=7)
    dy = cv2.Sobel(edge, cv2.CV_64F, 0, 1, ksize=7)
    # hough space
    H = edge.shape[0]
    W = edge.shape[1]
    centers = [[[] for i in range(H)] for j in range(W)]
    hough_space = [[0 for i in range(H)] for j in range(W)]
    # vote
    for i in range(W):
        for j in range(H):
            if edge[j][i] == 0:
                continue
            if dx[j][i] != 0:
                tan_theta = dy[j][i]/dx[j][i]
                for a in range(W):
                    b = int(a*tan_theta - i*tan_theta + j)
                    if 0 <= b < H:
                        if min_r <= math.hypot(a-i, b-j) <= max_r:
                            centers[a][b].append((i,j))
                            hough_space[a][b] += 1
            if dx[j][i] == 0 and dy[j][i] != 0:
                if dy[j][i] > 0:
                    for b in range(j+min_r, j+max_r+1):
                        if 0 <= b < H:
                            centers[i][b].append((i,j))
                            hough_space[i][b] += 1
                else:
                    for b in range(j-max_r, j-min_r-1):
                        if 0 <= b < H:
                            centers[i][b].append((i,j))
                            hough_space[i][b] += 1

    # draw the circle that over threshold
    for a in range(W):
        for b in range(H):
            if hough_space[a][b] > threshold:
                # for k in centers[a][b]:
                #     cv2.circle(img, k, 2, (255, 0, 0))
                if hough_space[a][b] == rough(hough_space, W, H, a, b, 50):
                    r = [[]]
                    area = [[0,0,0,0]]
                    for i in range(len(centers[a][b])):
                        x = centers[a][b][i][0] - a
                        y = centers[a][b][i][1] - b
                        radius = int(math.hypot(x,y))
                        flag = 0
                        for i in range(len(r)):
                            if len(r[i]) == 0 or r[i][0] - 30 <= radius <= r[i][0] + 30:
                                r[i].append(radius)
                                if x >= 0 and y >= 0:
                                    area[i][0] += 1
                                if x >= 0 and y < 0:
                                    area[i][1] += 1
                                if x < 0 and y >= 0:
                                    area[i][2] += 1
                                if x < 0 and y < 0:
                                    area[i][3] += 1
                                flag = 1
                                break
                        if flag == 0:
                            r.append([radius])
                            area.append([0,0,0,0])
                    hold = threshold//4
                    for i in range(len(r)):
                        if area[i][0] >= hold and area[i][1] >= hold and area[i][2] >= hold and area[i][3] >= hold: # if the picture is seal, let hold = 1
                            realr = sum(r[i])//len(r[i])
                            if min_r <= radius <= max_r:
                                cv2.circle(img, (a,b), realr, (0,0,255), thickness=2)

    cv2.imshow("circle_detection", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == "__main__":
    path = "pic/coin.jpg"
    img = cv2.imread(path)
    cv2.imshow("coin",img)
    # hough_line(img, 200, 20)
    hough_circle(img, 4, 10, 110)     # if the picture is seal, choose 
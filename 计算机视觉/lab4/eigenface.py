import cv2
import numpy as np
import matplotlib.pyplot as plt

PATH = "lab4/data/"

def mytrain(percent):
    print("Start training...")
    face2vec = []
    num = int(10*percent)
    for i in range(0, 41):
        for j in range(1, num+1):
            face_path = f'{PATH}s{i}/{j}.pgm'
            img = cv2.imread(face_path)
            # normalization
            img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            # Normalization
            img_norm = cv2.equalizeHist(img_gray)
            h, w = img_norm.shape
            # img to vec
            img_array = img_norm.reshape(h * w, 1)
            face2vec.append(img_array)
    # get average face
    ave_face = np.mean(face2vec, axis=0)
    ave_img = ave_face.reshape((h,w))
    cv2.imwrite("lab4/result/ave_face.jpg", ave_img)
    # get cov matrix
    cov = np.zeros((h*w, h*w))
    midres = []
    for face in face2vec:
        midres.append(face-ave_face)
    for face in midres:
        cov += np.transpose(face)*face
    cov = cov/len(midres)
    val,vec = np.linalg.eig(cov)
    val_vec = []
    for i in range(len(val)):
        val_vec.append((val[i].real, i))
    # sort by eigenval
    val_vec.sort(reverse=True, key=lambda e: e[0])
    # get first 100 eigenvec
    eigvec = []
    for i in range(100):
        eigvec.append(vec[:, val_vec[i][1]].real)
    # get first 10 eigenvec and show it
    eigimg = []
    plt.figure()
    for i in range(0, 10):
        eigimg.append(np.array(eigvec[i]).reshape(h,w))
        cv2.normalize(eigimg[i], eigimg[i], 0, 255, cv2.NORM_MINMAX)
        plt.subplot(2, 5, i+1)
        plt.imshow(eigimg[i], cmap=plt.cm.gray)
        plt.xticks(())
        plt.yticks(())
    plt.savefig(f"lab4/result/eigenimg10.jpg")
    plt.show()
    # save the eigenvec to file for recognize and restructe
    np.savetxt("lab4/eigenvec/eigenvec.txt", np.array(eigvec), fmt="%s")

def mytest(facename, pcs):
    print("Start testing...")
    model = np.loadtxt("lab4/eigenvec/eigenvec.txt")
    model = model[0:(pcs-1)]

    # recongnize
    test_face = cv2.imread(facename)
    test_gray = cv2.cvtColor(test_face, cv2.COLOR_BGR2GRAY)
    test_norm = cv2.equalizeHist(test_gray)
    h, w = test_norm.shape
    test_vec = test_norm.reshape(h*w, 1)
    y = np.matmul(model, test_vec)

    # find the most similar face(the minimum distance of the images)
    mindis = -1
    result_path = ''
    for i in range(0, 41):
        person_path = f'{PATH}s{i}'
        for j in range(1, 6):
            face_path = f'{person_path}/{j}.pgm'
            face = cv2.imread(face_path)
            face_gray = cv2.cvtColor(face, cv2.COLOR_BGR2GRAY)
            face_norm = cv2.equalizeHist(face_gray)
            face_vec = face_norm.reshape(h*w, 1)
            face_y = np.matmul(model, face_vec)

            # calculate the distance
            dis = np.linalg.norm(y-face_y)
            if mindis == -1:
                mindis = dis
                result_path = face_path
                result_face = face_gray
            else:
                if dis < mindis:
                    mindis = dis
                    result_path = face_path
                    result_face = face_gray
    # if the recongnize is correct
    iscorrect = False
    _, _, result_person, _ = result_path.split('/')
    _, _, test_person, _ = facename.split('/')
    if result_person == test_person:
        iscorrect = True
    
    # show the orign face
    plt.figure()
    plt.subplot(1,3,1)
    plt.imshow(test_norm, cmap=plt.cm.gray)
    plt.title("origin face")
    plt.xticks(())
    plt.yticks(())

    # show the result face and the add face
    plt.subplot(1,3,2)
    plt.imshow(result_face, cmap=plt.cm.gray)
    plt.title("result face")
    plt.xticks(())
    plt.yticks(())

    addface = test_gray + result_face
    cv2.normalize(addface, addface, 0, 255, cv2.NORM_MINMAX)
    plt.subplot(1,3,3)
    plt.imshow(addface, cmap=plt.cm.gray)
    plt.title("add face")
    plt.xticks(())
    plt.yticks(())

    # plt.show()
    plt.close()

    return iscorrect

def recongize_one():
    print("recongize one face")
    print("please input the facename:(e.g s1/1)")
    facename = input()
    facepath = PATH+facename+".pgm"
    isc = mytest(facepath, 10)
    print(isc)

def getrank_1():
    print("get the recongnize rank_1 rate")
    rank_1 = []
    for pcs in [10, 25, 50, 100]:
        correct_count = 0
        for i in range(0, 41):
            for j in range(6, 11):
                face_path = f'{PATH}s{i}/{j}.pgm'
                if mytest(face_path, pcs) == True:
                    correct_count += 1
        print(f'pcs:{pcs} correct num:{correct_count}/205 correct rate:{correct_count/205}')
        rank_1.append(correct_count/205)
    x = [10,25,50,100]
    plt.plot(x, rank_1, label='rank_1')
    plt.xlabel('PCs')
    plt.ylabel('rank_1 rate')
    plt.title('rank_1')
    plt.legend()
    plt.show()
    plt.close()

def myreconstruct(face_path, pcs):
    print("Start reconstructing")
    model = np.loadtxt("lab4/eigenvec/eigenvec.txt")
    model = model[0:(pcs-1)]

    face = cv2.imread(face_path)
    face_gray = cv2.cvtColor(face, cv2.COLOR_BGR2GRAY)
    face_norm = cv2.equalizeHist(face_gray)
    h, w = face_norm.shape
    face_vec = face_norm.reshape(h*w, 1)
    ave_face = cv2.imread("lab4/result/ave_face.jpg")
    ave_face = cv2.cvtColor(ave_face, cv2.COLOR_BGR2GRAY)
    ave_vec = ave_face.reshape(h*w, 1)
    f = face_vec.astype('float64')-ave_vec.astype('float64')
    face_y = np.matmul(model, f).astype('float64')
    face_f = np.matmul(model.T, face_y).astype('float64')
    face_f = face_f.reshape(h,w)
    face_f += ave_face
    cv2.normalize(face_f, face_f, 0, 255, cv2.NORM_MINMAX)
    
    return face_f

def reconstruct(face_path):
    face_name = PATH+face_path+".pgm"
    ori = cv2.imread(face_name)
    face_pcs = []
    p = [10, 25, 50, 100]
    for pcs in p:
        face = myreconstruct(face_name, pcs)
        face_pcs.append(face)
    plt.figure()
    plt.subplot(1, 5, 1)
    plt.imshow(ori, cmap=plt.cm.gray)
    plt.title("origin face")
    plt.xticks(())
    plt.yticks(())
    for i in range(4):
        plt.subplot(1, 5, i+2)
        plt.imshow(face_pcs[i], cmap=plt.cm.gray)
        plt.title(str(p[i])+"PCs")
        plt.xticks(())
        plt.yticks(())
    plt.show()
    plt.savefig("lab4/result/reconstruct.jpg")

if __name__ == "__main__":
    print("The process for eigenface!")
    print("Here are 3 subprocess for: train,recognize,reconstruct")
    print("You can input 1,2,3 to choose which to run. 1 for train, 2 for recognize, 3 for reconstruct")
    processnum = int(input())
    if processnum == 1:
        print("Step training")
        print("please input train data precent:(e.g 0.5)")
        percent = float(input())
        mytrain(percent)
    else:
        if processnum == 2:
            print("Step recognsize")
            print("input 1 for recongize one face and input 2 for rank_1 rate")
            print("please input:(e.g 1)")
            choice = int(input())
            if choice == 1:
                recongize_one()
            else:
                if choice == 2:
                    getrank_1()
        else:
            print("Step reconstruct")
            print("input face path to get reconstruct face:(e.g s1/1)")
            face_path = input()
            reconstruct(face_path)

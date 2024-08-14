import cv2
import os
import sys
import numpy
import matplotlib.pyplot as plt
from enhance import image_enhance
from skimage.morphology import skeletonize, thin

import requests
from flask import Flask, render_template, request
from threading import Lock

app = Flask(__name__)
app.debug = True
lock = Lock()           # Lock to safely access the counter variable to keep track of number of requets to the webserver
countReq = 0            # global count to keep track of number of requests. 

def matchingfinger(img):
    fingerprint_test = cv2.imread(".\\print1.bmp")
    cv2.imshow("Original", cv2.resize(fingerprint_test, None, fx=1, fy=1))
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    for file in [file for file in os.listdir("fingerku")]:
        print("./fingerku/"+file)
        fingerprint_database_image = cv2.imread("./fingerku/"+file)
        sift =cv2.SIFT_create()
        keypoints_1, descriptors_1 = sift.detectAndCompute(fingerprint_test, None)
        keypoints_2, descriptors_2 = sift.detectAndCompute(fingerprint_database_image, None)
        matches = cv2.FlannBasedMatcher(dict(algorithm=1, trees=10), dict()).knnMatch(descriptors_1, descriptors_2, k=2)
        match_points = []
    
        for p, q in matches:
            if p.distance < 0.1*q.distance:
                match_points.append(p)
                keypoints = 0
                if len(keypoints_1) <= len(keypoints_2):
                    keypoints = len(keypoints_1)            
                else:
                    keypoints = len(keypoints_2)

                if (len(match_points) / keypoints)>0.95:
                    print("% match: ", len(match_points) / keypoints * 100)
                    print("Figerprint ID: " + str(file)) 
                    result = cv2.drawMatches(fingerprint_test, keypoints_1, fingerprint_database_image, keypoints_2, match_points, None) 
                    result = cv2.resize(result, None, fx=2.5, fy=2.5)
                    cv2.imshow("result", result)
                    cv2.waitKey(0)
                    cv2.destroyAllWindows()
                    break;
    

## All GET requests are handled by this function
@app.route('/absen', methods=['POST'])
def do_absen():
    
   return True

@app.route('/', methods=['GET'])
def showCounter():
    global countReq
    lock.acquire()
    tmp = countReq
    tmp += 1
    countReq = tmp
    lock.release()
    ## template.html is the UI view which is being used
    return render_template('templates.html', count= countReq)

## Main Function to run the Flask app
if __name__ == "__main__":
    print("Server running at http://127.0.0.1:8888/")
    app.run(port=8888, threaded = True)


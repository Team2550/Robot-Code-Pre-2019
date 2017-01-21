#OpenCV_test_3.py



# this program tracks a red ball

# (no motor control is performed to move the camera, we will get to that later in the tutorial)



#import cv2

#import numpy as np

import os

import socket

from array import*



###################################################################################################

UDP_IP = "172.22.153.68"    #declares udp ip and port
UDP_PORT = 8890

def main():
##
##
##
##    capWebcam = cv2.VideoCapture(0)                     # declare a VideoCapture object and associate to webcam, 0 => use 1st webcam
##
##
##
##                                                        # show original resolution
##
##    print "default resolution = " + str(capWebcam.get(cv2.CAP_PROP_FRAME_WIDTH)) + "x" + str(capWebcam.get(cv2.CAP_PROP_FRAME_HEIGHT))
##
##
##
##    capWebcam.set(cv2.CAP_PROP_FRAME_WIDTH, 320.0)              # change resolution to 320x240 for faster processing
##
##    capWebcam.set(cv2.CAP_PROP_FRAME_HEIGHT, 240.0)
##
##
##
##                                                        # show updated resolution
##
##    print "updated resolution = " + str(capWebcam.get(cv2.CAP_PROP_FRAME_WIDTH)) + "x" + str(capWebcam.get(cv2.CAP_PROP_FRAME_HEIGHT))
##
##
##
##    if capWebcam.isOpened() == False:                           # check if VideoCapture object was associated to webcam successfully
##
##        print "error: capWebcam not accessed successfully\n\n"          # if not, print error message to std out
##
##        os.system("pause")                                              # pause until user presses a key so user can see error message
##
##        return                                                          # and exit function (which exits program)
##
##    # end if
##
##
##
##    while cv2.waitKey(1) != 27 and capWebcam.isOpened():                # until the Esc key is pressed or webcam connection is lost
##
##        blnFrameReadSuccessfully, imgOriginal = capWebcam.read()            # read next frame
##
##
##
##        if not blnFrameReadSuccessfully or imgOriginal is None:             # if frame was not read successfully
##
##            print "error: frame not read from webcam\n"                     # print error message to std out
##
##            os.system("pause")                                              # pause until user presses a key so user can see error message
##
##            break                                                           # exit while loop (which exits program)
##
##        # end if
##
##
##
##        imgHSV = cv2.cvtColor(imgOriginal, cv2.COLOR_BGR2HSV)
##
##
##
##        imgThreshLow = cv2.inRange(imgHSV, np.array([0, 135, 135]), np.array([18, 255, 255]))
##
##        imgThreshHigh = cv2.inRange(imgHSV, np.array([165, 135, 135]), np.array([179, 255, 255]))
##
##
##
##        imgThresh = cv2.add(imgThreshLow, imgThreshHigh)
##
##
##
##        imgThresh = cv2.GaussianBlur(imgThresh, (3, 3), 2)
##
##
##
##        imgThresh = cv2.dilate(imgThresh, np.ones((5,5),np.uint8))
##
##        imgThresh = cv2.erode(imgThresh, np.ones((5,5),np.uint8))
##
##
##
##        intRows, intColumns = imgThresh.shape
##
##
##
##        circles = cv2.HoughCircles(imgThresh, cv2.HOUGH_GRADIENT, 5, intRows / 4)      # fill variable circles with all circles in the processed image
##
##
##
##        if circles is not None:                     # this line is necessary to keep program from crashing on next line if no circles were found
##
##            for circle in circles[0]:                           # for each circle
##
##                x, y, radius = circle                                                                       # break out x, y, and radius
##
##                print "ball position x = " + str(x) + ", y = " + str(y) + ", radius = " + str(radius)       # print ball position and radius
##
    test_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #arr = array('i',[1,3,2])
    arr = "1 2 3" #places x and y into a string

    running = True
    while running:
        try:
            test_socket.connect((UDP_IP, UDP_PORT)) #connects to socket
            test_socket.sendto(bytes(arr, "ascii"), (UDP_IP, UDP_PORT)) #sends array to socket
            print("sent")

        except Exception:
            running = False
            test_socket.close()
            raise
        running=False
        #test_socket.close()

                # end for

            # end if





    return



###################################################################################################

if __name__ == "__main__":

    main()


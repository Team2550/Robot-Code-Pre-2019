import cv2
import numpy as np
import os
import socket
import math

###################################################################################################

TARGET_RECT_SIZE_INCHES = (2, 5)
TARGET_RECT_DIAGONAL_INCHES = (TARGET_RECT_SIZE_INCHES[0]**2 + TARGET_RECT_SIZE_INCHES[1]**2)**(0.5)

TARGET_ASPECT_RATIO = min(TARGET_RECT_SIZE_INCHES) / max(TARGET_RECT_SIZE_INCHES)
TARGET_ASPECT_MARGIN_OF_ERROR = 0.15 # Percentage that any rectangles seen can defer from the known aspect ratio

ACCEPTABLE_SIDE_PERCENT_DIFFERENCE = 0.5

IMAGE_SIZE = (320, 240)
IMAGE_DIAGONAL = (IMAGE_SIZE[0]**2 + IMAGE_SIZE[1]**2)**(0.5)

CAMERA_FOV = 55 # FOV of axis camera, needs to change for USB camera

###################################################################################################

COTANGENT_FOV = 1 / math.tan(math.radians(CAMERA_FOV / 2.0))

CAMERA_URL = 0

try:
    CAMERA_IP = socket.gethostbyname("axis-camera.local")
    CAMERA_URL = "http://" + CAMERA_IP + "/mjpg/video.mjpg"
    print("Found camera at", CAMERA_IP)
except Exception:
    CAMERA_URL = 0
    print("No IP camera found. Defaulting to USB camera.")

UDP_IP = socket.gethostbyname("roboRIO-2550-FRC.local")    #declares udp ip and port
UDP_PORT = 8890

print("Found roboRIO at", UDP_IP)

def dist(x1, y1, x2, y2):
    return ((x1 - x2)*(x1 - x2)+(y1 - y2)*(y1 - y2))**(0.5)

def processCamera(capWebcam):
    blnFrameReadSuccessfully, imgOriginal = capWebcam.read()            # read next frame

    if (not blnFrameReadSuccessfully) or (imgOriginal is None):             # if frame was not read successfully
        print("Error: frame not read from webcam\n")                     # print error message to std out

        return -1, -1, -1

    imgHSV = cv2.cvtColor(imgOriginal, cv2.COLOR_BGR2HSV)
    
    lowerBound = np.array([0, 0, 240])
    upperBound = np.array([255, 50, 255])

    mask = cv2.inRange(imgHSV, lowerBound, upperBound)

    # find contours in the thresholded image
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
                            cv2.CHAIN_APPROX_SIMPLE)[1]

    for c in cnts:
        peri = cv2.arcLength(c, True)
        approx = cv2.approxPolyDP(c, 0.04 * peri, True)

        if len(approx) == 4:
            sideLengths = [0, 0, 0, 0]
            for i in range(4):
                sideLengths[i] = dist(approx[i][0][0], approx[i][0][1],
                                      approx[(i+1)%4][0][0], approx[(i+1)%4][0][1])

            width = (sideLengths[0] + sideLengths[2])/2.0
            height = (sideLengths[1] + sideLengths[3])/2.0

            widthSideDiff = abs(sideLengths[0] - sideLengths[2]) / width # Difference in width-wise side lengths in percent
            heightSideDiff = abs(sideLengths[1] - sideLengths[3]) / height # Difference in hight-wise side lengths in percent

            shortSide = min(width, height)
            longSide = max(width, height)
            
            size = width * height
            ar = shortSide / longSide # Aspect ratio: will always be less than one
            
            if (size > 250 and ar >= TARGET_ASPECT_RATIO * (1 - TARGET_ASPECT_MARGIN_OF_ERROR) and
                               ar <= TARGET_ASPECT_RATIO * (1 + TARGET_ASPECT_MARGIN_OF_ERROR) and
                               widthSideDiff < ACCEPTABLE_SIDE_PERCENT_DIFFERENCE and
                               heightSideDiff < ACCEPTABLE_SIDE_PERCENT_DIFFERENCE):
                                  
                diagonal = (dist(approx[0][0][0],approx[0][0][1],
                                 approx[2][0][0],approx[2][0][1]) +
                            dist(approx[1][0][0],approx[1][0][1],
                                 approx[3][0][0],approx[3][0][1])) / 2

                percentOfView = diagonal / IMAGE_DIAGONAL
                objectDist = (TARGET_RECT_DIAGONAL_INCHES / percentOfView) * 0.5 * COTANGENT_FOV
                
                #print("Found rectangle,", size, ":", ar, ":", x, y, width, height, "Dist:", objectDist)

                return objectDist, 2.0, 3.0 # Test values
        
    return -1, -1, -1

def main():
    capWebcam = cv2.VideoCapture(CAMERA_URL) # declare a VideoCapture object and associate to webcam, 0 => use 1st webcam

    # show original resolution
    print("Default resolution = ", capWebcam.get(cv2.CAP_PROP_FRAME_WIDTH), "x", capWebcam.get(cv2.CAP_PROP_FRAME_HEIGHT))

    capWebcam.set(cv2.CAP_PROP_FRAME_WIDTH, IMAGE_SIZE[0]) # change resolution to 320x240 for faster processing
    capWebcam.set(cv2.CAP_PROP_FRAME_HEIGHT, IMAGE_SIZE[1])

    # show updated resolution
    print("Updated resolution = ", capWebcam.get(cv2.CAP_PROP_FRAME_WIDTH), "x", capWebcam.get(cv2.CAP_PROP_FRAME_HEIGHT))
    
    if capWebcam.isOpened() == False:                           # check if VideoCapture object was associated to webcam successfully
        print("Error: capWebcam not accessed successfully\n\n")          # if not, print error message to std out
        capWebcam.release()
        
        return                                                          # and exit function (which exits program)
        
    test_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    running = True

    try:
        while (running and cv2.waitKey(1) != 27 and capWebcam.isOpened()):
            try:            
                dist, horizAngle, vertAngle = processCamera(capWebcam)

                if (dist != -1 or horizAngle != -1 or vertAngle != -1):
                    print("Found target:", dist, horizAngle, vertAngle)
                    
                    arr = [dist, horizAngle, vertAngle]
                    data = ' '.join(str(x) for x in arr)
                    test_socket.sendto(bytes(data, 'utf-8'), (UDP_IP, UDP_PORT)) #sends array to socket
                    print("Sent to RoboRIO!")

            except Exception:
                running = False

                test_socket.close()
                capWebcam.release()
                
                raise
            
    except KeyboardInterrupt: # Catch exit by Ctrl-C
        print("\nTerminating script...")
        
        test_socket.close()
        capWebcam.release()
    
    return

###################################################################################################

if __name__ == "__main__":
    main()

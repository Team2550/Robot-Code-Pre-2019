import cv2
import numpy as np
import os
import socket
import math
import time

#################################################################################################################

MAX_FRAMERATE = 20

OPEN_WINDOWS = False

TARGET_RECT_SIZE_INCHES = (2, 5)
TARGET_RECT_DIAGONAL_INCHES = (TARGET_RECT_SIZE_INCHES[0]**2 + TARGET_RECT_SIZE_INCHES[1]**2)**(0.5)
TARGET_YOFFSET = 2.5
TARGET_MAX_YOFFSET = 20

TARGET_ASPECT_RATIO = min(TARGET_RECT_SIZE_INCHES) / max(TARGET_RECT_SIZE_INCHES) # Always less than one
TARGET_ASPECT_MARGIN_OF_ERROR = 0.40 # Percentage that any rectangles seen can defer from the known aspect ratio

SHAPE_BOUNDINGBOX_ASPECT_RATIO_MAX_DIFF = 0.40 # Maximum difference between a shapes aspect ratio and its bounding box's aspect ratio (percentage)

ACCEPTABLE_SIDE_PERCENT_DIFFERENCE = 0.5

IMAGE_SIZE = (IMAGE_WIDTH, IMAGE_HEIGHT) = (640, 480)
IMAGE_DIAGONAL = (IMAGE_SIZE[0]**2 + IMAGE_SIZE[1]**2)**(0.5)

CAMERA_FOV = 68.5 # FOV of axis camera, needs to change for USB camera 

#################################################################################################################

COTANGENT_FOV = 1 / math.tan(math.radians(CAMERA_FOV / 2.0))

CAMERA_URL = 0

print("Searching for local camera...")
try:
    CAMERA_IP = socket.gethostbyname("raspberrypi")
    CAMERA_URL = "http://" + CAMERA_IP + ":8080/?action=stream&amp;type=.mjpg"
    print("Found camera at", CAMERA_IP)
except Exception:
    try:
        print("Searching for network camera...")
        CAMERA_IP = socket.gethostbyname("raspberrypi.local")
        CAMERA_URL = "http://" + CAMERA_IP + ":8080/?action=stream&amp;type=.mjpg"
        print("Found camera at", CAMERA_IP)
    except Exception:
        CAMERA_URL = 0
        print("No IP camera found. Defaulting to USB camera.")

UDP_IP = "10.25.50.20"

print("Searching for roboRIO...")
try:
    UDP_IP = socket.gethostbyname("roboRIO-2550-FRC.local")    #declares udp ip and port
    print("Found roboRIO at", UDP_IP)
except Exception:
    UDP_IP = "10.25.50.21"
    print("RoboRIO not found. Defaulting to static IP.")
    
UDP_PORT = 8890

def dist(x1, y1, x2, y2):
    return ((x1 - x2)*(x1 - x2)+(y1 - y2)*(y1 - y2))**(0.5)

def processCamera(imgOriginal):
    imgHSV = cv2.cvtColor(imgOriginal, cv2.COLOR_BGR2HSV)
    
    lowerBound = np.array([0, 0, 200])
    upperBound = np.array([255, 105, 255])

    mask = cv2.inRange(imgHSV, lowerBound, upperBound)
    maskDraw = cv2.cvtColor(mask, cv2.COLOR_GRAY2BGR)
    
    # find contours in the thresholded image
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
                            cv2.CHAIN_APPROX_SIMPLE)[1]

    dataPoints = []
    
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

            # Find corners of bounding box
            maxX = max(approx[0][0][0],approx[1][0][0],approx[2][0][0],approx[3][0][0])
            maxY = max(approx[0][0][1],approx[1][0][1],approx[2][0][1],approx[3][0][1])
            minX = min(approx[0][0][0],approx[1][0][0],approx[2][0][0],approx[3][0][0])
            minY = min(approx[0][0][1],approx[1][0][1],approx[2][0][1],approx[3][0][1])

            # Calculate width, height, and aspect ratio of bounding box. 
            boundboxWidth = maxX - minX
            boundboxHeight = maxY - minY
            boundboxAR = boundboxWidth / boundboxHeight
            
            size = width * height
            ar = shortSide / longSide # Aspect ratio: will always be less than one
            
            if (size > 250 and ar >= TARGET_ASPECT_RATIO * (1 - TARGET_ASPECT_MARGIN_OF_ERROR) and
                               ar <= TARGET_ASPECT_RATIO * (1 + TARGET_ASPECT_MARGIN_OF_ERROR) and
                               widthSideDiff < ACCEPTABLE_SIDE_PERCENT_DIFFERENCE and
                               heightSideDiff < ACCEPTABLE_SIDE_PERCENT_DIFFERENCE and
                               abs(boundboxAR / ar - 1) < SHAPE_BOUNDINGBOX_ASPECT_RATIO_MAX_DIFF):
                                  
                diagonal = (dist(approx[0][0][0],approx[0][0][1],
                                 approx[2][0][0],approx[2][0][1]) +
                            dist(approx[1][0][0],approx[1][0][1],
                                 approx[3][0][0],approx[3][0][1])) / 2

                avgCoordinates = (avgX, avgY) = ((approx[0][0][0]+approx[1][0][0]+approx[2][0][0]+approx[3][0][0])/4,
                                                 (approx[0][0][1]+approx[1][0][1]+approx[2][0][1]+approx[3][0][1])/4)

                percentOfView = diagonal / IMAGE_DIAGONAL
                diagonalOfObjectPlaneInches = TARGET_RECT_DIAGONAL_INCHES / percentOfView
                widthOfObjectPlaneInches = (IMAGE_WIDTH / IMAGE_DIAGONAL) * diagonalOfObjectPlaneInches
                heightOfObjectPlaneInches = (IMAGE_HEIGHT / IMAGE_DIAGONAL) * diagonalOfObjectPlaneInches
                
                objectDist = diagonalOfObjectPlaneInches * 0.5 * COTANGENT_FOV
                objectXOffset = ((avgX / IMAGE_WIDTH) - (1/2)) * widthOfObjectPlaneInches
                objectYOffset = -((avgY / IMAGE_HEIGHT) - (1/2)) * heightOfObjectPlaneInches

                if (abs(objectYOffset - TARGET_YOFFSET) < TARGET_MAX_YOFFSET):
                    xAngle = math.degrees(math.atan2(objectXOffset, objectDist))
                    yAngle = math.degrees(math.atan2(objectYOffset, objectDist))

                    percentMatch = 1 - abs(ar / TARGET_ASPECT_RATIO - 1)
                    percentMatch *= 1 - abs(boundboxAR / ar - 1)
                    percentMatch *= 1 - ((objectYOffset - TARGET_YOFFSET) / (TARGET_MAX_YOFFSET))**2

                    if (OPEN_WINDOWS):
                        cv2.drawContours(imgOriginal, [approx], -1, (0,255,0), 2)
                        cv2.drawContours(maskDraw, [approx], -1, (0,255,0), 2)
                        cv2.putText(imgOriginal, "{0:.1f}".format(percentMatch * 100) + "% match, " + "{0:.2f}".format(objectDist) + " inches, " + 
                                    "{0:.2f}".format(xAngle) + " degrees",
                                    (minX, minY), cv2.FONT_HERSHEY_SIMPLEX, 0.3, (255,0,0))
                    
                    dataPoints += [(percentMatch, objectDist, objectXOffset, objectYOffset, xAngle, yAngle)] # Test values

            else:
                if (OPEN_WINDOWS):
                    cv2.drawContours(imgOriginal, [approx], -1, (255,0,0), 1)
                    cv2.drawContours(maskDraw, [approx], -1, (255,0,0), 1)
                    cv2.putText(imgOriginal, "{0:.1f}".format(abs(ar / TARGET_ASPECT_RATIO - 1) * 100) + "% AR mismatch",
                                (maxX, minY), cv2.FONT_HERSHEY_SIMPLEX, 0.3, (0,0,255))

    if (OPEN_WINDOWS):
        try:
            cv2.imshow('Original', imgOriginal)
            cv2.imshow('Mask', maskDraw)
        except Exception:
            print("Can't display window!")
    
    return dataPoints

def main():
    camCapture = cv2.VideoCapture(CAMERA_URL) # declare a VideoCapture object and associate to webcam, 0 => use 1st webcam

    # show original resolution
    print("Default resolution =", camCapture.get(cv2.CAP_PROP_FRAME_WIDTH), "x", camCapture.get(cv2.CAP_PROP_FRAME_HEIGHT))

    camCapture.set(cv2.CAP_PROP_FRAME_WIDTH, IMAGE_SIZE[0]) # change resolution to 320x240 for faster processing
    camCapture.set(cv2.CAP_PROP_FRAME_HEIGHT, IMAGE_SIZE[1])

    # show updated resolution
    print("Updated resolution =", camCapture.get(cv2.CAP_PROP_FRAME_WIDTH), "x", camCapture.get(cv2.CAP_PROP_FRAME_HEIGHT))
    
    if camCapture.isOpened() == False:                           # check if VideoCapture object was associated to webcam successfully
        print("Error: Camera not accessed successfully\n\n")          # if not, print error message to std out
        camCapture.release()
        
        return                                                          # and exit function (which exits program)
        
    sendingSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    receivingSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    receivingSocket.bind(("localhost", UDP_PORT))

    running = True

    try:
        print("Beginning image processing...")
        while (running and cv2.waitKey(1) != 27 and camCapture.isOpened()):
            startTime = time.time()
            try:
                blnFrameReadSuccessfully, imgOriginal = camCapture.read() # read next frame    

                if (not blnFrameReadSuccessfully) or (imgOriginal is None):             # if frame was not read successfully
                    print("Error: frame not read from webcam\n")                     # print error message to std out
                else:
                    data = processCamera(imgOriginal)

                    if (data is not None and len(data) > 1):
                        #print("Found", len(data), "targets")

                        for d in data:
                            percentMatch, dist, xOffset, yOffset, horizAngle, vertAngle = d
                            #print("Found target (percentMatch, dist, xOffset, yOffset, horizAngle, vertAngle):", percentMatch, dist, xOffset, yOffset, horizAngle, vertAngle)
                            #print("Found target: ", horizAngle)
                        
                        data = ','.join(' '.join("{0:.3f}".format(y) for y in x) for x in data)

                        sendingSocket.sendto(bytes(data, 'utf-8'), (UDP_IP, UDP_PORT)) #sends array to socket
                        #print("Sent data to RoboRIO!")
                    #else:
                        #print("No target found.")

                # Keep buffer empty until next frame
                while ((time.time() - startTime) < (1./MAX_FRAMERATE)):
                    blnFrameReadSuccessfully, imgOriginal = camCapture.read()

            except Exception:
                running = False

                sendingSocket.close()
                receivingSocket.close()
                camCapture.release()
                
                raise
            
    except KeyboardInterrupt: # Catch exit by Ctrl-C
        print("\nTerminating script...")
        
        sendingSocket.close()
        receivingSocket.close()
        camCapture.release()
    
    return

###################################################################################################

if __name__ == "__main__":
    main()

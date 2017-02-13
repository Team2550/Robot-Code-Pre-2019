import cv2
import numpy as np
import os
import socket
#from array import*

###################################################################################################

#CAMERA_IP = socket.gethostbyname("axis-camera.local")
#CAMERA_URL = "http://" + CAMERA_IP + "/mjpg/video.mjpg"

#UDP_IP = socket.gethostbyname("roboRIO-2550-FRC.local")    #declares udp ip and port
UDP_IP = socket.gethostbyname("NW-GAMING")
UDP_PORT = 8890

print("Found roboRIO at", UDP_IP)

def detectShapes(image):
    blurred = cv2.GaussianBlur(image, (5, 5), 0)
    thresh = cv2.threshold(blurred, 60, 255, cv2.THRESH_BINARY)[1]
    RGB_image = cv2.cvtColor(image, cv2.COLOR_GRAY2BGR)
    
    cv2.imshow("Image", thresh)
    cv2.waitKey(0)
    cv2.destroyAllWindows()    

    # find contours in the thresholded image
    cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL,
	cv2.CHAIN_APPROX_SIMPLE)[1]

    for c in cnts:
        # detect shape
        peri = cv2.arcLength(c, True)
        approx = cv2.approxPolyDP(c, 0.04 * peri, True)

        if len(approx) == 4:
            (x, y, w, h) = cv2.boundingRect(approx)
            
            print("Found rectangle, size", w * h, ":", x, y, w, h)
            #ar = w / float(h)
            
            #if ar >= 0.95 and ar <= 1.05:
            cv2.drawContours(RGB_image, [c], 0, (0, 255, 0), 4)
 
    # show the output image
    cv2.imshow("Image", RGB_image)
    #cv2.waitKey(0)    

def processCamera(capWebcam):
    blnFrameReadSuccessfully, imgOriginal = capWebcam.read()            # read next frame

    if (not blnFrameReadSuccessfully) or (imgOriginal is None):             # if frame was not read successfully
        print("Error: frame not read from webcam\n")                     # print error message to std out

        return -1, -1, -1

    #try:
    imgHSV = cv2.cvtColor(imgOriginal, cv2.COLOR_BGR2HSV)
    
    lowerBound = np.array([0, 0, 200])
    upperBound = np.array([255, 50, 255])

    mask = cv2.inRange(imgHSV, lowerBound, upperBound)

    #res = cv2.bitwise_and(imgOriginal, imgOriginal, mask = mask)
    
    #cv2.imshow("Original Image", imgOriginal);
    #cv2.imshow("Masked Image", mask);
    #cv2.waitKey(0)
    #cv2.destroyAllWindows()
    #cv2.imwrite("maskedImage.png", res);

    detectShapes(mask)
    
    #imgThresh = cv2.add(imgThreshLow, imgThreshHigh)
    #imgThresh = cv2.GaussianBlur(imgThresh, (3, 3), 2)
    #imgThresh = cv2.dilate(imgThresh, np.ones((5,5), np.uint8))
    #imgThresh = cv2.erode(imgThresh, np.ones((5,5), np.uint8))

    #intRows, intColumns = imgThresh.shape

    ##circles = cv2.HoughCircles(imgThresh, cv2.HOUGH_GRADIENT, 5, intRows / 4)      # fill variable circles with all circles in the processed image

    #if circles is not None: # this line is necessary to keep program from crashing on next line if no circles were found
    #    for circle in circles[0]:
    #        return circle
        
    return -1, -1, -1
        
    #except Exception:
    #    print("Error in image processing!")
        
    #    return -1, -1, -1

def main():
    #capWebcam = cv2.VideoCapture(CAMERA_URL) # declare a VideoCapture object and associate to webcam, 0 => use 1st webcam
    capWebcam = cv2.VideoCapture(0)

    # show original resolution

    print("Default resolution = ", capWebcam.get(cv2.CAP_PROP_FRAME_WIDTH), "x", capWebcam.get(cv2.CAP_PROP_FRAME_HEIGHT))

    capWebcam.set(cv2.CAP_PROP_FRAME_WIDTH, 320.0) # change resolution to 320x240 for faster processing
    capWebcam.set(cv2.CAP_PROP_FRAME_HEIGHT, 240.0)

    # show updated resolution
    print("Updated resolution = ", capWebcam.get(cv2.CAP_PROP_FRAME_WIDTH), "x", capWebcam.get(cv2.CAP_PROP_FRAME_HEIGHT))
    
    if capWebcam.isOpened() == False:                           # check if VideoCapture object was associated to webcam successfully
        print("Error: capWebcam not accessed successfully\n\n")          # if not, print error message to std out

        capWebcam.release()
        
        return                                                          # and exit function (which exits program)
        
    test_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    running = True
    
    while (running and cv2.waitKey(1) != 27 and capWebcam.isOpened()):
        try:            
            circle = processCamera(capWebcam)

            if (circle is not None):
                x, y, radius = circle

                if (x != -1 or y != -1 or radius != -1):
                    print("Found circle:", x, y, radius)
                    
                    arr = [x, y, radius]
                    data = ' '.join(str(x) for x in arr)
                    test_socket.sendto(bytes(data, 'utf-8'), (UDP_IP, UDP_PORT)) #sends array to socket
                    print("Sent!")

        except Exception:
            running = False

            test_socket.close()
            capWebcam.release()
            
            raise
            
        running = False

    capWebcam.release()
    
    return

###################################################################################################

if __name__ == "__main__":

    main()
    cv2.destroyAllWindows()

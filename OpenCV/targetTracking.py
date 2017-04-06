import cv2
import numpy as np
import os
import socket
import math
import time

#################################################################################################################

MAX_FRAMERATE = 20 # Limits the framerate if image processing finishes early.

OPEN_WINDOWS = True # Opens windows to view the mask created by the color range filters. DO NOT ENABLE ON PI.

# Range of colors to accept as target (Hue, Saturation, Value)
# Values range from 0 to 255. If using an online color picker, make sure to
# adjust their values for this range. Most websites use 0 to 360 for hue,
# and 0 to 100 for saturation and value.
MIN_HSV = (MIN_HUE, MIN_SAT, MIN_VAL) = (0, 0, 75)
MAX_HSV = (MAX_HUE, MAX_SAT, MAX_VAL) = (200, 255, 255)
# Values to test: MIN_HSV = (MIN_HUE, MIN_SAT, MIN_VAL) = (75, 100, 75)
#                 MAX_HSV = (MAX_HUE, MAX_SAT, MAX_VAL) = (180, 255, 180)

TARGET_RECT_SIZE_INCHES = (2, 5) # Target's size in inches (width, height).
TARGET_RECT_DIAGONAL_INCHES = (TARGET_RECT_SIZE_INCHES[0]**2 + TARGET_RECT_SIZE_INCHES[1]**2)**(0.5) # Diagonal of the rectangle of the target.
TARGET_YOFFSET = 2.5 # The vertical offset (in inches) where the target should be found.
TARGET_MAX_YOFFSET = 20 # The max difference that the target may be from the specified Y-Offset.

TARGET_ASPECT_RATIO = min(TARGET_RECT_SIZE_INCHES) / max(TARGET_RECT_SIZE_INCHES) # The aspect ratio of the target (short side divided by long side).
TARGET_ASPECT_MARGIN_OF_ERROR = 0.40 # Percentage that any rectangles that are found may defer from the known aspect ratio

SHAPE_BOUNDINGBOX_ASPECT_RATIO_MAX_DIFF = 0.40 # Maximum difference (in percent) between a shapes aspect ratio (based on distances between corners) and its bounding box's aspect ratio.

ACCEPTABLE_SIDE_PERCENT_DIFFERENCE = 0.5 # Maximum difference between a rectangles opposing sides to be accepted (in percent).

IMAGE_SIZE = (IMAGE_WIDTH, IMAGE_HEIGHT) = (640, 360) # Resolution of the input images
IMAGE_DIAGONAL = (IMAGE_SIZE[0]**2 + IMAGE_SIZE[1]**2)**(0.5) # Diagonal (in pixels) of the input images

CAMERA_FOV = 68.5 # Field of view of the camera, in degrees.

#################################################################################################################

COTANGENT_FOV = 1 / math.tan(math.radians(CAMERA_FOV / 2.0)) # Pre-processed for efficiency. The cotangent of half of the camera's FOV.

# Get the camera's address (defaults to 0, or USB camera).
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

# Get the RoboRIO's IP address (defaults to static IP 10.25.50.20).
UDP_IP = "10.25.50.20"

print("Searching for roboRIO...")
try:
    UDP_IP = socket.gethostbyname("roboRIO-2550-FRC.local")
    print("Found roboRIO at", UDP_IP)
except Exception:
    UDP_IP = "10.25.50.21"
    print("RoboRIO not found. Defaulting to static IP.")
    
UDP_PORT = 8890 # Port on RoboRIO to send data to.

# Distance between two points
def dist(x1, y1, x2, y2):
    return ((x1 - x2)*(x1 - x2)+(y1 - y2)*(y1 - y2))**(0.5)

# Process an image and find targets (Returns a list of tuples with 6 elements each).
def processCamera(imgOriginal):
    imgHSV = cv2.cvtColor(imgOriginal, cv2.COLOR_BGR2HSV) # Convert image to HSV format

    # Filter image based on color range.
    lowerBound = np.array(MIN_HSV)
    upperBound = np.array(MAX_HSV)

    mask = cv2.inRange(imgHSV, lowerBound, upperBound)
    maskDrawable = cv2.cvtColor(mask, cv2.COLOR_GRAY2BGR) # Drawable version of mask (RGB rather than Black and White)
    
    # Find contours in the thresholded image
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
                            cv2.CHAIN_APPROX_SIMPLE)[1]

    dataPoints = []
    
    for c in cnts:
        peri = cv2.arcLength(c, True) # Perimeter of contour
        approx = cv2.approxPolyDP(c, 0.04 * peri, True) # Simplified contour (should be a rectangle if the contour is a target).

        if len(approx) == 4:
            # Get lengths of sides (Either clockwise or counterclockwise)
            sideLengths = [0, 0, 0, 0]
            for i in range(4):
                sideLengths[i] = dist(approx[i][0][0], approx[i][0][1],
                                      approx[(i+1)%4][0][0], approx[(i+1)%4][0][1])

            # Average opposing sides to find width and height (Not necessarly in the correct orientation. Width and height are reversable here).
            width = (sideLengths[0] + sideLengths[2])/2.0
            height = (sideLengths[1] + sideLengths[3])/2.0

            widthSideDiff = abs(sideLengths[0] - sideLengths[2]) / width # Difference in width-wise side lengths in percent
            heightSideDiff = abs(sideLengths[1] - sideLengths[3]) / height # Difference in hight-wise side lengths in percent

            # Short and long sides
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

            # Calculate area and aspect ratio of contour
            size = width * height
            ar = shortSide / longSide # Will always be less than one, as short side is divided by long side.
            
            if (size > 250 and # Minimum area of 250 pixels
                ar >= TARGET_ASPECT_RATIO * (1 - TARGET_ASPECT_MARGIN_OF_ERROR) and # Aspect ratio of shape is within specified range of the given aspect ratio.
                ar <= TARGET_ASPECT_RATIO * (1 + TARGET_ASPECT_MARGIN_OF_ERROR) and
                widthSideDiff < ACCEPTABLE_SIDE_PERCENT_DIFFERENCE and # Width and height side differences are acceptable (not stretched out/distorted)
                heightSideDiff < ACCEPTABLE_SIDE_PERCENT_DIFFERENCE and
                abs(boundboxAR / ar - 1) < SHAPE_BOUNDINGBOX_ASPECT_RATIO_MAX_DIFF): # Bounding box's aspect ratio and the actual shape's aspect ratio are close enough to each other.

                # Calculate average diagonal of shape
                diagonal = (dist(approx[0][0][0],approx[0][0][1],
                                 approx[2][0][0],approx[2][0][1]) +
                            dist(approx[1][0][0],approx[1][0][1],
                                 approx[3][0][0],approx[3][0][1])) / 2

                # Find center of shape
                avgCoordinates = (avgX, avgY) = ((approx[0][0][0]+approx[1][0][0]+approx[2][0][0]+approx[3][0][0])/4,
                                                 (approx[0][0][1]+approx[1][0][1]+approx[2][0][1]+approx[3][0][1])/4)

                # Calculate distance, X Offset, and Y Offset of target
                percentOfView = diagonal / IMAGE_DIAGONAL
                diagonalOfObjectPlaneInches = TARGET_RECT_DIAGONAL_INCHES / percentOfView
                widthOfObjectPlaneInches = (IMAGE_WIDTH / IMAGE_DIAGONAL) * diagonalOfObjectPlaneInches
                heightOfObjectPlaneInches = (IMAGE_HEIGHT / IMAGE_DIAGONAL) * diagonalOfObjectPlaneInches
                
                objectDist = diagonalOfObjectPlaneInches * 0.5 * COTANGENT_FOV
                objectXOffset = ((avgX / IMAGE_WIDTH) - (1/2)) * widthOfObjectPlaneInches
                objectYOffset = -((avgY / IMAGE_HEIGHT) - (1/2)) * heightOfObjectPlaneInches

                if (abs(objectYOffset - TARGET_YOFFSET) < TARGET_MAX_YOFFSET): # Shape is within Y Offset range
                    # Calculate angles of target
                    xAngle = math.degrees(math.atan2(objectXOffset, objectDist))
                    yAngle = math.degrees(math.atan2(objectYOffset, objectDist))

                    # Calculate the percent match, which is an arbitrary number that represents how closely the shape matches the target.
                    percentMatch = 1 - abs(ar / TARGET_ASPECT_RATIO - 1)
                    percentMatch *= 1 - abs(boundboxAR / ar - 1)
                    percentMatch *= 1 - ((objectYOffset - TARGET_YOFFSET) / (TARGET_MAX_YOFFSET))**2

                    # Draw on image mask and original image if opening windows is enabled.
                    if (OPEN_WINDOWS):
                        cv2.drawContours(imgOriginal, [approx], -1, (0,255,0), 2)
                        cv2.drawContours(maskDrawable, [approx], -1, (0,255,0), 2)
                        cv2.putText(imgOriginal, "{0:.1f}".format(percentMatch * 100) + "% match, " + "{0:.2f}".format(objectDist) + " inches, " + 
                                    "{0:.2f}".format(xAngle) + " degrees",
                                    (minX, minY), cv2.FONT_HERSHEY_SIMPLEX, 0.3, (255,0,0))
                    
                    dataPoints += [(percentMatch, objectDist, objectXOffset, objectYOffset, xAngle, yAngle)] # Add data to list

            else:
                # Draw on image mask and original image if opening windows is enabled.
                if (OPEN_WINDOWS):
                    cv2.drawContours(imgOriginal, [approx], -1, (255,0,0), 1)
                    cv2.drawContours(maskDrawable, [approx], -1, (255,0,0), 1)
                    cv2.putText(imgOriginal, "{0:.1f}".format(abs(ar / TARGET_ASPECT_RATIO - 1) * 100) + "% AR mismatch",
                                (maxX, minY), cv2.FONT_HERSHEY_SIMPLEX, 0.3, (0,0,255))

    # Display image and image mask.
    if (OPEN_WINDOWS):
        try:
            cv2.imshow('Original', imgOriginal)
            cv2.imshow('Mask', maskDrawable)
        except Exception:
            print("Can't display window!")
    
    return dataPoints

def main():
    camCapture = cv2.VideoCapture(CAMERA_URL) # Declare a VideoCapture object and attach to webcam. 0 for USB camera or URL for IP camera.

    # Show original resolution.
    print("Default resolution =", camCapture.get(cv2.CAP_PROP_FRAME_WIDTH), "x", camCapture.get(cv2.CAP_PROP_FRAME_HEIGHT))

    # Change resolution to value given at top of script.
    camCapture.set(cv2.CAP_PROP_FRAME_WIDTH, IMAGE_SIZE[0])
    camCapture.set(cv2.CAP_PROP_FRAME_HEIGHT, IMAGE_SIZE[1])

    # Show updated resolution.
    print("Updated resolution =", camCapture.get(cv2.CAP_PROP_FRAME_WIDTH), "x", camCapture.get(cv2.CAP_PROP_FRAME_HEIGHT))

    # Check if VideoCapture object found the camera successfully
    if camCapture.isOpened() == False:                           
        print("Error: Camera not accessed successfully\n\n")
        camCapture.release()
        
        return

    # Initialize socket for sending data and socket for recieving data
    sendingSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    receivingSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    receivingSocket.bind(("localhost", UDP_PORT))

    print("Beginning image processing...")
    running = True

    try:
        while (running and cv2.waitKey(1) != 27 and camCapture.isOpened()):
            startTime = time.time() # Get time when frame began

            try:
                blnFrameReadSuccessfully, imgOriginal = camCapture.read() # Read next image    

                # Check if image read was successful.
                if (not blnFrameReadSuccessfully) or (imgOriginal is None):
                    print("Error: frame not read from webcam\n")
                else:
                    # Process image for target data
                    data = processCamera(imgOriginal)

                    if (data is not None and len(data) > 1):
                        # Output data to CLI (commented out for efficiency)
                        #print("Found", len(data), "targets")

                        #for d in data:
                            #percentMatch, dist, xOffset, yOffset, horizAngle, vertAngle = d
                            #print("Found target (percentMatch, dist, xOffset, yOffset, horizAngle, vertAngle):", percentMatch, dist, xOffset, yOffset, horizAngle, vertAngle)
                            #print("Found target: ", horizAngle)

                        # Compile data into comma and space separated list.
                        data = ','.join(' '.join("{0:.3f}".format(y) for y in x) for x in data)

                        sendingSocket.sendto(bytes(data, 'utf-8'), (UDP_IP, UDP_PORT)) # Send data
                    #else:
                        #print("No target found.")

                # Keep image buffer empty until next frame
                while ((time.time() - startTime) < (1./MAX_FRAMERATE)):
                    blnFrameReadSuccessfully, imgOriginal = camCapture.read()

            except Exception:
                running = False

                # Safely close all sockets and camera input.
                sendingSocket.close()
                receivingSocket.close()
                camCapture.release()
                
                raise
            
    except KeyboardInterrupt: # Catch exit by Ctrl-C
        print("\nTerminating script...")

        # Safely close all sockets and camera input.
        sendingSocket.close()
        receivingSocket.close()
        camCapture.release()
    
    return

###################################################################################################

if __name__ == "__main__":
    main()

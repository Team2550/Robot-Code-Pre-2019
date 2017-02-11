import cv2

import numpy as np

import os

import socket

from array import*

###################################################################################################

UDP_IP = socket.gethostbyname("roboRIO-2550-FRC.local")    #declares udp ip and port
UDP_PORT = 8890

print("Found roboRIO at", UDP_IP)

def main():
    test_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    arr = array('i',[1, 2, 3, 10])
    data = ' '.join(str(x) for x in arr.tolist())

    running = True
    while running:
        try:
            #test_socket.connect((UDP_IP, UDP_PORT)) #connects to socket
            test_socket.sendto(bytes(data, 'utf-8'), (UDP_IP, UDP_PORT)) #sends array to socket
            print("sent")

        except Exception:
            running = False
            test_socket.close()
            raise

        return

###################################################################################################

if __name__ == "__main__":

    main()


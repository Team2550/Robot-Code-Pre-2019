import os
import socket
import math

#################################################################################################################

UDP_IP = "10.25.50.20"

print("Searching for roboRIO...")
try:
    UDP_IP = socket.gethostbyname("roboRIO-2550-FRC.local")    #declares udp ip and port
    print("Found roboRIO at", UDP_IP)
except Exception:
    UDP_IP = "10.25.50.21"
    print("RoboRIO not found. Defaulting to static IP.")
    
UDP_PORT = 8890

def main():
    sendingSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    receivingSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    receivingSocket.bind(("127.0.0.1", UDP_PORT))
    receivingSocket.setblocking(0)

    running = True

    try:
        while (running):
            try:            
                data = [(1, 2, 3, 4, 5, 6), (3, 4, 5, 6, 7, 8), (5, 6, 7, 8, 9, 10)]

                if (data is not None and len(data) > 1):
                    #print("Found", len(data), "targets")

                    for d in data:
                        percentMatch, dist, xOffset, yOffset, horizAngle, vertAngle = d
                        #print("Found target (percentMatch, dist, xOffset, yOffset, horizAngle, vertAngle):", percentMatch, dist, xOffset, yOffset, horizAngle, vertAngle)
                        #print("Found target: ", horizAngle)
                    
                    data = ','.join(' '.join("{0:.3f}".format(y) for y in x) for x in data)

                    sendingSocket.sendto(bytes(data, 'utf-8'), (UDP_IP, UDP_PORT)) #sends array to socket
                    print("Sent data to RoboRIO!")
 
                try:
                    dataFromRobot, addr = receivingSocket.recvfrom(1024)

                    print("Received message:", dataFromRobot)
                except socket.error:
                    '''Do nothing'''

            except Exception:
                print("Error")
                
                running = False

                sendingSocket.close()
                receivingSocket.close()
                
                raise

    except KeyboardInterrupt: # Catch exit by Ctrl-C
        print("\nTerminating script...")
        
        sendingSocket.close()
        receivingSocket.close()
    
    return

###################################################################################################

if __name__ == "__main__":
    main()

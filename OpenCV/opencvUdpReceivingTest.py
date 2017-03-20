import socket

UDP_IP = "192.168.2.201"
UDP_PORT = 8890

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes

    if addr[0] == "192.168.2.202":
        arr = [int(x) for x in data.decode("utf-8").split(' ')]
        print("Received message: ", arr)

from socket import *

server_socket = socket(AF_INET, SOCK_DGRAM)

server_socket.bind(('',8888))

while True:
    message, address = server_socket.recvfrom(1024)
    print(message.decode('utf-8'))

import socket

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client.connect(("172.17.45.22", 6969))
print(client.recv(1024))
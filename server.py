import socket

HOST = "0.0.0.0"
PORT = 6969

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((HOST, PORT))
    server.listen()
    print(f"Server listening on {HOST}:{PORT}")
    
    while True:
        try:
            conn, addr = server.accept()
            with conn:
                print(f"Connected by {addr}")
                # Send welcome message
                welcome_msg = f"hello {addr}!\n"
                conn.send(welcome_msg.encode())
                
                data = conn.recv(1024)
                if data:
                    print(f"Received: {data.decode()}")
            conn.close() # close the TCP connection with the client and also close the socket fd
            # TCP connection is established with the newly rcvd socket and not the welcoming socket
                
        except ConnectionError as e:
            print(f"Connection error: {e}")
        except Exception as e:
            print(f"Error: {e}")
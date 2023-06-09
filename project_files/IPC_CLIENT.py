#CLIENT CODE FOR IPC
import socket

# Create a socket object
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Define the server host and port
server_host = '192.168.0.160'  # Updated IP address of the server
server_port = 12345

# Connect to the server
client_socket.connect((server_host, server_port))

# Send data to the server
message = "Hello from the client!"
client_socket.send(message.encode())

# Receive the response from the server
response = client_socket.recv(1024).decode()
print("Response from server:", response)

# Close the connection
client_socket.close()


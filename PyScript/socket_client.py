import socket

if __name__ == "__main__":
	ADDRESS = 'localhost'
	PORT = 12001
	socketClient = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	socketClient.connect((ADDRESS, PORT))
	while True:
		sendBuf = raw_input()
		socketClient.send(sendBuf)
	socketClient.close()
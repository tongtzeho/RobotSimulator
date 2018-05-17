import socket, struct, time

if __name__ == "__main__":
	ADDRESS = 'localhost'
	PORT = 12001
	socketClient = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	socketClient.connect((ADDRESS, PORT))
	socketAddr = socketClient.recv(16)
	socketAddr = struct.unpack("=I", socketAddr)[0]
	print socketAddr
	socketClient.send("import coolengine as ce\nimport robotsimulator as sim\nkinectRGB = sim.rgbSensor(ce.findEntity('Kinect').getComponent('RGBSensor'))")
	while True:
		socketClient.send("kinectRGB.getRemoteData("+str(socketAddr)+")")
		image = socketClient.recv(2000000)
		height, width, firstPixel = struct.unpack("=III", image[:12])
		lastPixel = struct.unpack("=I", image[-4:])[0]
		print time.time(), height, width, firstPixel, lastPixel
	socketClient.close()
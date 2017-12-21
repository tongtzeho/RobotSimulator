import coolengine as ce

class switchCamera:
	def __init__(self, param = None):
		self.overLookCameraAddr = int(param.split(' ')[0])
		self.freeCameraAddr = int(param.split(' ')[1])
	
	def update(self, dt, param = None):
		if ce.isPressed(49): # 1
			ce.setMainCameraAddr(self.overLookCameraAddr)
		elif ce.isPressed(50): # 2
			ce.setMainCameraAddr(self.freeCameraAddr)
		return 0
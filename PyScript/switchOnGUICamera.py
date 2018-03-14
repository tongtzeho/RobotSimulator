import coolengine as ce

class switchOnGUICamera:
	def __init__(self, entity, param = None):
		pass
	
	def start(self, param = None):
		epuckCameraEntities = ce.findEntities("Epuck2Camera")
		epuckCameras = [ce.camera(e.getComponent("Camera")) for e in epuckCameraEntities]
		if len(epuckCameras):
			onGUICamera = len(epuckCameras)-1
			for c in range(len(epuckCameras)):
				if epuckCameras[c].getDepth() == 2:
					break
			epuckCameras[c].setDepth(-1)
			epuckCameras[(c+1)%len(epuckCameras)].setDepth(2)
	
	def update(self, dt, param = None):
		if ce.isPressed(9): # TAB
			self.start()
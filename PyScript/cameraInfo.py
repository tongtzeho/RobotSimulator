import coolengine as ce

class cameraInfo:
	def start(self, sObj, param = None):
		self.guiText = ce.guiObject(sObj)
	
	def update(self, dt, param = None):
		mainCamera = ce.gameObject(ce.getMainCameraAddr())
		worldMatrix = mainCamera.getWorldMatrix()
		cameraDirWorld = ce.vector3(0, 0, 1).transform(worldMatrix)
		self.guiText.setText("Camera: Pos = (%f, %f, %f) Dir = (%f, %f, %f)" % (worldMatrix.m41, worldMatrix.m42, worldMatrix.m43, cameraDirWorld.x, cameraDirWorld.y, cameraDirWorld.z))
		return 0
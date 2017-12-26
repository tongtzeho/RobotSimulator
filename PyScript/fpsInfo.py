import coolengine as ce

class fpsInfo:
	def start(self, sObj, param = None):
		self.guiText = ce.guiObject(sObj)
	
	def update(self, dt, param = None):
		self.guiText.setText("FPS = %d" % ce.getFPS())
		return 0
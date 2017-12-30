import coolengine as ce

class fpsInfo:
	def start(self, sObj, param = None):
		self.guiText = ce.guiObject(sObj)
	
	def update(self, dt, param = None):
		self.guiText.setText("Logic FPS = %d\nRender FPS = %d" % (ce.getFPS(True), ce.getFPS(False)))
		return 0
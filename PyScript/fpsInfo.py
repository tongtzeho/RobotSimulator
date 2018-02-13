import coolengine as ce

class fpsInfo:
	def __init__(self, entity, param = None):
		self.text = ce.text(entity.getComponent("Text", "FPS"))
	
	def update(self, dt, param = None):
		self.text.setContent(" Main Thread FPS = %d\nRender Thread FPS = %d" % (ce.getFPS(True), ce.getFPS(False)))
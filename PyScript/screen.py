import coolengine as ce

class screen:
	def __init__(self, entity, param = None):
		self.texture = ce.texture(entity.getComponent("Texture"))
		self.width = 640
		self.height = 360
		self.entityWidth = 64
		self.entityHeight = 36
		self.pixels = [0xFFFFFFFF] * self.width * self.height
		self.mainCamera = ce.camera(ce.findEntity("FreeCamera").getComponent("Camera"))
		
	def update(self, dt, param = None):
		self.texture.write(self.pixels)
		if ce.isDown(1): # Left Mouse
			posW, dirW = self.mainCamera.cursorRayToWorldFromProj(ce.getCursorRayDirInProj())
			if dirW.y != 0:
				x, z = posW.x - dirW.x * posW.y / dirW.y, posW.z - dirW.z * posW.y / dirW.y
				if x >= -self.entityWidth and x <= self.entityWidth and z >= -self.entityHeight and z <= self.entityHeight:
					x, z = int((x + self.entityWidth) / (self.entityWidth * 2) * self.width), int((1 - (z + self.entityHeight) / (self.entityHeight * 2)) * self.height)
					self.pixels[x+z*self.width] = 0xFF00FF00 # ARGB
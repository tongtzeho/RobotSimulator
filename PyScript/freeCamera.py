import coolengine as ce
import math

class freeCamera:
	def start(self, sObj, param = None):
		self.speed = 36.0
		self.angleSpeedX = 256.0
		self.angleSpeedY = 144.0
		self.camera = ce.gameObject(sObj)
		self.prevCursorPos = None
		self.euler = self.camera.getEulerRadian()
		self.euler.z = 0
	
	def update(self, dt, param = None):
		if self.camera.getAddr() == ce.getMainCameraAddr():
			if ce.isDown(87): # W
				self.camera.translate(ce.vector3(0, 0, self.speed*dt).rotate(self.camera.getQuaternion()))
			if ce.isDown(83): # S
				self.camera.translate(ce.vector3(0, 0, -self.speed*dt).rotate(self.camera.getQuaternion()))
			if ce.isDown(65): # A
				self.camera.translate(ce.vector3(-self.speed*dt, 0, 0).rotate(self.camera.getQuaternion()))
			if ce.isDown(68): # D
				self.camera.translate(ce.vector3(self.speed*dt, 0, 0).rotate(self.camera.getQuaternion()))
			if ce.isDown(69): # E
				self.camera.translate(ce.vector3(0, self.speed*dt, 0).rotate(self.camera.getQuaternion()))
			if ce.isDown(81): # Q
				self.camera.translate(ce.vector3(0, -self.speed*dt, 0).rotate(self.camera.getQuaternion()))
		self.cursorPos = ce.getCursorRayDirInProj()
		if self.prevCursorPos == None:
			self.prevCursorPos = self.cursorPos
		if self.camera.getAddr() == ce.getMainCameraAddr():			
			if ce.isDown(2): # Right
				dy = self.angleSpeedX*dt*(self.cursorPos.x-self.prevCursorPos.x)
				dx = -self.angleSpeedY*dt*(self.cursorPos.y-self.prevCursorPos.y)
				self.euler.x = ce.clampRadian(self.euler.x+dx, -1.4, 1.4)
				self.euler.y = ce.clampRadian(self.euler.y+dy, -math.pi, math.pi)
				self.camera.setEulerRadian(self.euler)
		self.prevCursorPos = self.cursorPos
		return 0
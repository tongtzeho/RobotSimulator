import coolengine as ce
import math

class freeCamera:
	def __init__(self, entity, param = None):
		self.entity = entity
		self.speed = 30.0
		self.angleSpeedX = 256.0
		self.angleSpeedY = 144.0
	
	def start(self, param = None):
		self.prevCursorPos = ce.getCursorRayDirInProj()
		self.text = ce.text(self.entity.getComponent("Text"))
	
	def updateGUI(self):
		position = self.entity.getTranslation()
		direction = ce.vector3(0, 0, 1).rotate(self.entity.getQuaternion())
		self.text.setContent("Pos(%f, %f, %f)\nDir(%f, %f, %f)" % (position.x, position.y, position.z, direction.x, direction.y, direction.z))
	
	def update(self, dt, param = None):
		self.euler = self.entity.getEulerAngle()
		if ce.isDown(87): # W
			self.entity.translate(ce.vector3(0, 0, self.speed*dt).rotate(self.entity.getQuaternion()))
		if ce.isDown(83): # S
			self.entity.translate(ce.vector3(0, 0, -self.speed*dt).rotate(self.entity.getQuaternion()))
		if ce.isDown(65): # A
			self.entity.translate(ce.vector3(-self.speed*dt, 0, 0).rotate(self.entity.getQuaternion()))
		if ce.isDown(68): # D
			self.entity.translate(ce.vector3(self.speed*dt, 0, 0).rotate(self.entity.getQuaternion()))
		if ce.isDown(69): # E
			self.entity.translate(ce.vector3(0, self.speed*dt, 0).rotate(self.entity.getQuaternion()))
		if ce.isDown(81): # Q
			self.entity.translate(ce.vector3(0, -self.speed*dt, 0).rotate(self.entity.getQuaternion()))
		self.cursorPos = ce.getCursorRayDirInProj()
		if ce.isDown(2): # Right Mouse
			dy = self.angleSpeedX*dt*(self.cursorPos.x-self.prevCursorPos.x)
			dx = -self.angleSpeedY*dt*(self.cursorPos.y-self.prevCursorPos.y)
			self.euler.x = ce.clampAngle(self.euler.x+dx, -1.4, 1.4)
			self.euler.y = ce.clampAngle(self.euler.y+dy, -math.pi, math.pi)
			self.euler.z = 0
			self.entity.setEulerAngle(self.euler)
		self.prevCursorPos = self.cursorPos
		self.updateGUI()
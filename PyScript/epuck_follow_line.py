import epuck
import math
import random

"""
For Single Epuck:

EpuckName = "e-puck2_00001"
import coolengine as ce
ce.findEntity(EpuckName).addComponent("Script", "epuck_follow_line")

For All Epucks:

epuckNameRe = "e-puck2_[0-9]+"
import coolengine as ce
for e in ce.matchEntities(epuckNameRe):
  e.addComponent("Script", "epuck_follow_line")
"""

class epuck_follow_line:
	def __init__(self, entity, param = None):
		self.entity = entity
		self.randomThreshold = 0.85
		self.following = True
		self.minDist = 1.5
		self.avoidanceOmega = 0.5
		self.avoidanceVelocity = 1.5
		self.avoidanceRadius = 7.3+self.minDist
		self.avoidanceState = 0
		
	def start(self, param = None):
		self.epuck = epuck.epuck(self.entity)
		self.epuck.rgbSensor.setEnabled(True)
		
	def getStateFromRGBSensor(self):
		pixels = self.epuck.rgbSensor.getData()
		bottom = pixels[-1]
		width = len(bottom)
		redLeft = 0
		for i in range(int(width*0.2), int(width*0.4)):
			redLeft += bottom[i][0]
		redMid = 0
		for i in range(int(width*0.4), int(width*0.6)):
			redMid += bottom[i][0]
		redRight = 0
		for i in range(int(width*0.6), int(width*0.8)):
			redRight += bottom[i][0]
		if redMid >= redLeft and redMid >= redRight:
			return [5, 0]
		elif redLeft >= redMid and redLeft >= redRight:
			return [2, -1.4]
		else:
			return [2, 1.4]
	
	def getStateInAviodance(self, dt):
		if self.avoidanceState == 0:
			if self.epuck.ir2.getData():
				self.turnTime = 0
				self.avoidanceState = 1
			return [0, -self.avoidanceOmega]
		elif self.avoidanceState == 1:
			if self.epuck.ir2.getData():
				self.turnTime += dt
				return [0, -self.avoidanceOmega]
			else:
				self.turnTime /= 2
				self.avoidanceState = 2
				return [0, 0]
		elif self.avoidanceState == 2:
			self.turnTime -= dt
			if self.turnTime <= 0:
				self.avoidanceState = 3
			return [0, self.avoidanceOmega]
		elif self.avoidanceState == 3:
			pixels = self.epuck.rgbSensor.getData()
			bottom = pixels[-1]
			width = len(bottom)
			mid = bottom[int(width*0.5)]
			if mid[0] > 0.8 and (mid[1] > 0.8 or mid[2] < 0.2):
				self.following = True
				self.avoidanceState = 0
				return self.getStateFromRGBSensor()
			else:
				return [self.avoidanceVelocity, self.avoidanceVelocity/self.avoidanceRadius]
				
	def step(self, dt, param = None):
		if self.following:
			dist = self.epuck.tof.getData()
			if dist != None and dist < self.minDist:
				self.following = False
				self.epuck.actionController.setStates(self.getStateInAviodance(dt))
			else:
				self.epuck.actionController.setStates(self.getStateFromRGBSensor())
		else:
			self.epuck.actionController.setStates(self.getStateInAviodance(dt))
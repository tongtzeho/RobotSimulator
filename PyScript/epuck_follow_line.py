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
		self.minDist = 2
		self.avoidanceOmega = 0.37
		self.avoidanceVelocity = 2.1
		
	def start(self, param = None):
		self.epuck = epuck.epuck(self.entity)
		self.epuck.rgbSensor.setEnabled(True)
		
	def getStateFromRGBSensor(self):
		pixels = self.epuck.rgbSensor.getData()
		bottom = pixels[-1]
		width = len(bottom)
		left = 0
		for i in range(int(width*0.2), int(width*0.4)):
			left += bottom[i][0]
		mid = 0
		for i in range(int(width*0.4), int(width*0.6)):
			mid += bottom[i][0]
		right = 0
		for i in range(int(width*0.6), int(width*0.8)):
			right += bottom[i][0]
		sum = left+mid+right
		if sum > 0 and random.random() > self.randomThreshold:
			leftPossi = left/sum
			midPossi = mid/sum
			rightPossi = 1-leftPossi-midPossi
			rand = random.random()
			if rand <= leftPossi:
				return [2, -1.4]
			elif rand <= leftPossi+midPossi:
				return [5, 0]
			else:
				return [2, 1.4]
		else:
			if mid >= left and mid >= right:
				return [5, 0]
			elif left >= mid and left >= right:
				return [2, -1.4]
			else:
				return [2, 1.4]
	
	def getStateInAviodance(self):
		if (not self.epuck.ir0.getData()) and (not self.epuck.ir7.getData()):
			pixels = self.epuck.rgbSensor.getData()
			bottom = pixels[-1]
			width = len(bottom)
			mid = bottom[int(width*0.5)]
			if mid[0] > 0.8 and mid[1] < 0.3 and mid[2] < 0.3:
				self.following = True
				return self.getStateFromRGBSensor()
		ir1Result = self.epuck.ir1.getData()
		ir2Result = self.epuck.ir2.getData()
		if not ir2Result:
			return [0, -self.avoidanceOmega]
		else:
			if not ir1Result:
				return [self.avoidanceVelocity, self.avoidanceOmega/2]
			elif ir1Result:
				return [self.avoidanceVelocity, -self.avoidanceOmega/2]
	
	def step(self, dt, param = None):
		if self.following:
			dist = self.epuck.tof.getData()
			if dist != None and dist < self.minDist:
				self.following = False
				self.epuck.actionController.setStates(self.getStateInAviodance())
			else:
				self.epuck.actionController.setStates(self.getStateFromRGBSensor())
		else:
			self.epuck.actionController.setStates(self.getStateInAviodance())
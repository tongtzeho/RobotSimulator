import epuck
import math
import random

"""
epuckNameRe = "e-puck2_[0-9]+"
import coolengine as ce
for e in ce.matchEntities(epuckNameRe):
  e.addComponent("Script", "epuck_follow_line_relay", "0")

epuck01Name = ce.findEntity("e-puck2_00001")
ce.script(epuck01Name.getComponent("Script", "epuck_follow_line_relay")).awake()
"""

class epuck_follow_line_relay:
	def __init__(self, entity, param = None):
		self.entity = entity
		self.isAwake = bool(int(param))
		self.randomThreshold = 0.85
	
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
				return [2, -1.4, 0]
			elif rand <= leftPossi+midPossi:
				return [5, 0, 0]
			else:
				return [2, 1.4, 0]
		else:
			if mid >= left and mid >= right:
				return [5, 0, 0]
			elif left >= mid and left >= right:
				return [2, -1.4, 0]
			else:
				return [2, 1.4, 0]
	
	def step(self, dt, param = None):
		if self.isAwake:
			dist = self.epuck.tof.getData()
			if dist != None and dist < 1.5:
				self.isAwake = False
				self.epuck.actionController.setStates([0, 0, 1])
			else:
				self.epuck.actionController.setStates(self.getStateFromRGBSensor())
		else:
			self.epuck.actionController.setStates([0, 0, 0])
			if len(self.epuck.comm.read()):
				self.isAwake = True
			
	def awake(self):
		self.isAwake = True
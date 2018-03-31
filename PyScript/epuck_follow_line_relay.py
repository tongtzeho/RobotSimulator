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
		self.distThreshold = 5
	
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
	
	def step(self, dt, param = None):
		if self.isAwake:
			dist = self.epuck.tof.getData()
			if dist != None and dist < self.distThreshold:
				self.isAwake = False
				self.epuck.actionController.setStates([0, 0, 1])
			else:
				states = self.getStateFromRGBSensor()
				states.append(0)
				self.epuck.actionController.setStates(states)
		else:
			self.epuck.actionController.setStates([0, 0, 0])
			if len(self.epuck.comm.read()):
				self.isAwake = True
			
	def awake(self):
		self.isAwake = True
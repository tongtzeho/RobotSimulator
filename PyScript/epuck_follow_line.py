import coolengine as ce
import robotsimulator as sim
import random

"""
EpuckName = "e-puck2_00001"
import coolengine as ce
ce.findEntity(EpuckName).addComponent("Script", "epuck_follow_line")
"""

class epuck_follow_line:
	def __init__(self, entity, param = None):
		self.entity = entity
	
	def start(self, param = None):
		self.rgbSensor = sim.rgbSensor(self.entity.findChildEntity("Epuck2Camera").getComponent("RGBSensor"))
		self.rgbSensor.setEnabled(True)
		self.actionController = sim.actionController(self.entity.getComponent("ActionController"))
		self.randomThreshold = 0.7
	
	def getStateFromRGBSensor(self):
		pixels = self.rgbSensor.getData()
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
		if random.random() > self.randomThreshold:
			leftPossi = left/(left+mid+right)
			midPossi = mid/(left+mid+right)
			rightPossi = 1-leftPossi-midPossi
			rand = random.random()
			if rand <= leftPossi:
				return 9
			elif rand <= leftPossi+midPossi:
				return 2
			else:
				return 13
		else:
			if mid >= left and mid >= right:
				return 2
			elif left >= mid and left >= right:
				return 9
			else:
				return 13
	
	def step(self, dt, param = None):
		self.actionController.setState(self.getStateFromRGBSensor())
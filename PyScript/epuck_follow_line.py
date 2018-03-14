import coolengine as ce
import robotsimulator as sim
import random

"""
epuckNameRe = "e-puck2_[0-9]+"
import coolengine as ce
for e in ce.matchEntities(epuckNameRe):
  e.addComponent("Script", "epuck_follow_line", "0")

epuck01Name = ce.findEntity("e-puck2_00001")
ce.script(epuck01Name.getComponent("Script", "epuck_follow_line")).awake()
"""

class epuck_follow_line:
	def __init__(self, entity, param = None):
		self.entity = entity
		self.isAwake = bool(int(param))
	
	def start(self, param = None):
		self.rgbSensor = sim.rgbSensor(self.entity.findChildEntity("Epuck2Camera").getComponent("RGBSensor"))
		self.rgbSensor.setEnabled(True)
		self.actionController = sim.actionController(self.entity.getComponent("ActionController"))
		self.ir0 = sim.proximitySensor(self.entity.findChildEntity("Epuck2IR0").getComponent("ProximitySensor"))
		self.ir1 = sim.proximitySensor(self.entity.findChildEntity("Epuck2IR1").getComponent("ProximitySensor"))
		self.ir6 = sim.proximitySensor(self.entity.findChildEntity("Epuck2IR6").getComponent("ProximitySensor"))
		self.ir7 = sim.proximitySensor(self.entity.findChildEntity("Epuck2IR7").getComponent("ProximitySensor"))
		self.comm = sim.communicator(self.entity.getComponent("Communicator"))
		self.randomThreshold = 0.85
	
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
		sum = left+mid+right
		if sum > 0 and random.random() > self.randomThreshold:
			leftPossi = left/sum
			midPossi = mid/sum
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
		if self.isAwake:
			if self.ir0.getData() or self.ir1.getData() or self.ir6.getData() or self.ir7.getData():
				self.isAwake = False
				self.actionController.setState(0)
				self.comm.send("Go")
			else:
				self.actionController.setState(self.getStateFromRGBSensor())
		else:
			self.actionController.setState(0)
			if len(self.comm.read()):
				self.isAwake = True
			
	def awake(self):
		self.isAwake = True
import coolengine as ce
import robotsimulator as sim
import random

"""
epuckNameRe = "e-puck2_[0-9]+"
import coolengine as ce
for e in ce.matchEntities(epuckNameRe):
  e.addComponent("Script", "epuck_consensus")
"""

class epuck_consensus:
	def __init__(self, entity, param = None):
		self.entity = entity
		self.turnDir = random.random()
		self.state = 0 # 0 for init, 1 for turning to target dir, 2 for after sending a msg
		self.waitingTime = 1.5
		self.wrongDirPossi = 0.1
		self.round = 0
	
	def direction(self):
		r1 = self.ir1.getData()
		r2 = self.ir2.getData()
		r3 = self.ir3.getData()
		r4 = self.ir4.getData()
		r5 = self.ir5.getData()
		r6 = self.ir6.getData()
		if r1 and r2 and r3 and (not r4) and (not r5) and (not r6):
			return 1 # Right To Wall
		elif r4 and r5 and r6 and (not r1) and (not r2) and (not r3):
			return -1 # Left To Wall
		else:
			return 0
	
	def start(self, param = None):
		self.ir1 = sim.proximitySensor(self.entity.findChildEntity("Epuck2IR1").getComponent("ProximitySensor")) # Right Front
		self.ir2 = sim.proximitySensor(self.entity.findChildEntity("Epuck2IR2").getComponent("ProximitySensor")) # Right
		self.ir3 = sim.proximitySensor(self.entity.findChildEntity("Epuck2IR3").getComponent("ProximitySensor")) # Right Back
		self.ir4 = sim.proximitySensor(self.entity.findChildEntity("Epuck2IR4").getComponent("ProximitySensor")) # left Back
		self.ir5 = sim.proximitySensor(self.entity.findChildEntity("Epuck2IR5").getComponent("ProximitySensor")) # Left
		self.ir6 = sim.proximitySensor(self.entity.findChildEntity("Epuck2IR6").getComponent("ProximitySensor")) # Left Front
		self.comm = sim.communicator(self.entity.getComponent("Communicator"))
		self.actionController = sim.actionController(self.entity.getComponent("ActionController"))
	
	def step(self, dt, param = None):
		if self.state == 0:
			self.dir = self.direction()
			if self.dir == 0:
				if self.turnDir < 0.5:
					self.actionController.setState(12)
				else:
					self.actionController.setState(8)
			else:
				self.actionController.setState(0)
				self.state = 2
				self.currWaiting = 0
				self.round += 1
				self.comm.send(str(self.dir))
		elif self.state == 2:
			self.currWaiting += dt
			if self.currWaiting > self.waitingTime:
				msgs = self.comm.read()
				cnt1 = 0
				for m in msgs:
					if m == "1":
						cnt1 += 1
					else:
						cnt1 -= 1
				self.dir = self.direction()
				if self.dir == 1:
					if cnt1 < 0:
						self.targetDir = -1
						self.state = 1
					else:
						self.currWaiting = -0.5
						self.state = 2
						self.round += 1
						if random.random() < self.wrongDirPossi:
							self.comm.send(str(-self.dir))
						else:
							self.comm.send(str(self.dir))
				elif self.dir == -1:
					if cnt1 > 0:
						self.targetDir = 1
						self.state = 1
					else:
						self.currWaiting = -0.5
						self.state = 2
						self.round += 1
						if random.random() < self.wrongDirPossi:
							self.comm.send(str(-self.dir))
						else:
							self.comm.send(str(self.dir))
		elif self.state == 1:
			self.dir = self.direction()
			if self.dir != self.targetDir:
				if self.turnDir < 0.5:
					self.actionController.setState(12)
				else:
					self.actionController.setState(8)
			else:
				self.actionController.setState(0)
				self.currWaiting = 0
				self.state = 2
				self.round += 1
				if random.random() < self.wrongDirPossi:
					self.comm.send(str(-self.dir))
				else:
					self.comm.send(str(self.dir))
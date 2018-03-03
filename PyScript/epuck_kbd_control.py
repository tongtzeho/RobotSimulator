import coolengine as ce
import robotsimulator as sim

"""
EpuckName = "e-puck2_00001"
import coolengine as ce
ce.findEntity(EpuckName).addComponent("Script", "epuck_kbd_control")
"""

class epuck_kbd_control:
	def __init__(self, entity, param = None):
		self.entity = entity
	
	def start(self, param = None):
		self.actionController = sim.actionController(self.entity.getComponent("ActionController"))
	
	def step(self, dt, param = None):
		state = 0
		if ce.isDown(38): # UpArrow
			state += 1
		if ce.isDown(37): # LeftArrow
			state += 8
		elif ce.isDown(39): # RightArrow
			state += 12
		self.actionController.setState(state)
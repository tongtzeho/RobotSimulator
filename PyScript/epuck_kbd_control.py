import coolengine as ce
import epuck

"""
EpuckName = "e-puck2_00001"
import coolengine as ce
ce.findEntity(EpuckName).addComponent("Script", "epuck_kbd_control")
"""

class epuck_kbd_control:
	def __init__(self, entity, param = None):
		self.entity = entity
	
	def start(self, param = None):
		self.epuck = epuck.epuck(self.entity)
	
	def step(self, dt, param = None):
		states = [0, 0]
		if ce.isDown(38): # UpArrow
			states[0] = 3
		elif ce.isDown(40): # DownArrow
			states[0] = -3
		if ce.isDown(37): # LeftArrow
			states[1] = -1.3
		elif ce.isDown(39): # RightArrow
			states[1] = 1.3
		self.epuck.actionController.setStates(states)
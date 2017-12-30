import coolengine as ce

class kbdControl:
	def step(self, dt, param = None):
		actionCode = 0
		if ce.isDown(37): # LeftArrow
			actionCode += 8
		elif ce.isDown(39): # RightArrow
			actionCode += 4
		if ce.isDown(38): # UpArrow
			actionCode += 2
		elif ce.isDown(40): # DownArrow
			actionCode += 1
		return actionCode
import coolengine as ce

"""
coolengine.findEntity("GlobalSystem").addComponent("Script", "consensus_stat")
"""

class consensus_stat:
	def __init__(self, entity, param = None):
		self.printed = False
	
	def start(self, param = None):
		epuckNameRe = "e-puck2_[0-9]+"
		epucks = ce.matchEntities(epuckNameRe)
		self.scripts = [ce.script(e.getComponent("Script", "epuck_consensus")) for e in epucks]
	
	def step(self, dt, param = None):
		if not self.printed:
			dirs = [s.dir for s in self.scripts]
			if max(dirs) == 1 and min(dirs) == 1:
				print 1, self.scripts[0].round
				self.printed = True
			elif max(dirs) == -1 and min(dirs) == -1:
				print -1, self.scripts[0].round
				self.printed = True
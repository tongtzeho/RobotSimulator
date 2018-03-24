import coolengine as ce
import robotsimulator as sim

class safetyInfo:
	def __init__(self, entity, param = None):
		self.entity = entity
		
	def start(self):
		self.text = ce.text(self.entity.getComponent("Text", "Safety"))
	
	def update(self, dt, param = None):
		unsafe = sim.getUnsafe()
		if not len(unsafe):
			self.text.setContent("All Safe")
		else:
			content = "Unsafe:\n"
			for ue in unsafe:
				content += ue.getName()+"\n"
			self.text.setContent(content)
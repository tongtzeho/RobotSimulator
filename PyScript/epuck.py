import robotsimulator as sim

class epuck:
	def __init__(self, entity):
		self.rgbSensor = sim.rgbSensor(entity.findChildEntity("Epuck2Camera").getComponent("RGBSensor"))
		self.ir0 = sim.proximitySensor(entity.findChildEntity("Epuck2IR0").getComponent("ProximitySensor"))
		self.ir1 = sim.proximitySensor(entity.findChildEntity("Epuck2IR1").getComponent("ProximitySensor"))
		self.ir2 = sim.proximitySensor(entity.findChildEntity("Epuck2IR2").getComponent("ProximitySensor"))
		self.ir3 = sim.proximitySensor(entity.findChildEntity("Epuck2IR3").getComponent("ProximitySensor"))
		self.ir4 = sim.proximitySensor(entity.findChildEntity("Epuck2IR4").getComponent("ProximitySensor"))
		self.ir5 = sim.proximitySensor(entity.findChildEntity("Epuck2IR5").getComponent("ProximitySensor"))
		self.ir6 = sim.proximitySensor(entity.findChildEntity("Epuck2IR6").getComponent("ProximitySensor"))
		self.ir7 = sim.proximitySensor(entity.findChildEntity("Epuck2IR7").getComponent("ProximitySensor"))
		self.tof = sim.distanceSensor(entity.findChildEntity("Epuck2ToF").getComponent("DistanceSensor"))
		self.imu = sim.imuSensor(entity.getComponent("IMUSensor"))
		self.comm = sim.communicator(entity.getComponent("Communicator"))
		self.actionController = sim.actionController(entity.getComponent("ActionController"))
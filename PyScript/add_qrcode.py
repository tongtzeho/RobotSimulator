import coolengine as ce

class add_qrcode:
	def __init__(self, entity, param = None):
		pass
		
	def start(self, param = None):
		epuckNameRe = "e-puck2_[0-9]+"
		epucks = ce.matchEntities(epuckNameRe)
		i = 1
		for e in epucks:
			qrcode = e.findChildEntity("Epuck2QRCode")
			qrcode.addComponent("Texture", "DiffuseMap", "@qrcode"+str(i)+".png")
			i += 1
			qrcode.addComponent("Renderer", "MeshRenderer", "1 1")
def send(sender, msg, comms):
	distanceSq = 15*15
	delay = 1
	result = []
	for comm in comms:
		if comm != sender and (comm.getPosition()-sender.getPosition()).square() <= distanceSq:
			result.append((comm, msg, delay))
	return result


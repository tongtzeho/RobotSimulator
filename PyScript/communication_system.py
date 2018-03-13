def send(sender, msg, comms):
	distance = 12
	result = []
	for comm in comms:
		if comm != sender and (comm.getPosition()-sender.getPosition()).square() <= distance*distance:
			result.append((comm, msg))
	return result
function BuildWallMesh(radius, slice, height1, height2)
	mesh = {Type = "ffffffff", V = {}, I = {}}
	dTheta = 2*math.pi/slice
	theta = dTheta
	table.insert(mesh.V, {radius, height1, 0, -1, 0, 0, 0.5, 0.5})
	table.insert(mesh.V, {radius, height2, 0, -1, 0, 0, 0.5, 0.5})
	for i = 0, slice, 1 do
		table.insert(mesh.V, {radius*math.cos(theta), height1, radius*math.sin(theta), -math.cos(theta), 0, -math.sin(theta), 0.5, 0.5})
		table.insert(mesh.V, {radius*math.cos(theta), height2, radius*math.sin(theta), -math.cos(theta), 0, -math.sin(theta), 0.5, 0.5})
		table.insert(mesh.I, #mesh.V-1)
		table.insert(mesh.I, #mesh.V-2)
		table.insert(mesh.I, #mesh.V-3)
		table.insert(mesh.I, #mesh.V-2)
		table.insert(mesh.I, #mesh.V-3)
		table.insert(mesh.I, #mesh.V-4)
		table.insert(mesh.I, #mesh.V-1)
		table.insert(mesh.I, #mesh.V-3)
		table.insert(mesh.I, #mesh.V-2)
		table.insert(mesh.I, #mesh.V-2)
		table.insert(mesh.I, #mesh.V-4)
		table.insert(mesh.I, #mesh.V-3)
		theta = theta + dTheta
	end
	return mesh
end

Mesh = BuildWallMesh(36, 80, 0, 14)

Material = {
	Amb = "0.3 0.3 0.3 1.0",
	Diff = "1.0 1.0 1.0 1.0",
	Spec = "0.1 0.1 0.1",
	Power = 32.0,
	Reflect = "0 0 0 0"
}

Polygon = Mesh

Rigidbody = {
	Shape = "@circle_wall.lua:Polygon",
	Mass = 0,
}

Entity = {
	Comp = {
		{"Mesh", "@circle_wall.lua:Mesh"},
		{"Material", "@circle_wall.lua:Material"},
		{"Texture", "DiffuseMap", "@wall.png"},
		{"Renderer", "MeshRenderer", "1 1"},
		{"Rigidbody", "@circle_wall.lua:Rigidbody"},
	}
}
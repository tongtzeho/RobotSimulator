Mesh = {
	Type = "ffffffff",
	V = {
		{-2000, -4, -2000, 0, 1, 0, 0, 40},
		{-2000, -4, 2000, 0, 1, 0, 0, 0},
		{2000, -4, -2000, 0, 1, 0, 40, 40},
		{2000, -4, 2000, 0, 1, 0, 40, 0},
	},
	I = {0, 1, 2, 1, 3, 2, 1, 0, 2, 3, 1, 2}
}

Material = {
	0.4, 0.4, 0.4, 0.9,
	1, 1, 1, 1.0,
	0.1, 0.1, 0.1, 32.0,
	0.0, 0.0, 0.0, 0.0
}

Rigidbody = {
	Shape = "Box 2000 10 2000",
	T = "0 -14 0",
	Mass = 0,
}

Entity = {
	Comp = {
		{"Mesh", "@floor.lua:Mesh"},
		{"Material", "@floor.lua:Material"},
		{"Texture", "DiffuseMap", "@floor.dds"},
		{"Renderer", "MeshRenderer", "0 0"},
		{"Rigidbody", "@floor.lua:Rigidbody"}
	}
}
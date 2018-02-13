Mesh = {
	Type = "ffffffff",
	V = {
		{-66, -0.18, -38, -0.8233933772, 0.3118914308, -0.4740749748, 0, 1},
		{-66, -0.18, 38, -0.8233933772, 0.3118914308, 0.4740749748, 0, 0},
		{66, -0.18, -38, 0.8233933772, 0.3118914308, -0.4740749748, 1, 1},
		{66, -0.18, 38, 0.8233933772, 0.3118914308, 0.4740749748, 1, 0},
		{-66, -4, -38, -0.8233933772, -0.3118914308, -0.4740749748, 0, 1},
		{-66, -4, 38, -0.8233933772, -0.3118914308, 0.4740749748, 0, 0},
		{66, -4, -38, 0.8233933772, -0.3118914308, -0.4740749748, 1, 1},
		{66, -4, 38, 0.8233933772, -0.3118914308, 0.4740749748, 1, 0},
	},
	I = {
		0, 2, 4, 2, 6, 4,
		1, 0, 4, 1, 4, 5,
		3, 1, 5, 5, 7, 3,
		2, 3, 6, 6, 3, 7,
		0, 1, 2, 1, 3, 2
	}
}

Material = {
	0.6, 0.6, 0.6, 0.9,
	0.9, 0.9, 0.9, 1.0,
	0.4, 0.4, 0.4, 32.0,
	0.0, 0.0, 0.0, 0.0
}

Entity = {
	Comp = {
		{"Mesh", "@platform.lua:Mesh"},
		{"Material", "@platform.lua:Material"},
		{"Texture", "DiffuseMap", "@platform.png"},
		{"Renderer", "MeshRenderer", "0 0"}
	}
}
Mesh = {
	Type = "ffffffff",
	V = {
		{-64, 0, -36, 0, 1, 0, 0.001, 0.999},
		{-64, 0, 36, 0, 1, 0, 0.001, 0.001},
		{64, 0, -36, 0, 1, 0, 0.999, 0.999},
		{64, 0, 36, 0, 1, 0, 0.999, 0.001}
	},
	I = {
		0, 1, 2, 1, 3, 2
	}
}

Material = {
	0.9, 0.9, 0.9, 0.9,
	0.8, 0.8, 0.8, 1.0,
	0.5, 0.5, 0.5, 32.0,
	0.0, 0.0, 0.0, 0.0
}

Entity = {
	Comp = {
		{"Mesh", "@screen.lua:Mesh"},
		{"Material", "@screen.lua:Material"},
		{"Texture", "DiffuseMap", "@follow_line.png"},
		{"Renderer", "MeshRenderer", "0 1"}
	}
}
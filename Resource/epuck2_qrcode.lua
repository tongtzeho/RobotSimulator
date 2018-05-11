Mesh = {
	Type = "ffffffff",
	V = {
		{-1, 0, -1, 0, 0, -1, 0.01, 0.01},
		{-1, 0.1, -1, 0, 0, -1, 0.01, 0.01},
		{1, 0.1, -1, 0, 0, -1, 0.01, 0.01},
		{1, 0, -1, 0, 0, -1, 0.01, 0.01},
		
		{-1, 0, 1, 0, 0, 1, 0.01, 0.01},
		{1, 0, 1, 0, 0, 1, 0.01, 0.01},
		{1, 0.1, 1, 0, 0, 1, 0.01, 0.01},
		{-1, 0.1, 1, 0, 0, 1, 0.01, 0.01},
		
		{-1, 0.1, -1, 0, 1, 0, 0.02, 0.98},
		{-1, 0.1, 1, 0, 1, 0, 0.02, 0.02},
		{1, 0.1, 1, 0, 1, 0, 0.98, 0.02},
		{1, 0.1, -1, 0, 1, 0, 0.98, 0.98},
		
		{-1, 0, -1, 0, -1, 0, 0.01, 0.01},
		{1, 0, -1, 0, -1, 0, 0.01, 0.01},
		{1, 0, 1, 0, -1, 0, 0.01, 0.01},
		{-1, 0, 1, 0, -1, 0, 0.01, 0.01},
		
		{-1, 0, 1, -1, 0, 0, 0.01, 0.01},
		{-1, 0.1, 1, -1, 0, 0, 0.01, 0.01},
		{-1, 0.1, -1, -1, 0, 0, 0.01, 0.01},
		{-1, 0, -1, -1, 0, 0, 0.01, 0.01},
		
		{1, 0, -1, 1, 0, 0, 0.01, 0.01},
		{1, 0.1, -1, 1, 0, 0, 0.01, 0.01},
		{1, 0.1, 1, 1, 0, 0, 0.01, 0.01},
		{1, 0, 1, 1, 0, 0, 0.01, 0.01}
	},
	
	I = {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23}
}

Material = {
	Amb = "0.4 0.4 0.4 1.0",
	Diff = "0.8 0.8 0.8 1.0",
	Spec = "0.1 0.1 0.1",
	Power = 4.0,
	Reflect = "0 0 0 0"
}

Entity = {
	Comp = {
		{"Mesh", "@epuck2_qrcode.lua:Mesh"},
		{"Material", "@epuck2_qrcode.lua:Material"},
	}
}
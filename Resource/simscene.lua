Entity_FreeCamera = {
	Comp = {
		{"Camera", "1 0 1280 720 1 10000 1.308996939", "0.5 0.5 0.5 0.5 0 0 1 1 0 0"},
		{"Script", "freeCamera"}
	}
}

Light_Global = {
	Amb = "1 1 1 1",
	Diff = "0.9 0.9 0.9 1",
	Spec = "0.5 0.5 0.5 16",
	Dir = "0.5 -0.5 0.8",
	Pos = "0 0 0",
	Range = 70,
	Type = 1,
}

Text_FPS = {
	Rect = "1 0 1 0 -7 5 0 0.35 0 161",
	Color = "0 0 0 1",
	Depth = 15
}

Entity_GlobalSystem = {
	Child = {
		{ Config = "@simscene.lua:Entity_FreeCamera", Name = "FreeCamera", Q = "0.5 0 0 0.866025403784", T = "0 60 -50" }
	},
	Comp = {
		{ "Light", "@simscene.lua:Light_Global", "1" },
		{ "Text", "FPS", "@simscene.lua:Text_FPS" },
		{ "Script", "fpsInfo" },
	}
}

Scene = {
	Splash = "@splash.png",
	Root = {
		{ Config = "@simscene.lua:Entity_GlobalSystem", Name = "GlobalSystem" },
		{ Config = "@floor.lua:Entity", Name = "Floor" },
		{ Config = "@platform.lua:Entity", Name = "Platform" },
		{ Config = "@screen.lua:Entity", Name = "Screen" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_1", Q = "0 1 0 1", T = "-50 0 0" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_2", Q = "0 1 0 -1", T = "50 0 0" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_3", Q = "0 0 0 1", T = "0 0 -30" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_4", Q = "0 1 0 0", T = "0 0 30" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_5", Q = "0 0 0 1", T = "0 0 0" }
	}
}
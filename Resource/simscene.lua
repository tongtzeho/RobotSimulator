Rigidbody_Camera = {
	Shape = "sphere 0.3",
	Kinematic = true,
}

Entity_FreeCamera = {
	Comp = {
		{"Camera", "1 0 1280 720 0.1 5000 1.308996939", "0.5 0.5 0.5 0.5 0 0 1 1 0 0"},
		{"Script", "freeCamera"},
		{"Rigidbody", "@simscene.lua:Rigidbody_Camera"}
	}
}

Light_Global = {
	Amb = "1 1 1 1",
	Diff = "0.7 0.7 0.7 1",
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
		{ "Script", "switchOnGUICamera" }
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
		{ Config = "@epuck.lua:Entity", Name = "Epuck_5", Q = "0 0 0 1", T = "0 0 0" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_6", Q = "0 1 0 1", T = "-30 0 0" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_7", Q = "0 1 0 -1", T = "30 0 0" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_8", Q = "0 0 0 1", T = "0 0 -10" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_9", Q = "0 1 0 0", T = "0 0 10" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_10", Q = "0 0 0 1", T = "0 0 15" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_11", Q = "0 1 0 1", T = "-40 0 0" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_12", Q = "0 1 0 -1", T = "40 0 0" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_13", Q = "0 0 0 1", T = "0 0 -20" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_14", Q = "0 1 0 0", T = "0 0 20" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_15", Q = "0 0 0 1", T = "6 0 0" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_16", Q = "0 1 0 1", T = "-20 0 0" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_17", Q = "0 1 0 -1", T = "20 0 0" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_18", Q = "0 0 0 1", T = "0 0 -20" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_19", Q = "0 1 0 0", T = "0 0 20" },
		{ Config = "@epuck.lua:Entity", Name = "Epuck_20", Q = "0 0 0 1", T = "7 0 9" },
		{ Config = "@box.lua:Entity", Name = "Box_1", T = "-20 0 0" },
		{ Config = "@box.lua:Entity", Name = "Box_2", T = "-20 0 -2" },
		{ Config = "@box.lua:Entity", Name = "Box_3", T = "-20 0 2" },
		{ Config = "@box.lua:Entity", Name = "Box_4", T = "-20 3 0" },
		{ Config = "@box.lua:Entity", Name = "Box_5", T = "-20 3 -2" },
		{ Config = "@box.lua:Entity", Name = "Box_6", T = "-20 3 2" },
		{ Config = "@box.lua:Entity", Name = "Box_7", T = "-20 6 0" },
		{ Config = "@box.lua:Entity", Name = "Box_8", T = "-20 6 -2" },
		{ Config = "@box.lua:Entity", Name = "Box_9", T = "-20 6 2" },
	}
}
Rigidbody_Camera = {
	Shape = "sphere 0.3",
	Kinematic = true,
}

Text_CameraInfo = {
	Rect = "0 0 0 0 7 5 0 0.35 0 161",
	Color = "0 0 0 1",
	Depth = 15
}

Entity_FreeCamera = {
	Comp = {
		{"Camera", "1 0 1280 720 0.1 5000 1.308996939", "0.5 0.5 0.5 0.5 0 0 1 1 0 0"},
		{"Text", "CameraInfo", "@simscene.lua:Text_CameraInfo"},
		{"Script", "freeCamera"}
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

Text_Safety = {
	Rect = "0 0 0 0 7 60 0 0.35 0 161",
	Color = "0 0 1 1",
	Depth = 15
}

Entity_GlobalSystem = {
	Child = {
		{ Config = "@simscene.lua:Entity_FreeCamera", Name = "FreeCamera", Q = "0.5 0 0 0.866025403784", T = "0 20 -20" }
	},
	Comp = {
		{ "Light", "@simscene.lua:Light_Global", "1" },
		{ "Text", "FPS", "@simscene.lua:Text_FPS" },
		{ "Script", "fpsInfo" },
		{ "Text", "Safety", "@simscene.lua:Text_Safety" },
		{ "Script", "safetyInfo" },
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
		--{ Config = "@building.lua:Entity", Name = "Building", S = "2.5 2.5 2.5", Q = "0 1.2 0 -1.2", T = "-14 2.5 26"},
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00001", Q = "0 1 0 1", T = "-54 0 4" },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00002", Q = "0 1 0 1", T = "-19 0 7" },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00003", Q = "0 1 0 0.5", T = "34 0 11" },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00004", Q = "0 1 0 -1", T = "0 0 -23" },
		--{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00005", Q = "0 0 0 1", T = "0 0 0" },
		--{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00006", Q = "0 1 0 1", T = "-30 0 0" },
		--{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00007", Q = "0 1 0 -1", T = "30 0 0" },
		--{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00008", Q = "0 0 0 1", T = "5 0 -10" },
		--{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00009", Q = "0 1 0 0", T = "5 0 10" },
		--{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00010", Q = "0 0 0 1", T = "5 0 15" },
		--{ Config = "@box.lua:Entity", Name = "Box_1", T = "-20 0 0" },
		--{ Config = "@box.lua:Entity", Name = "Box_2", T = "-20 0 -2" },
		--{ Config = "@box.lua:Entity", Name = "Box_3", T = "-20 0 2" },
		--{ Config = "@box.lua:Entity", Name = "Box_4", T = "-20 3 0" },
		--{ Config = "@box.lua:Entity", Name = "Box_5", T = "-20 3 -2" },
		--{ Config = "@box.lua:Entity", Name = "Box_6", T = "-20 3 2" },
		--{ Config = "@box.lua:Entity", Name = "Box_7", T = "-20 6 0" },
		--{ Config = "@box.lua:Entity", Name = "Box_8", T = "-20 6 -2" },
		--{ Config = "@box.lua:Entity", Name = "Box_9", T = "-20 6 2" },
	}
}
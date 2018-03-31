Rigidbody_Camera = {
	Shape = "sphere 0.3",
	Kinematic = true,
}

Text_CameraInfo = {
	Rect = "0 0 0 0 7 5 0 0.35 0 161",
	Color = "0 0 0 1",
	Depth = 15
}

Light_Global = {
	Amb = "1 1 1 1",
	Diff = "0.7 0.7 0.7 1",
	Spec = "0.5 0.5 0.5 16",
	Dir = "0.5 -0.5 0.8",
	Pos = "0 0 0",
	Range = 80,
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

Entity_FreeCamera = {
	Comp = {
		{"Camera", "1 0 1280 720 0.1 5000 1.308996939", "0.5 0.5 0.5 0.5 0 0 1 1 0 0"},
		{"Text", "CameraInfo", "@simscene.lua:Text_CameraInfo"},
		{"Script", "freeCamera"}
	}
}

Entity_GlobalSystem = {
	Child = {
		{ Config = "@simscene.lua:Entity_FreeCamera", Name = "FreeCamera", Q = "0.58 0 0 0.81", T = "0 54 -32" }
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
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00001", Q = "0 1 0 1", T = "-54 0 4" },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00002", Q = "0 1 0 1", T = "-19 0 7" },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00003", Q = "0 1 0 0.5", T = "34 0 11" },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00004", Q = "0 1 0 -1", T = "0 0 -23" },
	}
}
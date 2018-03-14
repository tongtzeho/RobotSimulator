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

function PositionInCircle(radius, slice)
	result = {}
	theta = 0
	dTheta = 2*math.pi/slice
	for i = 0, slice, 1 do
		x = radius*math.cos(theta)
		y = 0
		z = radius*math.sin(theta)
		table.insert(result, tostring(x).." "..tostring(y).." "..tostring(z))
		theta = theta + dTheta
	end
	return result
end

circle = PositionInCircle(32, 14)

function RotationInCircle(slice)
	math.randomseed(tostring(os.time()):reverse():sub(1, 6))
	rotation = {}
	for i = 0, slice, 1 do
		table.insert(rotation, "0 "..tostring(math.random()*2).." 0 1")
	end
	return rotation
end

rotation = RotationInCircle(14)

Scene = {
	Splash = "@splash.png",
	Root = {
		{ Config = "@simscene.lua:Entity_GlobalSystem", Name = "GlobalSystem" },
		{ Config = "@floor.lua:Entity", Name = "Floor" },
		{ Config = "@platform.lua:Entity", Name = "Platform" },
		{ Config = "@screen.lua:Entity", Name = "Screen" },
		{ Config = "@circle_wall.lua:Entity", Name = "CircleWall" },
		--{ Config = "@building.lua:Entity", Name = "Building", S = "2.5 2.5 2.5", Q = "0 1.2 0 -1.2", T = "-14 2.5 26"},
		--{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00001", Q = "0 1 0 1", T = "-54 0 4" },
		--{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00002", Q = "0 1 0 1", T = "-19 0 7" },
		--{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00003", Q = "0 1 0 0.5", T = "34 0 11" },
		--{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00004", Q = "0 1 0 -1", T = "0 0 -23" },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00011", T = circle[1], Q = rotation[1] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00012", T = circle[2], Q = rotation[2] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00013", T = circle[3], Q = rotation[3] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00014", T = circle[4], Q = rotation[4] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00015", T = circle[5], Q = rotation[5] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00016", T = circle[6], Q = rotation[6] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00017", T = circle[7], Q = rotation[7] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00018", T = circle[8], Q = rotation[8] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00019", T = circle[9], Q = rotation[9] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00020", T = circle[10], Q = rotation[10] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00021", T = circle[11], Q = rotation[11] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00022", T = circle[12], Q = rotation[12] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00023", T = circle[13], Q = rotation[13] },
		{ Config = "@epuck2.lua:Entity", Name = "e-puck2_00024", T = circle[14], Q = rotation[14] },
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
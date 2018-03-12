Material_Body = {
	0.3, 0.3, 0.3, 1.0,
	0.7, 0.7, 0.7, 1.0,
	0.4, 0.4, 0.4, 8.0,
	0.0, 0.0, 0.0, 0.0
}

Material_Wheel = {
	0.3, 0.3, 0.3, 1.0,
	0.7, 0.7, 0.7, 1.0,
	0.4, 0.4, 0.4, 8.0,
	0.0, 0.0, 0.0, 0.0
}

Entity_Wheel = {
	Comp = {
		{"Mesh", "@epuck2wheel.lua:Mesh_Low"},
		{"Mesh", "@epuck2wheel.lua:Mesh_High"},
		{"Material", "@epuck2.lua:Material_Wheel"},
		{"Texture", "DiffuseMap", "@epuck2.png"},
		{"Renderer", "LODRenderer", "1 1 30"},
	}
}
--[[
Collider_Proximity = {
	Shape = "cylinder 8 0.8",
	T = "0 3.22 0",
	Trigger = true,
	Group = 0x40000000,
	Mask = 0x3FFFFFFF,
}
]]
Collider_OnGround = {
	Shape = "box 0.4 0.05 0.4",
	Trigger = true,
	Group = 0x40000000,
	Mask = 0x3FFFFFFF
}

Rigidbody = {
	Shape = "cylinder 3.625 2.12",
	T = "0 2.12 0",
	Mass = 3,
}

EpuckCameraIdentifier = "1 -1 52 39 0.05 500 1.0471975512"

Entity_Camera = {
	Comp = {
		{"Camera", EpuckCameraIdentifier, "0 1 0 1 0 0 0 0.2 0 0"},
		{"RGBSensor", EpuckCameraIdentifier, "8 0"},
		{"DepthSensor", EpuckCameraIdentifier, "8 0 100"}
	}
}

Mesh_Line = {
	Type = "ffffffff",
	V = {
		{-0.01, 0, 0, 0, 1, 0, 0.125, 0.625 },
		{0, 0, 5, 0, 1, 0, 0.125, 0.625 },
		{0.01, 0, 0, 0, 1, 0, 0.125, 0.625 },
	},
	I = {
		0, 1, 2, 0, 2, 1
	}
}

Entity_Ray = {
	Comp = {
		{"Mesh", "@epuck2.lua:Mesh_Line"},
		{"Material", "@epuck2.lua:Material_Body"},
		{"Texture", "DiffuseMap", "@epuck2.png"},
		{"Renderer", "MeshRenderer", "0 0"}
	}
}

Entity_ToFDist = {
	Child = {
		--{ Config = "@epuck2.lua:Entity_Ray" }
	},
	Comp = {
		{"DistanceSensor", "1000"}
	}
}

Entity_IRProx = {
	Child = {
		--{ Config = "@epuck2.lua:Entity_Ray" }
	},
	Comp = {
		{"ProximitySensor", "5"}
	}
}

Entity = {
	Child = {
		{ Config = "@epuck2.lua:Entity_Wheel", Name = "Epuck2Wheel", T = "0 2 0" },
		{ Config = "@epuck2.lua:Entity_Camera", Name = "Epuck2Camera", T = "0 2.78 3.475"},
		{ Config = "@epuck2.lua:Entity_ToFDist", Name = "Epuck2ToF", T = "0 4 3.63"}, -- Front
		{ Config = "@epuck2.lua:Entity_IRProx", Name = "Epuck2IR0", T = "1 3.3 3", Q = "0 0.149832 0 0.988712"}, -- Front Right
		{ Config = "@epuck2.lua:Entity_IRProx", Name = "Epuck2IR1", T = "2.5 3.3 2.2", Q = "0 0.389785 0 0.920906"}, -- Right Front
		{ Config = "@epuck2.lua:Entity_IRProx", Name = "Epuck2IR2", T = "3.1 3.3 0", Q = "0 0.707107 0 0.707107"}, -- Right
		{ Config = "@epuck2.lua:Entity_IRProx", Name = "Epuck2IR3", T = "1.5 3.3 -3", Q = "0 -0.969207 0 -0.246246"}, -- Right Back
		{ Config = "@epuck2.lua:Entity_IRProx", Name = "Epuck2IR4", T = "-1.5 3.3 -3", Q = "0 -0.968616 0 0.248561"}, -- Left Back
		{ Config = "@epuck2.lua:Entity_IRProx", Name = "Epuck2IR5", T = "-3.1 3.3 0", Q = "0 -0.707107 0 0.707107"}, -- Left
		{ Config = "@epuck2.lua:Entity_IRProx", Name = "Epuck2IR6", T = "-2.5 3.3 2.2", Q = "0 -0.389052 0 0.921216"}, -- Left Front
		{ Config = "@epuck2.lua:Entity_IRProx", Name = "Epuck2IR7", T = "-1 3.3 3", Q = "0 -0.149044 0 0.988831"}, -- Front Left
	},
	Comp = {
		{"Mesh", "@epuck2body.lua:Mesh"},
		{"Material", "@epuck2.lua:Material_Body"},
		{"Texture", "DiffuseMap", "@epuck2.png"},
		{"Renderer", "EpuckRenderer", "1 1 1050"},
		{"Collider", "@epuck2.lua:Collider_OnGround"},
		{"Rigidbody", "@epuck2.lua:Rigidbody"},
		{"Renderer", "ColliderRenderer", "1 0 0 1"},
		{"ActionController", "EpuckActionController", "3 1.6"}
	}
}
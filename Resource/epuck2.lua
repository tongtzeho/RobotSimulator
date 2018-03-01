Material_Body = {
	0.4, 0.4, 0.4, 1.0,
	0.8, 0.8, 0.8, 1.0,
	0.4, 0.4, 0.4, 8.0,
	0.0, 0.0, 0.0, 0.0
}

Material_Wheel = {
	0.4, 0.4, 0.4, 1.0,
	0.8, 0.8, 0.8, 1.0,
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
		{"Script", "epuck_wheel"}
	}
}

Collider_Proximity = {
	Shape = "cylinder 8 0.8",
	T = "0 3.22 0",
	Trigger = true,
	Group = 0x40000000,
	Mask = 0x3FFFFFFF,
}

Rigidbody = {
	Shape = "cylinder 3.625 2.12",
	T = "0 2.12 0",
	Mass = 3
}

EpuckCameraIdentifier = "1 -1 160 120 0.05 500 1.0471975512"

Entity_Camera = {
	Comp = {
		{"Camera", EpuckCameraIdentifier, "0 1 0 1 0 0 0 0.33333333 0 0"},
		{"RGBSensor", EpuckCameraIdentifier, "24 0"},
		{"DepthSensor", EpuckCameraIdentifier, "24 0 100"}
	}
}

Entity = {
	Child = {
		{ Config = "@epuck2.lua:Entity_Wheel", Name = "Epuck2Wheel", T = "0 2 0" },
		{ Config = "@epuck2.lua:Entity_Camera", Name = "Epuck2Camera", T = "0 2.78 3.475"}
	},
	Comp = {
		{"Mesh", "@epuck2body.lua:Mesh"},
		{"Material", "@epuck2.lua:Material_Body"},
		{"Texture", "DiffuseMap", "@epuck2.png"},
		{"Renderer", "EpuckRenderer", "1 1 1050"},
		{"Collider", "@epuck2.lua:Collider_Proximity"},
		{"Rigidbody", "@epuck2.lua:Rigidbody"},
		{"Renderer", "ColliderRenderer", "1 0 0 1"},
	}
}
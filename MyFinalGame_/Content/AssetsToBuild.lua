--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },

		{ path = "Shaders/Vertex/vertexInputLayout_mesh.shader", arguments = { "vertex" } },
		
		{ path = "Shaders/Fragment/animatedcolor.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/yellowcolor.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/green.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/damaged.shader", arguments = { "fragment" } },
	},
	meshes = 
	{
		{ path = "Meshes/obj_1_mesh_1.mesh" },
		{ path = "Meshes/obj_1_mesh_2.mesh" },
		{ path = "Meshes/obj_2_mesh_1.mesh" },
		{ path = "Meshes/obj_3_mesh_1.mesh" },
		{ path = "Meshes/MayaExportObject.mesh" },
		{ path = "Meshes/plane_1.mesh" },
		{ path = "Meshes/plane_2.mesh" },
		{ path = "Meshes/planePowerBar_1.mesh" },
		{ path = "Meshes/planePowerBar_2.mesh" },
		{ path = "Meshes/planePowerBar_3.mesh" },

	}
}
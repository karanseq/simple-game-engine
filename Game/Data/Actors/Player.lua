Actor = 
{
	name = "Player",
	type = "Player",

	bounding_box = { 0.0, 0.0, 44.0, 44.0 },
	transform = 
	{
		position = { 0.0, -450.0, 0.0 },
		rotation = { 0.0, 0.0, 0.0 },
		scale = { 1.0, 1.0, 1.0 }
	},

	physics_settings = 
	{
		type = "dynamic",
		mass = 50.0,
		drag = 0.1,
		collide = true,
		collision_filter = 0
	},

	render_settings = 
	{
		sprite_name = "Data\\Sprites\\Ship.dds"
	}
}
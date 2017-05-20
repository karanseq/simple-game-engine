Actor = 
{
    name = "PlayerBullet",
    type = "Bullet",

    bounding_box = { 0.0, 0.0, 15.0, 15.0 },
    transform = 
    {
        position = { 0.0, 0.0, 0.0 },
    },

    physics_settings = 
    {
        type = "dynamic",
        mass = 10.0,
        drag = 0.0,
        collide = true,
        collision_filter = 1
    },

    render_settings = 
    {
        sprite_name = "Data\\Sprites\\Bullet.dds"
    }
}


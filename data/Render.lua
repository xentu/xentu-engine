-- the drawing event.
game.on("draw", function()
    renderer.begin()
    -- renderer.set_blend_preset(BLEND_SOURCE_IN) // use a preset for blending
    -- renderer.set_blend_func(SRC_ALPHA, ONE_MINUS_SRC_ALPHA) // alternatively set blending like you do in OpenGL
	-- renderer.set_origin(50, 50)
    -- renderer.set_rotation(sprite.rotation)
    -- renderer.set_scale(sprite.scale.x, sprite.scale.y)
    renderer.draw_sprite(sprite)

    renderer.draw_text(font, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed imperdiet tortor eget pulvinar eleifend. Maecenas dapibus mauris " ..
	                         "sed felis laoreet pellentesque. Nam eget diam a lacus semper placerat luctus mattis arcu. Cras ac iaculis quam. Maecenas arcu " ..
							 "ligula, congue eget ligula nec, pellentesque bibendum lacus. Cras iaculis lectus ipsum, euismod posuere sem rutrum sed.",
     						 100, 200, viewport.width - 200)
    renderer.present()
end)
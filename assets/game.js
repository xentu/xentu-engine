import './xentu.js'


class MyGame extends XentuGame {
	constructor(width = 800, height = 600) {
		super()
		vfs.mount('/zip', './assets/test.zip')
		this.set_screen(width, height, false)
		this.set_clear_color('#5f78988')
	}

	update(self, dt) {
		print('update called')
	}

	draw(self, dt) {
		print('update called')
	}
}

game.run(new MyGame)
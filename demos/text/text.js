// load in our fonts.
const roboto16 = assets.load_font("/assets/Roboto-Regular.ttf", 16);
const roboto24 = assets.load_font("/assets/Roboto-Regular.ttf", 24);
const roboto20 = assets.load_font("/assets/Roboto-Regular.ttf", 20);

// setup default colours.
renderer.set_background('#444444');
renderer.set_foreground("#CCCCCC");

// create 1st textbox and measure the text on line0.
const line1 = "This is the Roboto font in size 16 (light gray)";
const textbox1 = assets.create_textbox(680, 40);
textbox.set_text(textbox1, roboto16, line1);
const size1 = textbox.measure_text(textbox1, roboto16, line1);

// create 2nd textbox.
const line2 = "This is the Roboto font in size 24 (magenta)";
const textbox2 = assets.create_textbox(680, 40);
textbox.set_text(textbox2, roboto24, line2);
textbox.set_color(textbox2, roboto24, "#FF00FF"); // give it a different colour.

// create 3rd textbox.
const textbox3 = assets.create_textbox(680, 40);
textbox.set_text(textbox3, roboto20, "The first line measures "+ size1.w + " x " + size1.h + " px");
textbox.set_color(textbox3, roboto20, "#00FFFF");

// create 4th textbox.
const textbox4 = assets.create_textbox(680, 40);
var stepper = assets.create_stepper(1, 0);
textbox.set_text(textbox4, roboto20, "This is a counter (1s delay): 0");

// handle the update event.
game.on("update", function(dt) {
	if (keyboard.key_clicked(KB_ESCAPE)) exit();
	if (stepper.next(dt)) {
		stepper.value++;
		textbox.set_text(textbox4, roboto20, "This is a counter (1s delay): " + stepper.value);	
	}
});

// handle the draw event
game.on("draw", function(dt) {
	renderer.clear();
	renderer.begin();
	renderer.draw_textbox(textbox1, 10, 10);
	renderer.draw_textbox(textbox2, 10, 40);
	renderer.draw_textbox(textbox3, 10, 80);
	renderer.draw_textbox(textbox4, 10, 120);
	renderer.present();
});
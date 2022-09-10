# 

class Rect:
	pass
class Polygon:
	pass

def create_window(title:str, x:int, y:int, width:int, height:int, flags:any) -> int:
	return 0

def create_rect(x:int, y:int, width:int, height:int) -> Rect:
	pass

def create_color(hex:str) -> int:
	pass

def create_polygon(coords) -> Polygon:
	pass

def path() -> str:
	pass

def fullscreen() -> bool:
	pass

def set_fullscreen(fullscreen:bool):
	pass

def on(event:str, callback_name:str):
	pass

def trigger(event:str):
	pass

def run():
	pass

def exit():
	pass
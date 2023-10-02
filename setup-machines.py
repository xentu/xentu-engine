import os


def join_files(cppVariableName, isPython, outputFile, inputFiles):
	# remove old output
	if os.path.isfile(outputFile): os.remove(outputFile)
	# open output file in append mode, and write the header.
	outFile = open(outputFile, 'a')
	outFile.write('#pragma once\n')
	if isPython == True: outFile.write('#if XEN_PY\n')
	outFile.write('namespace xen { \n')
	outFile.write('const int '+cppVariableName+'_c = '+str(len(inputFiles))+';\n')
	outFile.write('const char* '+cppVariableName+'['+cppVariableName+'_c] = {\n')

	for inFile in inputFiles:
		data = open(inFile, 'r').read()
		outFile.write('R"(' + data + ')"')
		if not inFile == inputFiles[-1]: outFile.write(',')
		outFile.write('\n')

	outFile.write('}; }')
	if isPython == True: outFile.write('\n#endif')
	
	outFile.close()
	print(outputFile + ' updated!\n', flush=True)
			


join_files('xen_js_script_init', False, './src/machines/javascript/JavaScriptMachineScripts.h', [
	'./src/machines/javascript/scripts/main.js',
	'./src/machines/javascript/scripts/lib-scenes.js',
	'./src/machines/javascript/scripts/lib-story.js'
])


join_files('xen_lua_script_init', False, './src/machines/lua/LuaMachineScripts.h', [
	'./src/machines/lua/scripts/main.lua',
	'./src/machines/lua/scripts/lib-scenes.lua',
	'./src/machines/lua/scripts/lib-json.lua',
	'./src/machines/lua/scripts/lib-story.lua'
])


join_files('xen_py_script_init', True, './src/machines/python/PythonMachineScripts.h', [
	'./src/machines/python/scripts/main.py',
	'./src/machines/python/scripts/lib-scenes.py',
	'./src/machines/python/scripts/lib-story.py'
])
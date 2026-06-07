const loadAsset = async (module, url, targetPath) => {
	// make sure target dir exists.
	const targetDir = targetPath.split("/").slice(0,-1).join("/");

	// base directory first..
	if (!module.FS.analyzePath('/resources').exists) {
		module.FS.mkdir('/resources');
		console.log(`JS: PATH /`);
	}

	// and sub-directory if specified.
	if (targetDir.length > 0) {
		if (!module.FS.analyzePath(`/resources/${targetDir}`).exists) {
			const res = module.FS.mkdirTree(`resources/${targetDir}`);
			console.log(`JS: PATH /${targetDir}`, res);
		}
	}

	// fetch the data.
	const res = await fetch(url);
	const buffer = await res.arrayBuffer();

	// convert the data into a Uint Array
	const realTargetPath = `resources/${targetPath}`;
	const uint8Array = new Uint8Array(buffer);
	const len = uint8Array.length;
	console.log(`JS: FILE /${targetPath} (${len} bytes)`);
	module.FS.writeFile(realTargetPath, uint8Array);
	console.log(`JS: FILE /${targetPath}`);
}


window.loadAssets = async function(module, subFolder, manifestUrl) {
	try {
		const response = await fetch(`${subFolder}/${manifestUrl}`);
		const manifest = await response.json();
		//const promises = [];

		for (const entry of manifest) {
			switch (entry.type) {
				case 'local':
					const localUrl = `./${subFolder}/${entry.source}`;
					if (typeof entry.alias !== 'undefined' && entry.alias.length > 0) {
						await loadAsset(module, localUrl, entry.alias);
					}
					else {
						await loadAsset(module, localUrl, entry.source);
					}
					break;
				case 'remote':
					await loadAsset(module, entry.source, entry.alias);
					break;
				default:
					throw new Error(`Invalid asset type: ${entry.type}`);
					break;
			}
		}

		module._notify_assets_ready();
	} catch (err) {
		console.log(`JS: ${err}`);
		console.error("Failed to inject assets:", err);
	}
}
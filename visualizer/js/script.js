const pane = new Tweakpane.Pane();

// Simulation playback state 
const sim_state = {
	is_playing: true,
	len: 250, // Total steps
	step: 0, // Current pos
	speed: 1, // Transition speed between cells
};
// Farm parameters
const parameters = {
	AntFarm: '',
	Simulation: '',
};


/**********************************\
[                                  ]
[ -- AntFarm configuration form -- ]
[                                  ]
\**********************************/
// Folder for AntFarm settings
const antFarmFolder = pane.addFolder({
	title: 'AntFarm Settings',
	expanded: true,
});
// Text field for AntFarm configuration
antFarmFolder.addInput(parameters, 'AntFarm', { label: 'AntFarm' }).on('change', (value) => {
	console.log('AntFarm:', value);
});
// AntFarm configuration validation button
antFarmFolder.addButton({ title: 'Valider AntFarm' }).on('click', () => {
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// [ -- implement parsing for parameters.AntFarm                     -- ] //
	// [ -- parameters.AntFarm = parseAntFarm(parameters.AntFarm);       -- ] //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	parameters.AntFarm = true; // To be removed after implementing the parsing
	updateInterface();
	console.log('AntFarm validé');
});


/*************************************\
[                                     ]
[ -- Simulation configuration form -- ]
[                                     ]
\*************************************/
// Folder for simulation settings
const simulationFolder = pane.addFolder({
	title: 'Simulation Settings',
	expanded: true,
});
// Text field for simulation insructions, deactivated while there is no valid AntFarm
const simulationInput = simulationFolder.addInput(parameters, 'Simulation', {
	label: 'Simulation',
	disabled: !parameters.AntFarm,
});
// Simulation instructions validation button
simulationFolder.addButton({ title: 'Valider Simulation' }).on('click', () => {
	if (parameters.AntFarm) {
		const simulationString = simulationInput.controller_.view.element.querySelector('input').value;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// [ -- implement parsing for simulationString                     -- ] //
		// [ -- parameters.Simulation = parseSimulation(simulationString); -- ] //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		parameters.Simulation = true; // To be removed after implementing the parsing
		updateInterface();
	}
});


// Interface and states updater
function updateInterface() {
	simulationInput.disabled = !parameters.IsFarm;

	if (parameters.Simulation) {
		// Playback controls container (sliders and buttons)
		playbackContainer = document.createElement('div');
		playbackContainer.id = 'playback-container';
		document.body.appendChild(playbackContainer);
		// Playback progress and ants speed sliders
		const playbackPane = new Tweakpane.Pane({
			container: playbackContainer,
		});
		// Sync playback progress bar with current step
		const stepInput = playbackPane.addInput(sim_state, 'step', {
			min: 0,
			max: sim_state.len,
			step: 1,
			label: 'Position',
			disabled: sim_state.is_playing
		});
		// Ants speed slider (time to move from one cell to another)
		playbackPane.addInput(sim_state, 'speed', {
			min: 0.1,
			max: 5,
			step: 0.1,
			label: 'Speed',
		});

		// playback buttons container
		const buttonContainer = document.createElement('div');
		buttonContainer.id = 'playback-buttons';
		playbackContainer.appendChild(buttonContainer);
		// Playback buttons (Backward, Play/Pause, Forward)
		['<i class="fa fa-step-backward"></i>',
		'<i class="fa fa-play"></i><i class="fa fa-pause"></i>',
		'<i class="fa fa-step-forward"></i>'].forEach((action, index) => {
			const button = document.createElement('div');
			button.innerHTML = action;
			button.className = 'playback-button';
			button.addEventListener('click', () => handlePlaybackAction(index, stepInput));
			buttonContainer.appendChild(button);
		});

		// Simulation loop
		setInterval(() => {
			if (sim_state.is_playing) {
				if (sim_state.step < sim_state.len) {
					sim_state.step++;
					stepInput.refresh(); // Update the slider
					console.log('Playing, step:', sim_state.step);
					// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
					// [ -- add here simulation updating function -- ] //
					// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				} else
					sim_state.is_playing = false; // Stop sim if reached the end
			}
		}, 250);
	}
}


// Playback control buttons handling
function handlePlaybackAction(actionIndex, stepInput) {
	switch (actionIndex) {
		case 0: // Step Back
			if (sim_state.step > 0 && !sim_state.is_playing && sim_state.step--)
				stepInput.refresh();
			break;

		case 1: // Play/Pause
			sim_state.is_playing = !sim_state.is_playing;
			console.log(sim_state.is_playing ? 'Playing' : 'Paused');
				stepInput.disabled = sim_state.is_playing;
			break;
	
		case 2: // Step Forward
			if (sim_state.step < sim_state.len && !sim_state.is_playing && ++sim_state.step)
				stepInput.refresh();
			break;
	}
}

// Init interface
updateInterface();
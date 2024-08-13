const pane = new Tweakpane.Pane();

/**********************************\
[                                  ]
[ -- AntFarm configuration form -- ]
[                                  ]
\**********************************/
class AntFarmForm {
	constructor(params) {
		// Folder for AntFarm settings
		this.Folder = pane.addFolder({
			title: 'AntFarm Settings',
			expanded: true,
		});

		// Text field for AntFarm configuration
		this.InputField = this.Folder.addInput(params, 'AntFarm', {
			label: 'AntFarm'
		}).on('change', (value) => {
			console.log('AntFarm:', value);
		});

		// AntFarm configuration validation button
		this.Folder.addButton({ title: 'Valider AntFarm' }).on('click', () => {
			this.InputStr = this.InputField.controller_.view.element.querySelector('input').value;
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			// [ -- implement parsing for params.AntFarm                     -- ] //
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			this.parse(params);
			params.AntFarm = true; // To be removed after implementing the parsing
			if (params.AntFarm) {
				console.log('AntFarm: OK');
				updateInterface();
			} else
				console.log('AntFarm: KO');
		});
	}

	parse(params) {
		console.log('Parsing AntFarm:', this.InputStr);
	}
}


/*************************************\
[                                     ]
[ -- Simulation configuration form -- ]
[                                     ]
\*************************************/
class SimulationForm {
	constructor(params) {
		// Folder for simulation settings
		this.Folder = pane.addFolder({
			title: 'Simulation Settings',
			expanded: true,
		});

		// Text field for simulation insructions, deactivated while there is no valid AntFarm
		this.InputField = this.Folder.addInput(params, 'Simulation', {
			label: 'Simulation',
			disabled: !params.AntFarm,
		});

		// Simulation instructions validation button
		this.Folder.addButton({ title: 'Valider Simulation' }).on('click', () => {
			if (params.AntFarm) {
				this.InputStr = this.InputField.controller_.view.element.querySelector('input').value;
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				// [ -- implement parsing for simulationString              -- ] //
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				this.parse(params);
				params.Simulation = true; // To be removed after implementing the parsing
				if (params.Simulation) {
					console.log('Simulation: OK');
					updateInterface();
				} else
					console.log('Simulation: KO');
			}
		});
	}

	parse(params) {
		console.log('Parsing Simulation:', this.InputStr);
	}
}


/*************************************\
[                                     ]
[ -- Simulation playback controller-- ]
[                                     ]
\*************************************/
class PlaybackController {
	constructor(params, sim_state) {
		// Playback controls container (sliders and buttons)
		this.Container = document.createElement('div');
		this.Container.id = 'playback-container';
		document.body.appendChild(this.Container);

		// Playback progress and ants speed sliders
		this.Pane = new Tweakpane.Pane({
			container: this.Container,
		});

		// Sync playback progress bar with current step
		this.StepInput = this.Pane.addInput(sim_state, 'step', {
			min: 0,
			max: sim_state.len,
			step: 1,
			label: 'Position',
			disabled: sim_state.is_playing
		});

		// Ants speed slider (time to move from one cell to another)
		this.Pane.addInput(sim_state, 'speed', {
			min: 0.1,
			max: 5,
			step: 0.1,
			label: 'Speed',
		});

		// playback buttons container
		this.ButtonContainer = document.createElement('div');
		this.ButtonContainer.id = 'playback-buttons';
		this.Container.appendChild(this.ButtonContainer);
		// Playback buttons (Backward, Play/Pause, Forward)
		['<i class="fa fa-step-backward"></i>',
		'<i class="fa fa-play"></i><i class="fa fa-pause"></i>',
		'<i class="fa fa-step-forward"></i>'].forEach((action, index) => {
			this.Button = document.createElement('div');
			this.Button.innerHTML = action;
			this.Button.className = 'playback-button';
			this.Button.addEventListener('click', () => this.playbackAction(index, this.StepInput, sim_state));
			this.ButtonContainer.appendChild(this.Button);
		});
	}

	//handle playback buttons (step backward, play/pause, step forward)
	playbackAction(index, stepInput, sim_state) {
		switch(index){
			case 0: // Step backward
				if (sim_state.step > 0) {
					sim_state.step--;
					stepInput.refresh(); // Update the slider
					console.log('Step Backward, step:', sim_state.step);
					// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
					// [ -- add here simulation updating function -- ] //
					// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				}
				break;
			case 1: // Play/Pause
				sim_state.is_playing = !sim_state.is_playing;
				console.log('Play/Pause, is_playing:', sim_state.is_playing);
				break;
			case 2: // Step forward
				if (sim_state.step < sim_state.len) {
					sim_state.step++;
					stepInput.refresh(); // Update the slider
					console.log('Step Forward, step:', sim_state.step);
					// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
					// [ -- add here simulation updating function -- ] //
					// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				}
				break;
		}
	}

	// upda
}
const pane = new Tweakpane.Pane();


export class AntFarmForm {
	constructor(params, onValidate) {
		this.params = params;
		this.onValidate = onValidate;

		this.folder = pane.addFolder({
			title: 'AntFarm Settings',
			expanded: true,
		});
		this.inputField = this.folder.addInput(this.params, 'AntFarm', {
			label: 'AntFarm',
		})
		this.folder.addButton({ title: 'Valider AntFarm' }).on('click', () => {
			this.validate();
		});
	}

	validate() {
		this.params.AntFarm = true; // Remplacer par une logique de validation réelle
		console.log('AntFarm validé:', this.params.AntFarm);
		if (this.onValidate)
			this.onValidate(this.params.AntFarm);
	}
}


export class SimulationForm {
	constructor(params, onValidate) {
		this.params = params;
		this.onValidate = onValidate;

		this.folder = pane.addFolder({
			title: 'Simulation Settings',
			expanded: true,
		});
		this.inputField = this.folder.addInput(this.params, 'Simulation', {
			label: 'Simulation',
			disabled: !this.params.AntFarm,
		});
		this.folder.addButton({ title: 'Valider Simulation' }).on('click', () => {
			this.validate();
		});
	}

	validate() {
		if (this.params.AntFarm) {
			this.params.Simulation = true; // Remplacer par une logique de validation réelle
			console.log('Simulation validée:', this.params.Simulation);
			if (this.onValidate)
				this.onValidate(this.params.Simulation);
		}
	}
}


export class PlaybackController {
	constructor(simState, simulation) {
		this.simState = simState;
		this.sim = simulation;

		this.container = document.createElement('div');
		this.container.id = 'playback-container';
		document.body.appendChild(this.container);

		this.pane = new Tweakpane.Pane({container: this.container});

		this.stepInput = this.pane.addInput(this.simState, 'step', {
			min: 0,
			max: this.simState.len,
			step: 1,
			label: 'Position',
			disabled: this.simState.is_playing,
		});
		this.speedInput = this.pane.addInput(this.simState, 'speed', {
			min: 0.1,
			max: 5,
			step: 0.1,
			label: 'Speed',
		});
		this.createPlaybackButtons();
	}

	createPlaybackButtons() {
		this.buttonContainer = document.createElement('div');
		this.buttonContainer.id = 'playback-buttons';
		this.container.appendChild(this.buttonContainer);

		['<i class="fa fa-step-backward"></i>',
		'<i class="fa fa-play"></i><i class="fa fa-pause"></i>',
		'<i class="fa fa-step-forward"></i>',
		'<i class="material-icons">replay</i>'].forEach((action, index) => {
			const button = document.createElement('div');
			button.innerHTML = action;
			button.className = 'playback-button';
			button.addEventListener('click', () => this.playbackAction(index));
			this.buttonContainer.appendChild(button);
		});
	}

	playbackAction(index) {
		switch (index) {
			case 0:
				this.sim.stepBack(); break;
			case 1:
				this.sim.playPause(); break;
			case 2:
				this.sim.stepForward(); break;
			case 3:
				this.sim.restart(); break;
		}
	}
}
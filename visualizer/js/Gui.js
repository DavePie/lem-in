	
	const pane = new Tweakpane.Pane();
import {Room, Link, Rooms, Links, Ants } from './AntFarm.js';
import { AntFarmVisualizer } from './visualization.js';

// GUI  component to input and validate the ant farm
// located on top right
// is in charge to get the ant farm, parse it and validate it
export class AntFarmForm {
	constructor(params, onValidate) {
		this.params = params;
		this.onValidate = onValidate;
		this.folder = pane.addFolder({title: 'AntFarm Settings', expanded: true});
		this.textArea = document.createElement('textarea');
		this.textArea.rows = 10;
		this.textArea.cols = 50;
		this.textArea.placeholder = 'Enter AntFarm configuration here...';
		this.textArea.style.width = '95%';
		this.folder.element.appendChild(this.textArea);
		this.folder.addButton({ title: 'Validate ant farm' }).on('click', () => {this.validate();});
	}

	validate() {
		if (this.params.vis)
			this.params.vis.clearScreen();
		this.resetParams();

		try{
			[this.params.rooms, this.params.links, this.params.ants] = this.parseInput(this.getInputValue());
		} catch (error) {
			this.params.farmValid = false;
			console.error(error);
			this.params.popup.show(error.message);
		}

		if (this.params.rooms && this.params.links && this.params.ants)
			this.params.farmValid = true;
		console.log('AntFarm validated:', this.params.farmValid);
		if (this.onValidate)
			this.onValidate(this.params.farmValid);
		if (this.params.farmValid)
			this.params.vis = new AntFarmVisualizer(this.params);
	}

	resetParams() {
		this.params.farmValid = false;
		this.params.simValid = false;;

		if (this.params.gui.playbackController) {
			console.log('Removing playback controller');
			this.params.gui.playbackController.container.remove();
			this.params.gui.playbackController = null;
		}

		this.params.rooms = null;
		this.params.links = null;
		this.params.ants = null;
		this.params.sim = null;
		this.params.vis = null;
	}

	getInputValue() {
		return this.textArea.value;
	}

	parseInput(input) {
		const lines = input.split('\n').map(line => line.trim()).filter(line => line.length > 0);
		const rooms = new Rooms();
		const links = new Links();
		let numAnts;
		let preproc = false;
		let firstInsruction = true;

		if (lines.length === 0)
			throw new Error('No input provided.');
	
		let numLines = 0;
		for (let line of lines) {
			numLines++;
			// If starts with # but not ##, skip the line
			if (line.startsWith('#') && !line.startsWith('##'))
				continue;
			// If starts with ##, it's a preprocessor command, if not "start" or "end" after, throw error
			else if (line.startsWith('##')) {
				if (line === '##start' || line === '##end')
					preproc = line.slice(2);
				else
					throw new Error('Invalid preprocessor command.\n Line ' + numLines + ': ' + line);
			}
			// If line shaped like "[int]" and is the first instruction, it's the number of ants
			else if (line.match(/^\d+$/) && firstInsruction) {
				if (preproc !== false)
					throw new Error('Preprocessor command before number of ants.\n Line ' + numLines + ': ' + line);
				numAnts = parseInt(line);
				firstInsruction = false;
			}
			else if (firstInsruction)
				throw new Error('First instruction must be number of ants.');
			// If line shaped like "[string] [int] [int]", it's a room
			else if (line.match(/^\w+ \d+ \d+$/)) {
				const [name, x, y] = line.split(' ');
				rooms.addRoom(name, parseInt(x), parseInt(y), preproc);
				preproc = false;
			}
			// If line shaped like "[string]-[string]", it's a link
			else if (line.match(/^\w+-\w+$/)) {
				if (preproc !== false)
					throw new Error('Preprocessor command before link.\n Line ' + numLines + ': ' + line);
				const [room1, room2] = line.split('-');
				if (!rooms.exists(room1) || !rooms.exists(room2))
					throw new Error('Link between non-existing rooms.\n Line ' + numLines + ': ' + line);
				links.addLink(rooms.rooms[room1], rooms.rooms[room2]);
			}
			else
				throw new Error('Invalid instruction.\n Line ' + numLines + ': ' + line);
		}
		if (firstInsruction)
			throw new Error('Empty input.');
		else if (!rooms.start || !rooms.end)
			throw new Error('No start or end room.');
		const ants = new Ants(numAnts, rooms.start);

		return [rooms, links, ants];
	}
}


// GUI component to input and validate the simulation
// located on top right just below the AntFarmForm
// is in charge to get the simulation, parse it and validate it
export class SimulationForm {
	constructor(params, onValidate) {
		this.params = params;
		this.onValidate = onValidate;
		this.folder = pane.addFolder({title: 'Simulation Settings', expanded: true});
		this.textArea = document.createElement('textarea');
		this.textArea.rows = 10;
		this.textArea.cols = 50;
		this.textArea.placeholder = 'Enter Simulation configuration here...';
		this.textArea.style.width = '95%';
		this.folder.element.appendChild(this.textArea);
		this.button = this.folder.addButton({ title: 'Validate Simulation' }).on('click', () => {this.validate();});
	}

	validate() {
		if (this.params.farmValid) {
			try {
				this.parseinput(this.getInputValue());
				this.params.simValid = true;
			} catch (error) {
				this.params.simValid = false;
				console.error(error);
			}
			console.log('Simulation validated:', this.params.simValid);
			if (this.onValidate)
				this.onValidate(this.params.simValid);
		}
	}

	getInputValue() {
		return this.textArea.value;
	}

	parseinput(input) {	
		let steps = input.trim().split('\n');

		for (let i = 0; i < steps.length; i++) {
			let step = steps[i].trim();
			if (step.length === 0)
				throw new Error('Empty step.');
			this.params.ants.addStep(step, i + 1, this.params.rooms, this.params.links);
		}
		if (steps.length === 0)
			throw new Error('No steps provided.');
	}
}


// GUI component to control the playback of the simulation
// includes buttons to play, pause, step forward, step backward and restart
// also include a playback progress slider and a speed slider to control transitions speed
export class PlaybackController {
	constructor(simState, simulation, params) {
		this.simState = simState;
		this.sim = simulation;
		this.params = params;
		this.container = document.createElement('div');
		this.container.id = 'playback-container';
		document.body.appendChild(this.container);
		this.pane = new Tweakpane.Pane({container: this.container});
		// playback slider
		this.stepInput = this.pane.addInput(this.simState, 'step', {
			min: 0,
			max: this.simState.len,
			step: 1,
			label: 'Position',
			disabled: this.simState.is_playing,
		});
		this.stepInput.on('change', () => {
			this.params.vis.updateStep();
		});
		// speed slider
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
		// buttons
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
		if (this.params.in_anim) {
			this.params.button_buff = index;
			return;
		}
		switch (index) {
			case 0:
				this.sim.stepBack(); break;
			case 1:
				this.sim.playPause();
				if (this.simState.is_playing)
					this.sim.run();
				break;
			case 2:
				this.sim.stepForward(); break;
			case 3:
				this.sim.restart(); break;
		}
		this.update();
	}

	update() {
		this.stepInput.disabled = this.simState.is_playing;
		this.speedInput.disabled = this.simState.is_playing;
		this.stepInput.refresh();
	}
}
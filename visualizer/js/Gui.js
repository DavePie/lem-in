const pane = new Tweakpane.Pane();
import { Rooms, Links, Ant } from './AntFarm.js';

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

		try{
			[this.params.rooms, this.params.links, this.params.ants] = this.parseInput(this.getInputValue());
		} catch (error) {
			this.params.farmValid = false;
			console.error(error);
		}
		if (this.params.rooms && this.params.links && this.params.ants)
			this.params.farmValid = true;
		else
			this.params.farmValid = false;
		console.log('AntFarm validated:', this.params.farmValid);
		if (this.onValidate)
			this.onValidate(this.params.farmValid);
	}

	getInputValue() {
		return this.textArea.value;
	}

	parseInput(input) {
		const lines = input.split('\n').map(line => line.trim()).filter(line => line.length > 0);
		
		const rooms = new Rooms();
		const links = new Links();
		
		let numAnts;
		let parsingRooms = false;
		let parsingLinks = false;
		let preproc = false;

		if (lines.length === 0)
			throw new Error('No input provided.');
	
		for (let line of lines) {
			if (line.startsWith('#')) {
				if (line.startsWith('##')) {
					const directive = line.slice(2).toLowerCase();
					if (directive === 'start' || directive === 'end') {
						preproc = directive;
						parsingRooms = true;
						parsingLinks = false;
					} else
						throw new Error(`Directive ${directive} is not recognized.`);
				}
				continue;
			}
	
			if (!parsingRooms && !parsingLinks) {
				numAnts = parseInt(line, 10);
				parsingRooms = true;
			} else if (parsingRooms) {
				if (line.includes('-')) {
					parsingRooms = false;
					parsingLinks = true;
				} else {
					const [name, x, y] = line.split(' ');

					rooms.add(name, [parseInt(x), parseInt(y)], preproc);
				}
				preproc = false;
			}
	
			if (parsingLinks && line.includes('-')) {
				const [room1, room2] = line.split('-');
				const pos1 = rooms.getPos(room1);
				const pos2 = rooms.getPos(room2);
				if (pos1 && pos2)
					links.add(room1, room2, pos1, pos2);
				else
					throw new Error(`One of the rooms in the link ${line} does not exist.`);
				preproc = false;
			}
		}
		const ants = new Ant(numAnts, rooms, links);

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
			this.params.simValid = this.parseinput(this.getInputValue());
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
			console.log('steps:', steps[i], 'steps in ant:', this.params.ants.steps, 'ants array length:', this.params.ants.positions[0].length);
			console.log('ants tab:', this.params.ants.positions);

			let step = steps[i].trim();
			if (step.length === 0) continue;
	
			let result = this.params.ants.addStep(step.split(' '));
	
			if (typeof result === 'string') {
				console.error(result);
				return false;
			}
		}
		return true;
	}
}


// GUI component to control the playback of the simulation
// includes buttons to play, pause, step forward, step backward and restart
// also include a playback progress slider and a speed slider to control transitions speed
export class PlaybackController {
	constructor(simState, simulation) {
		this.simState = simState;
		this.sim = simulation;

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
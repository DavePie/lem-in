import { AntFarmVisualizer } from './visualization.js';

export class Simulation {
	constructor(visualizer, simState, onUpdate, param, gui) {
		this.visualizer = visualizer;
		this.simState = simState;
		this.onUpdate = onUpdate;
		this.param = param;
		this.gui = gui;
		console.log('variables check in simulation:', '\nvisualizer:', this.visualizer, '\nsimState', this.simState,'\nonUpdate', this.onUpdate, '\nparam', this.param, 'gui', this.gui);
	}

	run() {
		if (this.simState.is_playing && this.simState.step < this.simState.len) {
			this.simState.step++;
			this.onUpdate(this.simState.step, this.simState);
			this.guiUpdate();
		} else if (this.simState.step === this.simState.len) {
			this.simState.is_playing = false;
			this.guiUpdate();
			console.log('Simulation ended');
		}
	}

	stepBack() {
		if (this.simState.step > 0 && !this.simState.is_playing) {
			this.simState.step--;
			this.onUpdate(this.simState.step, this.simState);
			this.guiUpdate();
			console.log('Step Backward, step:', this.simState.step);
		}
	}

	playPause() {
		this.simState.is_playing = !this.simState.is_playing;
		console.log('in play gui is:', this.gui);
		this.guiUpdate();
		console.log('Play/Pause, is_playing:', this.simState.is_playing);
	}

	stepForward() {
		if (this.simState.step < this.simState.len && !this.simState.is_playing) {
			this.simState.step++;
			this.onUpdate(this.simState.step, this.simState);
			this.guiUpdate();
			console.log('Step Forward, step:', this.simState.step);
		}
	}

	restart() {
		this.simState.step = 0;
		this.simState.is_playing = false;
		this.onUpdate(this.simState.step, this.simState);
		this.guiUpdate();
		console.log('Simulation restarted');
	}

	guiUpdate() {
		this.gui.playbackController.stepInput.disabled = this.simState.is_playing;
		this.gui.playbackController.speedInput.disabled = this.simState.is_playing;
		this.gui.playbackController.stepInput.refresh();
	}
}
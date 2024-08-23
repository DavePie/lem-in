import { AntFarmVisualizer } from './visualization.js';

export class Simulation {
	constructor(visualizer, simState, onUpdate, param, gui) {
		this.visualizer = visualizer;
		this.simState = simState;
		this.onUpdate = onUpdate;
		this.param = param;
		this.gui = gui;
		console.log('variables check in simulation:', this.visualizer, this.simState, this.onUpdate, this.param, this.gui);
	}

	run() {
		if (this.simState.is_playing && this.simState.step < this.simState.len) {
			this.simState.step++;
			this.onUpdate(this.simState.step);
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
			this.onUpdate(this.simState.step);
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
			this.onUpdate(this.simState.step);
			this.guiUpdate();
			console.log('Step Forward, step:', this.simState.step);
		}
	}

	restart() {
		this.simState.step = 0;
		this.simState.is_playing = false;
		this.onUpdate(this.simState.step);
		this.guiUpdate();
		console.log('Simulation restarted');
	}

	guiUpdate() {
		this.gui.playbackController.stepInput.disabled = simState.is_playing;
		this.gui.playbackController.speedInput.disabled = simState.is_playing;
		this.gui.playbackController.stepInput.refresh();
	}
}
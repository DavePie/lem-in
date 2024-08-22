import { AntFarmVisualizer } from './visualization.js';

export class Simulation {
	constructor(visualizer, simState, onUpdate, onGuiUpdate) {
		this.visualizer = visualizer;
		this.simState = simState;
		this.onUpdate = onUpdate;
		this.onGuiUpdate = onGuiUpdate;
	}

	run() {
		if (this.simState.is_playing && this.simState.step < this.simState.len) {
			this.simState.step++;
			this.onUpdate(this.simState.step);
			this.onGuiUpdate();
		} else if (this.simState.step === this.simState.len) {
			this.simState.is_playing = false;
			this.onGuiUpdate();
			console.log('Simulation ended');
		}
	}

	stepBack() {
		if (this.simState.step > 0 && !this.simState.is_playing) {
			this.simState.step--;
			this.onUpdate(this.simState.step);
			this.onGuiUpdate();
			console.log('Step Backward, step:', this.simState.step);
		}
	}

	playPause() {
		this.simState.is_playing = !this.simState.is_playing;
		this.onGuiUpdate();
		console.log('Play/Pause, is_playing:', this.simState.is_playing);
	}

	stepForward() {
		if (this.simState.step < this.simState.len && !this.simState.is_playing) {
			this.simState.step++;
			this.onUpdate(this.simState.step);
			this.onGuiUpdate();
			console.log('Step Forward, step:', this.simState.step);
		}
	}

	restart() {
		this.simState.step = 0;
		this.simState.is_playing = false;
		this.onUpdate(this.simState.step);
		this.onGuiUpdate();
		console.log('Simulation restarted');
	}
}
import { AntFarmVisualizer } from './visualization.js';

export class Simulation {
	constructor(visualizer, simState, param, gui) {
		this.visualizer = visualizer;
		this.simState = simState;
		this.param = param;
		this.gui = gui;
	}

	async run() {
		while (this.simState.is_playing && this.simState.step < this.simState.len) {
			this.simState.step++;
			this.param.in_anim = true;
			await this.param.vis.animateToStep();
			this.param.in_anim = false;
			// Do button action if one was blocked by animation
			if (this.param.button_buff !== -1) {
				this.gui.playbackController.playbackAction(this.param.button_buff);
				this.param.button_buff = -1;
			}
			this.gui.playbackController.update();
		}
		this.simState.is_playing = false;
		this.gui.playbackController.update();
	}

	stepBack() {
		if (this.simState.step > 0 && !this.simState.is_playing) {
			this.simState.step--;
			this.param.vis.updateStep();
		}
	}

	playPause() {
		this.simState.is_playing = !this.simState.is_playing;	
	}

	stepForward() {
		if (this.simState.step < this.simState.len && !this.simState.is_playing) {
			this.simState.step++;
			this.param.vis.updateStep();
		}
	}

	restart() {
		this.simState.step = 0;
		this.simState.is_playing = false;
		this.param.vis.updateStep();
	}

	guiUpdate() {
		this.gui.playbackController.stepInput.disabled = this.simState.is_playing;
		this.gui.playbackController.speedInput.disabled = this.simState.is_playing;
		this.gui.playbackController.stepInput.refresh();
	}
}
export class Simulation {
	constructor(simState, onUpdate, onGuiUpdate) {
		this.simState = simState;
		this.onUpdate = onUpdate;
		this.onGuiUpdate = onGuiUpdate; // Fonction de rappel pour mettre à jour le GUI
	}

	run() {
		if (this.simState.is_playing && this.simState.step < this.simState.len) {
			this.simState.step++;
			this.onUpdate(this.simState.step); // Mise à jour de la simulation
			this.onGuiUpdate(); // Appeler la fonction pour mettre à jour le GUI
		} else if (this.simState.step === this.simState.len) {
			this.simState.is_playing = false;
			this.onGuiUpdate(); // Appeler la fonction pour mettre à jour le GUI
			console.log('Simulation ended');
		}
	}

	stepBack() {
		if (this.simState.step > 0 && !this.simState.is_playing) {
			this.simState.step--;
			this.onUpdate(this.simState.step); // Mise à jour de la simulation
			this.onGuiUpdate(); // Appeler la fonction pour mettre à jour le GUI
			console.log('Step Backward, step:', this.simState.step);
		}
	}

	playPause() {
		this.simState.is_playing = !this.simState.is_playing;
		this.onGuiUpdate(); // Appeler la fonction pour mettre à jour le GUI
		console.log('Play/Pause, is_playing:', this.simState.is_playing);
	}

	stepForward() {
		if (this.simState.step < this.simState.len && !this.simState.is_playing) {
			this.simState.step++;
			this.onUpdate(this.simState.step); // Mise à jour de la simulation
			this.onGuiUpdate(); // Appeler la fonction pour mettre à jour le GUI
			console.log('Step Forward, step:', this.simState.step);
		}
	}

	restart() {
		this.simState.step = 0;
		this.simState.is_playing = false;
		this.onUpdate(this.simState.step); // Mise à jour de la simulation
		this.onGuiUpdate(); // Appeler la fonction pour mettre à jour le GUI
		console.log('Simulation restarted');
	}
}
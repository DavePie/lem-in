const pane = new Tweakpane.Pane();
import { AntFarmForm, SimulationForm, PlaybackController } from './Gui.js';
import { Simulation } from './Simulation.js';
import { AntFarmVisualizer } from './visualization.js';

// Initial states
const simState = {
	is_playing: false,
	len: 0,
	step: 0,
	speed: 1,
};
const param = {
	farmValid: false,
	simValid: false,
	
	rooms: null,
	links: null,
	ants: null,
	sim: null,
};
let playbackController = null;
let visualizer = null;


//
//   Updaters
//
function updateGui() {
	playbackController.stepInput.disabled = simState.is_playing;
	playbackController.speedInput.disabled = simState.is_playing;
	playbackController.stepInput.refresh();
}
// Function to update the simulation
function updateSimulation(step, simState) {
	// Add logic to apply changes to the rendered ant farm
	console.log('Simulation updated to step:', step);
	if (visualizer){
		if (is_playing){
			visualizer.animateToStep(step - 1, step);
		} else {
			visualizer.moveToStep(step);
		}
	}
}

const canvas = document.createElement('canvas');

// Définir la taille du canvas (optionnel, sinon utilise la taille par défaut)
canvas.width = window.innerWidth;
canvas.height = window.innerHeight;
// Ajouter le canvas au body de la page
document.body.appendChild(canvas);


//
//   GUI components setup
//
const antFarmForm = new AntFarmForm(param, (isValid) => {
	simulationForm.button.disabled = !isValid;
});
const simulationForm = new SimulationForm(param, (isValid) => {
	if (isValid && param.simValid) {
		if (!playbackController)  {
			console.log('number of steps:', param.ants.steps);
			playbackController = new PlaybackController(simState, simulation);
		}
		if (!visualizer){
			visualizer = new AntFarmVisualizer(canvas, param);
			simState.len = param.ants.positions[0].length;
			updateGui();
		}
		simState.is_playing = false;
		simState.step = 0;
		simState.speed = 1;
		updateGui();	
	}
});


// Simulation orchestrator
const simulation = new Simulation(simState, updateSimulation, updateGui);

// Main loop
setInterval(() => {
	if (simState.is_playing)
		simulation.run();
}, 1000);

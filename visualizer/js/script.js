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
const gui = {
	antFarmForm: null,
	simulationForm: null,
	playbackController: null,
};
let playbackController = null;
let visualizer = null;


//
//   Updaters
//
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
gui.antFarmForm = new AntFarmForm(param, (isValid) => {
	gui.simulationForm.button.disabled = !isValid;
});
gui.simulationForm = new SimulationForm(param, (isValid) => {
	if (isValid && param.simValid) {
		if (!playbackController)  {
			if (!visualizer && param.ants){
				visualizer = new AntFarmVisualizer(canvas, param);
				simState.len = param.ants.steps;
			}
			console.log('number of steps:', param.ants.steps);
			console.log('positions tab:', param.ants.positions);
			gui.playbackController = new PlaybackController(simState, simulation);
		}
		simState.is_playing = false;
		simState.step = 0;
		simState.speed = 1;
		
		param.simulation = new Simulation(visualizer, simState, updateSimulation, param, gui);

		gui.playbackController.stepInput.disabled = simState.is_playing;
		gui.playbackController.speedInput.disabled = simState.is_playing;
		gui.playbackController.stepInput.refresh();
	}
});


// Simulation orchestrator
param.simulation = null;

// Main loop
setInterval(() => {
	if (param.simulation && param.simState.is_playing)
		param.simulation.run();
}, 1000);

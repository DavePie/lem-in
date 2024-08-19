const pane = new Tweakpane.Pane();
import { AntFarmForm, SimulationForm, PlaybackController } from './Gui.js';
import { Simulation } from './Simulation.js';


// Initial states
const simState = {
	is_playing: false,
	len: 0,
	step: 0,
	speed: 1,
};
const parameters = {
	AntFarm: '',
	Simulation: '',
};
let playbackController;


//
//   Updaters
//
function updateGui() {
	playbackController.stepInput.disabled = simState.is_playing;
	playbackController.speedInput.disabled = simState.is_playing;
	playbackController.stepInput.refresh();
}
// Function to update the simulation
function updateSimulation(step) {
	// Add logic to apply changes to the rendered ant farm
	console.log('Simulation updated to step:', step);
}


//
//   GUI components setup
//
const antFarmForm = new AntFarmForm(parameters, (isValid) => {
	simulationForm.inputField.disabled = !isValid;
});
const simulationForm = new SimulationForm(parameters, (isValid) => {
	if (isValid && parameters.Simulation)
		playbackController = new PlaybackController(simState, simulation);
});


// Simulation orchestrator
const simulation = new Simulation(simState, updateSimulation, updateGui);

// Main loop
setInterval(() => {
	if (simState.is_playing)
		simulation.run();
}, 250);

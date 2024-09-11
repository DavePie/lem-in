const pane = new Tweakpane.Pane();
import { AntFarmForm, SimulationForm, PlaybackController } from './Gui.js';
import { Simulation } from './Simulation.js';
import { AntFarmVisualizer } from './visualization.js';
import { ErrorPopup } from './ErrorPopup.js';

// Initial states
const simState = {
	is_playing: false,
	in_anim: false,
	button_buff: -1,
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
	simState: simState,
	vis: null,
	canvas: null,
	gui: null,

	popup: new ErrorPopup(),
};
const gui = {
	antFarmForm: null,
	simulationForm: null,
	playbackController: null,
};
param.gui = gui;
let playbackController = null;
let visualizer = null;


// Create a canvas to draw in
const canvas = document.createElement('canvas');
canvas.width = window.innerWidth;
canvas.height = window.innerHeight;
document.body.appendChild(canvas);
param.canvas = canvas;


//   GUI components setup
gui.antFarmForm = new AntFarmForm(param, (isValid) => {
	gui.simulationForm.button.disabled = !isValid;
});
gui.simulationForm = new SimulationForm(param, (isValid) => {
	if (isValid && param.simValid) {
		if (!playbackController)  {
			if (param.ants)
				simState.len = param.ants.steps;
			gui.playbackController = new PlaybackController(simState, gui.simulation, param);
		}
		simState.is_playing = false;
		simState.step = 0;
		simState.speed = 1;
		param.simulation = new Simulation(param.vis, simState, param, gui);
		gui.playbackController.sim = param.simulation;
		gui.playbackController.stepInput.disabled = simState.is_playing;
		gui.playbackController.speedInput.disabled = simState.is_playing;
		gui.playbackController.stepInput.refresh();
	}
});


let placeholderFarm = `9
##start
a 2 5
##end
p 18 12
b 5 10
c 8 4
d 11 7
e 14 2
f 16 9
g 12 14
h 6 12
i 3 14
j 10 2
k 15 4
l 7 8
m 13 11
n 9 11
o 4 7
a-b
a-o
b-c
b-l
c-j
c-d
d-k
d-n
e-j
e-k
f-m
f-n
g-m
g-p
h-i
h-n
i-o
k-f
k-p
l-h
m-n
o-l`;

let placeholderSim = `L1-b L2-o 
L1-c L2-i L3-b L4-o 
L1-d L2-h L3-c L4-i L5-b L6-o 
L1-k L2-n L3-d L4-h L5-c L6-i L7-b L8-o 
L1-p L2-m L3-k L4-n L5-d L6-h L7-c L8-i L9-b 
L2-g L3-p L4-m L5-k L6-n L7-d L8-h L9-c 
L2-p L4-g L5-p L6-m L7-k L8-n L9-d 
L4-p L6-g L7-p L8-m L9-k 
L6-p L8-g L9-p 
L8-p`;


// Put placeholder data in the text areas
gui.antFarmForm.textArea.value = placeholderFarm;
gui.simulationForm.textArea.value = placeholderSim;


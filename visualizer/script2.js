const pane = new Tweakpane.Pane();

/**********************************\
[                                  ]
[ -- AntFarm configuration form -- ]
[                                  ]
\**********************************/
class AntFarmForm {
	constructor(params) {
		// Folder for AntFarm settings
		this.Folder = pane.addFolder({
			title: 'AntFarm Settings',
			expanded: true,
		});
		// Text field for AntFarm configuration
		this.InputField = this.Folder.addInput(params, 'AntFarm', {
			label: 'AntFarm'
		}).on('change', (value) => {
			console.log('AntFarm:', value);
		});
		// AntFarm configuration validation button
		this.Folder.addButton({ title: 'Valider AntFarm' }).on('click', () => {
			this.InputStr = this.InputField.controller_.view.element.querySelector('input').value;
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			// [ -- implement parsing for parameters.AntFarm                     -- ] //
			// [ -- parameters.AntFarm = parseAntFarm(parameters.AntFarm);       -- ] //
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			params.AntFarm = true; // To be removed after implementing the parsing
			updateInterface();
			console.log('AntFarm validé');
		});
	}
}


/*************************************\
[                                     ]
[ -- Simulation configuration form -- ]
[                                     ]
\*************************************/
class SimulationForm {
	constructor(params) {
		// Folder for simulation settings
		this.Folder = pane.addFolder({
			title: 'Simulation Settings',
			expanded: true,
		});
		// Text field for simulation insructions, deactivated while there is no valid AntFarm
		this.InputField = this.Folder.addInput(parameters, 'Simulation', {
			label: 'Simulation',
			disabled: !parameters.AntFarm,
		});
		// Simulation instructions validation button
		this.Folder.addButton({ title: 'Valider Simulation' }).on('click', () => {
			if (parameters.AntFarm) {
				this.InputStr = this.InputField.controller_.view.element.querySelector('input').value;
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				// [ -- implement parsing for simulationString                  -- ] //
				// [ -- parameters.Simulation = parseSimulation(this.InputStr); -- ] //
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				parameters.Simulation = true; // To be removed after implementing the parsing
				updateInterface();
			}
		});
	}
}
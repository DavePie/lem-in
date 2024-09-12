export class AntFarmVisualizer {
	constructor(param) {
		this.param = param;
		this.simState = param.simState;
		this.canvas = param.canvas;
		this.ctx = this.canvas.getContext('2d');
		this.rooms = param.rooms;
		this.links = param.links;
		this.ants = param.ants;

		this.margin = 50;
		this.scale = 1;
		this.offsetX = 0;
		this.offsetY = 0;
		this.dragging = false;
		this.lastMousePos = { x: 0, y: 0 };

		this.setupEventListeners();
		this.resize();
		window.addEventListener('resize', () => this.resize());
		this.updateStep();
	}

	setupEventListeners() {
		// scroll button to zoom in and out
		this.canvas.addEventListener('wheel', (event) => this.handleZoom(event));

		// Clic and drag to move the view
		this.canvas.addEventListener('mousedown', (event) => this.handleMouseDown(event));
		this.canvas.addEventListener('mousemove', (event) => this.handleMouseMove(event));
		this.canvas.addEventListener('mouseup', () => this.handleMouseUp());
		this.canvas.addEventListener('mouseleave', () => this.handleMouseUp());
	}

	handleZoom(event) {
		event.preventDefault();
		const zoomFactor = 1.1;
		const zoom = event.deltaY < 0 ? zoomFactor : 1 / zoomFactor;
		const { offsetX, offsetY } = event;

		// update the new scale and offset
		this.scale *= zoom;
		this.offsetX = offsetX - (offsetX - this.offsetX) * zoom;
		this.offsetY = offsetY - (offsetY - this.offsetY) * zoom;
		// redraw
		this.updateStep();
	}

	handleMouseDown(event) {
		this.dragging = true;
		this.lastMousePos = { x: event.clientX, y: event.clientY };
	}

	handleMouseMove(event) {
		if (this.dragging) {
			const dx = event.clientX - this.lastMousePos.x;
			const dy = event.clientY - this.lastMousePos.y;
			// update the offset
			this.offsetX += dx;
			this.offsetY += dy;
			this.lastMousePos = { x: event.clientX, y: event.clientY };
			// redraw
			this.updateStep();
		}
	}

	handleMouseUp() {
		this.dragging = false;
	}

	resize() {
		this.canvas.width = window.innerWidth;
		this.canvas.height = window.innerHeight;

		this.calculateScaling();
		this.updateStep();
	}

	calculateScaling() {
		const positions = this.rooms.getPositionsTab();
		const xCoords = positions.map(pos => pos[0]);
		const yCoords = positions.map(pos => pos[1]);

		const minX = Math.min(...xCoords);
		const maxX = Math.max(...xCoords);
		const minY = Math.min(...yCoords);
		const maxY = Math.max(...yCoords);

		this.scale = Math.min(
			(this.canvas.width - 2 * this.margin) / (maxX - minX),
			(this.canvas.height - 2 * this.margin) / (maxY - minY)
		);
		this.offsetX = (this.canvas.width - (maxX - minX) * this.scale) / 2 - minX * this.scale;
		this.offsetY = (this.canvas.height - (maxY - minY) * this.scale) / 2 - minY * this.scale;

		let minDistance = Infinity;
		for (let i = 0; i < positions.length; i++) {
			for (let j = i + 1; j < positions.length; j++) {
				const dist = Math.hypot(
					positions[i][0] - positions[j][0],
					positions[i][1] - positions[j][1]
				);
				if (dist < minDistance && dist !== 0)
					minDistance = dist;
			}
		}
		this.roomDiameter = (minDistance / 3) * this.scale;
	}

	clearScreen() {
		this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
		this.ctx.fillStyle = 'black';
		this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);
	}

	draw() {
		this.clearScreen();
		this.drawLinks();
		this.drawRooms();
	}

	drawRooms() {
		for (const key in this.rooms.rooms) {
			const room = this.rooms.rooms[key];
			const [x, y] = this.transform(room.pos);

			if (room.isSpecial === "start" || room.isSpecial === "end") {
				// outer white band
				this.ctx.beginPath();
				this.ctx.arc(x, y, this.roomDiameter * 0.65, 0, 2 * Math.PI);
				this.ctx.fillStyle = 'white';
				this.ctx.fill();
				// middle black band
				this.ctx.beginPath();
				this.ctx.arc(x, y, this.roomDiameter * 0.55, 0, 2 * Math.PI);
				this.ctx.fillStyle = 'black';
				this.ctx.fill();
			}
			// room circle
			this.ctx.beginPath();
			this.ctx.arc(x, y, this.roomDiameter / 2, 0, 2 * Math.PI);
			this.ctx.fillStyle = 'white';
			this.ctx.fill();
		}
	}

	drawLinks() {
		const links = this.links.getLinksTab();
		links.forEach(link => {
			const [room1, room2, color] = link;
			const [x1, y1] = this.transform(room1);
			const [x2, y2] = this.transform(room2);

			this.ctx.fillStyle = `rgb(${color[0]}, ${color[1]}, ${color[2]})`;

			const angle = Math.atan2(y2 - y1, x2 - x1);
			const dist = Math.hypot(x2 - x1, y2 - y1);
			const thickness = this.roomDiameter / 6; // width

			this.ctx.save();
			this.ctx.translate(x1, y1);
			this.ctx.rotate(angle);
			this.ctx.fillRect(0, -thickness / 2, dist, thickness);
			this.ctx.restore();
		});
	}

	// Update to current step
	updateStep() {
		this.clearScreen();
		this.drawLinks();
		this.drawRooms();
		const step = this.simState.step;
		for (let i = 0; i < this.ants.num; i++) {
			const room = this.ants.getPosition(i + 1, step);
			const [x, y] = this.transform(room.pos);

			this.ctx.fillStyle = `rgb(${this.ants.colors[i][0]}, ${this.ants.colors[i][1]}, ${this.ants.colors[i][2]})`;
			this.ctx.beginPath();
			this.ctx.arc(x, y, this.roomDiameter / 4, 0, 2 * Math.PI);
			this.ctx.fill();
		}
	}

	// Animate ants translation from step - 1 to step
	animateToStep() {
		return new Promise((resolve) => {
			const previousStep = this.simState.step - 1;
			const nextStep = this.simState.step;
			const duration = 1000 / this.simState.speed;
			let startTime;

			const animate = (timestamp) => {
				if (!startTime) startTime = timestamp;
				const progress = Math.min((timestamp - startTime) / duration, 1);

				this.clearScreen();
				this.drawLinks();
				this.drawRooms();

				for (let i = 0; i < this.ants.num; i++) {
					const startRoom = this.ants.getPosition(i + 1, previousStep);
					const endRoom = this.ants.getPosition(i + 1, nextStep);
					// change the color of the link the ant used if was white
					if (startRoom != endRoom && this.links.getLink(startRoom, endRoom).color[0] === 255)
						this.links.getLink(startRoom, endRoom).color = this.ants.colors[i];

					const [startX, startY] = this.transform(startRoom.pos);
					const [endX, endY] = this.transform(endRoom.pos);
					const currentX = startX + (endX - startX) * progress;
					const currentY = startY + (endY - startY) * progress;

					this.ctx.fillStyle = `rgb(${this.ants.colors[i][0]}, ${this.ants.colors[i][1]}, ${this.ants.colors[i][2]})`;
					this.ctx.beginPath();
					this.ctx.arc(currentX, currentY, this.roomDiameter / 4, 0, 2 * Math.PI);
					this.ctx.fill();
				}

				if (progress < 1)
					requestAnimationFrame(animate);
				else {
					this.updateStep();
					resolve();
				}
			};
			requestAnimationFrame(animate);
		});
	}

	// Transform a position from the simulation to a position on the canvas
	transform(pos) {
		return [
			(pos[0] * this.scale) + this.offsetX,
			(pos[1] * this.scale) + this.offsetY
		];
	}
}

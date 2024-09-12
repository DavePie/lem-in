// Stores information for a room
export class Room {
	constructor(name, x, y, isSpecial = false) {
		this.name = name;
		this.pos = [x, y];
		this.isSpecial = isSpecial; // "start", "end", or false
	}
}


// Room class to manage all rooms
export class Rooms {
	constructor() {
		this.rooms = {}; // Use an object to store rooms by name
	}

	// Add a room
	addRoom(name, x, y, isSpecial = false) {
		if (!(name in this.rooms))
			this.rooms[name] = new Room(name, x, y, isSpecial);
		else
			throw new Error(`Room "${name}" already exists.`);
		if (isSpecial === "start") {
			if (this.start)
				throw new Error(`Start room already exists.`);
			this.start = this.rooms[name];
		} else if (isSpecial === "end") {
			if (this.end)
				throw new Error(`End room already exists.`);
			this.end = this.rooms[name];
		}
	}

	// Check if a room exists
	exists(name) {
		return name in this.rooms;
	}

	// To iterate and draw all rooms easily
	getPositionsTab() {
		let positions = [];
		for (let key in this.rooms)
			positions.push(this.rooms[key].pos);
		return positions;
	}
}


// Stores information for a link
export class Link {
	constructor(room1, room2) {
		this.room1 = room1.pos;
		this.room2 = room2.pos;
		this.color = [255, 255, 255];
	}
}


// Link class to manage all links
export class Links {
	constructor() {
		this.links = {}; // Use an object to store links by key
		this.splitKey = this.generateRandomKey(12); // Key to split room1 and room2 in the map key
	}

	// Generate a random key of a given length to avoid conflicts in the map keys
	generateRandomKey(length) {
		const chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()_+[]{}|;:,./<>?';
		let result = '';
		for (let i = 0; i < length; i++)
			result += chars.charAt(Math.floor(Math.random() * chars.length));
		return result;
	}

	// Add a link
	addLink(room1, room2) {
		const key = room1.name < room2.name
			? `${room1.name}${this.splitKey}${room2.name}`
			: `${room2.name}${this.splitKey}${room1.name}`;
		if (!(key in this.links))
			this.links[key] = new Link(room1, room2);
		else
			throw new Error(`Link between rooms ${room1.name} and ${room2.name} already exists.`);
	}

	// Check if a link exists
	exists(room1, room2) {
		room1 = typeof room1 === 'string' ? room1 : room1.name;
		room2 = typeof room2 === 'string' ? room2 : room2.name;
		const key = room1 < room2
			? `${room1}${this.splitKey}${room2}`
			: `${room2}${this.splitKey}${room1}`;
		return key in this.links;
	}

	// Get a link
	getLink(room1, room2) {
		room1 = typeof room1 === 'string' ? room1 : room1.name;
		room2 = typeof room2 === 'string' ? room2 : room2.name;
		const key = room1 < room2
			? `${room1}${this.splitKey}${room2}`
			: `${room2}${this.splitKey}${room1}`;
		const link = this.links[key];
		if (link)
			return link;
		else
			throw new Error(`Link between rooms ${room1} and ${room2} does not exist.`);
	}

	// To iterate and draw all links easily
	getLinksTab() {
		let links = [];
		for (let key in this.links) {
			const link = this.links[key];
			links.push([link.room1, link.room2, link.color]);
		}
		return links;
	}
}



// Ant class to manage all ants, their positions and the links taken
export class Ants {
	constructor(num, roomStart) {
		this.num = num;
		this.positions = Array.from({ length: num }, () => [roomStart]);
		this.links = Array.from({ length: num }, () => []);
		// Color of each ant generated randomly
		this.colors = Array.from({ length: num }, () => [
			Math.floor(Math.random() * 256),
			Math.floor(Math.random() * 256),
			Math.floor(Math.random() * 256)
		]);
		this.steps = 0;
	}

	// Add position of ant[ant_id] at step index
	addPosition(id, room, step) {
		if (id > 0 && id <= this.num)
			this.positions[id - 1][step] = room;
		else
			throw new Error(`Invalid ant ID: ${id}.`);
	}

	// Add link used by ant[ant_id] to get to step
	addLink(ant_id, link, step) {
		if (ant_id > 0 && ant_id <= this.num)
			this.links[ant_id - 1][step] = link;
		else
			throw new Error(`Invalid ant ID: ${ant_id}.`);
	}


	addStep(moves, step, rooms, links) {
		moves = moves.split(' ');
		// add new step to positions and links with null by default
		this.positions.forEach(positions => {
			positions.push(null);
		});
		this.links.forEach(links => {
			links.push(null);
		});
		// process each move in the current step
		moves.forEach(move => {
			// make sure the shape is L[ant_id]-[room_name]
			if (!move.match(/^L\d+-\w+$/))
				throw new Error(`Invalid move "${move}" at step ${step}.`);
			// get ant_id and room_name
			let [ant_id, room_name] = move.split('-');
			ant_id = parseInt(ant_id.slice(1));
			// check if room exists and add it
			if (!rooms.exists(room_name))
				throw new Error(`Room "${room_name}" does not exist.`);
			else
				this.addPosition(ant_id, rooms.rooms[room_name], step);	
			// check if link exists and add it
			if (step > 0 && !links.exists(this.getPosition(ant_id, step - 1), rooms.rooms[room_name]))
				throw new Error(`Link between rooms ${this.getPosition(ant_id, step - 1).name} and ${room_name} does not exist.`);
			else if (step > 0)
				this.addLink(ant_id, links.getLink(this.getPosition(ant_id, step - 1), room_name), step);
		})
		//replace null positions at step with previous positions
		this.positions.forEach((positions, index) => {
			if (positions[step] === null)
				positions[step] = step > 0 
					? positions[step - 1]
					: rooms.start;
		});
		if (step > this.steps)
			this.steps = step;
	}

	// Get position of ant[ant_id] at step index
	getPosition(ant_id, index) {
		if (ant_id > 0 && ant_id <= this.num)
			return this.positions[ant_id - 1][index];
		else
			throw new Error(`Invalid ant ID: ${ant_id}.`);
	}

	// Get link used by ant[ant_id] to get to step
	getLinkUsed(ant_id, step) {
		if (ant_id > 0 && ant_id <= this.num)
			return this.links[ant_id - 1][step];
		else
			throw new Error(`Invalid ant ID: ${ant_id}.`);
	}
}

export class Links {
	constructor() {
		this.links = {};  // Objet pour stocker les liens entre les salles
		this.linkList = []; // Liste pour garder les liens (utile pour le parcours)
	}

	_add(room1, room2) {
		if (!this.links[room1])
			this.links[room1] = {};
		this.links[room1][room2] = true;
		this.linkList.push([room1, room2]);
	}

	add(room1, room2) {
		if (room1 === room2)
			return "Can't link a room to itself";

		// Assurer que room1 est toujours < room2 pour simplifier
		if (room1 > room2)
			[room1, room2] = [room2, room1];
		// check if already linked
		if (this.check(room1, room2))
			return "Rooms are already linked";

		if (!this.links[room1])
			this.links[room1] = {};
		this.links[room1][room2] = true;
		this.linkList.push([room1, room2]);
	}

	check(room1, room2) {
		if (room1 === room2)
			return false;
		// Assurer que room1 est toujours < room2 pour simplifier
		if (room1 > room2)
			[room1, room2] = [room2, room1];
		return this.links[room1] && this.links[room1][room2] || false;
	}

	getAll() {
		return this.linkList;
	}
}


export class Rooms {
	constructor() {
		this.rooms = {};
	}

	add(name, pos, endpoint=false) {
		if (this.rooms[name])
			return "Room already exists";
		this.rooms[name] = pos;
		if (endpoint === "start")
			this.start = name;
		else if (endpoint === "end")
			this.end = name;
	}

	getPos(name) {
		return this.rooms[name] || null;
	}

	exists(name) {
		return !!this.rooms[name];
	}

	getAll() {
		return Object.keys(this.rooms);
	}
}

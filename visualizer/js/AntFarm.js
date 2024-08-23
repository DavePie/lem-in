// stores every link between rooms
export class Links {
	constructor() {
		this.links = {}; // dictionary of dictionaries, used to easily check if a link exists
		this.linkList = []; // list of links, used to easily iterate over all links to draw them
	}

	add(room1, room2, pos1, pos2) {
		if (room1 === room2)
			return "Can't link a room to itself";
		if (room1 > room2)// sort rooms because stored as [smaller, bigger]
			[room1, room2, pos1, pos2] = [room2, room1, pos2, pos1];
		if (this.check(room1, room2)) // check if already linked
			return "Rooms are already linked";
		if (!this.links[room1])
			this.links[room1] = {};
		this.links[room1][room2] = {pos1, pos2};
		this.linkList.push({room1: pos1, room2: pos2, color: [255, 255, 255]});
	}

	check(room1, room2) {
		if (room1 === room2)
			return false;
		if (room1 > room2)
			[room1, room2] = [room2, room1];
		return this.links[room1] && this.links[room1][room2] || false;
	}
}


// stores every room
export class Rooms {
	constructor() {
		this.rooms = {}; // dictionary of rooms, used to easily check if a room exists
		this.pos = []; // list of positions, used to easily iterate over all rooms to draw them
	}

	add(name, position, endpoint=false) {
		if (this.rooms[name])
			return "Room already exists";
		if (endpoint === "start")
			this.start = name;
		else if (endpoint === "end")
			this.end = name;
			this.pos.push(position);
		this.rooms[name] = position;
	}

	getPos(name) {
		return this.rooms[name] || null;
	}

	check(name) {
		return !!this.rooms[name];
	}
}

// Class for ants storing their positions at each step
export class Ant {
    constructor(numAnts, rooms, links) {
        this.numAnts = numAnts; // number of ants
        this.steps = 0;
        this.rooms = rooms; //rooms object
        this.links = links; //links object

        let startPos = rooms.getPos(rooms.start);
        this.lastRooms = Array(numAnts).fill(rooms.start); // last room visited by each ant to verify the link exists
        this.positions = Array.from({ length: numAnts }, () => [startPos.slice()]); // positions of each ant at each step
    }

    addStep(moves) {
        this.steps++;

        // Make sure each ant's position is copied correctly from the last step
        for (let i = 0; i < this.numAnts; i++) {
            // Copy the last position for the new step
            let lastPosition = this.positions[i][this.steps - 1];
            this.positions[i].push(lastPosition.slice());
        }

        // Process each move in the current step
        moves.forEach(move => {
            let antId = parseInt(move[1]) - 1;
            let roomName = move.split('-')[1];

            if (!this.rooms.check(roomName))
                throw new Error(`Room '${roomName}' does not exist.`);

            let previousRoom = this.lastRooms[antId] || this.rooms.start;
            if (!this.links.check(previousRoom, roomName))
                throw new Error(`Link between '${previousRoom}' and '${roomName}' does not exist.`);

            // console.log('antId:', antId, 'moving from room:', this.lastRooms[antId], 'to room:', roomName, 'pos:', this.rooms.getPos(roomName));

            // Update the ant's position for the current step
            this.positions[antId][this.steps] = this.rooms.getPos(roomName).slice();
            this.lastRooms[antId] = roomName;
        });
    }
}
	

let rooms = [];
let links = [];
let ants = [];
let zoomLevel = 1;
let offsetX = 0;
let offsetY = 0;
let speed = 0.005;

function setup() {
    createCanvas(windowWidth, windowHeight);
    
    // Exemple de salles
    rooms = [
        {name: 'A', x: 100, y: 50},
        {name: 'B', x: 300, y: 150},
        {name: 'C', x: 500, y: 300},
		{name: 'D', x: 700, y: 150},
		{name: 'E', x: 900, y: 800},
		{name: 'F', x: 1100, y: 150},
		{name: 'G', x: 1300, y: 500},
    ];

    // Exemple de liens
    links = [
        {room1: 'A', room2: 'B', color: [255, 255, 255]},
        {room1: 'B', room2: 'C', color: [255, 255, 255]},
		{room1: 'C', room2: 'D', color: [255, 255, 255]},
		{room1: 'D', room2: 'E', color: [255, 255, 255]},
		{room1: 'E', room2: 'D', color: [255, 255, 255]},
		{room1: 'F', room2: 'A', color: [255, 255, 255]},
		{room1: 'G', room2: 'F', color: [255, 255, 255]},
		{room1: 'F', room2: 'D', color: [255, 255, 255]},

    ];

    // Exemple de fourmis
    ants = [
        {id: 1, currentRoom: 'A', targetRoom: 'B', progress: 0, color: [255, 0, 0]},
        {id: 2, currentRoom: 'B', targetRoom: 'C', progress: 0, color: [0, 255, 0]},
		{id: 3, currentRoom: 'C', targetRoom: 'D', progress: 0, color: [0, 0, 255]},
    ];
}

function draw() {
    background(0);

    translate(offsetX, offsetY);
    scale(zoomLevel);

    drawLinks();
    drawRooms();
    drawAnts();

    animateAnts();
}

function drawRooms() {
    fill(200);
    noStroke();
    for (let room of rooms) {
        ellipse(room.x, room.y, 30, 30);
    }
}

function drawLinks() {
    for (let link of links) {
        stroke(link.color);
        strokeWeight(5);
        let room1 = rooms.find(r => r.name === link.room1);
        let room2 = rooms.find(r => r.name === link.room2);
        line(room1.x, room1.y, room2.x, room2.y);
    }
}

function drawAnts() {
    noStroke();
    for (let ant of ants) {
        fill(ant.color);
        let room1 = rooms.find(r => r.name === ant.currentRoom);
        let room2 = rooms.find(r => r.name === ant.targetRoom);
        let antX = lerp(room1.x, room2.x, ant.progress);
        let antY = lerp(room1.y, room2.y, ant.progress);
        ellipse(antX, antY, 20, 20);
    }
}

function animateAnts() {
    for (let ant of ants) {
        ant.progress += speed; // Incrémente la progression de l'animation

        if (ant.progress >= 1) {
            // Met à jour la couleur du lien une fois qu'une fourmi le traverse
            let link = links.find(l => (l.room1 === ant.currentRoom && l.room2 === ant.targetRoom) ||
                                       (l.room1 === ant.targetRoom && l.room2 === ant.currentRoom));
            if (link.color[0] === 255 && link.color[1] === 255 && link.color[2] === 255) {
                link.color = ant.color; // Change la couleur du lien à la couleur de la fourmi
            }

            // Déplace la fourmi à la salle cible et redéfinis la cible
            ant.currentRoom = ant.targetRoom;
            ant.targetRoom = getNextTargetRoom(ant.currentRoom);
            ant.progress = 0;
        }
    }
}

function getNextTargetRoom(currentRoom) {
    // Exemple simple, peut être modifié pour des logiques plus complexes
    let possibleLinks = links.filter(l => l.room1 === currentRoom || l.room2 === currentRoom);
    let nextLink = random(possibleLinks);
    return nextLink.room1 === currentRoom ? nextLink.room2 : nextLink.room1;
}

function mouseDragged() {
    offsetX += movedX;
    offsetY += movedY;
}

function mouseWheel(event) {
    zoomLevel += event.deltaY * -0.001;
    zoomLevel = constrain(zoomLevel, 0.5, 3);
}

function windowResized() {
    resizeCanvas(windowWidth, windowHeight);
}

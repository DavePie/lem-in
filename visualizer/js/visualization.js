export class AntFarmVisualizer {
    constructor(canvas, param) {
        this.canvas = canvas;
        this.rooms = param.rooms;
        this.links = param.links;
        this.ants = param.ants;

        console.log('inside visualizer rooms:', param.rooms.pos);

        this.zoomLevel = 1;
        this.offsetX = 0;
        this.offsetY = 0;

        this.setup();
    }

    setup() {
        this.calculateDefaultZoom();
        window.addEventListener('resize', () => this.resizeCanvas());
        this.canvas.addEventListener('wheel', (e) => this.mouseWheel(e));
        this.canvas.addEventListener('mousedown', (e) => this.mouseDown(e));
        this.canvas.addEventListener('mousemove', (e) => this.mouseDrag(e));
        this.canvas.addEventListener('mouseup', () => this.mouseUp());

        this.isDragging = false;
        this.lastMouseX = 0;
        this.lastMouseY = 0;

        this.ctx = this.canvas.getContext('2d');
        this.draw();
    }

    calculateDefaultZoom() {
        // print type of rooms.pos
        console.log(`rooms.pos type: ${typeof this.rooms.pos}`);
        const minX = Math.min(...this.rooms.pos.map(pos => pos[0]));
        const maxX = Math.max(...this.rooms.pos.map(pos => pos[0]));
        const minY = Math.min(...this.rooms.pos.map(pos => pos[1]));
        const maxY = Math.max(...this.rooms.pos.map(pos => pos[1]));

        const mapWidth = maxX - minX;
        const mapHeight = maxY - minY;

        this.zoomLevel = Math.min(this.canvas.width / mapWidth, this.canvas.height / mapHeight) * 0.9;
        this.offsetX = -minX * this.zoomLevel + (this.canvas.width - mapWidth * this.zoomLevel) / 2;
        this.offsetY = -minY * this.zoomLevel + (this.canvas.height - mapHeight * this.zoomLevel) / 2;
    }

    resizeCanvas() {
        this.canvas.width = window.innerWidth;
        this.canvas.height = window.innerHeight;
        this.calculateDefaultZoom();
        this.draw();
    }

    mouseWheel(event) {
        const zoomFactor = 1.05;
        const zoom = event.deltaY > 0 ? 1 / zoomFactor : zoomFactor;

        const mouseXWorld = (event.offsetX - this.offsetX) / this.zoomLevel;
        const mouseYWorld = (event.offsetY - this.offsetY) / this.zoomLevel;

        this.zoomLevel *= zoom;
        this.zoomLevel = Math.max(this.zoomLevel, 0.25);
        this.zoomLevel = Math.min(this.zoomLevel, 3);

        this.offsetX = event.offsetX - mouseXWorld * this.zoomLevel;
        this.offsetY = event.offsetY - mouseYWorld * this.zoomLevel;

        this.draw();
    }

    mouseDown(event) {
        this.isDragging = true;
        this.lastMouseX = event.clientX;
        this.lastMouseY = event.clientY;
    }

    mouseDrag(event) {
        if (this.isDragging) {
            const dx = event.clientX - this.lastMouseX;
            const dy = event.clientY - this.lastMouseY;

            this.offsetX += dx;
            this.offsetY += dy;

            this.lastMouseX = event.clientX;
            this.lastMouseY = event.clientY;

            this.draw();
        }
    }

    mouseUp() {
        this.isDragging = false;
    }

    draw() {
        const ctx = this.ctx;
        ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

        ctx.save();
        ctx.translate(this.offsetX, this.offsetY);
        ctx.scale(this.zoomLevel, this.zoomLevel);

        this.drawLinks();
        this.drawRooms();
        this.drawAnts();

        ctx.restore();
    }

    drawRooms() {
        const ctx = this.ctx;
        ctx.fillStyle = 'rgb(200, 200, 200)';
        for (let [name, pos] of Object.entries(this.rooms.rooms)) {
            ctx.beginPath();
            ctx.arc(pos[0], pos[1], 15, 0, 2 * Math.PI);
            ctx.fill();
        }
    }

    drawLinks() {
        const ctx = this.ctx;
        ctx.lineWidth = 5;
        for (let link of this.links.linkList) {
            const pos1 = link.room1;
            const pos2 = link.room2;

            ctx.strokeStyle = `rgb(${link.color.join(',')})`;
            ctx.beginPath();
            ctx.moveTo(pos1[0], pos1[1]);
            ctx.lineTo(pos2[0], pos2[1]);
            ctx.stroke();
        }
    }

    drawAnts() {
        const ctx = this.ctx;
        for (let ant of this.ants.positions) {
            const currentRoom = this.rooms.getPos(ant.currentRoom);
            const targetRoom = this.rooms.getPos(ant.targetRoom);

            if (currentRoom && targetRoom) {
                const x = currentRoom[0] + (targetRoom[0] - currentRoom[0]) * ant.progress;
                const y = currentRoom[1] + (targetRoom[1] - currentRoom[1]) * ant.progress;

                ctx.fillStyle = `rgb(${ant.color.join(',')})`;
                ctx.beginPath();
                ctx.arc(x, y, 10, 0, 2 * Math.PI);
                ctx.fill();
            }
        }
    }

    animateToStep(step_a, step_b, duration = 1000) {
        let progress = 0;
        const startTime = performance.now();
        const stepDirection = Math.sign(step_b - step_a);
        const stepDelta = Math.abs(step_b - step_a);

        const animate = (time) => {
            progress = (time - startTime) / duration;

            if (progress >= 1)
                progress = 1;

            this.updateAntsPosition(step_a, step_a + stepDirection * progress * stepDelta);
            this.draw();

            if (progress < 1)
                requestAnimationFrame(animate);
            else if (step_b === step_a + stepDirection)
                this.updateLinksColor();
        };
        requestAnimationFrame(animate);
    }

    moveToStep(step_id) {
        this.updateAntsPosition(step_id, step_id);
        this.draw();
    }

    updateAntsPosition(step_a, step_b) {
        // Mise à jour des positions des fourmis entre step_a et step_b
        // Implémentation à adapter en fonction de la structure des données de positions de fourmis
    }

    updateLinksColor() {
        // Mise à jour des couleurs des liens traversés par les fourmis
        for (let ant of this.ants) {
            const currentLink = this.links.linkList.find(link =>
                (link.room1 === this.rooms.getPos(ant.currentRoom) && link.room2 === this.rooms.getPos(ant.targetRoom)) ||
                (link.room2 === this.rooms.getPos(ant.currentRoom) && link.room1 === this.rooms.getPos(ant.targetRoom))
            );

            if (currentLink && currentLink.color.join(',') === '255,255,255')
                currentLink.color = ant.color;
        }
    }
}

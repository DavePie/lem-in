export class ErrorPopup {
	constructor() {
		this.popupElement = document.createElement('div');
		this.popupElement.style.position = 'fixed';
		this.popupElement.style.top = '50%';
		this.popupElement.style.left = '50%';
		this.popupElement.style.transform = 'translate(-50%, -50%)';
		this.popupElement.style.backgroundColor = '#1a1a1a';
		this.popupElement.style.border = '2px solid #ff4d4d';
		this.popupElement.style.color = '#ffffff';
		this.popupElement.style.padding = '20px';
		this.popupElement.style.borderRadius = '10px';
		this.popupElement.style.boxShadow = '0 0 20px rgba(255, 77, 77, 0.7)';
		this.popupElement.style.display = 'none';
		this.popupElement.style.zIndex = '1000';
		this.popupElement.style.width = '350px';
		this.popupElement.style.maxWidth = '90%';
		this.popupElement.style.wordWrap = 'break-word';
		this.popupElement.style.textAlign = 'center';
		this.popupElement.style.fontFamily = 'Arial, sans-serif';
		this.popupElement.style.fontSize = '16px';
		this.messageElement = document.createElement('p');
		this.messageElement.style.whiteSpace = 'pre-line';
		this.popupElement.appendChild(this.messageElement);
		this.closeButton = document.createElement('span');
		this.closeButton.innerHTML = '&times;';
		this.closeButton.style.position = 'absolute';
		this.closeButton.style.top = '10px';
		this.closeButton.style.right = '10px';
		this.closeButton.style.cursor = 'pointer';
		this.closeButton.style.fontSize = '24px';
		this.closeButton.style.color = '#ff4d4d';
		this.popupElement.appendChild(this.closeButton);
	
		document.body.appendChild(this.popupElement);
	
		this.closeButton.addEventListener('click', () => {
			this.hide();
		});
	
		this.isVisible = false;
	}
  
	show(message) {
		if (!this.isVisible) {
			this.messageElement.textContent = message;
			this.popupElement.style.display = 'block';
			this.isVisible = true;
		}
	}
  
	hide() {
		this.popupElement.style.display = 'none';
		this.isVisible = false;
	}
}
  
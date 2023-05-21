let isMathSupportLoaded = false;
function onVisibleClick() {
	if (this.id == 'visibled') {
		this.id = '';
		this.classList.remove('up');
		this.classList.add('down');
		this.textContent = '展開';
		this.parentElement?.parentElement?.parentElement?.classList.remove('visibled');
	} else {
		this.id = 'visibled';
		this.classList.add('up');
		this.classList.remove('down');
		this.textContent = '縮小';
		this.parentElement?.parentElement?.parentElement?.classList.add('visibled');
	}
}
function onErrorLoaded() {
	this.setAttribute('pre-deferred-src', this.getAttribute('src'));
	this.removeAttribute('src');
}
function isScrollable(node) {
	let scrollableX = node.scrollWidth > node.clientWidth;
	let scrollableY = node.scrollHeight > node.clientHeight;
	return scrollableX || scrollableY;
}
function inScrollable(node) {
	function isInside(node, parentNode) {
		let rect = node.getBoundingClientRect();
		let parentRect = parentNode.getBoundingClientRect();
		let left = rect.x < parentRect.x + parentRect.width;
		let top = rect.y < parentRect.y + parentRect.height;
		let right = rect.x + rect.width > parentRect.x;
		let bottom = rect.y + rect.height > parentRect.y;
		return (left && right) && (top && bottom);
	}
	let parentNode = node.parentElement;
	while (parentNode != null && !isScrollable(parentNode)) {
		parentNode = parentNode.parentElement;
	}
	return parentNode != null ? isInside(node, parentNode) : inClient(node);
}
requestAnimationFrame(function delegate() {
	requestAnimationFrame(delegate);
	if (document.readyState != 'complete') {
		return;
	}
	/* [ structured-tag ] */
	if (isLoaded == true && isMathSupportLoaded == false) {
		isMathSupportLoaded = true;
		/* notice */ {
			/* structuring for the 'notice's */ {
				insertSurround('notice', 'sub-notice');
				insertSurround('notice > sub-notice', 'notice-content');
			}
		}
		/* post */ {
			/* events for the 'post > sub-post > post-leader > post-leader-advance > button.visibility's */ {
				let postNode = forAll('post[with-collapsed]');
				for (let i = 0; i < postNode.length; i++) {
					let postLeaderAdvanceNode = postNode[i].get('post > sub-post > post-leader > post-leader-advance');
					if (postLeaderAdvanceNode == null) {
						continue;
					}
					let buttonNode = postLeaderAdvanceNode.get('button.advance.visibility');
					if (buttonNode == null) {
						buttonNode = document.createElement('button');
						buttonNode.classList.add('advance');
						buttonNode.classList.add('visibility');
						postLeaderAdvanceNode.append(buttonNode);
					}
					buttonNode.id = '';
					buttonNode.classList.remove('up');
					buttonNode.classList.add('down');
					buttonNode.textContent = '展開';
					buttonNode.parentElement?.parentElement?.parentElement?.classList.remove('visibled');
					buttonNode.addEventListener('click', onVisibleClick);
				}
			}
			/* integrating the 'post-leader-date's by including the '[date-string]'s */ {
				let postNode = forAll('post[date-string]');
				for (let i = 0; i < postNode.length; i++) {
					let postLeaderDateNode = document.createElement('post-leader-date');
					postLeaderDateNode.textContent = postNode[i].getAttribute('date-string');
					let postLeaderSectionNode = postNode[i].get(':scope > sub-post > post-leader > post-leader-section');
					postLeaderSectionNode.insertBefore(postLeaderDateNode, postLeaderSectionNode.get('post-leader-order').nextSibling);
				}
			}
		}
		/* img */ {
			/* '[deferred-src]' for the 'img's */ {
				let imgNode = forAll('img[deferred-src]');
				for (let i = 0; i < imgNode.length; i++) {
					imgNode[i].addEventListener('error', onErrorLoaded);
				}
			}
		}
	}
	/* [ pseudo-style ] */
	{
		/* notice */ {
			let postNode = forAllTag('post');
			/* '.has-notice' for the 'post's */ {
				for (let i = 0; i < postNode.length; i++) {
					if (postNode[i].has(':scope > sub-post > post-content > notice')) {
						postNode[i].classList.add('has-notice');
					} else {
						postNode[i].classList.remove('has-notice');
					}
				}
			}
		}
		/* notice */ {
			let noticeNode = forAllTag('notice');
			/* '.no-space' for the 'notice's */ {
				for (let i = 0; i < noticeNode.length; i++) {
					let noticeContentNode = noticeNode[i].get(':scope > sub-notice > notice-content');
					if (!noticeContentNode.classList.contains('no-space')) {
						noticeContentNode.classList.add('no-space');
					}
				}
			}
		}
		/* img */ {
			/* '[deferred-src]' for the 'img's */ {
				let imgNode = forAll('img[deferred-src]');
				for (let i = 0; i < imgNode.length; i++) {
					if (inScrollable(imgNode[i])) {
						imgNode[i].setAttribute('src', imgNode[i].getAttribute('deferred-src'));
						imgNode[i].removeAttribute('deferred-src');
					}
				}
			}
			/* '[pre-deferred-src]' for the 'img's */ {
				let imgNode = forAll('img[pre-deferred-src]');
				for (let i = 0; i < imgNode.length; i++) {
					if (!inScrollable(imgNode[i])) {
						imgNode[i].setAttribute('deferred-src', imgNode[i].getAttribute('pre-deferred-src'));
						imgNode[i].removeAttribute('pre-deferred-src');
					}
				}
			}
		}
	}
});
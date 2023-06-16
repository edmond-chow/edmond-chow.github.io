let isMathSupportLoaded = false;
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
	while (parentNode != document.body && !isScrollable(parentNode)) {
		parentNode = parentNode.parentElement;
	}
	return parentNode != document.body ? isInside(node, parentNode) : inClient(node);
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
					if (!postNode[i].has(':scope > sub-post > post-leader > post-leader-advance')) {
						continue;
					}
					let postLeaderAdvanceNode = postNode[i].get(':scope > sub-post > post-leader > post-leader-advance');
					let buttonNode = function() {
						if (postLeaderAdvanceNode.has(':scope > button.advance.visibility')) {
							return postLeaderAdvanceNode.get(':scope > button.advance.visibility');
						} else {
							let buttonNode = document.createElement('button');
							buttonNode.classList.add('advance');
							buttonNode.classList.add('visibility');
							postLeaderAdvanceNode.append(buttonNode);
							return buttonNode;
						}
					}();
					buttonNode.id = '';
					buttonNode.classList.remove('up');
					buttonNode.classList.add('down');
					buttonNode.textContent = '展開';
					let capturedPostNode = postNode[i];
					function onVisibleClick() {
						if (!capturedPostNode.has(':scope > sub-post > post-leader > post-leader-advance') || !capturedPostNode.has(':scope > sub-post > post-content')) {
							return;
						}
						let postLeaderAdvanceNode = capturedPostNode.get(':scope > sub-post > post-leader > post-leader-advance');
						let postContentNode = capturedPostNode.get(':scope > sub-post > post-content');
						if (buttonNode.parentElement != postLeaderAdvanceNode) {
							return;
						}
						if (buttonNode.id == 'visibled') {
							buttonNode.id = '';
							buttonNode.classList.remove('up');
							buttonNode.classList.add('down');
							buttonNode.textContent = '展開';
							postContentNode.style.aspectRatio = '';
						} else {
							buttonNode.id = 'visibled';
							buttonNode.classList.add('up');
							buttonNode.classList.remove('down');
							buttonNode.textContent = '縮小';
							postContentNode.style.aspectRatio = postContentNode.offsetWidth.toString() + ' / ' + postContentNode.scrollHeight.toString();
						}
					}
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
					function onErrorLoaded() {
						imgNode[i].setAttribute('pre-deferred-src', imgNode[i].getAttribute('src'));
						imgNode[i].removeAttribute('src');
					}
					imgNode[i].addEventListener('error', onErrorLoaded);
				}
			}
		}
		/* iframe */ {
			/* '[deferred-src]' for the 'iframe's */ {
				let iframeNode = forAll('iframe[deferred-src]');
				for (let i = 0; i < iframeNode.length; i++) {
					iframeNode[i].request = new XMLHttpRequest();
					function onIframeErrorLoaded() {
						if (iframeNode[i].request.status >= 400 && iframeNode[i].request.status <= 599) {
							iframeNode[i].setAttribute('pre-deferred-src', '');
						}
					}
					iframeNode[i].request.addEventListener('load', onIframeErrorLoaded);
				}
			}
		}
	}
	/* [ pseudo-style ] */
	{
		/* notice */ {
			let postNode = forAllTag('post');
			/* '[with-notice]' for the 'post's */ {
				for (let i = 0; i < postNode.length; i++) {
					if (postNode[i].has(':scope > sub-post > post-content > notice') && !postNode[i].has(':scope > sub-post > post-content > post') && !postNode[i].hasAttribute('with-collapsed')) {
						postNode[i].setAttribute('with-notice', '');
					} else {
						postNode[i].removeAttribute('with-notice');
					}
				}
			}
		}
		/* notice */ {
			let noticeNode = forAllTag('notice');
			/* '.no-space' for the 'notice's */ {
				for (let i = 0; i < noticeNode.length; i++) {
					if (noticeNode[i].has(':scope > sub-notice > notice-content')) {
						let noticeContentNode = noticeNode[i].get(':scope > sub-notice > notice-content');
						if (!noticeContentNode.classList.contains('no-space')) {
							noticeContentNode.classList.add('no-space');
						}
					}
				}
			}
		}
		/* inline-frame */ {
			let postNode = forAllTag('post');
			/* '[with-inline-frame]' for the 'post's */ {
				for (let i = 0; i < postNode.length; i++) {
					if (postNode[i].has(':scope > sub-post > post-content > iframe:first-of-type:last-child') && !postNode[i].has(':scope > sub-post > post-content > post') && !postNode[i].hasAttribute('with-collapsed')) {
						postNode[i].setAttribute('with-inline-frame', '');
					} else {
						postNode[i].removeAttribute('with-inline-frame');
					}
				}
			}
		}
		/* graphics */ {
			let postNode = forAllTag('post');
			/* '[with-graphics]' for the 'post's */ {
				for (let i = 0; i < postNode.length; i++) {
					if (postNode[i].has(':scope > sub-post > post-content > img:first-child:last-of-type') && !postNode[i].has(':scope > sub-post > post-content > post') && !postNode[i].hasAttribute('with-collapsed')) {
						postNode[i].setAttribute('with-graphics', '');
					} else {
						postNode[i].removeAttribute('with-graphics');
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
		/* iframe */ {
			/* '[deferred-src]' for the 'iframe's */ {
				let iframeNode = forAll('iframe[deferred-src]');
				for (let i = 0; i < iframeNode.length; i++) {
					if (inScrollable(iframeNode[i])) {
						iframeNode[i].request.open('GET', iframeNode[i].getAttribute('deferred-src'), true);
						iframeNode[i].request.send();
						iframeNode[i].setAttribute('src', iframeNode[i].getAttribute('deferred-src'));
						iframeNode[i].removeAttribute('deferred-src');
					}
				}
			}
			/* '[pre-deferred-src]' for the 'iframe's */ {
				let iframeNode = forAll('iframe[pre-deferred-src]');
				for (let i = 0; i < iframeNode.length; i++) {
					if (!inScrollable(iframeNode[i])) {
						iframeNode[i].setAttribute('deferred-src', iframeNode[i].getAttribute('src'));
						iframeNode[i].removeAttribute('pre-deferred-src');
						iframeNode[i].removeAttribute('src');
					}
				}
			}
		}
	}
});
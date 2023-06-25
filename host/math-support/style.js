let isMathSupportLoaded = false;
function switchBlurredState() {
	if (document.body.id == 'blur') {
		document.body.id = '';
	} else {
		document.body.id = 'blur';
	}
}
function hasPartialSubstance(node) {
	if (node.nodeName == '#comment') {
		return false;
	} else if (node.nodeName == '#text' && removeSpace(node.wholeText) == '') {
		return false;
	} else if (node instanceof Element && node.nodeName == 'br'.toUpperCase()) {
		return false;
	} else if (node instanceof Element && window.getComputedStyle(node).display == 'none') {
		return false;
	}
	return true;
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
	while (parentNode != document.body && !isScrollable(parentNode)) {
		parentNode = parentNode.parentElement;
	}
	return parentNode != document.body ? isInside(node, parentNode) : inClient(node);
}
function inView(node) {
	return inScrollable(node) && inClient(node);
}
requestAnimationFrame(function delegate() {
	requestAnimationFrame(delegate);
	if (document.readyState != 'complete') {
		return;
	}
	/* [ structured-tag ] */
	if (isLoaded == true && isMathSupportLoaded == false) {
		isMathSupportLoaded = true;
		/* post */ {
			/* events for the 'post > sub-post > post-leader > post-leader-advance > button.visibility's */ {
				/* onVisibleClick */
				let postNode = forAll('post[with-collapsed]');
				for (let i = 0; i < postNode.length; i++) {
					if (!postNode[i].has(':scope > sub-post > post-leader > post-leader-advance')) {
						continue;
					}
					let postLeaderAdvanceNode = postNode[i].get(':scope > sub-post > post-leader > post-leader-advance');
					let buttonNode = function () {
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
					buttonNode.lastOnVisibleClickRelease?.call();
					function onVisibleClick() {
						if (!capturedPostNode.has(':scope > sub-post > post-leader > post-leader-advance') || !capturedPostNode.has(':scope > sub-post > post-content')) {
							return;
						}
						let postLeaderAdvanceNode = capturedPostNode.get(':scope > sub-post > post-leader > post-leader-advance');
						let postContentNode = capturedPostNode.get(':scope > sub-post > post-content');
						if (buttonNode.parentElement != postLeaderAdvanceNode) {
							return;
						}
						function freezer(action) {
							let childNode = postContentNode.children;
							for (let i = 0; i < childNode.length; i++) {
								if (childNode[i].tagName.toUpperCase() == 'img'.toUpperCase() || childNode[i].tagName.toUpperCase() == 'iframe'.toUpperCase()) {
									action(childNode[i]);
								}
							}
						}
						freezer(function (node) {
							node.setAttribute('frozen', '');
						});
						clearTimeout(buttonNode.lastReleased);
						buttonNode.lastReleased = setTimeout(function asyncRelease() {
							freezer(function (node) {
								node.removeAttribute('frozen');
							});
							buttonNode.classList.remove('disabled');
						}, 1000);
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
						buttonNode.classList.add('disabled');
					}
					buttonNode.lastOnVisibleClickRelease = function () {
						buttonNode.removeEventListener('click', onVisibleClick);
						buttonNode.lastOnVisibleClickRelease = null;
					}
					buttonNode.addEventListener('click', onVisibleClick);
				}
				/* onResized */
				window.lastOnResizedRelease?.call();
				function onResized() {
					let postNode = forAll('post[with-collapsed]');
					for (let i = 0; i < postNode.length; i++) {
						if (!postNode[i].has(':scope > sub-post > post-leader > post-leader-advance') || !postNode[i].has(':scope > sub-post > post-content')) {
							continue;
						}
						let postLeaderAdvanceNode = postNode[i].get(':scope > sub-post > post-leader > post-leader-advance');
						let postContentNode = postNode[i].get(':scope > sub-post > post-content');
						if (!postLeaderAdvanceNode.has(':scope > button.advance.visibility')) {
							continue;
						}
						let buttonNode = postLeaderAdvanceNode.get(':scope > button.advance.visibility');
						if (buttonNode.id == 'visibled') {
							postContentNode.style.aspectRatio = 'auto';
						}
						postContentNode.classList.add('resized');
						buttonNode.classList.add('pseudo-disabled');
					}
					clearTimeout(window.lastResized);
					window.lastResized = setTimeout(function asyncRelease() {
						let postNode = forAll('post[with-collapsed]');
						for (let i = 0; i < postNode.length; i++) {
							if (!postNode[i].has(':scope > sub-post > post-leader > post-leader-advance') || !postNode[i].has(':scope > sub-post > post-content')) {
								continue;
							}
							let postLeaderAdvanceNode = postNode[i].get(':scope > sub-post > post-leader > post-leader-advance');
							let postContentNode = postNode[i].get(':scope > sub-post > post-content');
							if (!postLeaderAdvanceNode.has(':scope > button.advance.visibility')) {
								continue;
							}
							let buttonNode = postLeaderAdvanceNode.get(':scope > button.advance.visibility');
							if (buttonNode.id == 'visibled') {
								postContentNode.style.aspectRatio = postContentNode.offsetWidth.toString() + ' / ' + postContentNode.scrollHeight.toString();
							}
							postContentNode.classList.remove('resized');
							buttonNode.classList.remove('pseudo-disabled');
						}
					}, 250);
				}
				window.lastOnResizedRelease = function () {
					window.removeEventListener('resize', onResized);
					window.lastOnResizedRelease = null;
				}
				window.addEventListener('resize', onResized);
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
		/* post[with-notice] */ {
			/* '[with-notice]' for the 'post's */ {
				insertSurround('post > sub-post > post-content > notice', 'sub-notice');
				insertSurround('post > sub-post > post-content > notice > sub-notice', 'notice-content');
			}
		}
	}
	/* [ pseudo-style ] */
	{
		/* post[with-graphics, with-notice, with-inline-frame] */ {
			let postNode = forAllTag('post');
			/* '[with-graphics, with-notice, with-inline-frame]' for the 'post's */ {
				for (let i = 0; i < postNode.length; i++) {
					function operate(node, attribute, selector, bind) {
						function is(node, condition) {
							if (node.hasAttribute('with-collapsed') || node.has(':scope > sub-post > post-content > post')) {
								return false;
							}
							return condition(node);
						}
						function hasCondition(selector) {
							let condition = function(node) {
								return node.has(selector);
							}
							return condition;
						}
						function visibilityChecking(node, selector) {
							function previousCondition(selector) {
								let condition = function(node) {
									let previousNode = node.get(selector).previousSibling;
									while (previousNode != null) {
										if (hasPartialSubstance(previousNode)) {
											return false;
										}
										previousNode = previousNode.previousSibling;
									}
									return true;
								}
								return condition;
							}
							if (is(node, previousCondition(selector))) {
								node.get(selector).classList.add('first-visible-child');
							} else {
								node.get(selector).classList.remove('first-visible-child');
							}
							function nextCondition(selector) {
								let condition = function(node) {
									let nextNode = node.get(selector).nextSibling;
									while (nextNode != null) {
										if (hasPartialSubstance(nextNode)) {
											return false;
										}
										nextNode = nextNode.nextSibling;
									}
									return true;
								}
								return condition;
							}
							if (is(node, nextCondition(selector))) {
								node.get(selector).classList.add('last-visible-child');
							} else {
								node.get(selector).classList.remove('last-visible-child');
							}
						}
						if (is(node, hasCondition(selector))) {
							node.setAttribute(attribute, '');
							visibilityChecking(node, selector);
							bind?.call(null, node);
						} else {
							node.removeAttribute(attribute);
						}
					}
					operate(postNode[i], 'with-graphics', ':scope > sub-post > post-content > img:first-of-type:last-of-type');
					operate(postNode[i], 'with-notice', ':scope > sub-post > post-content > notice', function(node) {
						let noticeNode = node.get(':scope > sub-post > post-content > notice');
						if (noticeNode.has(':scope > sub-notice > notice-content')) {
							let noticeContentNode = noticeNode.get(':scope > sub-notice > notice-content');
							noticeContentNode.classList.add('no-space');
						}
					});
					operate(postNode[i], 'with-inline-frame', ':scope > sub-post > post-content > iframe:first-of-type:last-of-type');
				}
			}
		}
		/* img */ {
			/* '[deferred-src]' for the 'img's */ {
				let imgNode = forAll('img[deferred-src]:not([frozen])');
				for (let i = 0; i < imgNode.length; i++) {
					if (inView(imgNode[i])) {
						imgNode[i].setAttribute('src', imgNode[i].getAttribute('deferred-src'));
						imgNode[i].removeAttribute('deferred-src');
						let capturedImgNode = imgNode[i];
						function onError() {
							capturedImgNode.setAttribute('pre-deferred-src', capturedImgNode.getAttribute('src'));
							capturedImgNode.removeAttribute('src');
							capturedImgNode.removeEventListener('load', onLoad);
							capturedImgNode.removeEventListener('error', onError);
						}
						function onLoad() {
							capturedImgNode.removeEventListener('load', onLoad);
							capturedImgNode.removeEventListener('error', onError);
						}
						imgNode[i].addEventListener('error', onError);
						imgNode[i].addEventListener('load', onLoad);
					}
				}
			}
			/* '[pre-deferred-src]' for the 'img's */ {
				let imgNode = forAll('img[pre-deferred-src]:not([frozen])');
				for (let i = 0; i < imgNode.length; i++) {
					if (!inView(imgNode[i])) {
						imgNode[i].setAttribute('deferred-src', imgNode[i].getAttribute('pre-deferred-src'));
						imgNode[i].removeAttribute('pre-deferred-src');
					}
				}
			}
		}
		/* iframe */ {
			/* '[deferred-src]' for the 'iframe's */ {
				let iframeNode = forAll('iframe[deferred-src]:not([frozen])');
				for (let i = 0; i < iframeNode.length; i++) {
					if (inView(iframeNode[i])) {
						iframeNode[i].request = new XMLHttpRequest();
						let capturedIframeNode = iframeNode[i];
						iframeNode[i].request.addEventListener('load', function onLoad() {
							if (capturedIframeNode.request.status >= 400 && capturedIframeNode.request.status <= 599) {
								capturedIframeNode.setAttribute('referred', '');
							}
							capturedIframeNode.request.removeEventListener('load', onLoad);
						});
						iframeNode[i].request.open('GET', iframeNode[i].getAttribute('deferred-src'), true);
						iframeNode[i].request.send();
						iframeNode[i].setAttribute('src', iframeNode[i].getAttribute('deferred-src'));
						iframeNode[i].removeAttribute('deferred-src');
					}
				}
			}
			/* '[referred]' for the 'iframe's */ {
				let iframeNode = forAll('iframe[referred]:not([frozen])');
				for (let i = 0; i < iframeNode.length; i++) {
					if (!inView(iframeNode[i])) {
						iframeNode[i].setAttribute('deferred-src', iframeNode[i].getAttribute('src'));
						iframeNode[i].removeAttribute('referred');
						iframeNode[i].removeAttribute('src');
					}
				}
			}
		}
	}
});
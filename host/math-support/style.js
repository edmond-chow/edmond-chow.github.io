[
	function setCookie(property, value) {
		arguments.constrainedWithAndThrow(String, String);
		document.cookie = property + '=' + value;
	},
	function getCookie(property) {
		arguments.constrainedWithAndThrow(String);
		let cookies = document.cookie.split('; ');
		for (let i = 0; i < cookies.length; i++) {
			if (cookies[i].substring(0, property.length) == property) {
				return cookies[i].substring(property.length + 1, cookies[i].length);
			}
		}
		return '';
	},
	function switchBlurredState() {
		arguments.constrainedWithAndThrow();
		if (document.body.id == 'blur') {
			document.body.id = '';
			setCookie('non-blur', 'true');
		} else {
			document.body.id = 'blur';
			setCookie('non-blur', 'false');
		}
	},
	function isInstance(node) {
		arguments.constrainedWithAndThrow(Node);
		if (node.nodeName == '#comment') {
			return false;
		} else if (node.nodeName == '#text' && node.wholeText.removeSpace() == '') {
			return false;
		} else if (node instanceof Element && node.nodeName == 'br'.toUpperCase()) {
			return false;
		} else if (node instanceof Element && window.getComputedStyle(node).display == 'none') {
			return false;
		}
		return true;
	}
].bindTo(window);
document.addEventListener('structuredTag', async function structuredTag() {
	captureSpan();
	/* switchBlurredState() */ {
		if (getCookie('non-blur') == 'true') {
			switchBlurredState();
		}
	}
	await mustSuspend();
	/* post[date-string, with-collapsed, with-notice] */ {
		/* integrating the 'post-leader-date's by including the '[date-string]'s */ {
			let postNode = forAll('post[date-string]');
			for (let i = 0; i < postNode.length; i++) {
				let postLeaderDateNode = document.createElement('post-leader-date');
				postLeaderDateNode.textContent = postNode[i].getAttribute('date-string');
				let postLeaderSectionNode = postNode[i].get(':scope > sub-post > post-leader > post-leader-section');
				postLeaderSectionNode.insertBefore(postLeaderDateNode, postLeaderSectionNode.get('post-leader-order').nextSibling);
			}
		}
		await suspend();
		/* events for the 'post > sub-post > post-leader > post-leader-advance > button.visibility's */ {
			/* onVisibleClick */
			let postNode = forAll('post[with-collapsed]');
			let onVisibleClickDelegate = [];
			for (let i = 0; i < postNode.length; i++) {
				if (!postNode[i].has(':scope > sub-post > post-leader > post-leader-advance')) {
					continue;
				}
				let postLeaderAdvanceNode = postNode[i].get(':scope > sub-post > post-leader > post-leader-advance');
				let buttonNode = function getButtonNode() {
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
				onVisibleClickDelegate.push(onVisibleClick);
				buttonNode.addEventListener('click', onVisibleClick);
			}
			document.addEventListener('structuredTag', function lastOnVisibleClickRelease() {
				for (let i = 0; i < onVisibleClickDelegate.length; i++) {
					buttonNode.removeEventListener('click', onVisibleClickDelegate[i]);
				}
				document.removeEventListener('structuredTag', lastOnVisibleClickRelease);
			});
			/* onResized */
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
			document.addEventListener('structuredTag', function lastOnResizedRelease() {
				window.removeEventListener('resize', onResized);
				document.removeEventListener('structuredTag', lastOnResizedRelease);
			});
			window.addEventListener('resize', onResized);
		}
		await suspend();
		/* '[with-notice]' for the 'post's */ {
			insertSurround('post > sub-post > post-content > notice', 'sub-notice');
			insertSurround('post > sub-post > post-content > notice > sub-notice', 'notice-content');
		}
	}
});
document.addEventListener('formedStyle', async function formedStyle() {
	captureSpan();
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
									if (isInstance(previousNode)) {
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
									if (isInstance(nextNode)) {
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
				await suspend();
				operate(postNode[i], 'with-notice', ':scope > sub-post > post-content > notice', function(node) {
					let noticeNode = node.get(':scope > sub-post > post-content > notice');
					if (noticeNode.has(':scope > sub-notice > notice-content')) {
						let noticeContentNode = noticeNode.get(':scope > sub-notice > notice-content');
						noticeContentNode.classList.add('no-space');
					}
				});
				await suspend();
				operate(postNode[i], 'with-inline-frame', ':scope > sub-post > post-content > iframe:first-of-type:last-of-type');
			}
		}
	}
	await mustSuspend();
	/* img */ {
		/* '[deferred-src]' for the 'img's */ {
			let imgNode = forAll('img[deferred-src]:not([frozen])');
			for (let i = 0; i < imgNode.length; i++) {
				if (inScrollable(imgNode[i])) {
					imgNode[i].setAttribute('src', imgNode[i].getAttribute('deferred-src'));
					imgNode[i].removeAttribute('deferred-src');
					let capturedImgNode = imgNode[i];
					function onError() {
						capturedImgNode.setAttribute('pre-deferred-src', capturedImgNode.getAttribute('src'));
						capturedImgNode.removeAttribute('src');
						capturedImgNode.removeEventListener('error', onError);
						capturedImgNode.removeEventListener('load', onLoad);
					}
					function onLoad() {
						capturedImgNode.removeEventListener('error', onError);
						capturedImgNode.removeEventListener('load', onLoad);
					}
					imgNode[i].addEventListener('error', onError);
					imgNode[i].addEventListener('load', onLoad);
				}
			}
		}
		await suspend();
		/* '[pre-deferred-src]' for the 'img's */ {
			let imgNode = forAll('img[pre-deferred-src]:not([frozen])');
			for (let i = 0; i < imgNode.length; i++) {
				if (!inScrollable(imgNode[i])) {
					imgNode[i].setAttribute('deferred-src', imgNode[i].getAttribute('pre-deferred-src'));
					imgNode[i].removeAttribute('pre-deferred-src');
				}
			}
		}
	}
	await mustSuspend();
	/* iframe */ {
		/* '[deferred-src]' for the 'iframe's */ {
			let iframeNode = forAll('iframe[deferred-src]:not([frozen])');
			for (let i = 0; i < iframeNode.length; i++) {
				if (inScrollable(iframeNode[i])) {
					let url = new URL(iframeNode[i].getAttribute('deferred-src'), document.baseURI);
					if (document.location.origin == url.origin) {
						let request = new XMLHttpRequest();
						let capturedIframeNode = iframeNode[i];
						request.addEventListener('load', function onLoad() {
							if (request.status >= 400 && request.status <= 599) {
								capturedIframeNode.setAttribute('referred', '');
							}
							request.removeEventListener('load', onLoad);
						});
						request.open('GET', url.href, true);
						request.send();
					}
					iframeNode[i].setAttribute('src', iframeNode[i].getAttribute('deferred-src'));
					iframeNode[i].removeAttribute('deferred-src');
				}
			}
		}
		await suspend();
		/* '[referred]' for the 'iframe's */ {
			let iframeNode = forAll('iframe[referred]:not([frozen])');
			for (let i = 0; i < iframeNode.length; i++) {
				if (!inScrollable(iframeNode[i])) {
					iframeNode[i].setAttribute('deferred-src', iframeNode[i].getAttribute('src'));
					iframeNode[i].removeAttribute('referred');
					iframeNode[i].removeAttribute('src');
				}
			}
		}
	}
});
[
	function hasTitle(node) {
		return node.hasAttribute('title');
	},
	function makeTitle(node) {
		node.setAttribute('title', '{Name}');
	},
	function hasAlt(node) {
		return node.hasAttribute('alt');
	},
	function makeAlt(node) {
		node.setAttribute('alt', '{Name}');
	},
	function hasAriaLabel(node) {
		return node.hasAttribute('aria-label') && node.getAttribute('aria-label') != '';
	},
	function makeAriaLabel(node) {
		if (hasTextOnly(node) && value.innerText.removeSpace() != '') {
			node.setAttribute('aria-label', node.innerText);
		} else {
			node.setAttribute('aria-label', '{Name}');
		}
	},
	function hasAriaLabelBy(node) {
		if (!node.hasAttribute('aria-label-by')) {
			return false;
		}
		let referenceNode = document.getElementById(value.getAttribute('aria-label-by'));
		if (referenceNode == null) {
			return false;
		} else if (!hasTextOnly(referenceNode)) {
			return false;
		} else if (referenceNode.innerText.removeSpace() == '') {
			return false;
		}
		return true;
	}
].bindTo(window);
[
	function setCookie(property, value) {
		arguments.constrainedWithAndThrow(String, String);
		document.cookie = property + '=' + value;
	},
	function getCookie(property) {
		arguments.constrainedWithAndThrow(String);
		let cookie = '';
		document.cookie.split('; ').every((value) => {
			if (value.substring(0, property.length) == property) {
				cookie = value.substring(property.length + 1, value.length);
				return false;
			}
			return true;
		});
		return cookie;
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
/* { async } */ {
	function deffer(timeout) {
		return new Promise(function executor(resolve) {
			setTimeout(function handler() {
				resolve();
			}, timeout);
		});
	}
}
/* { hidden } */ {
	let resizedCount = 0;
	document.addEventListener('structuredTag', async function structuredTag() {
		captureSpan();
		/* switchBlurredState() */
		if (getCookie('non-blur') == 'true') {
			switchBlurredState();
		}
		await mustSuspend();
		/* integrating the 'post-leader-date's by including the '[date-string]'s */
		forAll('post[date-string]').forEach((value) => {
			let postLeaderDateNode = document.createElement('post-leader-date');
			postLeaderDateNode.textContent = value.getAttribute('date-string');
			let postLeaderSectionNode = value.get(':scope > sub-post > post-leader > post-leader-section');
			postLeaderSectionNode.insertBefore(postLeaderDateNode, postLeaderSectionNode.get('post-leader-order').nextSibling);
		});
		await suspend();
		/* 'onVisibleClick' events for the 'post > sub-post > post-leader > post-leader-advance > button.visibility's */
		let onVisibleClickEvent = [];
		forAll('post[with-collapsed]').map((value) => {
			return new Post(value);
		}).filter((value) => {
			return value.complete;
		}).forEach((postValue) => {
			let buttonNode = (() => {
				if (postValue.postLeaderAdvanceNode.has(':scope > button.advance.visibility')) {
					return postValue.postLeaderAdvanceNode.get(':scope > button.advance.visibility');
				} else {
					let buttonNode = document.createElement('button');
					buttonNode.classList.add('advance');
					buttonNode.classList.add('visibility');
					postValue.postLeaderAdvanceNode.append(buttonNode);
					return buttonNode;
				}
			})();
			buttonNode.id = '';
			buttonNode.classList.remove('up');
			buttonNode.classList.add('down');
			buttonNode.textContent = '展開';
			async function onVisibleClick() {
				if (!new Post(postValue.postNode).complete || buttonNode.parentElement != postValue.postLeaderAdvanceNode) {
					return;
				}
				function freezer(action) {
					Array.from(postValue.postContentNode.children).forEach((value) => {
						if (value.tagName.toUpperCase() == 'img'.toUpperCase() || value.tagName.toUpperCase() == 'iframe'.toUpperCase()) {
							action(value);
						}
					});
				}
				freezer((node) => {
					node.setAttribute('frozen', '');
				});
				if (buttonNode.id == 'visibled') {
					buttonNode.id = '';
					buttonNode.classList.remove('up');
					buttonNode.classList.add('down');
					buttonNode.textContent = '展開';
					postValue.postContentNode.style.aspectRatio = '';
					postValue.postContentNode.classList.remove('no-scrollbar');
				} else {
					buttonNode.id = 'visibled';
					buttonNode.classList.add('up');
					buttonNode.classList.remove('down');
					buttonNode.textContent = '縮小';
					postValue.postContentNode.style.aspectRatio = postValue.postContentNode.offsetWidth.toString() + ' / ' + postValue.postContentNode.scrollHeight.toString();
					postValue.postContentNode.classList.add('no-scrollbar');
				}
				buttonNode.classList.add('disabled');
				await deffer(1000);
				freezer((node) => {
					node.removeAttribute('frozen');
				});
				buttonNode.classList.remove('disabled');
			}
			onVisibleClickEvent.push({ node: buttonNode, delegate: onVisibleClick });
			buttonNode.addEventListener('click', onVisibleClick);
		});
		document.addEventListener('structuredTag', function lastOnVisibleClickRelease() {
			onVisibleClickEvent.forEach((value) => {
				value.node.removeEventListener('click', value.delegate);
			});
			document.removeEventListener('structuredTag', lastOnVisibleClickRelease);
		});
		/* an 'onResized' event for the 'post > sub-post > post-leader > post-leader-advance > button.visibility's */
		async function onResized() {
			resizedCount += 1;
			function Perform(action) {
				forAll('post[with-collapsed]').map((value) => {
					return new Post(value);
				}).filter((value) => {
					return value.complete;
				}).forEach((postValue) => {
					if (postValue.postLeaderAdvanceNode.has(':scope > button.advance.visibility')) {
						let buttonNode = postValue.postLeaderAdvanceNode.get(':scope > button.advance.visibility');
						action(buttonNode, postValue.postContentNode);
					}
				});
			}
			Perform((buttonNode, postContentNode) => {
				if (buttonNode.id == 'visibled') {
					postContentNode.style.aspectRatio = 'auto';
				}
				postContentNode.classList.add('resized');
				buttonNode.classList.add('pseudo-disabled');
			});
			await deffer(250);
			resizedCount -= 1;
			if (resizedCount > 0) {
				return;
			}
			Perform((buttonNode, postContentNode) => {
				if (buttonNode.id == 'visibled') {
					postContentNode.style.aspectRatio = postContentNode.offsetWidth.toString() + ' / ' + postContentNode.scrollHeight.toString();
				}
				postContentNode.classList.remove('resized');
				buttonNode.classList.remove('pseudo-disabled');
			});
		}
		document.addEventListener('structuredTag', function lastOnResizedRelease() {
			window.removeEventListener('resize', onResized);
			document.removeEventListener('structuredTag', lastOnResizedRelease);
		});
		window.addEventListener('resize', onResized);
		await suspend();
		/* '[with-notice]' for the 'post's */
		insertSurround('post > sub-post > post-content > notice', 'sub-notice');
		insertSurround('post > sub-post > post-content > notice > sub-notice', 'notice-content');
		await suspend();
		/* '[alt]' for the 'img's */
		forAllTag('img').forEach((value) => {
			if (!hasAlt(value) && !hasAriaLabel(value) && !hasAriaLabelBy(value)) {
				makeAlt(value);
			}
		});
		await suspend();
		/* '[title]' for the 'iframe's */
		forAllTag('iframe').forEach((value) => {
			if (!hasTitle(value)) {
				makeTitle(value);
			}
		});
		await suspend();
		/* '[aria-label]' for the 'button, [role="button"]'s */
		forAll('button, [role="button"]').forEach((value) => {
			if (!hasTitle(value) && !hasAriaLabel(value) && !hasAriaLabelBy(value)) {
				makeAriaLabel(value);
			}
			if (!value.hasAttribute('type')) {
				value.setAttribute('type', 'button');
			}
		});
		await suspend();
		/* '[aria-label]' for the 'a, [role="link"]'s */
		forAll('a, [role="link"]').forEach((value) => {
			if (value.hasAttribute('href') && !hasAriaLabel(value)) {
				makeAriaLabel(value);
			}
		});
	});
	document.addEventListener('formedStyle', async function formedStyle() {
		captureSpan();
		/* '[with-graphics, with-notice, with-inline-frame]' for the 'post's */
		forAllTag('post').map((value) => {
			return new Post(value);
		}).filter((value) => {
			return value.complete;
		}).forEach(async(postValue) => {
			function operate(node, attribute, selector, bind) {
				function is(node, condition) {
					if (node.hasAttribute('as-is') || node.hasAttribute('with-collapsed') || node.has(':scope > sub-post > post-content > post')) {
						return false;
					}
					return condition(node);
				}
				function hasCondition(selector) {
					let condition = function (node) {
						return node.has(selector);
					}
					return condition;
				}
				function visibilityChecking(node, selector) {
					function previousCondition(selector) {
						let condition = function (node) {
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
						let condition = function (node) {
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
			operate(postValue.postNode, 'with-graphics', ':scope > sub-post > post-content > img:first-of-type:last-of-type');
			await suspend();
			operate(postValue.postNode, 'with-notice', ':scope > sub-post > post-content > notice', function (node) {
				let noticeNode = node.get(':scope > sub-post > post-content > notice');
				if (noticeNode.has(':scope > sub-notice > notice-content')) {
					let noticeContentNode = noticeNode.get(':scope > sub-notice > notice-content');
					noticeContentNode.classList.add('no-space');
				}
			});
			await suspend();
			operate(postValue.postNode, 'with-inline-frame', ':scope > sub-post > post-content > iframe:first-of-type:last-of-type');
		});		
		await suspend();
		/* '[deferred-src]' for the 'img's */
		forAll('img[deferred-src]:not([frozen])').forEach((value) => {
			if (inScrollable(value)) {
				let url = new URL(value.getAttribute('deferred-src'), document.baseURI);
				value.setAttribute('alt', url.href.substring(url.href.lastIndexOf('/') + 1));
				value.setAttribute('src', value.getAttribute('deferred-src'));
				value.removeAttribute('deferred-src');
				let capturedImgNode = value;
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
				value.addEventListener('error', onError);
				value.addEventListener('load', onLoad);
			}
		});
		await suspend();
		/* '[pre-deferred-src]' for the 'img's */
		forAll('img[pre-deferred-src]:not([frozen])').forEach((value) => {
			if (!inScrollable(value)) {
				value.setAttribute('deferred-src', value.getAttribute('pre-deferred-src'));
				value.removeAttribute('pre-deferred-src');
			}
		});
		await suspend();
		/* '[deferred-src]' for the 'iframe's */
		forAll('iframe[deferred-src]:not([frozen])').forEach((value) => {
			if (inScrollable(value)) {
				let url = new URL(value.getAttribute('deferred-src'), document.baseURI);
				if (document.location.origin == url.origin) {
					let request = new XMLHttpRequest();
					let capturedIframeNode = value;
					request.addEventListener('load', function onLoad() {
						if (request.status >= 400 && request.status <= 599) {
							capturedIframeNode.setAttribute('referred', '');
						}
						request.removeEventListener('load', onLoad);
					});
					request.open('GET', url.href, true);
					request.send();
				}
				value.setAttribute('src', value.getAttribute('deferred-src'));
				value.removeAttribute('deferred-src');
			}
		});
		await suspend();
		/* '[referred]' for the 'iframe's */
		forAll('iframe[referred]:not([frozen])').forEach((value) => {
			if (!inScrollable(value)) {
				value.setAttribute('deferred-src', value.getAttribute('src'));
				value.removeAttribute('referred');
				value.removeAttribute('src');
			}
		});
	});
}
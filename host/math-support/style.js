(() => {
	/* { accessibility } */
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
			if (hasTextOnly(node) && node.innerText.removeSpace() != '') {
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
	/* { functionality } */
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
	/* { binder } */
	let resizedCount = 0;
	let rescrollState = 0;
	let deferredImgNode = [];
	let lastHash = '';
	document.addEventListener('structuredTag', async function structuredTag() {
		captureSpan();
		/* switchBlurredState() */
		if (getCookie('non-blur') == 'true') {
			switchBlurredState();
		}
		await mustSuspend();
		/* integrating the 'post-leader-date's by including the '[date-string]'s */
		forAllTag('post').map((value) => {
			return new Post(value);
		}).forEach((postValue) => {
			if (postValue.postNode.hasAttribute('date-string')) {
				let postLeaderDateNode = document.createElement('post-leader-date');
				postLeaderDateNode.textContent = postValue.postNode.getAttribute('date-string');
				postValue.postLeaderSectionNode.insertBefore(postLeaderDateNode, postValue.postLeaderOrderNode.nextSibling);
			} else {
				postValue.postLeaderSectionNode.getAll(':scope > post-leader-date').forEach((value) => {
					value.remove();
				});
			}
		});
		await suspend();
		/* 'onVisibleClick' events for the 'post > sub-post > post-leader > post-leader-advance > button.visibility's */
		let onVisibleClickEvent = [];
		forAll('post').map((value) => {
			return new Post(value);
		}).filter((value) => {
			return value.complete;
		}).forEach((postValue) => {
			if (!postValue.postNode.hasAttribute('with-collapsed')) {
				postValue.postLeaderAdvanceNode.getAll(':scope > button.advance.visibility').forEach((value) => {
					value.remove();
				});
				return;
			}
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
				if (!new Post(postValue.postNode).complete || this.parentElement != postValue.postLeaderAdvanceNode) {
					return;
				}
				let freezer = (action) => {
					Array.from(postValue.postContentNode.children).forEach((value) => {
						if (value.tagName.toUpperCase() == 'img'.toUpperCase() || value.tagName.toUpperCase() == 'iframe'.toUpperCase()) {
							action(value);
						}
					});
				};
				freezer((node) => {
					node.setAttribute('frozen', '');
				});
				this.classList.add('disabled');
				if (this.id == 'visibled') {
					this.classList.remove('up');
					this.classList.add('down');
					this.textContent = '展開';
					postValue.postContentNode.style.aspectRatio = '';
					postValue.postContentNode.classList.remove('no-scrollbar');
				} else {
					this.classList.add('up');
					this.classList.remove('down');
					this.textContent = '縮小';
					postValue.postContentNode.style.aspectRatio = postValue.postContentNode.offsetWidth.toString() + ' / ' + postValue.postContentNode.scrollHeight.toString();
					postValue.postContentNode.classList.remove('no-scrollbar');
				}
				await defer(1000);
				freezer((node) => {
					node.removeAttribute('frozen');
				});
				this.classList.remove('disabled');
				if (this.id == 'visibled') {
					postValue.postContentNode.classList.remove('no-scrollbar');
					this.id = '';
				} else {
					postValue.postContentNode.classList.add('no-scrollbar');
					this.id = 'visibled';
				}
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
			let Perform = (action) => {
				forAll('post[with-collapsed]').map((value) => {
					return new Post(value);
				}).filter((value) => {
					return value.complete;
				}).forEach((postValue) => {
					if (postValue.postLeaderAdvanceNode.has(':scope > button.advance.visibility')) {
						let buttonNode = postValue.postLeaderAdvanceNode.get(':scope > button.advance.visibility');
						action.call(buttonNode, postValue.postContentNode);
					}
				});
			};
			Perform(function action(postContentNode) {
				if (this.id == 'visibled') {
					postContentNode.style.aspectRatio = 'auto';
				}
				postContentNode.classList.add('resized');
				this.classList.add('pseudo-disabled');
			});
			await defer(250);
			resizedCount -= 1;
			if (resizedCount > 0) {
				return;
			}
			Perform(function action(postContentNode) {
				if (this.id == 'visibled') {
					postContentNode.style.aspectRatio = postContentNode.offsetWidth.toString() + ' / ' + postContentNode.scrollHeight.toString();
				}
				postContentNode.classList.remove('resized');
				this.classList.remove('pseudo-disabled');
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
		}).forEach(async (postValue) => {
			let operate = (node, attribute, selector, bind) => {
				let is = (node, determiner) => {
					if (node.hasAttribute('as-is') || node.hasAttribute('with-collapsed') || node.has(':scope > sub-post > post-content > post')) {
						return false;
					}
					return determiner(node);
				};
				let getDeterminer = (selector) => {
					return (node) => {
						return node.has(selector);
					};
				};
				let visibilityChecking = (node, selector) => {
					let matched = (siblingProperty, classSelector) => {
						let determiner = (selector) => {
							return (node) => {
								let siblingNode = node.get(selector)[siblingProperty];
								while (siblingNode != null) {
									if (isInstance(siblingNode)) {
										return false;
									}
									siblingNode = siblingNode[siblingProperty];
								}
								return true;
							};
						};
						if (is(node, determiner(selector))) {
							node.get(selector).classList.add(classSelector);
						} else {
							node.get(selector).classList.remove(classSelector);
						}
					};
					matched('previousSibling', 'first-visible-child');
					matched('nextSibling', 'last-visible-child');
				};
				if (is(node, getDeterminer(selector))) {
					node.setAttribute(attribute, '');
					visibilityChecking(node, selector);
					bind?.call(null, node);
				} else {
					node.removeAttribute(attribute);
				}
			};
			operate(postValue.postNode, 'with-graphics', ':scope > sub-post > post-content > img:first-of-type:last-of-type');
			await suspend();
			operate(postValue.postNode, 'with-notice', ':scope > sub-post > post-content > notice', (node) => {
				let noticeNode = node.get(':scope > sub-post > post-content > notice');
				if (noticeNode.has(':scope > sub-notice > notice-content')) {
					let noticeContentNode = noticeNode.get(':scope > sub-notice > notice-content');
					noticeContentNode.classList.add('no-space');
				}
			});
			await suspend();
			operate(postValue.postNode, 'with-inline-frame', ':scope > sub-post > post-content > iframe:first-of-type:last-of-type');
		});
		await mustSuspend();
		/* a stack machine for a scrolling coroutine */
		let operate = (selector, scrollable, action) => {
			forAll(selector).filter((value) => {
				return scrollable ? inScrollable(value) : !inScrollable(value);
			}).forEach(action);
		};
		if (rescrollState == 0 && scrolledInto()) {
			/* initial state of '[deferred-src]' for the 'img's */
			operate('img[deferred-src]:not([frozen])', true, (value) => {
				deferredImgNode.push(value);
				let url = new URL(value.getAttribute('deferred-src'), document.baseURI);
				value.setAttribute('alt', url.href.substring(url.href.lastIndexOf('/') + 1));
				value.setAttribute('src', value.getAttribute('deferred-src'));
				value.removeAttribute('deferred-src');
				function onError() {
					this.setAttribute('pre-deferred-src', this.getAttribute('src'));
					this.removeAttribute('src');
					this.removeEventListener('error', onError);
					this.removeEventListener('load', onLoad);
					deferredImgNode = deferredImgNode.filter((value) => {
						return value != this;
					});
				}
				function onLoad() {
					this.removeEventListener('error', onError);
					this.removeEventListener('load', onLoad);
					deferredImgNode = deferredImgNode.filter((value) => {
						return value != this;
					});
				}
				value.addEventListener('error', onError);
				value.addEventListener('load', onLoad);
			});
			rescrollState = 1;
		} else if (rescrollState == 1) {
			deferredImgNode = deferredImgNode.filter((value) => {
				return value.naturalWidth == 0;
			});
			if (deferredImgNode.length == 0) {
				rescrollState = 2;
			}
		} else if (rescrollState == 2) {
			rescroll();
			lastHash = document.location.hash;
			rescrollState = 3;
		} else if (rescrollState == 3 && lastHash != document.location.hash) {
			rescrollState = 0;
		} else if (rescrollState == 3 && scrolledInto()) {
			/* '[deferred-src]' for the 'img's */
			operate('img[deferred-src]:not([frozen])', true, (value) => {
				let url = new URL(value.getAttribute('deferred-src'), document.baseURI);
				value.setAttribute('alt', url.href.substring(url.href.lastIndexOf('/') + 1));
				value.setAttribute('src', value.getAttribute('deferred-src'));
				value.removeAttribute('deferred-src');
				function onError() {
					this.setAttribute('pre-deferred-src', this.getAttribute('src'));
					this.removeAttribute('src');
					this.removeEventListener('error', onError);
					this.removeEventListener('load', onLoad);
				}
				function onLoad() {
					this.removeEventListener('error', onError);
					this.removeEventListener('load', onLoad);
				}
				value.addEventListener('error', onError);
				value.addEventListener('load', onLoad);
			});
			await suspend();
			/* '[pre-deferred-src]' for the 'img's */
			operate('img[pre-deferred-src]:not([frozen])', false, (value) => {
				value.setAttribute('deferred-src', value.getAttribute('pre-deferred-src'));
				value.removeAttribute('pre-deferred-src');
			});
			await suspend();
			/* '[deferred-src]' for the 'iframe's */
			operate('iframe[deferred-src]:not([frozen])', true, (value) => {
				let url = new URL(value.getAttribute('deferred-src'), document.baseURI);
				if (document.location.origin == url.origin) {
					let request = new XMLHttpRequest();
					request.addEventListener('load', function onLoad() {
						if (this.status >= 400 && this.status <= 599) {
							value.setAttribute('referred', '');
						}
						this.removeEventListener('load', onLoad);
					});
					request.open('GET', url.href, true);
					request.send();
				}
				value.setAttribute('src', value.getAttribute('deferred-src'));
				value.removeAttribute('deferred-src');
			});
			await suspend();
			/* '[referred]' for the 'iframe's */
			operate('iframe[referred]:not([frozen])', false, (value) => {
				value.setAttribute('deferred-src', value.getAttribute('src'));
				value.removeAttribute('referred');
				value.removeAttribute('src');
			});
		}
	});
})();
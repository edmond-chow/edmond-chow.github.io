(() => {
	/* { asynchronous } */
	[
		async function defer(timeout = 0) {
			[timeout].constrainedWithAndThrow(Number);
			let timespan = performance.now() + timeout;
			await suspend(() => {
				return performance.now() > timespan;
			});
		}
	].bindTo(window);
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
			[property, value].constrainedWithAndThrow(String, String);
			document.cookie = property + '=' + value;
		},
		function getCookie(property) {
			[property].constrainedWithAndThrow(String);
			let cookie = '';
			document.cookie.split(';').every((value) => {
				let pair = value.split('=');
				pair = pair.map((value) => {
					return value.trim();
				});
				if (pair[0] == property) {
					cookie = pair[1];
					return false;
				}
				return true;
			});
			return cookie;
		},
		function switchBlurredState() {
			if (document.body.id == 'blur') {
				document.body.id = '';
				setCookie('non-blur', 'true');
			} else {
				document.body.id = 'blur';
				setCookie('non-blur', 'false');
			}
		},
		function isInstance(visualizedNode) {
			[visualizedNode].constrainedWithAndThrow(Node);
			if (visualizedNode.nodeName == '#comment') {
				return false;
			} else if (visualizedNode.nodeName == '#text' && visualizedNode.wholeText.removeSpace() == '') {
				return false;
			} else if (visualizedNode instanceof Element && visualizedNode.nodeName == 'br'.toUpperCase()) {
				return false;
			} else if (visualizedNode instanceof Element && window.getComputedStyle(visualizedNode).display == 'none') {
				return false;
			} else {
				return true;
			}
		}
	].bindTo(window);
	/* { event-dispatcher } */
	document.addEventListener('structuredTag', () => {
		/* switchBlurredState() */
		if (getCookie('non-blur') == 'true') {
			switchBlurredState();
		}
	});
	class Event {
		constructor(object, trigger) {
			this.object = object;
			this.trigger = trigger;
		}
	};
	class DispatcherStateMachine {
		constructor() {
			this.state = 0;
			this.isLoaded = false;
			this.hasScrolled = false;
			this.visibleClickEvents = [];
			this.resizedCount = 0;
		}
		onPostDateString() {
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
		}
		static VisibilitySelector = ':scope > button.advance.visibility'
		onLazyFrozen(postContentNode, freeze) {
			Array.from(postContentNode.childNodes).filter((value) => {
				return value.nodeName == 'img'.toUpperCase() || value.nodeName == 'iframe'.toUpperCase();
			}).forEach((value) => {
				if (freeze) {
					value.setAttribute('frozen', '');
				} else {
					value.removeAttribute('frozen');
				}
			});
		}
		newPostWithCollapsedButton(postLeaderAdvanceNode) {
			if (postLeaderAdvanceNode.has(DispatcherStateMachine.VisibilitySelector)) {
				return postLeaderAdvanceNode.get(DispatcherStateMachine.VisibilitySelector);
			} else {
				let buttonNode = document.createElement('button');
				buttonNode.classList.add('advance', 'visibility');
				postLeaderAdvanceNode.append(buttonNode);
				return buttonNode;
			}
		}
		sizePostWithCollapsedButton(postContentNode, visibled) {
			if (visibled) {
				postContentNode.style.aspectRatio = '';
			} else {
				postContentNode.style.aspectRatio = postContentNode.offsetWidth.toString() + ' / ' + postContentNode.scrollHeight.toString();
			}
		}
		drawPostWithCollapsedButton(postContentNode, buttonNode, visibled) {
			if (visibled) {
				postContentNode.classList.remove('no-scrollbar');
				buttonNode.classList.remove('up');
				buttonNode.classList.add('down');
				buttonNode.textContent = '展開';
				buttonNode.id = '';
			} else {
				postContentNode.classList.add('no-scrollbar');
				buttonNode.classList.add('up');
				buttonNode.classList.remove('down');
				buttonNode.textContent = '縮小';
				buttonNode.id = 'visibled';
			}
		}
		async onPostWithCollapsedVisibleClick(postNode, e) {
			/* 'onVisibleClick' events for the 'post > sub-post > post-leader > post-leader-advance > button.visibility's */
			let postValue = new Post(postNode);
			if (postValue.complete && e.target.parentElement == postValue.postLeaderAdvanceNode) {
				postValue.postContentNode.classList.remove('no-scrollbar');
				let visibled = e.target.id == 'visibled';
				this.onLazyFrozen(postValue.postContentNode, true);
				e.target.classList.add('disabled');
				this.sizePostWithCollapsedButton(postValue.postContentNode, visibled);
				await defer(1000);
				this.onLazyFrozen(postValue.postContentNode, false);
				e.target.classList.remove('disabled');
				this.drawPostWithCollapsedButton(postValue.postContentNode, e.target, visibled);
			}
		}
		operatePostWithCollapsedResizingAgent(resizingOperation) {
			forAll('post[with-collapsed]').map((value) => {
				return new Post(value);
			}).filter((value) => {
				return value.complete;
			}).filter((value) => {
				return value.postLeaderAdvanceNode.has(DispatcherStateMachine.VisibilitySelector);
			}).forEach((postValue) => {
				let buttonNode = postValue.postLeaderAdvanceNode.get(DispatcherStateMachine.VisibilitySelector);
				resizingOperation(buttonNode, postValue.postContentNode);
			});
		}
		async onPostWithCollapsedResized() {
			this.operatePostWithCollapsedResizingAgent((buttonNode, postContentNode) => {
				if (buttonNode.id == 'visibled') {
					postContentNode.style.aspectRatio = 'auto';
				}
				postContentNode.classList.add('resized');
				buttonNode.classList.add('pseudo-disabled');
			});
			++this.resizedCount;
			await defer(250);
			if (--this.resizedCount == 0) {
				this.operatePostWithCollapsedResizingAgent((buttonNode, postContentNode) => {
					if (buttonNode.id == 'visibled') {
						postContentNode.style.aspectRatio = postContentNode.offsetWidth.toString() + ' / ' + postContentNode.scrollHeight.toString();
					}
					postContentNode.classList.remove('resized');
					buttonNode.classList.remove('pseudo-disabled');
				});
			}
		}
		onPostWithCollapsed() {
			/* remove 'visibility' class name for the 'post:not([with-collapsed]) > sub-post > post-leader > post-leader-advance > button.visibility's */
			forAll('post').map((value) => {
				return new Post(value);
			}).filter((value) => {
				return value.complete && !value.postNode.hasAttribute('with-collapsed');
			}).map((value) => {
				return value.postLeaderAdvanceNode;
			}).forEach((postLeaderAdvanceNode) => {
				postLeaderAdvanceNode.getAll(DispatcherStateMachine.VisibilitySelector).forEach((value) => {
					value.classList.remove('visibility');
				});
			});
			/* 'onVisibleClick' events for the 'post[with-collapsed] > sub-post > post-leader > post-leader-advance > button.visibility's */
			forAll('post').map((value) => {
				return new Post(value);
			}).filter((value) => {
				return value.complete && value.postNode.hasAttribute('with-collapsed');
			}).forEach((postValue) => {
				let buttonNode = this.newPostWithCollapsedButton(postValue.postLeaderAdvanceNode);
				this.drawPostWithCollapsedButton(postValue.postContentNode, buttonNode, true);
				let onVisibleClick = this.onPostWithCollapsedVisibleClick.bind(this, postValue.postNode);
				this.visibleClickEvents.push(new Event(buttonNode, onVisibleClick));
				buttonNode.addEventListener('click', onVisibleClick);
			});
			document.addEventListener('structuredTag', () => {
				this.visibleClickEvents.forEach((value) => {
					value.object.removeEventListener('click', value.trigger);
				});
			}, { once: true });
			/* an 'onResized' event for the 'post[with-collapsed] > sub-post > post-leader > post-leader-advance > button.visibility's */
			let onResized = this.onPostWithCollapsedResized.bind(this);
			document.addEventListener('structuredTag', () => {
				window.removeEventListener('resize', onResized);
			}, { once: true });
			window.addEventListener('resize', onResized);
		}
		onPostWithNotice() {
			/* '[with-notice]' for the 'post's */
			insertSurround('post > sub-post > post-content > notice', 'sub-notice');
			insertSurround('post > sub-post > post-content > notice > sub-notice', 'notice-content');
		}
		onImgAlt() {
			/* '[alt]' for the 'img's */
			forAllTag('img').forEach((value) => {
				if (!hasAlt(value) && !hasAriaLabel(value) && !hasAriaLabelBy(value)) {
					makeAlt(value);
				}
			});
		}
		onIframeTitle() {
			/* '[title]' for the 'iframe's */
			forAllTag('iframe').forEach((value) => {
				if (!hasTitle(value)) {
					makeTitle(value);
				}
			});
		}
		onButtonRoleAriaLabel() {
			/* '[aria-label]' for the 'button, [role="button"]'s */
			forAll('button, [role="button"]').forEach((value) => {
				if (!hasTitle(value) && !hasAriaLabel(value) && !hasAriaLabelBy(value)) {
					makeAriaLabel(value);
				}
				if (!value.hasAttribute('type')) {
					value.setAttribute('type', 'button');
				}
			});
		}
		onARoleAriaLabel() {
			/* '[aria-label]' for the 'a, [role="link"]'s */
			forAll('a, [role="link"]').forEach((value) => {
				if (value.hasAttribute('href') && !hasAriaLabel(value)) {
					makeAriaLabel(value);
				}
			});
		}
		static ProceedTag = ['onPostDateString', 'onPostWithCollapsed', 'onPostWithNotice', 'onImgAlt', 'onIframeTitle', 'onButtonRoleAriaLabel', 'onARoleAriaLabel']
		operatePost(node, attribute, selector, bind = null) {
			let original = !node.hasAttribute('as-is') && !node.hasAttribute('with-collapsed') && !node.has(':scope > sub-post > post-content > post');
			let matched = (siblingProperty, classSelector) => {
				let isMatched = true;
				let siblingNode = node.get(selector)[siblingProperty];
				while (siblingNode != null) {
					if (isInstance(siblingNode)) {
						isMatched = false;
					}
					siblingNode = siblingNode[siblingProperty];
				}
				if (original && isMatched) {
					node.get(selector).classList.add(classSelector);
				} else {
					node.get(selector).classList.remove(classSelector);
				}
			};
			if (original && node.has(selector)) {
				node.setAttribute(attribute, '');
				matched('previousSibling', 'first-visible-child');
				matched('nextSibling', 'last-visible-child');
				bind?.call(null, node);
			} else {
				node.removeAttribute(attribute);
			}
		}
		onPost() {
			/* '[with-graphics, with-notice, with-inline-frame]' for the 'post's */
			forAllTag('post').map((value) => {
				return new Post(value);
			}).filter((value) => {
				return value.complete;
			}).forEach((postValue) => {
				this.operatePost(postValue.postNode, 'with-graphics', ':scope > sub-post > post-content > img:first-of-type:last-of-type');
				this.operatePost(postValue.postNode, 'with-notice', ':scope > sub-post > post-content > notice', (postNode) => {
					let noticeNode = postNode.get(':scope > sub-post > post-content > notice');
					if (noticeNode.has(':scope > sub-notice > notice-content')) {
						let noticeContentNode = noticeNode.get(':scope > sub-notice > notice-content');
						noticeContentNode.classList.add('no-space');
					}
				});
				this.operatePost(postValue.postNode, 'with-inline-frame', ':scope > sub-post > post-content > iframe:first-of-type:last-of-type');
			});
		}
		operateLazy(selector, scrollable, action) {
			forAll(selector).filter((value) => {
				return scrollable ? inScrollable(value) : !inScrollable(value);
			}).forEach(action);
		}
		imgDeferredSrc() {
			/* '[deferred-src]' for the 'img's */
			this.operateLazy('img[deferred-src]:not([frozen])', true, (value) => {
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
		}
		iframeDeferredSrc() {
			/* '[deferred-src]' for the 'iframe's */
			this.operateLazy('iframe[deferred-src]:not([frozen])', true, (value) => {
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
		}
		imgLoadingSrc() {
			/* '[pre-deferred-src]' for the 'img's */
			this.operateLazy('img[pre-deferred-src]:not([frozen])', false, (value) => {
				value.setAttribute('deferred-src', value.getAttribute('pre-deferred-src'));
				value.removeAttribute('pre-deferred-src');
			});
		}
		iframeLoadingSrc() {
			/* '[referred]' for the 'iframe's */
			this.operateLazy('iframe[referred]:not([frozen])', false, (value) => {
				value.setAttribute('deferred-src', value.getAttribute('src'));
				value.removeAttribute('referred');
				value.removeAttribute('src');
			});
		}
		firstScroll() {
			if (this.hasScrolled == false && scrolledInto()) {
				rescrollView();
				this.hasScrolled = true;
			}
		}
		static ProceedStyle = ['onPost', 'imgDeferredSrc', 'iframeDeferredSrc', 'imgLoadingSrc', 'iframeLoadingSrc', 'firstScroll']
		moveNext() {
			let Proceeder = !this.isLoaded ? DispatcherStateMachine.ProceedTag : DispatcherStateMachine.ProceedStyle;
			this[Proceeder[this.state++]]();
			if (this.state == Proceeder.length) {
				this.isLoaded = true;
				this.state = 0;
			}
		}
	};
	let dispatcher = new DispatcherStateMachine();
	document.addEventListener('formedStyle',() => {
		dispatcher.moveNext();
	});
})();
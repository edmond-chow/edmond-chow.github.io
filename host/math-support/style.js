/*
 *   MIT License
 *   
 *   Copyright (c) 2023 Edmond Chow
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */
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
			document.cookie = property + '=' + value + '; SameSite=Struct';
		},
		function getCookie(property) {
			[property].constrainedWithAndThrow(String);
			let value = '';
			document.cookie.split(';').every((cookie) => {
				let pair = cookie.split('=').map((value) => {
					return value.trim();
				});
				if (property.trim() == pair[0]) {
					value = pair[1];
					return false;
				}
				return true;
			});
			return value;
		},
		function setFrameState(isDarkMode) {
			if (isDarkMode) {
				document.body.classList.remove('blur');
			} else {
				document.body.classList.add('blur');
			}
			setCookie('classical-black-mode', isDarkMode.toString());
		},
		function getFrameState() {
			return !document.body.classList.contains('blur');
		},
		function popFrameState() {
			return getCookie('classical-black-mode') == 'true';
		},
		function switchFrameState() {
			setFrameState(!getFrameState());
		},
		function renderFrameState() {
			setFrameState(popFrameState());
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
	class Event {
		constructor(target, type, listener, options) {
			[target, type, listener, options].constrainedWithAndThrow(EventTarget, String, Function, Object.toNullable());
			this.target = target;
			this.type = type;
			this.listener = listener;
			this.options = options;
			lockFields(this, ['target', 'type', 'listener', 'options'], false);
		}
		add() {
			this.target.addEventListener(this.type, this.listener, this.options);
		}
		remove() {
			this.target.removeEventListener(this.type, this.listener, this.options);
		}
	};
	class DispatcherStateMachine {
		constructor() {
			this.registeredEvents = [];
			this.resetState();
			lockFields(this, ['state', 'isLoaded', 'hasScrolled', 'scrollingStarted', 'resizingAgentCounter', 'registeredEvents'], true);
		}
		onButtonFrameState() {
			forAll('button#frame-state').forEach((node) => {
				this.pushEvent(node, 'click', switchFrameState);
			});
		}
		onPostDateString() {
			/* integrating the 'post-leader-date's by including the '[date-string]' */
			forAllTag('post').map((node) => {
				return new Post(node);
			}).filter((value) => {
				return value.completed;
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
		getAspectRatioFor(postContentSubstanceNode) {
			return postContentSubstanceNode.offsetWidth.toString() + ' / ' + postContentSubstanceNode.scrollHeight.toString();
		}
		getGutterSizeFor(postContentSubstanceNode) {
			return (postContentSubstanceNode.offsetWidth - postContentSubstanceNode.clientWidth).toString() + 'px';
		}
		onLazyFrozen(postContentSubstanceNode, freeze) {
			Array.from(postContentSubstanceNode.childNodes).filter((node) => {
				return node.nodeName == 'img'.toUpperCase() || node.nodeName == 'iframe'.toUpperCase();
			}).forEach((node) => {
				if (freeze) {
					node.setAttribute('frozen', '');
				} else {
					node.removeAttribute('frozen');
				}
			});
		}
		newPostWithCollapsedButton(postLeaderAdvanceNode) {
			if (postLeaderAdvanceNode.has(':scope > button.advance.visibility')) {
				return postLeaderAdvanceNode.get(':scope > button.advance.visibility');
			} else {
				let buttonNode = document.createElement('button');
				buttonNode.classList.add('advance', 'visibility');
				postLeaderAdvanceNode.append(buttonNode);
				return buttonNode;
			}
		}
		renderPostWithCollapsedBegin(postContentSubstanceNode, buttonNode, narrowFrame) {
			if (narrowFrame) {
				postContentSubstanceNode.style.aspectRatio = '';
				buttonNode.classList.remove('up');
				buttonNode.classList.add('down');
				buttonNode.textContent = '展開';
			} else {
				postContentSubstanceNode.style.aspectRatio = this.getAspectRatioFor(postContentSubstanceNode);
				buttonNode.classList.add('up');
				buttonNode.classList.remove('down');
				buttonNode.textContent = '縮小';
			}
			postContentSubstanceNode.style.paddingRight = '';
			buttonNode.classList.add('disabled');
		}
		renderPostWithCollapsedEnd(postContentSubstanceNode, buttonNode, narrowFrame) {
			if (narrowFrame) {
				postContentSubstanceNode.style.paddingRight = '';
				postContentSubstanceNode.classList.remove('no-scrollbar');
				buttonNode.classList.remove('extensied');
			} else {
				postContentSubstanceNode.style.paddingRight = this.getGutterSizeFor(postContentSubstanceNode);
				postContentSubstanceNode.classList.add('no-scrollbar');
				buttonNode.classList.add('extensied');
			}
			buttonNode.classList.remove('disabled');
		}
		renderPostWithCollapsedImmediately(postContentSubstanceNode, buttonNode, narrowFrame) {
			this.renderPostWithCollapsedBegin(postContentSubstanceNode, buttonNode, narrowFrame);
			this.renderPostWithCollapsedEnd(postContentSubstanceNode, buttonNode, narrowFrame);
		}
		async onPostWithCollapsedVisibleClick(postNode, e) {
			/* 'onVisibleClick' events for the 'post > sub-post > post-leader > post-leader-advance > button.visibility's */
			let postValue = new Post(postNode);
			if (postValue.completed && e.target.parentElement == postValue.postLeaderAdvanceNode) {
				postValue.postContentSubstanceNode.classList.remove('no-scrollbar');
				let extensied = e.target.classList.contains('extensied');
				this.renderPostWithCollapsedBegin(postValue.postContentSubstanceNode, e.target, extensied);
				this.onLazyFrozen(postValue.postContentSubstanceNode, true);
				await defer(1000);
				this.renderPostWithCollapsedEnd(postValue.postContentSubstanceNode, e.target, extensied);
				this.onLazyFrozen(postValue.postContentSubstanceNode, false);
			}
		}
		operatePostWithCollapsedResizingAgent(resizingOperation) {
			forAll('post[with-collapsed]').map((node) => {
				return new Post(node);
			}).filter((value) => {
				return value.completed;
			}).filter((value) => {
				return value.postLeaderAdvanceNode.has(':scope > button.advance.visibility');
			}).forEach((postValue) => {
				let buttonNode = postValue.postLeaderAdvanceNode.get(':scope > button.advance.visibility');
				resizingOperation(postValue.postContentSubstanceNode, buttonNode);
			});
		}
		async onPostWithCollapsedResized() {
			this.operatePostWithCollapsedResizingAgent((postContentSubstanceNode, buttonNode) => {
				if (buttonNode.classList.contains('extensied')) {
					postContentSubstanceNode.style.aspectRatio = 'auto';
				}
				postContentSubstanceNode.classList.add('resizing');
				buttonNode.classList.add('pseudo-disabled');
			});
			++this.resizingAgentCounter;
			await defer(250);
			if (--this.resizingAgentCounter == 0) {
				this.operatePostWithCollapsedResizingAgent((postContentSubstanceNode, buttonNode) => {
					if (buttonNode.classList.contains('extensied')) {
						postContentSubstanceNode.style.aspectRatio = this.getAspectRatioFor(postContentSubstanceNode);
					}
					postContentSubstanceNode.classList.remove('resizing');
					buttonNode.classList.remove('pseudo-disabled');
				});
			}
		}
		onPostWithCollapsed() {
			/* remove 'visibility' class name for the 'post:not([with-collapsed]) > sub-post > post-leader > post-leader-advance > button.visibility's */
			forAll('post').map((node) => {
				return new Post(node);
			}).filter((value) => {
				return value.completed && !value.postNode.hasAttribute('with-collapsed');
			}).map((value) => {
				return value.postLeaderAdvanceNode;
			}).forEach((postLeaderAdvanceNode) => {
				postLeaderAdvanceNode.getAll(':scope > button.advance.visibility').forEach((node) => {
					node.classList.remove('visibility');
				});
			});
			/* 'onVisibleClick' events for the 'post[with-collapsed] > sub-post > post-leader > post-leader-advance > button.visibility's */
			forAll('post').map((value) => {
				return new Post(value);
			}).filter((value) => {
				return value.completed && value.postNode.hasAttribute('with-collapsed');
			}).forEach((postValue) => {
				let buttonNode = this.newPostWithCollapsedButton(postValue.postLeaderAdvanceNode);
				this.renderPostWithCollapsedImmediately(postValue.postContentSubstanceNode, buttonNode, true);
				let onVisibleClick = this.onPostWithCollapsedVisibleClick.bind(this, postValue.postNode);
				this.pushEvent(buttonNode, 'click', onVisibleClick);
			});
			/* an 'onResized' event for the 'post[with-collapsed] > sub-post > post-leader > post-leader-advance > button.visibility's */
			let onResized = this.onPostWithCollapsedResized.bind(this);
			this.pushEvent(window, 'resize', onResized);
		}
		onPostWithNotice() {
			/* '[with-notice]' for the 'post's */
			insertSurround('post > sub-post > post-content > post-content-substance > notice', 'sub-notice');
			insertSurround('post > sub-post > post-content > post-content-substance > notice > sub-notice', 'notice-content');
		}
		onImgAlt() {
			/* '[alt]' for the 'img's */
			forAllTag('img').forEach((node) => {
				if (!hasAlt(node) && !hasAriaLabel(node) && !hasAriaLabelBy(node)) {
					makeAlt(node);
				}
			});
		}
		onIframeTitle() {
			/* '[title]' for the 'iframe's */
			forAllTag('iframe').forEach((node) => {
				if (!hasTitle(node)) {
					makeTitle(node);
				}
			});
		}
		onButtonRoleAriaLabel() {
			/* '[aria-label]' for the 'button, [role="button"]'s */
			forAll('button, [role="button"]').forEach((node) => {
				if (!hasTitle(node) && !hasAriaLabel(node) && !hasAriaLabelBy(node)) {
					makeAriaLabel(node);
				}
				if (!node.hasAttribute('type')) {
					node.setAttribute('type', 'button');
				}
			});
		}
		onARoleAriaLabel() {
			/* '[aria-label]' for the 'a, [role="link"]'s */
			forAll('a, [role="link"]').forEach((node) => {
				if (node.hasAttribute('href') && !hasAriaLabel(node)) {
					makeAriaLabel(node);
				}
			});
		}
		static TagCoroutines = [
			DispatcherStateMachine.prototype.onButtonFrameState,
			DispatcherStateMachine.prototype.onPostDateString,
			DispatcherStateMachine.prototype.onPostWithCollapsed,
			DispatcherStateMachine.prototype.onPostWithNotice,
			DispatcherStateMachine.prototype.onImgAlt,
			DispatcherStateMachine.prototype.onIframeTitle,
			DispatcherStateMachine.prototype.onButtonRoleAriaLabel,
			DispatcherStateMachine.prototype.onARoleAriaLabel
		]
		operatePost(node, attribute, selector, bind = null) {
			let original = !node.hasAttribute('as-is') && !node.hasAttribute('with-collapsed') && !node.has(':scope > sub-post > post-content > post-content-substance > post');
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
			/* '.no-content' for the 'post-leader-date's */
			forAllTag('post').map((node) => {
				return new Post(node);
			}).filter((value) => {
				return value.completed;
			}).forEach((postValue) => {
				if (postValue.postNode.has(':scope > sub-post > post-leader > post-leader-section > post-leader-date')) {
					let postLeaderDateNode = postValue.postNode.get(':scope > sub-post > post-leader > post-leader-section > post-leader-date');
					if (hasSubstance(postLeaderDateNode)) {
						postLeaderDateNode.classList.remove('no-content');
					} else {
						postLeaderDateNode.classList.add('no-content');
					}
				}
			});
			/* '[with-graphics, with-notice, with-inline-frame]' for the 'post's */
			forAllTag('post').map((value) => {
				return new Post(value);
			}).filter((value) => {
				return value.completed;
			}).forEach((postValue) => {
				this.operatePost(postValue.postNode, 'with-graphics', ':scope > sub-post > post-content > post-content-substance > img:first-of-type:last-of-type');
				this.operatePost(postValue.postNode, 'with-notice', ':scope > sub-post > post-content > post-content-substance > notice', (postNode) => {
					let noticeNode = postNode.get(':scope > sub-post > post-content > post-content-substance > notice');
					if (noticeNode.classList.contains('first-visible-child')) {
						postNode.classList.add('has-first-notice');
					} else {
						postNode.classList.remove('has-first-notice');
					}
				});
				this.operatePost(postValue.postNode, 'with-inline-frame', ':scope > sub-post > post-content > post-content-substance > iframe:first-of-type:last-of-type');
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
				let listener = (e) => {
					if (e.type == 'error') {
						e.target.setAttribute('pre-deferred-src', e.target.getAttribute('src'));
						e.target.removeAttribute('src');
					}
					e.target.removeEventListener('error', listener);
					e.target.removeEventListener('load', listener);
				};
				value.addEventListener('error', listener);
				value.addEventListener('load', listener);
			});
		}
		iframeDeferredSrc() {
			/* '[deferred-src]' for the 'iframe's */
			this.operateLazy('iframe[deferred-src]:not([frozen])', true, (value) => {
				let url = new URL(value.getAttribute('deferred-src'), document.baseURI);
				if (document.location.origin == url.origin) {
					let request = new XMLHttpRequest();
					let listener = (e) => {
						if (e.target.status >= 400 && e.target.status <= 599) {
							value.setAttribute('referred', '');
						}
						e.target.removeEventListener('load', listener);
					};
					request.addEventListener('load', listener);
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
		beginFrameScroll() {
			if (!this.hasScrolled) {
				if (scrolledInto()) {
					rescrollView();
					this.hasScrolled = true;
					this.scrollingStarted = true;
				} else {
					this.state = DispatcherStateMachine.StyleCoroutines.indexOf(this.beginFrameScroll);
				}
			}
		}
		endFrameScroll() {
			if (this.scrollingStarted) {
				if (scrolledInto()) {
					this.scrollingStarted = false;
				} else {
					this.state = DispatcherStateMachine.StyleCoroutines.indexOf(this.endFrameScroll);
				}
			}
		}
		static StyleCoroutines = [
			DispatcherStateMachine.prototype.onPost,
			DispatcherStateMachine.prototype.imgDeferredSrc,
			DispatcherStateMachine.prototype.iframeDeferredSrc,
			DispatcherStateMachine.prototype.imgLoadingSrc,
			DispatcherStateMachine.prototype.iframeLoadingSrc,
			DispatcherStateMachine.prototype.beginFrameScroll,
			DispatcherStateMachine.prototype.endFrameScroll
		]
		moveNext() {
			let coroutines = !this.isLoaded ? DispatcherStateMachine.TagCoroutines : DispatcherStateMachine.StyleCoroutines;
			coroutines[this.state++].call(this);
			if (this.state == coroutines.length) {
				this.isLoaded = true;
				this.state = 0;
			}
		}
		resetState() {
			this.state = 0;
			this.isLoaded = false;
			this.hasScrolled = false;
			this.scrollingStarted = false;
			this.resizingAgentCounter = 0;
			while (this.registeredEvents.length > 0) {
				let event = this.registeredEvents.pop();
				event.remove();
			}
			renderFrameState();
		}
		pushEvent(target, type, listener, options = null) {
			let event = new Event(target, type, listener, options);
			this.registeredEvents.push(event);
			event.add();
		}
	};
	shareProperties(Event, ['add', 'remove'], false);
	hardFreeze(DispatcherStateMachine, [Event], false);
	Object.freeze(DispatcherStateMachine.TagCoroutines);
	Object.freeze(DispatcherStateMachine.StyleCoroutines);
	shareProperties(DispatcherStateMachine, ['moveNext', 'resetState', 'pushEvent'], false);
	shareProperties(DispatcherStateMachine, ['TagCoroutines', 'StyleCoroutines'], true);
	hardFreeze(window, [DispatcherStateMachine], false);
	let activatedStructured = false;
	document.addEventListener('dispatcherInitialized', () => {
		Object.defineProperty(window, 'dispatcher', makeDescriptor(new DispatcherStateMachine(), true));
	});
	document.addEventListener('structuredTag', () => {
		if (!activatedStructured) {
			(async () => {
				while (true) {
					dispatcher.moveNext();
					await suspend();
				}
			})();
		} else {
			dispatcher.resetState();
		}
		activatedStructured = true;
	});
})();
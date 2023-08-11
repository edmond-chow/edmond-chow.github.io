(async () => {
	/* [Array.prototype.bindTo].bindTo(instance = window) */
	(() => {
		let lock = { configurable: false, writable: false };
		Array.prototype.bindTo = function bindTo(instance = window) {
			this.forEach((value) => {
				if (value instanceof Function) {
					instance[value.name] = value;
					Object.defineProperty(instance, value.name, lock);
					if (value.prototype instanceof Object) {
						Object.defineProperty(value, 'prototype', lock);
						value.prototype.constructor = value;
						Object.defineProperty(value.prototype, 'constructor', lock);
					}
				}
			});
		};
		[Array.prototype.bindTo].bindTo(Array.prototype);
		let Nullable = function Nullable(type) {
			this.type = type;
			Object.defineProperty(this, 'type', lock);
		};
		[
			function toNullable() {
				return new Nullable(this);
			}
		].bindTo(Function.prototype);
		[
			function isNullable(container) {
				return container instanceof Nullable;
			},
			function removeNullable(container) {
				if (container instanceof Nullable) {
					return container.type;
				} else if (container instanceof Function) {
					return container;
				} else {
					throw 'The \'container\' argument should be either a \'Nullable\' type or a \'Function\' type.';
				}
			}
		].bindTo(window);
		[
			function constrainedWith(...types) {
				let arguments = Array.from(this);
				if (arguments.length != types.length) {
					return false;
				}
				let constrained = true;
				arguments.forEach((value, index) => {
					if (isNullable(types[index]) && value == null) {
						return;
					} else if (value instanceof removeNullable(types[index]) || Object.getPrototypeOf(value) == removeNullable(types[index]).prototype) {
						return;
					}
					constrained = false;
				});
				return constrained;
			},
			function constrainedWithAndThrow(...types) {
				if (!this.constrainedWith(...types)) {
					throw 'The function arguments should match up the parameter types.';
				}
			}
		].bindTo(Object.prototype);
	})();
	/* { control-flow } */
	(() => {
		let loop = [];
		setInterval(() => {
			loop = loop.filter((value) => {
				return value.callback != null;
			});
			loop.forEach((value) => {
				if (value.deferred < performance.now()) {
					value.callback.call(null);
					value.callback = null;
				}
			});
		}, 5);
		[
			function defer(timeout) {
				arguments.constrainedWithAndThrow(Number);
				return new Promise((resolve) => {
					loop.push({
						deferred: performance.now() + timeout,
						callback: () => {
							resolve();
						}
					});
				});
			}
		].bindTo(window);
	})();
	await defer(5);
	/* constructor() */
	function LineNodeWrapper(node) {
		Object.defineProperty(this, 'Self', {
			value: node,
			writable: false,
			configurable: false,
		});
		Object.defineProperty(this, 'SpanNodes', {
			configurable: false,
			get: function getter() {
				return Array.from(node.childNodes).filter((value) => {
					return value.nodeName == 'span'.toUpperCase();
				});
			}
		});
		Object.defineProperty(this, 'LastSpanNode', {
			configurable: false,
			get: function getter() {
				return this.SpanNodes.length == 0 ? null : this.SpanNodes[this.SpanNodes.length - 1];
			}
		});
	}
	[
		function Console() {
			if (!new.target) {
				return;
			}
			arguments.constrainedWithAndThrow();
			Object.defineProperty(this, 'ConsoleNode', {
				value: document.createElement('console'),
				writable: false,
				configurable: false
			});
			this.ConsoleNode.setAttribute('foreground', 'dark-white');
			this.ConsoleNode.setAttribute('background', 'default');
			this.ConsoleNode.setAttribute('scheme', 'campbell');
			Object.defineProperty(this, 'BufferNode', {
				value: document.createElement('buffer'),
				writable: false,
				configurable: false
			});
			this.BufferNode.classList.add('lock');
			this.BufferNode.addEventListener('scroll', () => {
				this.BufferNode.classList.remove('lock');
			});
			this.BufferNode.addEventListener('scrollend', () => {
				if (Math.round(this.BufferNode.scrollTop) == this.BufferNode.scrollHeight - this.BufferNode.clientHeight) {
					this.BufferNode.classList.add('lock');
				} else {
					this.BufferNode.classList.remove('lock');
				}
			});
			this.ConsoleNode.append(this.BufferNode);
			Object.defineProperty(this, 'ControlNode', {
				value: document.createElement('control'),
				writable: false,
				configurable: false
			});
			this.ConsoleNode.append(this.ControlNode);
			Object.defineProperty(this, 'CanType', {
				get: () => {
					return !this.InputNode.readOnly;
				},
				set: (value) => {
					this.InputNode.readOnly = !value;
					this.ButtonNode.disabled = !value;
				},
				configurable: false,
				enumerable: false
			});
			Object.defineProperty(this, 'ForAnyKeyType', {
				value: () => {
					if (this.CanType == true) {
						this.ConsoleNode.removeAttribute('for-any-key');
						this.InputNode.value = '';
						this.CanType = false;
					}
				},
				writable: false,
				configurable: false,
				enumerable: false
			});
			Object.defineProperty(this, 'ReadLineType', {
				value: () => {
					if (this.CanType == true) {
						this.pushInput(this.InputNode.value);
						this.InputNode.value = '';
						this.CanType = false;
					}
				},
				writable: false,
				configurable: false,
				enumerable: false
			});
			Object.defineProperty(this, 'InputNode', {
				value: document.createElement('input'),
				writable: false,
				configurable: false
			});
			this.InputNode.type = 'text';
			this.InputNode.placeholder = 'type in something for interacting with the console . . . . .';
			this.InputNode.addEventListener('keydown', (e) => {
				if (this.ConsoleNode.hasAttribute('for-any-key')) {
					this.ForAnyKeyType();
				} else if (e.code == 'Enter') {
					this.ReadLineType();
				}
			});
			this.ControlNode.append(this.InputNode);
			Object.defineProperty(this, 'ButtonNode', {
				value: document.createElement('button'),
				writable: false,
				configurable: false
			});
			this.ButtonNode.addEventListener('click', () => {
				if (this.ConsoleNode.hasAttribute('for-any-key')) {
					this.ForAnyKeyType();
				} else {
					this.ReadLineType();
				}
			});
			this.ControlNode.append(this.ButtonNode);
			this.CanType = false;
			Object.defineProperty(this, 'LineNodes', {
				configurable: false,
				get: function getter() {
					return Array.from(this.BufferNode.childNodes).filter((value) => {
						return value.nodeName == 'line'.toUpperCase();
					}).map((value) => {
						return new LineNodeWrapper(value);
					});
				}
			});
			Object.defineProperty(this, 'LastLineNode', {
				configurable: false,
				get: function getter() {
					return this.LineNodes.length == 0 ? null : this.LineNodes[this.LineNodes.length - 1];
				}
			});
			Object.defineProperty(this, 'istream', {
				value: [],
				writable: true,
				configurable: false,
				enumerable: false
			});
			Object.defineProperty(this, 'readed', {
				value: 0,
				writable: true,
				configurable: false,
				enumerable: false
			});
			Object.defineProperty(this, 'counted', {
				value: 0,
				writable: true,
				configurable: false,
				enumerable: false
			});
		}
	].bindTo(window);
	Object.defineProperty(Console, 'Themes', {
		value: [
			'campbell',
			'campbell-powershell',
			'solarized-dark',
			'solarized',
			'solarized-light',
			'tango-dark',
			'tango',
			'tango-light',
			'gnome',
			'linux',
			'xterm',
			'rxvt',
			'vintage'
		],
		writable: false,
		configurable: false
	});
	Object.freeze(Console.Themes);
	Object.defineProperty(Console.prototype, 'scheme', {
		configurable: false,
		get: function getter() {
			return this.ConsoleNode.getAttribute('scheme');
		},
		set: function setter(scheme) {
			this.ConsoleNode.setAttribute('scheme', scheme);
		}
	});
	[
		function getForegroundColor() {
			arguments.constrainedWithAndThrow();
			return this.ConsoleNode.getAttribute('foreground');
		},
		function getBackgroundColor() {
			arguments.constrainedWithAndThrow();
			return this.ConsoleNode.getAttribute('background');
		},
		function setForegroundColor(color) {
			arguments.constrainedWithAndThrow(String);
			return this.ConsoleNode.setAttribute('foreground', color);
		},
		function setBackgroundColor(color) {
			arguments.constrainedWithAndThrow(String);
			return this.ConsoleNode.setAttribute('background', color);
		},
		function write(content) {
			arguments.constrainedWithAndThrow(String);
			content.split('\n').forEach((value, index) => {
				if (this.LineNodes.length == 0 || index > 0) {
					this.BufferNode.append(document.createElement('line'));
					if (this.BufferNode.classList.contains('lock')) {
						requestAnimationFrame(() => {
							this.BufferNode.scrollTo(this.BufferNode.scrollLeft, this.BufferNode.scrollHeight - this.BufferNode.clientHeight);
						});
					}
				}
				if (this.LastLineNode.SpanNodes.length == 0 || this.LastLineNode.LastSpanNode.getAttribute('foreground') != this.ConsoleNode.getAttribute('foreground') || this.LastLineNode.LastSpanNode.getAttribute('background') != this.ConsoleNode.getAttribute('background')) {
					let SpanNode = document.createElement('span');
					SpanNode.setAttribute('foreground', this.ConsoleNode.getAttribute('foreground'));
					SpanNode.setAttribute('background', this.ConsoleNode.getAttribute('background'));
					this.LastLineNode.Self.append(SpanNode);
				}
				this.LastLineNode.LastSpanNode.textContent += value;
			});
		},
		function writeLine(content) {
			arguments.constrainedWithAndThrow(String);
			this.write(content);
			this.write("\n");
		},
		async function read() {
			arguments.constrainedWithAndThrow();
			let capturedCounted = this.counted;
			this.counted++;
			while (this.readed >= this.istream.length || capturedCounted != this.readed) {
				this.CanType = true;
				await defer(5);
			}
			let output = this.istream[this.readed++];
			if (this.readed == this.counted) {
				this.istream = [];
				this.readed = 0;
				this.counted = 0;
			}
			return output;
		},
		function pushInput(content) {
			arguments.constrainedWithAndThrow(String);
			this.istream.push(content);
		},
		function clear() {
			arguments.constrainedWithAndThrow();
			this.LineNodes.forEach((value) => {
				value.Self.remove();
			});
		},
		async function pressAnyKey() {
			arguments.constrainedWithAndThrow();
			this.ConsoleNode.setAttribute('for-any-key', '');
			this.CanType = true;
			while (this.ConsoleNode.hasAttribute('for-any-key')) {
				await defer(5);
			}
		},
		function registerReaded(line) {
			arguments.constrainedWithAndThrow(String);
			this.writeLine(line);
			this.ConsoleNode.setAttribute('read', line);
		},
		function resolveReaded() {
			arguments.constrainedWithAndThrow();
			return this.ConsoleNode.getAttribute('read');
		},
		function bindTo(node) {
			arguments.constrainedWithAndThrow(Element);
			node.append(this.ConsoleNode);
		}
	].bindTo(Console.prototype);
	/* { functionality } */
	[
		function getTitle() {
			return document.title;
		},
		function setTitle(title) {
			document.title = title;
		}
	].bindTo(window);
	/* { event-dispatcher } */
	let isLoaded = false;
	let structuredTag = () => {
		Object.defineProperty(window, 'iostream', {
			value: new Console(),
			writable: false,
			configurable: false
		});
		window.iostream.bindTo(document.body);
		let scriptNode = document.createElement('script');
		scriptNode.src = 'Sedenion.js';
		scriptNode.defer = true;
		document.head.append(scriptNode);
	};
	let formedStyle = () => {
	};
	let delegate = () => {
		if (document.readyState == 'complete') {
			if (isLoaded == false) {
				structuredTag();
				isLoaded = true;
			}
			formedStyle();
		}
	};
	while (true) {
		await defer(5);
		delegate();
	}
})();

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
			function send(callback, timeout) {
				arguments.constrainedWithAndThrow(Function, Number);
				loop.push({
					deferred: performance.now() + timeout,
					callback: callback
				});
			},
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
			this.ConsoleNode.setAttribute('foreground', 'gray');
			this.ConsoleNode.setAttribute('background', 'default');
			this.ConsoleNode.setAttribute('scheme', 'campbell');
			Object.defineProperty(this, 'BufferNode', {
				value: document.createElement('buffer'),
				writable: false,
				configurable: false
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
	[
		function removeSpace() {
			arguments.constrainedWithAndThrow();
			return this.replace(/\t/g, '').replace(/\r/g, '').replace(/\n/g, '').replace(/\f/g, '')
				.replace(/\u0020/g, '')
				.replace(/\u00A0/g, '')
				.replace(/\u1680/g, '')
				.replace(/\u180E/g, '')
				.replace(/\u2000/g, '')
				.replace(/\u2001/g, '')
				.replace(/\u2002/g, '')
				.replace(/\u2003/g, '')
				.replace(/\u2004/g, '')
				.replace(/\u2005/g, '')
				.replace(/\u2006/g, '')
				.replace(/\u2007/g, '')
				.replace(/\u2008/g, '')
				.replace(/\u2009/g, '')
				.replace(/\u200A/g, '')
				.replace(/\u200B/g, '')
				.replace(/\u202F/g, '')
				.replace(/\u205F/g, '')
				.replace(/\u3000/g, '')
				.replace(/\uFEFF/g, '');
		}
	].bindTo(String.prototype);
	Object.defineProperty(Console, 'Colors', {
		value: [
			'default',
			'black',
			'dark-blue',
			'dark-green',
			'dark-cyan',
			'dark-red',
			'dark-magenta',
			'dark-yellow',
			'gray',
			'dark-gray',
			'blue',
			'green',
			'cyan',
			'red',
			'magenta',
			'yellow',
			'white',
		],
		writable: false,
		configurable: false
	});
	Object.freeze(Console.Colors);
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
					if (Math.round(this.BufferNode.scrollTop) == this.BufferNode.scrollHeight - this.BufferNode.clientHeight) {
						send(() => {
							this.BufferNode.scrollTo(this.BufferNode.scrollLeft, this.BufferNode.scrollHeight - this.BufferNode.clientHeight);
						}, 5);
					}
					this.BufferNode.append(document.createElement('line'));
				}
				if (value.removeSpace() != '') {
					let colorStateChanged = () => {
						return this.LastLineNode.LastSpanNode.getAttribute('foreground') != this.ConsoleNode.getAttribute('foreground') || this.LastLineNode.LastSpanNode.getAttribute('background') != this.ConsoleNode.getAttribute('background');
					};
					if (this.LastLineNode.SpanNodes.length == 0 || colorStateChanged()) {
						let SpanNode = document.createElement('span');
						this.LastLineNode.Self.append(SpanNode);
					}
					if (this.LastLineNode.LastSpanNode.textContent == '' || colorStateChanged()) {
						this.LastLineNode.LastSpanNode.setAttribute('foreground', this.ConsoleNode.getAttribute('foreground'));
						this.LastLineNode.LastSpanNode.setAttribute('background', this.ConsoleNode.getAttribute('background'));
					}
					if (Math.round(this.BufferNode.scrollTop) == this.BufferNode.scrollHeight - this.BufferNode.clientHeight) {
						send(() => {
							this.BufferNode.scrollTo(this.BufferNode.scrollLeft, this.BufferNode.scrollHeight - this.BufferNode.clientHeight);
						}, 5);
					}
					this.LastLineNode.LastSpanNode.textContent += value;
				}
			});
		},
		function writeLine(content) {
			arguments.constrainedWithAndThrow(String);
			this.write(content);
			this.write('\n');
		},
		function writeWithColorCodes(content) {
			arguments.constrainedWithAndThrow(String);
			let foreground = this.ConsoleNode.getAttribute('foreground');
			let background = this.ConsoleNode.getAttribute('background');
			content.split('\n').forEach((value, index) => {
				if (this.LineNodes.length == 0 || index > 0) {
					if (Math.round(this.BufferNode.scrollTop) == this.BufferNode.scrollHeight - this.BufferNode.clientHeight) {
						send(() => {
							this.BufferNode.scrollTo(this.BufferNode.scrollLeft, this.BufferNode.scrollHeight - this.BufferNode.clientHeight);
						}, 5);
					}
					this.BufferNode.append(document.createElement('line'));
				}
				if (value.removeSpace() != '') {
					value.split('\\').forEach((value, index) => {
						if (index % 2 == 1) {
							if (value.substring(0, 11) == 'foreground:') {
								foreground = value.substring(11, value.length);
							} else if (value.substring(0, 11) == 'background:') {
								background = value.substring(11, value.length);
							} else if (value.substring(0, 6) == 'title:') {
								setTitle(value.substring(6, value.length));
							}
							let SpanNode = document.createElement('span');
							SpanNode.setAttribute('foreground', foreground);
							SpanNode.setAttribute('background', background);
							if (this.LastLineNode.LastSpanNode.textContent.removeSpace() == '') {
								SpanNode.textContent = this.LastLineNode.LastSpanNode.textContent;
								this.LastLineNode.LastSpanNode.remove();
							}
							this.LastLineNode.Self.append(SpanNode);
						} else {
							if (this.LastLineNode.SpanNodes.length == 0) {
								let SpanNode = document.createElement('span');
								SpanNode.setAttribute('foreground', foreground);
								SpanNode.setAttribute('background', background);
								this.LastLineNode.Self.append(SpanNode);
							}
							if (Math.round(this.BufferNode.scrollTop) == this.BufferNode.scrollHeight - this.BufferNode.clientHeight) {
								send(() => {
									this.BufferNode.scrollTo(this.BufferNode.scrollLeft, this.BufferNode.scrollHeight - this.BufferNode.clientHeight);
								}, 5);
							}
							this.LastLineNode.LastSpanNode.textContent += value;
						}
					});
				}
			});
			this.ConsoleNode.setAttribute('foreground', foreground);
			this.ConsoleNode.setAttribute('background', background);
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
			this.LineNodes.forEach(async (value) => {
				value.Self.remove();
				await defer(5);
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
		async function operateExit(code) {
			this.ConsoleNode.setAttribute('foreground', 'gray');
			this.ConsoleNode.setAttribute('background', 'default');
			this.writeLine("");
			this.writeLine("   The program ended with a return code " + code.toString() + ".");
			this.writeLine("");
			this.writeLine("      >> Press any key to continue with restart the program . . .   ");
			await this.pressAnyKey();
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
		/* [iostream] */
		Object.defineProperty(window, 'iostream', {
			value: new Console(),
			writable: false,
			configurable: false
		});
		window.iostream.bindTo(document.body);
		document.body.classList.add('no-text');
		let scriptNode = document.createElement('script');
		scriptNode.src = 'Sedenion.js';
		scriptNode.defer = true;
		document.head.append(scriptNode);
	};
	let formedStyle = () => {
		/* .no-text */
		Array.from(document.getElementsByClassName('no-text')).map((value) => {
			return value.childNodes;
		}).forEach((value) => {
			value.forEach((value) => {
				if (value.nodeName == '#text') {
					value.textContent = '';
				}
			});
		});
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

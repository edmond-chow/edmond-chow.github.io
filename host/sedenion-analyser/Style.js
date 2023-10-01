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
			async function forEachAsync(delegate) {
				for (let i = 0; i < this.length; i++) {
					delegate.call(null, this[i], i, this);
					await defer(0);
				}
			}
		].bindTo(Array.prototype);
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
				if (!window.constrained) {
					return true;
				}
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
				if (constrained && !this.constrainedWith(...types)) {
					throw 'The function arguments should match up the parameter types.';
				}
			}
		].bindTo(Object.prototype);
	})();
	Object.defineProperty(window, 'constrained', {
		value: false,
		writable: true,
		configurable: false,
	});
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
	await defer(0);
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
					if (value == true && this.focused == false) {
						this.InputNode.focus();
					}
					this.focused = value;
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
					let value = this.InputNode.value;
					if (value.substring(0, 8) == '$scheme ') {
						this.Scheme = value.substring(8, value.length);
						this.writeWithColorCodes('\n\\foreground:gray\\ &   \\foreground:white\\' + value).then(() => {
							this.BufferNode.append(this.LastLineNode.Self.previousElementSibling);
						});
						this.InputNode.value = '';
						return;
					}
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
			Object.defineProperty(this, 'received', {
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
			Object.defineProperty(this, 'scroll', {
				value: false,
				writable: true,
				configurable: false,
				enumerable: false
			});
			Object.defineProperty(this, 'focused', {
				value: false,
				writable: true,
				configurable: false,
				enumerable: false
			});
		}
	].bindTo(window);
	Object.defineProperty(Console, 'Colors', {
		value: [
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
	Console.Colors[0xFF] = 'default';
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
	Object.defineProperty(Console.prototype, 'Scheme', {
		configurable: false,
		get: function getter() {
			return this.ConsoleNode.getAttribute('scheme');
		},
		set: function setter(Scheme) {
			this.ConsoleNode.setAttribute('scheme', Scheme);
		}
	});
	[
		function fromConsoleColor() {
			arguments.constrainedWithAndThrow();
			return Console.Colors[this];
		}
	].bindTo(Number.prototype);
	[
		function toConsoleColor() {
			arguments.constrainedWithAndThrow();
			return Console.Colors.indexOf(this.toString());
		}
	].bindTo(String.prototype);
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
		async function write(content) {
			arguments.constrainedWithAndThrow(String);
			let Foreground = this.ConsoleNode.getAttribute('foreground');
			let Background = this.ConsoleNode.getAttribute('background');
			let Fragment = document.createDocumentFragment();
			let LineNode = null;
			let SpanNode = null;
			let NoLineNodes = this.LineNodes.length == 0;
			let Colorless = () => {
				let SpanNode = this.LastLineNode.LastSpanNode;
				return SpanNode == null ? false : SpanNode.getAttribute('foreground') == Foreground && SpanNode.getAttribute('background') == Background;
			};
			let pushNode = () => {
				this.BufferNode.append(Fragment);
				Fragment = document.createDocumentFragment();
				let Lines = this.LineNodes;
				for (let i = 0; i < Lines.length - 8192; i++) {
					Lines[i].Self.remove();
				}
				this.BufferNode.scrollTo(this.BufferNode.scrollLeft, this.BufferNode.scrollHeight - this.BufferNode.clientHeight);
			};
			await content.split('\n').forEachAsync((value, index) => {
				if ((NoLineNodes ? index : index - 1) % 512 == 0) {
					pushNode();
				}
				let AddSpan = () => {
					if (value != '') {
						SpanNode = document.createElement('span');
						SpanNode.setAttribute('foreground', Foreground);
						SpanNode.setAttribute('background', Background);
						SpanNode.textContent += value;
						LineNode.append(SpanNode);
					}
				};
				if (index > 0 || NoLineNodes) {
					LineNode = document.createElement('line');
					AddSpan();
					Fragment.append(LineNode);
				} else {
					LineNode = this.LastLineNode.Self;
					if (Colorless()) {
						SpanNode = this.LastLineNode.LastSpanNode;
						SpanNode.textContent += value;
					} else {
						AddSpan();
					}
				}
			});
			pushNode();
		},
		async function writeLine(content) {
			arguments.constrainedWithAndThrow(String);
			await this.write(content + '\n');
		},
		async function writeWithColorCodes(content) {
			arguments.constrainedWithAndThrow(String);
			let Foreground = this.ConsoleNode.getAttribute('foreground');
			let Background = this.ConsoleNode.getAttribute('background');
			let Title = getTitle();
			let Fragment = document.createDocumentFragment();
			let LineNode = null;
			let SpanNode = null;
			let NoLineNodes = this.LineNodes.length == 0;
			let Colorless = () => {
				let SpanNode = this.LastLineNode.LastSpanNode;
				return SpanNode == null ? false : SpanNode.getAttribute('foreground') == Foreground && SpanNode.getAttribute('background') == Background;
			};
			let pushNode = () => {
				this.BufferNode.append(Fragment);
				Fragment = document.createDocumentFragment();
				let Lines = this.LineNodes;
				for (let i = 0; i < Lines.length - 8192; i++) {
					Lines[i].Self.remove();
				}
				setTitle(Title);
				this.BufferNode.scrollTo(this.BufferNode.scrollLeft, this.BufferNode.scrollHeight - this.BufferNode.clientHeight);
			};
			await content.split('\n').forEachAsync((value, index) => {
				if ((NoLineNodes ? index : index - 1) % 512 == 0) {
					pushNode();
				}
				let NonFirstLine = true;
				if (index > 0 || NoLineNodes) {
					LineNode = document.createElement('line');
					Fragment.append(LineNode);
				} else {
					LineNode = this.LastLineNode.Self;
					NonFirstLine = false;
				}
				if (value != '') {
					value.split('\\').forEach((value, index) => {
						if (index % 2 == 1) {
							if (value.substring(0, 11) == 'foreground:') {
								Foreground = value.substring(11, value.length);
							} else if (value.substring(0, 11) == 'background:') {
								Background = value.substring(11, value.length);
							} else if (value.substring(0, 6) == 'title:') {
								Title = value.substring(6, value.length);
							}
						} else if (NonFirstLine == false && index == 0 && Colorless()) {
							SpanNode = this.LastLineNode.LastSpanNode;
							SpanNode.textContent += value;
						} else if (value != '') {
							SpanNode = document.createElement('span');
							SpanNode.setAttribute('foreground', Foreground);
							SpanNode.setAttribute('background', Background);
							SpanNode.textContent += value;
							LineNode.append(SpanNode);
						}
					});
				}
			});
			pushNode();
			this.ConsoleNode.setAttribute('foreground', Foreground);
			this.ConsoleNode.setAttribute('background', Background);
		},
		async function read() {
			arguments.constrainedWithAndThrow();
			let capturedCounted = this.counted;
			this.counted++;
			while (this.received >= this.istream.length || capturedCounted != this.received) {
				this.CanType = true;
				await defer(0);
			}
			let output = this.istream[this.received++];
			if (this.received == this.counted) {
				this.istream = [];
				this.received = 0;
				this.counted = 0;
			}
			return output;
		},
		function pushInput(content) {
			arguments.constrainedWithAndThrow(String);
			this.istream.push(content);
		},
		async function clear() {
			arguments.constrainedWithAndThrow();
			await this.LineNodes.forEachAsync((value) => {
				value.Self.remove();
			});
		},
		async function pressAnyKey() {
			arguments.constrainedWithAndThrow();
			this.ConsoleNode.setAttribute('for-any-key', '');
			this.CanType = true;
			while (this.ConsoleNode.hasAttribute('for-any-key')) {
				await defer(0);
			}
		},
		async function completed(code) {
			this.ConsoleNode.setAttribute('foreground', 'gray');
			this.ConsoleNode.setAttribute('background', 'default');
			await this.clear();
			await this.writeLine('');
			await this.writeLine('   The program completed with a return code ' + code.toString() + '.');
			await this.writeLine('');
			await this.writeLine('      >> Press any key to continue with restart the program . . .   ');
			await this.pressAnyKey();
			await this.clear();
		},
		async function terminated(code) {
			this.ConsoleNode.setAttribute('foreground', 'gray');
			this.ConsoleNode.setAttribute('background', 'default');
			await this.clear();
			await this.writeLine('');
			await this.writeLine('   The program terminated with a return code ' + code.toString() + '.');
			await this.writeLine('');
			await this.writeLine('      >> Press any key to continue with restart the program . . .   ');
			await this.pressAnyKey();
			await this.clear();
		},
		function bindTo(node) {
			arguments.constrainedWithAndThrow(Element);
			node.append(this.ConsoleNode);
		}
	].bindTo(Console.prototype);
	/* { functionality } */
	[
		function getUTF8String(fnScope, jsString) {
			arguments.constrainedWithAndThrow(Function.toNullable(), String.toNullable());
			let myScope = fnScope == null ? getUTF8String : fnScope;
			let stringObject = jsString == null ? "" : jsString;
			let stringLength = lengthBytesUTF8(stringObject) + 1;
			if (!myScope.bufferSize || myScope.bufferSize < stringLength) {
				if (myScope.bufferSize) {
					_free(myScope.bufferData);
				}
				myScope.bufferData = _malloc(stringLength);
				myScope.bufferSize = stringLength;
			}
			stringToUTF8(stringObject, myScope.bufferData, myScope.bufferSize);
			return myScope.bufferData;
		},
		function getTitle() {
			arguments.constrainedWithAndThrow();
			return document.title;
		},
		function setTitle(title) {
			arguments.constrainedWithAndThrow(String);
			document.title = title;
		}
	].bindTo(window);
	/* { event-dispatcher } */
	let isLoaded = false;
	let structuredTag = async () => {
		/* [iostream] */
		Object.defineProperty(window, 'iostream', {
			value: new Console(),
			writable: false,
			configurable: false
		});
		window.iostream.bindTo(document.body);
		iostream.InputNode.focus();
		let initialized = false;
		Module = {
			onRuntimeInitialized: () => {
				initialized = true;
			},
			onAbort: () => {
				Module.__Z12abort_unwindv();
			}
		};
		let scriptNode = document.createElement('script');
		scriptNode.src = 'Sedenion.js';
		scriptNode.defer = true;
		document.head.append(scriptNode);
		await new Promise(async (resolve) => {
			while (initialized == false) {
				await defer(0);
			}
			resolve();
		});
		/* .no-text */
		document.body.classList.add('no-text');
	};
	let formedStyle = async () => {
		/* [iostream] */
		if (keepRuntimeAlive() == false) {
			if (ABORT == false) {
				await iostream.completed(EXITSTATUS);
				Module._main();
			} else {
				await iostream.terminated(EXITSTATUS);
				document.location = document.location;
			}
		}
		/* .no-text */
		await Array.from(document.getElementsByClassName('no-text')).map((value) => {
			return value.childNodes;
		}).forEachAsync((value) => {
			value.forEach((value) => {
				if (value.nodeName == '#text') {
					value.textContent = '';
				}
			});
		});
	};
	let delegate = async () => {
		if (document.readyState == 'complete') {
			if (isLoaded == false) {
				await structuredTag();
				isLoaded = true;
			}
			await formedStyle();
		}
	};
	while (true) {
		await defer(5);
		await delegate();
	}
})();

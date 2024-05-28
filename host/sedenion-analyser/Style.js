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
					await defer();
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
				if (window.constrained && !this.constrainedWith(...types)) {
					throw 'The function arguments should match up the parameter types.';
				}
			}
		].bindTo(Object.prototype);
		[
			function defineField(instance, key, data, mutable = true) {
				[instance, key].constrainedWithAndThrow(Object, String);
				Object.defineProperty(instance, key, {
					value: data,
					writable: mutable,
					enumerable: false,
					configurable: false,
				});
			},
			function defineSharedField(instance, key, data, mutable = false) {
				[instance, key].constrainedWithAndThrow(Object, String);
				Object.defineProperty(instance, key, {
					value: data,
					writable: mutable,
					enumerable: true,
					configurable: false,
				});
			},
			function defineSharedProperty(instance, key, getter, setter) {
				[instance, key].constrainedWithAndThrow(Object, String);
				Object.defineProperty(instance, key, {
					get: getter,
					set: setter,
					enumerable: true,
					configurable: false,
				});
			},
			function defineProperty(instance, key, getter, setter) {
				[instance, key].constrainedWithAndThrow(Object, String);
				Object.defineProperty(instance, key, {
					get: getter,
					set: setter,
					enumerable: false,
					configurable: false,
				});
			}
		].bindTo(window);
	})();
	defineSharedField(window, 'constrained', false, true);
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
		}, 0);
		[
			function send(callback, timeout = 0) {
				arguments.constrainedWithAndThrow(Function, Number);
				loop.push({
					deferred: performance.now() + timeout,
					callback: callback
				});
			},
			function defer(timeout = 0) {
				arguments.constrainedWithAndThrow(Number);
				return new Promise((resolve) => {
					loop.push({
						deferred: performance.now() + timeout,
						callback: resolve
					});
				});
			}
		].bindTo(window);
	})();
	await defer();
	/* constructor() */
	function LineNodeWrapper(node) {
		defineSharedField(this, 'Self', node);
		defineSharedProperty(this, 'SpanNodes', () => {
			return node == null ? null : Array.from(this.Self.childNodes).filter((value) => {
				return value.nodeName == 'span'.toUpperCase();
			});
		});
		defineSharedProperty(this, 'LastSpanNode', () => {
			return node == null || this.SpanNodes.length == 0 ? null : this.SpanNodes[this.SpanNodes.length - 1];
		});
	}
	[
		function Console() {
			if (!new.target) {
				return;
			}
			arguments.constrainedWithAndThrow();
			defineSharedField(this, 'ConsoleNode', document.createElement('console'));
			this.ConsoleNode.setAttribute('foreground', 'gray');
			this.ConsoleNode.setAttribute('background', 'default');
			this.ConsoleNode.setAttribute('scheme', 'campbell');
			let BufferNode = document.createElement('buffer');
			this.ConsoleNode.prepend(BufferNode);
			defineField(this, 'ClearBufferNode', () => {
				document.createDocumentFragment().append(BufferNode);
				BufferNode = document.createElement('buffer');
				this.ConsoleNode.prepend(BufferNode);
			}, false);
			defineSharedProperty(this, 'BufferNode', () => {
				return BufferNode;
			});
			defineSharedField(this, 'ControlNode', document.createElement('control'));
			this.ConsoleNode.append(this.ControlNode);
			let typing = false;
			let DataContentNode = document.createElement('data-content');
			defineProperty(this, 'CanType', () => {
				return typing;
			}, (value) => {
				this.InputNode.readOnly = !value;
				this.ButtonNode.disabled = !value;
				if (value && !typing) {
					this.LastLineNode.LastSpanNode?.append(DataContentNode);
					this.InputNode.focus();
				} else if (!value && typing) {
					document.createDocumentFragment().append(DataContentNode);
					DataContentNode.textContent = '';
				}
				typing = value;
			});
			defineField(this, 'ForAnyKeyType', async () => {
				if (this.CanType) {
					this.InputNode.value = '';
					this.keyfreeze = false;
					this.CanType = false;
				}
			}, false);
			defineField(this, 'ReadLineType', async () => {
				let value = this.InputNode.value;
				if (value.substring(0, 8) == '$scheme ') {
					this.Scheme = value.substring(8, value.length);
					this.InputNode.value = '';
					await this.write('\n\\f7\\ &   \\ff\\');
					await this.write(value, false);
					this.BufferNode.append(this.LastLineNode.Self.previousElementSibling);
				} else if (this.CanType) {
					this.pushInput(value + '\n');
					this.InputNode.value = '';
					this.CanType = false;
				}
			}, false);
			defineSharedField(this, 'InputNode', document.createElement('input'));
			this.InputNode.type = 'text';
			this.InputNode.placeholder = 'type in something for interacting with the console . . . . .';
			this.InputNode.addEventListener('keydown', async (e) => {
				if (this.keyfreeze) {
					this.ForAnyKeyType();
				} else if (e.code == 'Enter') {
					await this.ReadLineType();
				}
			});
			this.InputNode.addEventListener('input', async () => {
				let space = String.fromCharCode(0x200B);
				let value = this.InputNode.value;
				let content = '';
				for (let i = 0; i < value.length; i++) {
					content += value[i];
					if (value.codePointAt(i) > 0xFFFF) {
						content += value[++i];
					}
					content += space;
				}
				await defer();
				DataContentNode.textContent = content;
				this.BufferNode.scrollTo(this.BufferNode.scrollLeft, this.BufferNode.scrollHeight - this.BufferNode.clientHeight);
			});
			this.ControlNode.append(this.InputNode);
			defineSharedField(this, 'ButtonNode', document.createElement('button'));
			this.ButtonNode.addEventListener('click', async () => {
				if (this.keyfreeze) {
					this.ForAnyKeyType();
				} else {
					await this.ReadLineType();
				}
			});
			this.ControlNode.append(this.ButtonNode);
			this.CanType = false;
			defineSharedProperty(this, 'LineNodes', () => {
				return Array.from(this.BufferNode.childNodes).filter((value) => {
					return value.nodeName == 'line'.toUpperCase();
				}).map((value) => {
					return new LineNodeWrapper(value);
				});
			});
			defineSharedProperty(this, 'LastLineNode', () => {
				return this.LineNodes.length == 0 ? new LineNodeWrapper(null) : this.LineNodes[this.LineNodes.length - 1];
			});
			defineField(this, 'keyfreeze', false);
			defineField(this, 'istream', '');
			defineField(this, 'istream', '');
			defineField(this, 'icursor', '');
			defineSharedProperty(this, 'Scheme', () => {
				return this.ConsoleNode.getAttribute('scheme');
			}, (Scheme) => {
				this.ConsoleNode.setAttribute('scheme', Scheme);
			});
		}
	].bindTo(window);
	let Colors = [
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
	];
	Colors[0xFF] = 'default';
	defineSharedField(Console, 'Colors', Colors);
	Object.freeze(Console.Colors);
	let Themes = [
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
	];
	defineSharedField(Console, 'Themes', Themes);
	let GetColorCharCode = (code) => {
		if (code >= 0 && code <= 9) {
			return code + 48;
		} else if (code >= 10 && code <= 15) {
			return code + 87;
		} else if (code == 0xFF) {
			return 120;
		} else {
			throw 'The code out of range!';
		}
	};
	defineSharedField(Console, 'GetColorCharCode', GetColorCharCode);
	Object.freeze(Console.Themes);
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
		async function writeLine(content, controlized = true) {
			arguments.constrainedWithAndThrow(String, Boolean);
			await this.write(content + '\n', controlized);
		},
		async function write(content, controlized = true) {
			arguments.constrainedWithAndThrow(String, Boolean);
			let value = '';
			let count = 0;
			let config = false;
			let pending = false;
			let control = null;
			let breaking = false;
			let foreground = Console.Colors.indexOf(this.ConsoleNode.getAttribute('foreground'));
			let background = Console.Colors.indexOf(this.ConsoleNode.getAttribute('background'));
			let title = getTitle();
			let Fragment = document.createDocumentFragment();
			let LineNode = this.LastLineNode.Self;
			let SpanNode = this.LastLineNode.LastSpanNode;
			let pushNode = () => {
				this.BufferNode.append(Fragment);
				Fragment = document.createDocumentFragment();
				for (let i = 0; i < this.LineNodes.length - 8192; i++) {
					this.LineNodes[i].Self.remove();
				}
				this.ConsoleNode.setAttribute('foreground', Console.Colors[foreground]);
				this.ConsoleNode.setAttribute('background', Console.Colors[background]);
				setTitle(title);
				this.BufferNode.scrollTo(this.BufferNode.scrollLeft, this.BufferNode.scrollHeight - this.BufferNode.clientHeight);
			};
			let pushSpan = () => {
				if (value.length > 0) {
					SpanNode.textContent += value;
					value = '';
					return true;
				} else {
					return false;
				}
			};
			let newLine = () => {
				pushSpan();
				LineNode = document.createElement('line');
				Fragment.append(LineNode);
				SpanNode = null;
			};
			let newSpan = () => {
				if (SpanNode == null || pushSpan()) {
					SpanNode = document.createElement('span');
					LineNode.append(SpanNode);
				}
				SpanNode.setAttribute('foreground', Console.Colors[foreground]);
				SpanNode.setAttribute('background', Console.Colors[background]);
			};
			let endUp = () => {
				pushSpan();
				pushNode();
			};
			let throwNow = () => {
				endUp();
				throw 'The control code doesn\'t match up!';
			};
			let isColorChanged = () => {
				if (SpanNode.getAttribute('foreground') != Console.Colors[foreground]) {
					return true;
				} else if (SpanNode.getAttribute('background') != Console.Colors[background]) {
					return true;
				} else {
					return false;
				}
			};
			let getColorCode = (code) => {
				if (code >= 48 && code <= 57) {
					return code - 48;
				} else if (code >= 65 && code <= 70) {
					return code - 55;
				} else if (code >= 97 && code <= 102) {
					return code - 87;
				} else if (code == 88 || code == 120) {
					return 0xFF;
				} else {
					throwNow();
				}
			};
			let BreakNow = (code) => {
				let Unbreakables = [33, 34, 36, 39, 40, 41, 44, 46, 47, 58, 59, 63, 91, 92, 93, 123, 125];
				for (let i = 0; i < Unbreakables.length; i++) {
					if (Unbreakables[i] == code) {
						return true;
					}
				}
				return false;
			};
			if (LineNode == null) {
				newLine();
			} else if (SpanNode == null) {
				newSpan();
			}
			for (let i = 0; i < content.length; i++) {
				if (content[i] == '\n') {
					if (config && pending) {
						throwNow();
					} else if (count >= 512) {
						count = 0;
						pushNode();
						await defer();
					} else {
						count += 1;
					}
					newLine();
					newSpan();
					config = false;
					control = null;
					breaking = false;
				} else if (controlized && content[i] == '\\') {
					if (config) {
						if (SpanNode != null && isColorChanged()) {
							newSpan();
						} else if (control == null) {
							newSpan();
							value = '\\';
						} else if (pending) {
							throwNow();
						}
					}
					config = !config;
					control = null;
					breaking = value == '\\';
				} else if (control != null) {
					pending = false;
					if (control == '+t') {
						title += content[i];
					} else if (control.toLowerCase() == 't') {
						title = content[i];
						control = '+t';
					} else if (control.toLowerCase() == 'f') {
						foreground = getColorCode(content.charCodeAt(i));
						control = '';
					} else if (control.toLowerCase() == 'b') {
						background = getColorCode(content.charCodeAt(i));
						control = '';
					} else {
						throwNow();
					}
				} else if (config) {
					pending = true;
					control = content[i];
				} else {
					let release = BreakNow(content.charCodeAt(i));
					if (breaking || release) {
						newSpan();
						breaking = release;
					}
					value += content[i];
				}
			}
			if (config && pending) {
				throwNow();
			} else {
				endUp();
			}
		},
		async function readLine() {
			arguments.constrainedWithAndThrow();
			let result = '';
			while (true) {
				while (this.icursor < this.istream.length) {
					let char = this.istream[this.icursor++];
					if (char == '\n') {
						return result;
					} else {
						result += char;
					}
				}
				this.istream = '';
				this.icursor = 0;
				this.CanType = true;
				await defer();
			}
		},
		async function read(unicode = true) {
			arguments.constrainedWithAndThrow(Boolean);
			let result = '';
			while (true) {
				if (this.icursor < this.istream.length) {
					if (unicode && this.istream.codePointAt(this.icursor) > 0xFFFF) {
						result += this.istream[this.icursor++];
					}
					result += this.istream[this.icursor++];
					return result;
				}
				this.istream = '';
				this.icursor = 0;
				this.CanType = true;
				await defer();
			}
		},
		function putBack(content) {
			arguments.constrainedWithAndThrow(String);
			this.istream = content + this.istream.substring(this.icursor);
			this.icursor = 0;
		},
		function pushInput(content) {
			arguments.constrainedWithAndThrow(String);
			this.istream += content;
		},
		function clear() {
			arguments.constrainedWithAndThrow();
			this.ClearBufferNode();
		},
		async function pressAnyKey() {
			arguments.constrainedWithAndThrow();
			this.keyfreeze = true;
			this.CanType = true;
			while (this.keyfreeze) {
				await defer();
			}
		},
		async function completed(code) {
			arguments.constrainedWithAndThrow(Number);
			this.ConsoleNode.setAttribute('foreground', 'gray');
			this.ConsoleNode.setAttribute('background', 'default');
			this.clear();
			await this.writeLine('');
			await this.writeLine('   The program completed with a return code ' + code.toString() + '.');
			await this.writeLine('');
			await this.writeLine('      >> Press any key to continue with restart the program . . .   ');
			await this.pressAnyKey();
			this.clear();
		},
		async function terminated() {
			arguments.constrainedWithAndThrow();
			this.ConsoleNode.setAttribute('foreground', 'gray');
			this.ConsoleNode.setAttribute('background', 'default');
			this.clear();
			await this.writeLine('');
			await this.writeLine('   The program terminated with no return codes.');
			await this.writeLine('');
			await this.writeLine('      >> Press any key to continue with restart the program . . .   ');
			await this.pressAnyKey();
			this.clear();
		},
		function bindTo(node) {
			arguments.constrainedWithAndThrow(Element);
			node.append(this.ConsoleNode);
		}
	].bindTo(Console.prototype);
	/* { functionality } */
	let getUTFString = (caller, converter, counter, width, fnScope, jsString) => {
		[fnScope, jsString].constrainedWithAndThrow(Function.toNullable(), String.toNullable());
		let myScope = fnScope == null ? caller : fnScope;
		let stringObject = jsString == null ? "" : jsString;
		let stringLength = counter(stringObject) + width;
		if (!myScope.bufferSize || myScope.bufferSize < stringLength) {
			if (myScope.bufferSize) {
				_free(myScope.bufferData);
			}
			myScope.bufferData = _malloc(stringLength);
			myScope.bufferSize = stringLength;
		}
		converter(stringObject, myScope.bufferData, myScope.bufferSize);
		return myScope.bufferData;
	};
	[
		function getUTF8String(fnScope, jsString) {
			return getUTFString(getUTF8String, stringToUTF8, lengthBytesUTF8, 1, fnScope, jsString);
		},
		function getUTF16String(fnScope, jsString) {
			return getUTFString(getUTF16String, stringToUTF16, lengthBytesUTF16, 2, fnScope, jsString);
		},
		function getUTF32String(fnScope, jsString) {
			return getUTFString(getUTF32String, stringToUTF32, lengthBytesUTF32, 4, fnScope, jsString);
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
	let isAborted = false;
	let memorySlice = null;
	let bindToAsyncify = () => {
		Asyncify.asyncPromiseHandlers = {
			reject: (e) => {
				if (e instanceof ExitStatus) {
					return handleException(e);
				}
			},
			resolve: (code) => {
				if (code instanceof Number || Object.getPrototypeOf(code) == Number.prototype) {
					try {
						var ret = code;
						exitJS(ret, true);
						return ret;
					} catch (e) {
						return handleException(e);
					}
				}
			}
		};
	};
	let structuredTag = async () => {
		/* [iostream] */
		defineSharedField(window, 'iostream', new Console());
		window.iostream.bindTo(document.body);
		iostream.InputNode.focus();
		let fetched = false;
		Module = {
			preRun: () => {
				exitRuntime = Module.__Z13invoke_atexitv;
				memorySlice = new Int8Array(wasmMemory);
				quit_ = new Function();
				fetched = true;
			},
			onRuntimeInitialized: () => {
				bindToAsyncify();
			},
			onAbort: () => {
				Module.__Z12abort_unwindv();
				isAborted = true;
				throw new ExitStatus(1);
			},
			onExit: (code) => {
				throw new ExitStatus(code);
			}
		};
		let scriptNode = document.createElement('script');
		scriptNode.src = 'Sedenion.js';
		scriptNode.defer = true;
		document.head.append(scriptNode);
		await new Promise(async (resolve) => {
			while (fetched == false) {
				await defer();
			}
			resolve();
		});
		/* .no-text */
		document.body.classList.add('no-text');
	};
	let formedStyle = async () => {
		/* [iostream] */
		if (keepRuntimeAlive() == false) {
			if (isAborted == false) {
				await iostream.completed(EXITSTATUS != undefined ? EXITSTATUS : 0);
			} else {
				await iostream.terminated();
				isAborted = false;
			}
			wasmMemory.buffer = new Int8Array(memorySlice).buffer;
			EXITSTATUS = undefined;
			___wasm_call_ctors();
			bindToAsyncify();
			callMain();
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
		await delegate();
		await defer();
	}
})();

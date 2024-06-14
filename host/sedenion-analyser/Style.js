(async () => {
	/* { binder } */
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
	/* { reflection } */
	class Nullable {
		constructor(type) {
			this.type = type;
			Object.freeze(this);
		}
	};
	[Nullable].bindTo({});
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
	/* { asynchronous } */
	class Continuation {
		constructor(resolve, condition) {
			this.resolve = resolve;
			this.condition = condition;
			this.accomplished = false;
		}
		completed() {
			return this.condition != null ? this.condition.call(null) : true;
		}
		pending() {
			return !this.accomplished;
		}
		invoke() {
			if (!this.accomplished) {
				this.resolve();
				this.accomplished = true;
			}
		}
	};
	let continuations = [];
	setInterval(() => {
		continuations = continuations.filter((continuation) => {
			return continuation.pending();
		});
		continuations.forEach((continuation) => {
			if (continuation.completed()) {
				continuation.invoke();
			}
		});
	}, 0);
	[
		function suspend(condition = null) {
			[condition].constrainedWithAndThrow(Function.toNullable());
			return new Promise((resolve) => {
				continuations.push(new Continuation(resolve, condition));
			});
		}
	].bindTo(window);
	await suspend();
	/* { property-definer } */
	[
		function hardFreeze(type, instance = window) {
			if (type instanceof Function && type.prototype instanceof Object) {
				[type].bindTo(instance);
				Object.freeze(type.prototype);
			}
		},
		function lockFields(instance, keys, mutable = true) {
			if (instance instanceof Object && [mutable].constrainedWith(Boolean)) {
				keys.forEach((key) => {
					Object.defineProperty(instance, key, { writable: mutable, enumerable: !mutable, configurable: false });
				});
			}
		},
		function shareProperties(type, keys) {
			if (type instanceof Function && type.prototype instanceof Object) {
				keys.forEach((key) => {
					Object.defineProperty(type.prototype, key, { enumerable: true, configurable: false });
				});
			}
		}
	].bindTo(window);
	/* { constructors } */
	class LineNodeWrapper {
		constructor(head) {
			this.Self = head;
			lockFields(this, ['Self'], false);
		}
		get SpanNodes() {
			return this.Self == null ? null : Array.from(this.Self.childNodes).filter((value) => {
				return value.nodeName == 'span'.toUpperCase();
			});
		}
		get LastSpanNode() {
			return this.Self == null || this.SpanNodes.length == 0 ? null : this.SpanNodes[this.SpanNodes.length - 1];
		}
	};
	shareProperties(LineNodeWrapper, ['SpanNodes', 'LastSpanNode']);
	hardFreeze(LineNodeWrapper, {});
	class Console {
		constructor() {
			this.ConsoleNode = document.createElement('console');
			this.ConsoleNode.setAttribute('foreground', 'gray');
			this.ConsoleNode.setAttribute('background', 'default');
			this.ConsoleNode.setAttribute('scheme', 'campbell');
			this.BufferNode = document.createElement('buffer');
			this.ConsoleNode.prepend(this.BufferNode);
			this.ControlNode = document.createElement('control');
			this.ConsoleNode.append(this.ControlNode);
			this.DataContentNode = document.createElement('data-content');
			this.typing = false;
			this.freezing = false;
			this.keys = '';
			this.InputNode = document.createElement('input');
			this.InputNode.type = 'text';
			this.InputNode.placeholder = 'type in something for interacting with the console . . . . .';
			this.InputNode.addEventListener('keydown', async (e) => {
				if (this.KeyFreeze) {
					let EscapeKeys = ['ContextMenu', 'Backspace', 'CapsLock', 'Control', 'Escape', 'Shift', 'Meta', 'Alt', 'Tab'];
					if (EscapeKeys.indexOf(e.key) == -1) {
						if (e.key == 'Enter') {
							this.Keys = '\n';
							this.ForAnyKeyType();
						}
					}
				} else if (this.CanType && e.key == 'Enter') {
					await this.ReadLineType();
				}
			});
			this.InputNode.addEventListener('input', async () => {
				let value = this.InputNode.value;
				if (this.KeyFreeze) {
					this.Keys = value;
					this.ForAnyKeyType();
				} else if (this.CanType) {
					let space = String.fromCharCode(0x200B);
					let content = '';
					for (let i = 0; i < value.length; i++) {
						content += value[i];
						if (value.codePointAt(i) > 0xFFFF) {
							content += value[++i];
						}
						content += space;
					}
					await suspend();
					this.DataContentNode.textContent = content;
					this.BufferNode.scrollTo(this.BufferNode.scrollLeft, this.BufferNode.scrollHeight - this.BufferNode.clientHeight);
				}
			});
			this.ControlNode.append(this.InputNode);
			this.ButtonNode = document.createElement('button');
			this.ButtonNode.addEventListener('click', async () => {
				if (this.KeyFreeze) {
					this.Keys = '\n';
					this.ForAnyKeyType();
				} else if (this.CanType) {
					await this.ReadLineType();
				}
			});
			this.ControlNode.append(this.ButtonNode);
			this.CanType = false;
			this.istream = '';
			this.icursor = 0;
			lockFields(this, ['typing', 'freezing', 'keys', 'istream', 'icursor'], true);
			lockFields(this, ['ConsoleNode', 'BufferNode', 'ControlNode', 'DataContentNode', 'InputNode', 'ButtonNode'], false);
		}
		ClearBufferNode() {
			this.BufferNode.innerText = '';
		}
		get CanType() {
			return this.typing;
		}
		set CanType(value) {
			this.InputNode.readOnly = !value;
			this.ButtonNode.disabled = !value;
			if (value && !this.typing) {
				this.LastLineNode.LastSpanNode?.append(this.DataContentNode);
				this.InputNode.focus();
			} else if (!value && this.typing) {
				document.createDocumentFragment().append(this.DataContentNode);
				this.DataContentNode.textContent = '';
			}
			this.typing = value;
		}
		get KeyFreeze() {
			return this.freezing;
		}
		set KeyFreeze(value) {
			this.CanType = value;
			if (value && !this.freezing) {
				this.keys = '';
			}
			this.freezing = value;
		}
		get Keys() {
			return this.keys;
		}
		set Keys(value) {
			if (this.freezing) {
				this.keys = value;
			}
		}
		ForAnyKeyType() {
			if (this.KeyFreeze) {
				this.InputNode.value = '';
				this.KeyFreeze = false;
			}
		}
		async ReadLineType() {
			let value = this.InputNode.value;
			if (this.CanType) {
				if (value.length > 0 && value[0] == '$') {
					let args = [];
					let temp = '';
					let wording = false;
					let quoting = false;
					let push = () => {
						if (temp.length > 0) {
							args.push(temp);
							temp = '';
						}
					};
					for (let i = 1; i < value.length; i++) {
						if (value[i] == ' ' && !quoting) {
							if (wording) {
								push();
								wording = false;
							}
							wording = false;
						} else if (value[i] == '"' && !quoting) {
							if (wording) {
								push();
								wording = false;
							}
							quoting = true;
						} else if (value[i] == '"' && quoting) {
							push();
							quoting = false;
						} else {
							temp += value[i];
							if (!quoting) {
								wording = true;
							}
						}
					}
					push();
					if (args.length == 2 && args[0] == 'scheme') {
						this.Scheme = args[1];
					}
					await this.write('\n\\f7\\ &   \\ff\\');
					await this.write(value, false);
					this.BufferNode.append(this.LastLineNode.Self.previousElementSibling);
				} else {
					if (value.length > 1 && value.substring(0, 2) == '\\$') {
						value = value.substring(1);
					}
					this.pushInput(value + '\n');
				}
				this.InputNode.value = '';
				this.CanType = false;
			}
		}
		get LineNodes() {
			return Array.from(this.BufferNode.childNodes).filter((value) => {
				return value.nodeName == 'line'.toUpperCase();
			}).map((value) => {
				return new LineNodeWrapper(value);
			});
		}
		get LastLineNode() {
			return this.LineNodes.length == 0 ? new LineNodeWrapper(null) : this.LineNodes[this.LineNodes.length - 1];
		}
		get Scheme() {
			return this.ConsoleNode.getAttribute('scheme');
		}
		set Scheme(value) {
			this.ConsoleNode.setAttribute('scheme', value);
		}
		static Colors = [
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
		]
		static Themes = [
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
		]
		static GetColorCharCode(code) {
			if (code >= 0 && code <= 9) {
				return code + 48;
			} else if (code >= 10 && code <= 15) {
				return code + 87;
			} else if (code == 0xFF) {
				return 120;
			} else {
				throw 'The code out of range!';
			}
		}
		getForegroundColor() {
			return this.ConsoleNode.getAttribute('foreground');
		}
		getBackgroundColor() {
			return this.ConsoleNode.getAttribute('background');
		}
		setForegroundColor(color) {
			[color].constrainedWithAndThrow(String);
			return this.ConsoleNode.setAttribute('foreground', color);
		}
		setBackgroundColor(color) {
			[color].constrainedWithAndThrow(String);
			return this.ConsoleNode.setAttribute('background', color);
		}
		async writeLine(content, controlized = true) {
			[content, controlized].constrainedWithAndThrow(String, Boolean);
			await this.write(content + '\n', controlized);
		}
		async write(content, controlized = true) {
			[content, controlized].constrainedWithAndThrow(String, Boolean);
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
				if (SpanNode == null) {
					return true;
				} else if (value.length > 0) {
					SpanNode.textContent += value;
					value = '';
					return true;
				} else if (SpanNode.textContent.length > 0) {
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
				if (pushSpan()) {
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
				if (SpanNode == null) {
					return true;
				} else if (SpanNode.getAttribute('foreground') != Console.Colors[foreground]) {
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
			} else if (isColorChanged()) {
				SpanNode = null;
				newSpan();
			}
			for (let i = 0; i < content.length; i++) {
				if (content[i] == '\n') {
					if (config && pending) {
						throwNow();
					} else if (count >= 512) {
						count = 0;
						pushNode();
						await suspend();
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
			await suspend();
			if (config && pending) {
				throwNow();
			} else {
				endUp();
			}
		}
		async readLine() {
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
				await suspend();
			}
		}
		async read(unicode = true) {
			[unicode].constrainedWithAndThrow(Boolean);
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
				await suspend();
			}
		}
		putBack(content) {
			[content].constrainedWithAndThrow(String);
			this.istream = content + this.istream.substring(this.icursor);
			this.icursor = 0;
		}
		pushInput(content) {
			[content].constrainedWithAndThrow(String);
			this.istream += content;
		}
		clear() {
			this.ClearBufferNode();
		}
		async pressAnyKey() {
			this.KeyFreeze = true;
			while (this.KeyFreeze) {
				await suspend();
			}
			return this.Keys;
		}
		async completed(code) {
			[code].constrainedWithAndThrow(Number);
			this.ConsoleNode.setAttribute('foreground', 'gray');
			this.ConsoleNode.setAttribute('background', 'default');
			this.clear();
			await this.writeLine('');
			await this.writeLine('   The program completed with a return code ' + code.toString() + '.');
			await this.writeLine('');
			await this.writeLine('      >> Press any key to continue with restart the program . . .   ');
			await this.pressAnyKey();
			this.clear();
		}
		async terminated() {
			this.ConsoleNode.setAttribute('foreground', 'gray');
			this.ConsoleNode.setAttribute('background', 'default');
			this.clear();
			await this.writeLine('');
			await this.writeLine('   The program terminated with no return codes.');
			await this.writeLine('');
			await this.writeLine('      >> Press any key to continue with restart the program . . .   ');
			await this.pressAnyKey();
			this.clear();
		}
		bindTo(node) {
			[node].constrainedWithAndThrow(Element);
			node.append(this.ConsoleNode);
		}
	}
	Console.Colors[0xFF] = 'default';
	Object.freeze(Console.Colors);
	Object.freeze(Console.Themes);
	shareProperties(Console, ['LineNodes', 'LastLineNode', 'Scheme', 'getForegroundColor', 'getBackgroundColor', 'setForegroundColor', 'setBackgroundColor', 'writeLine', 'write', 'readLine', 'read', 'putBack', 'pushInput', 'clear', 'pressAnyKey', 'completed', 'terminated', 'bindTo']);
	hardFreeze(Console);
	[
		function fromConsoleColor() {
			return Console.Colors[this];
		}
	].bindTo(Number.prototype);
	[
		function toConsoleColor() {
			return Console.Colors.indexOf(this.toString());
		}
	].bindTo(String.prototype);
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
			return document.title;
		},
		function setTitle(title) {
			[title].constrainedWithAndThrow(String);
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
		let iostream = new Console();
		iostream.bindTo(document.body);
		iostream.InputNode.focus();
		Object.defineProperty(window, 'iostream', { value: iostream, writable: false, enumerable: true, configurable: false });
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
				await suspend();
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
		}).forEach((value) => {
			value.forEach((value) => {
				if (value.nodeName == '#text') {
					value.textContent = '';
				}
			});
		});
	};
	let delegate = async () => {
		if (isLoaded == false) {
			await structuredTag();
			isLoaded = true;
		}
		await formedStyle();
	};
	await suspend(() => {
		return document.readyState == 'complete';
	});
	while (true) {
		await delegate();
		await suspend();
	}
})();
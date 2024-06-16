(async () => {
	/* { binder } */
	let getDescriptor = (value, enumerable) => {
		return { value: value, writable: false, enumerable: enumerable, configurable: false };
	};
	Array.prototype.bindTo = function bindTo(scope = window) {
		this.forEach((value) => {
			if (value instanceof Function) {
				Object.defineProperty(value, 'name', getDescriptor(value.name, true));
				Object.defineProperty(scope, value.name, getDescriptor(value, true));
				if (value.prototype instanceof Object) {
					Object.defineProperty(value, 'prototype', getDescriptor(value.prototype, false));
					Object.defineProperty(value.prototype, 'constructor', getDescriptor(value.prototype.constructor, false));
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
			for (let i = 0; i < arguments.length; i++) {
				let argument = arguments[i];
				let type = types[i];
				if (isNullable(type)) {
					if (argument == null) {
						continue;
					}
					type = removeNullable(type);
				}
				if (argument instanceof type) {
					continue;
				} else if (Object.getPrototypeOf(argument) == type.prototype) {
					continue;
				} else {
					return false;
				}
			}
			return true;
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
		function hardFreeze(type, scope = window) {
			if (type instanceof Function && type.prototype instanceof Object) {
				[type].bindTo(scope);
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
		function shareProperties(type, keys, static = false) {
			if (type instanceof Function && type.prototype instanceof Object) {
				keys.forEach((key) => {
					Object.defineProperty(static ? type : type.prototype, key, { enumerable: true, configurable: false });
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
	shareProperties(LineNodeWrapper, ['SpanNodes', 'LastSpanNode'], false);
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
		static GetColorCode(name) {
			return Console.Colors.indexOf(name);
		}
		static GetColorName(code) {
			return Console.Colors[code];
		}
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
		get ForegroundColor() {
			return this.ConsoleNode.getAttribute('foreground');
		}
		set ForegroundColor(value) {
			[value].constrainedWithAndThrow(String);
			return this.ConsoleNode.setAttribute('foreground', value);
		}
		get BackgroundColor() {
			return this.ConsoleNode.getAttribute('background');
		}
		set BackgroundColor(value) {
			[value].constrainedWithAndThrow(String);
			return this.ConsoleNode.setAttribute('background', value);
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
			let foreground = Console.GetColorCode(this.ForegroundColor);
			let background = Console.GetColorCode(this.BackgroundColor);
			let title = Console.Title;
			let Fragment = document.createDocumentFragment();
			let LineNode = this.LastLineNode.Self;
			let SpanNode = this.LastLineNode.LastSpanNode;
			let pushNode = () => {
				this.BufferNode.append(Fragment);
				Fragment = document.createDocumentFragment();
				for (let i = 0; i < this.LineNodes.length - 8192; i++) {
					this.LineNodes[i].Self.remove();
				}
				this.ForegroundColor = Console.GetColorName(foreground);
				this.BackgroundColor = Console.GetColorName(background);
				Console.Title = title;
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
		static get Title() {
			return document.title;
		}
		static set Title(value) {
			[value].constrainedWithAndThrow(String);
			document.title = value;
		}
	};
	Console.Colors[0xFF] = 'default';
	Object.freeze(Console.Colors);
	Object.freeze(Console.Themes);
	shareProperties(Console, ['LineNodes', 'LastLineNode', 'Scheme', 'ForegroundColor', 'BackgroundColor', 'writeLine', 'write', 'readLine', 'read', 'putBack', 'pushInput', 'clear', 'pressAnyKey', 'completed', 'terminated', 'bindTo'], false);
	shareProperties(Console, ['Colors', 'Themes', 'GetColorCode', 'GetColorName', 'GetColorCharCode', 'Title'], true);
	hardFreeze(Console, window);
	/* { event-dispatcher } */
	class ModuleState {
		/* { infrastructure } */
		constructor(head) {
			this.isLoaded = false;
			this.headNode = head;
			this.iostream = new Console();
			lockFields(this, ['isLoaded', 'functionList', 'abortState', 'exitCode'], true);
			lockFields(this, ['headNode', 'iostream'], false);
		}
		async structuredTag() {
			this.initStream();
			await this.fetchModule();
		}
		async formedStyle() {
			this.clearText();
			await this.monitorProcess();
		}
		startAsync() {
			(async () => {
				await suspend(() => {
					return document.readyState == 'complete';
				});
				while (true) {
					if (!this.isLoaded) {
						await this.structuredTag();
						this.isLoaded = true;
					}
					await this.formedStyle();
					await suspend();
				}
			})();
		}
		/* { processor } */
		initStream() {
			this.iostream.bindTo(this.headNode);
			this.iostream.InputNode.focus();
		}
		async fetchModule() {
			let overrides = {
				iostream: this.iostream,
				getUTF8String: this.getUTF8String.bind(this),
				getUTF16String: this.getUTF16String.bind(this),
				getUTF32String: this.getUTF32String.bind(this),
				onRuntimeInitialized: () => {
					this.abortState = false;
					this.exitCode = 0;
				},
				onAbort: () => {
					this.functionList['__Z12abort_unwindv']();
					this.abortState = true;
				},
				onExit: (code) => {
					this.exitCode = code;
				}
			};
			this.functionList = await Module(overrides);
			this.functionList.Asyncify.asyncPromiseHandlers = {
				reject: (e) => {
					if (e instanceof this.functionList['ExitStatus']) {
						this.exitCode = e.status;
					} else {
						this.abortState = true;
					}
				},
				resolve: (code) => {
					this.exitCode = code;
				}
			};
		}
		clearText() {
			this.headNode.childNodes.forEach((value) => {
				if (value.nodeName == '#text') {
					value.textContent = '';
				}
			});			
		}
		async monitorProcess() {
			if (this.functionList['keepRuntimeAlive']() == false) {
				if (!this.abortState) {
					await this.iostream.completed(this.exitCode);
				} else {
					await this.iostream.terminated();
				}
				await this.fetchModule();
			}
		}
		/* { functionality } */
		getUTFString(caller, converter, counter, width, fnScope, jsString) {
			[fnScope, jsString].constrainedWithAndThrow(Function.toNullable(), String.toNullable());
			let myScope = fnScope == null ? caller : fnScope;
			let stringObject = jsString == null ? "" : jsString;
			let stringLength = counter(stringObject) + width;
			if (!myScope.bufferSize || myScope.bufferSize < stringLength) {
				if (myScope.bufferSize) {
					this.functionList['_free'](myScope.bufferData);
				}
				myScope.bufferData = this.functionList['_malloc'](stringLength);
				myScope.bufferSize = stringLength;
			}
			converter(stringObject, myScope.bufferData, myScope.bufferSize);
			return myScope.bufferData;
		}
		getUTF8String(fnScope, jsString) {
			return this.getUTFString(this.getUTF8String, this.functionList['stringToUTF8'], this.functionList['lengthBytesUTF8'], 1, fnScope, jsString);
		}
		getUTF16String(fnScope, jsString) {
			return this.getUTFString(this.getUTF16String, this.functionList['stringToUTF16'], this.functionList['lengthBytesUTF16'], 2, fnScope, jsString);
		}
		getUTF32String(fnScope, jsString) {
			return this.getUTFString(this.getUTF32String, this.functionList['stringToUTF32'], this.functionList['lengthBytesUTF32'], 4, fnScope, jsString);
		}
	};
	shareProperties(ModuleState, ['startAsync'], false);
	hardFreeze(ModuleState, window);
	Object.defineProperty(window, 'dispatcher', getDescriptor(new ModuleState(document.body), true));
	dispatcher.startAsync();
})();
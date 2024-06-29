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
(async () => {
	/* { binder } */
	let makeDescriptor = (value, enumerable = true) => {
		return { value: value, writable: false, enumerable: enumerable, configurable: false };
	};
	let makeObject = (object, extensible, constantize) => {
		let names = Object.getOwnPropertyNames(object);
		if (object instanceof Function) {
			names = names.filter((name) => {
				return name != 'arguments' && name != 'caller';
			});
		}
		names.forEach((name) => {
			let descriptor = Object.getOwnPropertyDescriptor(object, name);
			if (constantize && descriptor.hasOwnProperty('value')) {
				descriptor.writable = false;
			}
			descriptor.configurable = false;
			Object.defineProperty(object, name, descriptor);
		});
		if (!extensible) {
			Object.preventExtensions(object);
		}
		return object;
	};
	let makeSealed = (object, extensible = false) => {
		return makeObject(object, extensible, false);
	};
	let makeFrozen = (object, extensible = false) => {
		return makeObject(object, extensible, true);
	};
	Array.prototype.bindTo = function bindTo(scope, sealed = true, protoize = false) {
		this.filter((value) => {
			return value instanceof Function;
		}).forEach((value) => {
			Object.defineProperty(scope, value['name'], makeDescriptor(value, true));
			Object.defineProperty(value, 'name', makeDescriptor(value['name'], false));
			Object.defineProperty(value, 'length', makeDescriptor(value['length'], false));
			if (value.hasOwnProperty('prototype')) {
				let prototype = value['prototype'];
				Object.defineProperty(value, 'prototype', makeDescriptor(prototype, false));
				Object.defineProperty(prototype, 'constructor', makeDescriptor(value, false));
				makeFrozen(prototype, !protoize);
			}
			makeSealed(value, sealed);
		});
	};
	[Array.prototype.bindTo].bindTo(Array.prototype);
	[makeDescriptor, makeSealed, makeFrozen].bindTo(window);
	/* { reflection } */
	class Nullable {
		constructor(type) {
			this.type = type;
			makeFrozen(this, true);
		}
	};
	[Nullable].bindTo(window);
	[
		function toNullable() {
			return new Nullable(this);
		}
	].bindTo(Function.prototype);
	[
		function isNullable(container) {
			return container instanceof Nullable && container.type instanceof Function;
		},
		function removeNullable(container) {
			let decay = container instanceof Nullable ? container.type : container;
			if (decay instanceof Function) {
				return decay;
			} else {
				throw 'The decayed type are not function types.';
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
				let decay = removeNullable(types[i]);
				if (arguments[i] === undefined) {
					throw 'Some sort of function arguments should be exist.';
				} else if (arguments[i] == null) {
					if (isNullable(types[i])) {
						continue;
					} else {
						return false;
					}
				} else if (arguments[i] instanceof decay) {
					continue;
				} else if (Object.getPrototypeOf(arguments[i]) == decay.prototype) {
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
	/* { property-definer } */
	[
		function hardFreeze(scope, types, protoize = true) {
			[scope, types, protoize].constrainedWithAndThrow(Object, Array, Boolean);
			types.bindTo(scope, true, protoize);
		},
		function lockFields(instance, keys, mutable = true) {
			[instance, keys, mutable].constrainedWithAndThrow(Object, Array, Boolean);
			keys.forEach((key) => {
				Object.defineProperty(instance, key, { writable: mutable, enumerable: !mutable, configurable: false });
			});
		},
		function shareProperties(type, keys, static = false) {
			[type, keys, static, type.prototype].constrainedWithAndThrow(Function, Array, Boolean, Object);
			keys.forEach((key) => {
				let scope = static ? type : type.prototype;
				if (Object.getOwnPropertyDescriptor(scope, key).hasOwnProperty('value')) {
					Object.defineProperty(scope, key, { writable: false, enumerable: true, configurable: false });
				} else {
					Object.defineProperty(scope, key, { enumerable: true, configurable: false });
				}
			});
		}
	].bindTo(window);
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
	/* { constructors } */
	class LineNodeWrapper {
		constructor(head) {
			[head].constrainedWithAndThrow(Element.toNullable());
			this.Self = head != null && head.nodeName == 'line'.toUpperCase() ? head : null;
			lockFields(this, ['Self'], false);
		}
		get SpanNodes() {
			return this.Self != null ? Array.from(this.Self.childNodes).filter((value) => {
				return value.nodeName == 'span'.toUpperCase();
			}) : null;
		}
		get LastSpanNode() {
			return this.Self != null && this.SpanNodes.length > 0 ? this.SpanNodes.pop() : null;
		}
	};
	class Console {
		constructor() {
			this.ConsoleNode = document.createElement('console');
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
					this.ScrollIntoBottom();
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
			this.ForegroundColor = 'gray';
			this.BackgroundColor = 'default';
			this.Scheme = 'campbell';
			this.CanType = false;
			this.istream = '';
			this.icursor = 0;
			lockFields(this, ['typing', 'freezing', 'keys', 'istream', 'icursor'], true);
			lockFields(this, ['ConsoleNode', 'BufferNode', 'ControlNode', 'DataContentNode', 'InputNode', 'ButtonNode'], false);
		}
		ClearBufferNode() {
			this.BufferNode.innerText = '';
		}
		ScrollIntoBottom() {
			this.BufferNode.scrollTo(this.BufferNode.scrollLeft, this.BufferNode.scrollHeight - this.BufferNode.clientHeight);
		}
		get CanType() {
			return this.typing;
		}
		set CanType(value) {
			this.InputNode.readOnly = !value;
			this.ButtonNode.disabled = !value;
			if (value && !this.typing) {
				this.LastLineNode.LastSpanNode?.append(this.DataContentNode);
				if (window == window.top) {
					this.InputNode.focus();
				}
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
			return this.LineNodes.length > 0 ? this.LineNodes.pop() : new LineNodeWrapper(null);
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
				this.ScrollIntoBottom();
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
				SpanNode.setAttribute('foreground', Console.GetColorName(foreground));
				SpanNode.setAttribute('background', Console.GetColorName(background));
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
				} else if (SpanNode.getAttribute('foreground') != Console.GetColorName(foreground)) {
					return true;
				} else if (SpanNode.getAttribute('background') != Console.GetColorName(background)) {
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
			this.ForegroundColor = 'gray';
			this.BackgroundColor = 'default';
			this.clear();
			await this.writeLine('');
			await this.writeLine('   The program completed with a return code ' + code.toString() + '.');
			await this.writeLine('');
			await this.writeLine('      >> Press any key to continue with restart the program . . .   ');
			await this.pressAnyKey();
			this.clear();
		}
		async terminated() {
			this.ForegroundColor = 'gray';
			this.BackgroundColor = 'default';
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
	shareProperties(LineNodeWrapper, ['SpanNodes', 'LastSpanNode'], false);
	hardFreeze(Console, [LineNodeWrapper], false);
	Console.Colors[0xFF] = 'default';
	Object.freeze(Console.Colors);
	Object.freeze(Console.Themes);
	shareProperties(Console, ['LineNodes', 'LastLineNode', 'Scheme', 'ForegroundColor', 'BackgroundColor', 'writeLine', 'write', 'readLine', 'read', 'putBack', 'pushInput', 'clear', 'pressAnyKey', 'completed', 'terminated', 'bindTo'], false);
	shareProperties(Console, ['Colors', 'Themes', 'GetColorCode', 'GetColorName', 'GetColorCharCode', 'Title'], true);
	hardFreeze(window, [Console], false);
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
		}
		async fetchModule() {
			let captured = {};
			let overrides = {
				iostream: this.iostream,
				getUTF8String: (fnScope, jsString) => {
					return this.getUTFString(captured, this.functionList['stringToUTF8'], this.functionList['lengthBytesUTF8'], 1, fnScope, jsString);
				},
				getUTF16String: (fnScope, jsString) => {
					return this.getUTFString(captured, this.functionList['stringToUTF16'], this.functionList['lengthBytesUTF16'], 2, fnScope, jsString);
				},
				getUTF32String: (fnScope, jsString) => {
					return this.getUTFString(captured, this.functionList['stringToUTF32'], this.functionList['lengthBytesUTF32'], 4, fnScope, jsString);
				},
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
		getUTFString(callerCaptured, converterCaptured, counterCaptured, sizeCaptured, fnScope, jsString) {
			[fnScope, jsString].constrainedWithAndThrow(Function.toNullable(), String.toNullable());
			let myScope = fnScope == null ? callerCaptured : fnScope;
			let stringSource = jsString == null ? "" : jsString;
			let sizeCapacity = counterCaptured(stringSource) + sizeCaptured;
			if (!myScope.bufferData || myScope.bufferSize < sizeCapacity) {
				this.functionList['_free'](myScope.bufferData);
				myScope.bufferData = this.functionList['_malloc'](sizeCapacity);
				myScope.bufferSize = sizeCapacity;
			}
			converterCaptured(stringSource, myScope.bufferData, myScope.bufferSize);
			return myScope.bufferData;
		}
	};
	shareProperties(ModuleState, ['startAsync'], false);
	hardFreeze(window, [ModuleState], false);
	Object.defineProperty(window, 'dispatcher', makeDescriptor(new ModuleState(document.body), true));
	dispatcher.startAsync();
})();
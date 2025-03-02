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
			}) : [];
		}
		get LastSpanNode() {
			let SpanNodes = this.SpanNodes;
			return SpanNodes != null && SpanNodes.length > 0 ? SpanNodes.pop() : null;
		}
	};
	class BoxNodeWrapper {
		constructor(head) {
			[head].constrainedWithAndThrow(Element.toNullable());
			this.Self = head != null && head.nodeName == 'box'.toUpperCase() ? head : null;
			lockFields(this, ['Self'], false);
		}
		get LineNodes() {
			return this.Self != null ? Array.from(this.Self.childNodes).filter((value) => {
				return value.nodeName == 'line'.toUpperCase();
			}).map((value) => {
				return new LineNodeWrapper(value);
			}) : [];
		}
		get FirstLineNode() {
			let LineNodes = this.LineNodes;
			return LineNodes.length > 0 ? LineNodes.shift() : new LineNodeWrapper(null);
		}
		get LastLineNode() {
			let LineNodes = this.LineNodes;
			return LineNodes.length > 0 ? LineNodes.pop() : new LineNodeWrapper(null);
		}
	};
	let ConsoleIntervals = [];
	setInterval(() => {
		ConsoleIntervals.forEach((value) => {
			value();
		});
	}, 500);
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
			this.InputNode.placeholder = ' > ';
			this.InputNode.addEventListener('keydown', async (e) => {
				let ModifierKeys = e.metaKey || e.ctrlKey || e.shiftKey || e.altKey;
				let Enter = !ModifierKeys && e.key == 'Enter';
				if (this.KeyFreeze) {
					this.SetReadKey(this.PopInputLine() + '\n');
				} else if (this.CanType && Enter) {
					await this.SetReadLine(this.PopInputLine());
				}
			});
			this.InputNode.addEventListener('keyup', () => {
				if (this.CanType) {
					this.DataContentNode.textContent = this.InputNode.value;
				}
				this.CanScrollIntoBottom();
			});
			this.ControlNode.append(this.InputNode);
			this.ButtonNode = document.createElement('button');
			this.ButtonNode.addEventListener('click', async () => {
				if (this.KeyFreeze) {
					this.SetReadKey(this.PopInputLine() + '\n');
				} else if (this.CanType) {
					await this.SetReadLine(this.PopInputLine());
				}
				this.CanScrollIntoBottom();
			});
			this.ControlNode.append(this.ButtonNode);
			this.ForegroundColor = 'gray';
			this.BackgroundColor = 'default';
			this.Scheme = 'campbell';
			this.CanType = false;
			this.ibuffer = [];
			this.obuffer = {
				controlized: true,
				content: '',
				scroll: false,
				characters: 32768,
				lines: 32
			};
			lockFields(this.obuffer, ['controlized', 'content', 'scroll'], true);
			lockFields(this.obuffer, ['characters', 'lines'], false);
			lockFields(this, ['typing', 'freezing', 'keys', 'ibuffer', 'obuffer'], true);
			lockFields(this, ['ConsoleNode', 'BufferNode', 'ControlNode', 'DataContentNode', 'InputNode', 'ButtonNode'], false);
			let FlushNow = 0;
			ConsoleIntervals.push(async () => {
				if (this.obuffer.scroll) {
					this.ScrollIntoBottom();
					this.obuffer.scroll = false;
				} else if (FlushNow == 50) {
					this.FlushOutStream();
				} else if (FlushNow == 100) {
					await this.out();
					FlushNow = 0;
				} else {
					FlushNow++;
				}
			});
		}
		ClearBufferNode() {
			this.BufferNode.textContent = '';
			this.obuffer.controlized = true;
			this.obuffer.content = '';
		}
		ScrollIntoBottom() {
			this.BufferNode.scrollTo(this.BufferNode.scrollLeft, this.BufferNode.scrollHeight - this.BufferNode.clientHeight);
		}
		CanScrollIntoBottom() {
			this.obuffer.scroll = true;
		}
		FlushOutStream() {
			let BoxNodes = this.BoxNodes;
			while (BoxNodes.length > this.obuffer.lines) {
				let FirstBoxNode = BoxNodes.shift();
				if (BoxNodes.length == this.obuffer.lines) {
					let LastBoxNode = BoxNodes.pop();
					let drop = LastBoxNode.LineNodes.length;
					let keep = drop <= this.obuffer.lines ? this.obuffer.lines - drop : 0;
					let LineNodes = FirstBoxNode.LineNodes;
					while (LineNodes.length > keep) {
						LineNodes.shift().remove();
					}
				} else {
					FirstBoxNode.Self.remove();
				}
			}
		}
		get CanType() {
			return this.typing;
		}
		set CanType(value) {
			this.InputNode.readOnly = !value;
			this.ButtonNode.disabled = !value;
			if (value && !this.typing) {
				let LastLineNode = this.LastLineNode;
				let LastSpanNode = LastLineNode.LastSpanNode;
				if (LastSpanNode == null) {
					LastLineNode.Self.append(this.DataContentNode);
				} else {
					LastSpanNode.append(this.DataContentNode);
				}
				if (window == window.top) {
					this.InputNode.focus();
				}
			} else if (!value && this.typing) {
				this.DataContentNode.textContent = '';
				this.DataContentNode.remove();
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
		PopInputLine() {
			let result = this.InputNode.value;
			this.InputNode.value = '';
			return result;
		}
		SetReadKey(key) {
			this.Keys = key;
			this.KeyFreeze = false;
		}
		async SetReadLine(line) {
			if (line.length > 0 && line[0] == '$') {
				let args = [];
				let temp = '';
				let state = 0;
				let push = () => {
					if (temp.length > 0) {
						args.push(temp);
						temp = '';
					}
				};
				for (let i = 1; i < line.length; i++) {
					if (line[i] == ' ' && state == 0) {
						continue;
					} else if (line[i] == ' ' && state == 1) {
						push();
						state = 0;
					} else if (line[i] == '"' && state == 0) {
						state = 2;
					} else if (line[i] == '"' && state == 1) {
						push();
						state = 2;
					} else if (line[i] == '"' && state == 2) {
						push();
						state = 0;
					} else if (state == 0) {
						temp += line[i];
						state = 1;
					} else {
						temp += line[i];
					}
				}
				push();
				if (args.length == 2 && args[0] == 'scheme') {
					this.Scheme = args[1];
				}
				let foreground = this.ForegroundColor;
				let Last2ndBoxNode = this.LastBoxNode;
				let Last2ndLineNode = this.LastLineNode;
				await this.write('\n\\f7\\ &   \\ff\\', true);
				await this.write(line, false);
				await this.out();
				let Last1stBoxNode = this.LastBoxNode;
				Last1stBoxNode.Self.append(Last2ndLineNode.Self);
				if (Last1stBoxNode.Self != Last2ndBoxNode.Self) {
					Last2ndBoxNode.Self.append(Last1stBoxNode.FirstLineNode.Self);
				}
				this.ForegroundColor = foreground;
			} else {
				if (line.length > 1 && line.substring(0, 2) == '\\$') {
					line = line.substring(1);
				}
				this.in(line);
			}
			this.CanType = false;
		}
		get BoxNodes() {
			return Array.from(this.BufferNode.childNodes).filter((value) => {
				return value.nodeName == 'box'.toUpperCase();
			}).map((value) => {
				return new BoxNodeWrapper(value);
			});
		}
		get LastBoxNode() {
			let BoxNodes = this.BoxNodes;
			return BoxNodes.length > 0 ? BoxNodes.pop() : new BoxNodeWrapper(null);
		}
		get LineNodes() {
			let LineNodes = [];
			this.BoxNodes.forEach((value) => {
				LineNodes.push(...value.LineNodes);
			});
			return LineNodes;
		}
		get LastLineNode() {
			let LastBoxNode = this.LastBoxNode;
			return LastBoxNode.Self != null ? LastBoxNode.LastLineNode : new LineNodeWrapper(null);
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
			[name].constrainedWithAndThrow(String);
			return Console.Colors.indexOf(name);
		}
		static GetColorName(code) {
			[code].constrainedWithAndThrow(Number);
			return Console.Colors[code];
		}
		static GetColorCharFromCode(code) {
			[code].constrainedWithAndThrow(Number);
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
		static GetColorCodeFromChar(code) {
			[code].constrainedWithAndThrow(Number);
			if (code >= 48 && code <= 57) {
				return code - 48;
			} else if (code >= 65 && code <= 70) {
				return code - 55;
			} else if (code >= 97 && code <= 102) {
				return code - 87;
			} else if (code == 88 || code == 120) {
				return 0xFF;
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
		back(content) {
			[content].constrainedWithAndThrow(String);
			let lines = content.split('\n');
			if (this.ibuffer.length > 0) {
				let last = lines.pop() + this.ibuffer.shift();
				this.ibuffer.unshift(last);
			}
			this.ibuffer.unshift(...lines);
		}
		in(content) {
			[content].constrainedWithAndThrow(String);
			let lines = content.split('\n');
			if (this.ibuffer.length > 0) {
				let first = this.ibuffer.pop() + lines.shift();
				this.ibuffer.push(first);
			}
			this.ibuffer.push(...lines);
		}
		async out() {
			let controlized = this.obuffer.controlized = true;
			let content = this.obuffer.content;
			let scroll = content.length > 0;
			this.obuffer.content = '';
			let lines = 0;
			let value = '';
			let config = false;
			let control = null;
			let breaking = false;
			let foreground = this.ForegroundColor;
			let background = this.BackgroundColor;
			let title = Console.Title;
			let Fragment = document.createDocumentFragment();
			let BoxNode = this.LastBoxNode;
			let LineNode = this.LastLineNode.Self;
			let SpanNode = this.LastLineNode.LastSpanNode;
			if (BoxNode.Self != null) {
				lines = BoxNode.LineNodes.length;
				if (lines > 0) {
					lines--;
				}
				if (lines >= this.obuffer.lines) {
					BoxNode = new BoxNodeWrapper(null);
					lines = 0;
				}
			}
			let pushNode = () => {
				if (Fragment.childNodes.length > 0) {
					let NewBoxNode = BoxNode.Self;
					if (NewBoxNode == null) {
						NewBoxNode = document.createElement('box');
					}
					NewBoxNode.append(Fragment);
					this.BufferNode.append(NewBoxNode);
					Fragment = document.createDocumentFragment();
				}
				BoxNode = new BoxNodeWrapper(null);
				this.ForegroundColor = foreground;
				this.BackgroundColor = background;
				Console.Title = title;
				if (scroll) {
					this.CanScrollIntoBottom();
				}
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
				SpanNode.setAttribute('foreground', foreground);
				SpanNode.setAttribute('background', background);
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
				} else if (SpanNode.getAttribute('foreground') != foreground) {
					return true;
				} else if (SpanNode.getAttribute('background') != background) {
					return true;
				} else {
					return false;
				}
			};
			let getColorName = (code) => {
				try {
					return Console.GetColorName(Console.GetColorCodeFromChar(code));
				} catch (e) {
					throwNow();
				}
			};
			let shallBreakChar = (code) => {
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
				if (content[i] == '\0') {
					if (config) {
						throwNow();
					}
					controlized = !controlized;
				} else if (content[i] == '\n') {
					if (config) {
						throwNow();
					} else if (++lines >= this.obuffer.lines) {
						pushNode();
						lines = 0;
						await suspend();
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
						}
					}
					config = !config;
					control = null;
					breaking = value == '\\';
				} else if (control != null) {
					if (control == '+t') {
						title += content[i];
					} else if (control.toLowerCase() == 't') {
						title = content[i];
						control = '+t';
					} else if (control.toLowerCase() == 'f') {
						foreground = getColorName(content.charCodeAt(i));
						control = '';
					} else if (control.toLowerCase() == 'b') {
						background = getColorName(content.charCodeAt(i));
						control = '';
					} else {
						throwNow();
					}
				} else if (config) {
					control = content[i];
				} else {
					let release = shallBreakChar(content.charCodeAt(i));
					if (breaking || release) {
						newSpan();
						breaking = release;
					}
					value += content[i];
				}
			}
			await suspend();
			if (config) {
				throwNow();
			} else {
				endUp();
			}
		}
		async read(echoing = true, unicode = true) {
			[echoing, unicode].constrainedWithAndThrow(Boolean, Boolean);
			await this.out();
			while (this.ibuffer.length == 0) {
				this.CanType = true;
				await suspend();
			}
			let first = this.ibuffer.shift();
			if (first == '') {
				return '\n';
			}
			let result = first.codePointAt(0);
			if (unicode && result > 0xFFFF) {
				result = String.fromCodePoint(result);
			} else {
				result = first.charAt(0);
			}
			first = first.substring(result.length);
			this.ibuffer.unshift(first);
			if (echoing) {
				await this.write(result, false);
			}
			return result;
		}
		async readLine(echoing = true) {
			[echoing].constrainedWithAndThrow(Boolean);
			await this.out();
			while (this.ibuffer.length == 0) {
				this.CanType = true;
				await suspend();
			}
			let result = this.ibuffer.shift();
			if (echoing) {
				await this.writeLine(result, false);
			}
			return result;
		}
		async readKey(echoing = true) {
			[echoing].constrainedWithAndThrow(Boolean);
			await this.out();
			this.KeyFreeze = true;
			while (this.KeyFreeze) {
				await suspend();
			}
			let result = this.Keys;
			if (echoing) {
				await this.write(result, false);
			}
			return result;
		}
		async write(content, controlized = true) {
			[content, controlized].constrainedWithAndThrow(String, Boolean);
			if (this.obuffer.controlized != controlized) {
				this.obuffer.controlized = !this.obuffer.controlized;
				this.obuffer.content += '\0';
			}
			this.obuffer.content += content;
			if (this.obuffer.content.length >= this.obuffer.characters) {
				await this.out();
			}
		}
		async writeLine(content, controlized = true) {
			[content, controlized].constrainedWithAndThrow(String, Boolean);
			if (this.obuffer.controlized != controlized) {
				this.obuffer.controlized = !this.obuffer.controlized;
				this.obuffer.content += '\0';
			}
			this.obuffer.content += content;
			this.obuffer.content += '\n';
			if (this.obuffer.content.length >= this.obuffer.characters) {
				await this.out();
			}
		}
		clear() {
			this.ClearBufferNode();
		}
		async completed(code = 0) {
			[code].constrainedWithAndThrow(Number);
			this.ForegroundColor = 'gray';
			this.BackgroundColor = 'default';
			this.clear();
			await this.writeLine('', false);
			await this.write('   The program is completed with a return code ', false);
			await this.write(code.toString(), false);
			await this.writeLine('.', false);
			await this.writeLine('', false);
			await this.writeLine('      >> Press any key to continue with restart the program . . .   ', false);
			await this.readKey(false);
			this.clear();
		}
		async terminated(type = '', what = '', stack = '') {
			[type, what, stack].constrainedWithAndThrow(String, String, String);
			let list = (content) => {
				return content.split('\n').map((value) => {
					return value.trim();
				}).filter((value) => {
					return value.length > 0;
				});
			};
			let types = list(type);
			let whats = list(what);
			let stacks = list(stack);
			this.ForegroundColor = 'gray';
			this.BackgroundColor = 'default';
			this.clear();
			await this.writeLine('', false);
			await this.write('   The program is terminated', false);
			if (types.length > 0) {
				await this.write(' through types < ', false);
			}
			for (let i = 0; i < types.length; i++) {
				await this.write(i == 0 ? '' : ', ', false);
				await this.write(types[i], false);
			}
			if (types.length > 0) {
				await this.write(' > ', false);
			}
			await this.writeLine('.', false);
			if (whats.length > 0) {
				await this.writeLine('', false);
			}
			for (let i = 0; i < whats.length; i++) {
				await this.write('   ~>   ', false);
				await this.writeLine(whats[i], false);
			}
			if (stacks.length > 0) {
				await this.writeLine('', false);
				await this.writeLine('   The stack trace is as follow,', false);
				await this.writeLine('', false);
			}
			for (let i = 0; i < stacks.length; i++) {
				await this.write('   ->   ', false);
				await this.writeLine(stacks[i], false);
			}
			await this.writeLine('', false);
			await this.writeLine('      >> Press any key to continue with restart the program . . .   ', false);
			await this.readKey(false);
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
	shareProperties(BoxNodeWrapper, ['LineNodes', 'LastLineNode'], false);
	hardFreeze(Console, [BoxNodeWrapper], false);
	Console.Colors[0xFF] = 'default';
	Object.freeze(Console.Colors);
	Object.freeze(Console.Themes);
	shareProperties(Console, [
		'BoxNodes',
		'LastBoxNode',
		'LineNodes',
		'LastLineNode',
		'Scheme',
		'ForegroundColor',
		'BackgroundColor',
		'back',
		'in',
		'out',
		'write',
		'writeLine',
		'read',
		'readLine',
		'readKey',
		'clear',
		'completed',
		'terminated',
		'bindTo'
	], false);
	shareProperties(Console, [
		'Colors',
		'Themes',
		'GetColorCode',
		'GetColorName',
		'GetColorCharFromCode',
		'GetColorCodeFromChar',
		'Title'
	], true);
	hardFreeze(window, [Console], false);
	/* { event-dispatcher } */
	class ModuleState {
		/* { infrastructures } */
		static SetDefaultModule(module) {
			[module].constrainedWithAndThrow(ModuleState);
			Object.defineProperty(window, 'dispatcher', makeDescriptor(module, true));
			dispatcher.startAsync();
		}
		constructor(head, fetch, alive, dispose) {
			[head, fetch, alive, dispose].constrainedWithAndThrow(Element, Function, Function, Function);
			this.isLoaded = false;
			this.headNode = head;
			this.fetchModule = fetch;
			this.runtimeAlive = alive;
			this.disposeModule = dispose;
			this.iostream = new Console();
			lockFields(this, ['isLoaded', 'exitCode', 'abortType', 'abortWhat', 'abortStack'], true);
			lockFields(this, ['headNode', 'fetchModule', 'runtimeAlive', 'disposeModule', 'iostream'], false);
		}
		async structuredTag() {
			this.initStream();
			await this.renderModule();
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
		/* { fields } */
		get ExitCode() {
			return this.exitCode;
		}
		set ExitCode(value) {
			[value].constrainedWithAndThrow(Number);
			this.exitCode = value;
		}
		get AbortType() {
			return this.abortType;
		}
		set AbortType(value) {
			[value].constrainedWithAndThrow(String);
			this.abortType = value;
		}
		get AbortWhat() {
			return this.abortWhat;
		}
		set AbortWhat(value) {
			[value].constrainedWithAndThrow(String);
			this.abortWhat = value;
		}
		get AbortStack() {
			return this.abortStack;
		}
		set AbortStack(value) {
			[value].constrainedWithAndThrow(String);
			this.abortStack = value;
		}
		get AbortState() {
			return this.abortType.length > 0 || this.abortWhat.length > 0 || this.abortStack.length > 0;
		}
		set AbortState(value) {
			[value].constrainedWithAndThrow(Boolean);
			if (!value) {
				this.abortType = '';
				this.abortWhat = '';
				this.abortStack = '';
			} else if (this.abortType.length == 0 && this.abortWhat.length == 0 && this.abortStack.length == 0) {
				this.abortType = '[ModuleState]';
			}
		}
		/* { processors } */
		initStream() {
			this.iostream.bindTo(this.headNode);
		}
		async renderModule() {
			this.ExitCode = 0;
			this.AbortState = false;
			await this.fetchModule();
		}
		clearText() {
			this.headNode.childNodes.forEach((value) => {
				if (value.nodeName == '#text') {
					value.remove();
				}
			});
		}
		async monitorProcess() {
			if (!this.runtimeAlive()) {
				await this.disposeModule();
				if (!this.AbortState) {
					await this.iostream.completed(this.ExitCode);
				} else {
					await this.iostream.terminated(this.AbortType, this.AbortWhat, this.AbortStack);
				}
				await this.renderModule();
			}
		}
	};
	shareProperties(ModuleState, ['startAsync', 'ExitCode', 'AbortType', 'AbortWhat', 'AbortStack', 'AbortState'], false);
	shareProperties(ModuleState, ['SetDefaultModule'], true);
	hardFreeze(window, [ModuleState], false);
})();
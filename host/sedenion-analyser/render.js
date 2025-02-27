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
	let functionList = null;
	let fetch = async function () {
		class UTFString {
			constructor(allocate, release, converter, counter, size) {
				[allocate, release, converter, counter, size].constrainedWithAndThrow(Function, Function, Function, Function, Number);
				this.allocate = allocate;
				this.release = release;
				this.converter = converter;
				this.counter = counter;
				this.size = size;
				lockFields(this, ['allocate', 'release', 'converter', 'counter', 'size'], false);
			}
			get(scope, content) {
				[scope, content].constrainedWithAndThrow(Function, String);
				let sizeCapacity = this.counter(content) + this.size;
				if (!scope.bufferData || scope.bufferSize < sizeCapacity) {
					this.release(scope.bufferData);
					scope.bufferData = this.allocate(sizeCapacity);
					scope.bufferSize = sizeCapacity;
				}
				this.converter(content, scope.bufferData, scope.bufferSize);
				return scope.bufferData;
			}
		}
		shareProperties(UTFString, ['get'], false);
		let ErrorList = [];
		let exit = (c) => {
			this.ExitCode = c;
		};
		let abort = (e) => {
			if (e instanceof WebAssembly.RuntimeError) {
				this.AbortType = e.name;
				this.AbortWhat = e.message;
				this.AbortStack = e.stack;
			} else if (e instanceof String || Object.getPrototypeOf(e) == String.prototype) {
				this.AbortWhat = e;
			} else if (functionList == null) {
				ErrorList.push(e);
			} else if (functionList['ExitStatus'] && e instanceof functionList['ExitStatus']) {
				this.ExitCode = e.status;
			} else if (functionList['getExceptionMessage']) {
				let pair = functionList['getExceptionMessage'](e);
				this.AbortType = pair[0];
				if (pair[1]) {
					this.AbortWhat = pair[1];
				}
			} else {
				this.AbortState = true;
			}
		};
		let quit = (c, e) => {
			exit(c);
			abort(e);
			throw e;
		};
		let overrides = {
			iostream: this.iostream,
			getUTF8String: (scope, content) => {
				return functionList['UTF8String'].get(scope, content);
			},
			getUTF16String: (scope, content) => {
				return functionList['UTF16String'].get(scope, content);
			},
			getUTF32String: (scope, content) => {
				return functionList['UTF32String'].get(scope, content);
			},
			onRuntimeInitialized: () => {
				functionList = null;
			},
			onExit: exit,
			onAbort: abort,
			quit: quit
		};
		functionList = await Module(overrides);
		functionList['UTF8String'] = new UTFString(functionList['_malloc'], functionList['_free'], functionList['stringToUTF8'], functionList['lengthBytesUTF8'], 1);
		functionList['UTF16String'] = new UTFString(functionList['_malloc'], functionList['_free'], functionList['stringToUTF16'], functionList['lengthBytesUTF16'], 2);
		functionList['UTF32String'] = new UTFString(functionList['_malloc'], functionList['_free'], functionList['stringToUTF32'], functionList['lengthBytesUTF32'], 4);
		functionList.Asyncify.asyncPromiseHandlers = {
			resolve: exit,
			reject: abort
		};
		while (ErrorList.length > 0) {
			abort(ErrorList.shift());
		}
	};
	let alive = function () {
		if (this.AbortState) { return false; }
		return functionList['keepRuntimeAlive']();
	};
	while (!window['ModuleState']) {
		await suspend();
	}
	ModuleState.SetDefaultParams(document.body, fetch, alive);
})();
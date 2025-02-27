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
	let functions = {};
	let fetch = async (ths) => {
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
		let exit = (c) => {
			ths.ExitCode = c;
		};
		let abort = (e) => {
			if (e instanceof WebAssembly.RuntimeError) {
				ths.AbortType = e.name;
				ths.AbortWhat = e.message;
				ths.AbortStack = e.stack;
			} else if (functions['ExitStatus'] && e instanceof functions['ExitStatus']) {
				ths.ExitCode = e.status;
			} else if (functions['getExceptionMessage']) {
				let pair = functions['getExceptionMessage'](e);
				ths.AbortType = pair[0];
				if (pair[1]) {
					ths.AbortWhat = pair[1];
				}
			} else {
				ths.AbortState = true;
			}
		};
		let quit = (c, e) => {
			exit(c);
			abort(e);
			throw e;
		};
		let overrides = {
			iostream: ths.iostream,
			getUTF8String: (scope, content) => {
				return functions['UTF8String'].get(scope, content);
			},
			getUTF16String: (scope, content) => {
				return functions['UTF16String'].get(scope, content);
			},
			getUTF32String: (scope, content) => {
				return functions['UTF32String'].get(scope, content);
			},
			onExit: exit,
			onAbort: abort,
			quit: quit
		};
		functions = await Module(overrides);
		functions['UTF8String'] = new UTFString(functions['_malloc'], functions['_free'], functions['stringToUTF8'], functions['lengthBytesUTF8'], 1);
		functions['UTF16String'] = new UTFString(functions['_malloc'], functions['_free'], functions['stringToUTF16'], functions['lengthBytesUTF16'], 2);
		functions['UTF32String'] = new UTFString(functions['_malloc'], functions['_free'], functions['stringToUTF32'], functions['lengthBytesUTF32'], 4);
		functions.Asyncify.asyncPromiseHandlers = {
			resolve: exit,
			reject: abort
		};
	};
	let alive = (ths) => {
		if (ths.AbortState) { return false; }
		return functions['keepRuntimeAlive']();
	};
	while (!window['ModuleState']) {
		await suspend();
	}
	ModuleState.SetDefaultParams(document.body, fetch, alive);
})();
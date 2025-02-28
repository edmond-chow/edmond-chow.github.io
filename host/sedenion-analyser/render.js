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
	/* { string-conversions } */
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
	};
	shareProperties(UTFString, ['get'], false);
	/* { wasm-module } */
	while (!window['ModuleState']) {
		await suspend();
	}
	class WasmModuleState extends ModuleState {
		constructor(head) {
			let fetch = async () => {
				await this.fetch();
			};
			let alive = () => {
				return this.alive();
			};
			let dispose = () => {
				this.dispose();
			};
			super(head, fetch, alive, dispose);
			this.fn = null;
			this.er = [];
			lockFields(this, ['fn'], true);
			lockFields(this, ['er'], false);
		}
		/* { event-handlers } */
		exit(c) {
			this.ExitCode = c;
		}
		abort(e) {
			if (e instanceof WebAssembly.RuntimeError) {
				this.AbortType = e.name;
				this.AbortWhat = e.message;
				this.AbortStack = e.stack;
			} else if (e instanceof String || Object.getPrototypeOf(e) == String.prototype) {
				this.AbortWhat = e;
			} else if (this.fn == null) {
				this.er.push(e);
			} else if (this.fn['ExitStatus'] && e instanceof this.fn['ExitStatus']) {
				this.ExitCode = e.status;
			} else if (this.fn['getExceptionMessage']) {
				let pair = this.fn['getExceptionMessage'](e);
				this.AbortType = pair[0];
				if (pair[1]) {
					this.AbortWhat = pair[1];
				}
			} else {
				this.AbortState = true;
			}
		}
		quit(c, e) {
			this.exit(c);
			this.abort(e);
			throw e;
		}
		/* { method-injections } */
		async fetch() {
			let exit = (c) => {
				this.exit(c);
			};
			let abort = (e) => {
				this.abort(e);
			};
			let quit = (c, e) => {
				this.quit(c, e);
			};
			let overrides = {
				iostream: this.iostream,
				getUTF8String: (scope, content) => {
					return this.fn['UTF8String'].get(scope, content);
				},
				getUTF16String: (scope, content) => {
					return this.fn['UTF16String'].get(scope, content);
				},
				getUTF32String: (scope, content) => {
					return this.fn['UTF32String'].get(scope, content);
				},
				onRuntimeInitialized: () => {
					this.fn = null;
				},
				onExit: exit,
				onAbort: abort,
				quit: quit
			};
			this.fn = await Module(overrides);
			this.fn['UTF8String'] = new UTFString(this.fn['_malloc'], this.fn['_free'], this.fn['stringToUTF8'], this.fn['lengthBytesUTF8'], 1);
			this.fn['UTF16String'] = new UTFString(this.fn['_malloc'], this.fn['_free'], this.fn['stringToUTF16'], this.fn['lengthBytesUTF16'], 2);
			this.fn['UTF32String'] = new UTFString(this.fn['_malloc'], this.fn['_free'], this.fn['stringToUTF32'], this.fn['lengthBytesUTF32'], 4);
			this.fn.Asyncify.asyncPromiseHandlers = {
				resolve: exit,
				reject: abort
			};
			while (this.er.length > 0) {
				this.abort(this.er.shift());
			}
		}
		alive() {
			if (this.AbortState) { return false; }
			return this.fn['keepRuntimeAlive']();
		}
		dispose() {
			try {
				this.fn['exitJS'](this.ExitCode);
			} catch (e) {
				this.abort(e);
			}
		}
	};
	ModuleState.SetDefaultModule(new WasmModuleState(document.body));
})();
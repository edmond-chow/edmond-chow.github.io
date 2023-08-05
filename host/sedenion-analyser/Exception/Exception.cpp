#include <emscripten.h>
EM_JS(void, throw_with_branches_impl, (), {
	console.error('[std::exception]', 'The branch should ensure not instantiated at compile time.');
});
EM_JS(void, throw_with_invalid_argument_impl, (), {
	console.error('[std::invalid_argument]', '"size" must be a number which is 2 to the power of a natural number.');
});
void throw_with_branches()
{
	throw_with_branches_impl();
};
void throw_with_invalid_argument()
{
	throw_with_invalid_argument_impl();
};

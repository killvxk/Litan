# Define
Defines are used to declare global constants. Function calls are not allowed in defines. Global variables cannot be used.

```javascript
namespace my_namespace {
	def foo = "Hello World";
	def bar = 42;
}

function main() {
	std::println(my_namespace::foo);
	std::println(my_namespace::bar);
}
```

The output is
```
Hello World
42
```
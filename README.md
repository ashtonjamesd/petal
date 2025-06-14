# Petal

Petal is a lightweight, extensible tokenizer (lexer) framework written in C. It is designed to tokenize source strings into categories for later processing such as parsing, interpreting, or compiling. 

Petal is flexible enough to support different languages or syntax patterns by allowing you to configure recognized token types.

## Usage

### Load Source from File

```c
Petal *petal = init_with_file("input.txt");
```

### Load Source Directly

```c
Petal *petal = init_with_string(">>>++--<<+---.");
```


### Configure the Tokenizer

```c
// tells the tokenizer to recognize identifiers as valid tokens
with_identifiers(petal);

with_integers(petal);
with_strings(petal);

// will ignore whitespace characters in the input source
skip_whitespace(petal);

// register recognized keywords
with_keywords(petal,
    "true", "false", "null"
);

// register recognized symbols
with_symbols(petal,
    "+", "-", "*", "/"
);

// performs the tokenization
parse(petal);
```


### Built-in Configurations

```c
petal_configure_json(petal);       // for json tokenizing
petal_configure_brainfuck(petal);  // for brainfuck language
petal_configure_math(petal);       // for math expressions
```


### Token Inspection

```c
petal_inspect(petal); // outputs the generated tokens
```


### Cleaning up

Ensure to clean up after you have finished using the tokens.

```c
petal_free(petal);
```


### Full Example

```c
Petal *petal = init_with_string("int main() { return 0; }");
with_identifiers(petal);
with_integers(petal);
skip_whitespace(petal);

with_keywords(petal, 
    "int", "main", "return"
);

with_symbols(petal, 
    "(", ")", "{", "}", ";"
);

parse(petal);
petal_inspect(petal);
petal_free(petal);
```
# BF Interpreter in C

A simple [BF](https://en.wikipedia.org/wiki/Brainfuck) interpreter written in C.

It's not the most efficient nor fast. It's just for practice.

## Features

- Parse and execute BF code.
- Support for all vanilla BF keys: `+`, `-`, `>`, `<`, `[`, `]`, `.`, `,` .
- Supports passing BF files to be compiled.

## Getting Started

### Installation

Clone the repository:

```bash
git clone https://github.com/M0hanad1/bf-interpreter.git
cd bf-interpreter
```

Or download the executable from the [latest release](https://github.com/M0hanad1/bf-interpreter/releases/latest).

### Usage

Using make:

```bash
make
./bin/bf <bf_file>
```

Using the executable

```bash
./bf <bf_file>
```

#### Example

```brainfuck
++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.
```

You can run the above Hello World program by saving it to a file and running:

```bash
./bf hello_world.bf
```

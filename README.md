# Abel on C

```
The MIT License

Copyright © 2024 Matt Wang

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the “Software”),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

## Containers, JSON Parsers, and Beyond

Abel-on-C is the C version of Abel library orginall written in C++.
Abel library provides Python-like dictionary and list containers that are capable of storing *heterogeneous* data types.

An immediate use case of Abel is parsing a `JSON`-formatted file and loading it as an (Abel) container. The standard datatypes that `JSON` accepts, i.e.
`Null`, `Bool`, `Double`, and string are part of Abel instrinsic types.

Beyond `JSON`, Abel introduces a more relaxed `JSON`-based format,
accepting comments and more datatypes.

## Accepted types

Current version (0.0.1) of Abel-on-C accepts the following datatypes.

- Null `Null`

- Bool `Bool` (`bool`)

- Double `double`

- String `astring`

- Integer `int` (To be fully implemented)

- Binary `Binary` (Planned)

- Bitstring `Bitstring` (Planned)

## Container types

Current version (0.0.1) provides two containers.

- Dictionary `abel_dict`

  Dictionary stores a key-value pair. The key must be a `string` and
  the value can be any legitimate type.

- List `abel_list`

  A vector-like container that accepts any legitimate type as its element.

### Parsing and loading JSON

Examples on how to use Abel-on-C can be found in [this](/examples/) directory.

## Version information

- Version: 0.0.1

- Updated: 29 June 2024.

- Compile: gcc 11.4.0 on Ubuntu 22.04

## Compile

To compile with Abel library, the only required header is `abelc.h`.
Proper use of this library doesn't cause memory leak. Tested using
Valgrind.

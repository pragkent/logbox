# Logbox
[![Build Status](https://travis-ci.org/pragkent/logbox.svg?branch=master)](https://travis-ci.org/pragkent/logbox)

Logbox is a pragmatic logging library for C++.
## Introduction
It could be used like this:

```cpp
#include <logbox/logging.h>

int main(int argc, char* argv[]) {
  logbox::InitializeLogging();

  LOG(DEBUG) << "Hello world!";

  return 0;
}
```

## Logging Style
Logbox supports three different styles of logging:

* Stream Style:
```cpp
   LOG(DEBUG) << "This is a cool debug log";
```

* Format Style:
```cpp
   LOG(INFO).Write("We find {} cookies", num_of_cookies);
```

* Printf Style
```cpp
   LOG(ERROR).Printf("We can use printf! %s", "Yeah!");
```

## Conditional Logging
You can do conditional logging this way:
```cpp
   LOG_IF(DEBUG, num > 10) << "Too many cookies to eat!";
   LOG_IF(INFO, num > 10).Write("We got {} cookies!", num);
```

## Format String Syntax
Logbox use `cppformat`(https://github.com/cppformat/cppformat) library for
formatting. You can check format string syntax here:
http://cppformat.readthedocs.org/en/latest/syntax.html

## Install
```bash
git clone https://github.com/pragkent/logbox.git
cd logbox
./build.sh
```

## Acknowledgments
Thanks `Victor Zverovich`(https://github.com/vitaut) for `cppformat`(https://github.com/cppformat/cppformat).

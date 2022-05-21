[![NPM version][npm-image]][npm-url]
[![Build Status][travis-image]][travis-url]

# postal-async

Async NAPI based interface for [libpostal]

## Install

```sh
$ npm install --save postal-async
```

## Usage

```js
const { parse, expand } = require('postal-async');


expand('781 Franklin Ave Crown Hts Brooklyn NY', function(err, expansions) {
  // expansion is an array of expanded addresses
  assert(expansions.includes('781 franklin avenue crown heights brooklyn new york'));
});

parse('781 Franklin Ave Crown Hts Brooklyn NY', function(err, parsed) {
  // parsed is an object with house_number, street etc.
  assert(parsed.find(({ value, label }) => label === 'house_number' && value === '781'));
});

```

## License

MIT © [Damian Krzeminski](https://pirxpilot.me)

[npm-image]: https://img.shields.io/npm/v/postal-async.svg
[npm-url]: https://npmjs.org/package/postal-async

[travis-url]: https://app.travis-ci.com/github/pirxpilot/postal-async
[travis-image]: https://img.shields.io/travis/com/pirxpilot/postal-async.svg

[libpostal]: https://github.com/openvenues/libpostal

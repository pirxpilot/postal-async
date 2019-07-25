[![NPM version][npm-image]][npm-url]
[![Build Status][travis-image]][travis-url]
[![Dependency Status][deps-image]][deps-url]
[![Dev Dependency Status][deps-dev-image]][deps-dev-url]

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
  assert(parsed.house_number === 781);
});

```

## License

MIT © [Damian Krzeminski](https://pirxpilot.me)

[npm-image]: https://img.shields.io/npm/v/postal-async.svg
[npm-url]: https://npmjs.org/package/postal-async

[travis-url]: https://travis-ci.org/pirxpilot/postal-async
[travis-image]: https://img.shields.io/travis/pirxpilot/postal-async.svg

[deps-image]: https://img.shields.io/david/pirxpilot/postal-async.svg
[deps-url]: https://david-dm.org/pirxpilot/postal-async

[deps-dev-image]: https://img.shields.io/david/dev/pirxpilot/postal-async.svg
[deps-dev-url]: https://david-dm.org/pirxpilot/postal-async?type=dev

[libpostal]: https://github.com/openvenues/libpostal

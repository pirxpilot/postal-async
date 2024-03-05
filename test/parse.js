const test = require('node:test');
const assert = require('node:assert/strict');

const { parse } = require('../');

function toObject(parsed) {
  return Object.fromEntries(parsed.map(({ label, value }) => [label, value]));
}

test('should parse a US address', function (_, done) {
  parse('781 Franklin Ave Crown Heights Brooklyn NYC NY 11216 USA', function (err, parsed) {
    assert.ifError(err);
    assert.deepEqual(toObject(parsed), {
      house_number: '781',
      road: 'franklin ave',
      suburb: 'crown heights',
      city_district: 'brooklyn',
      city: 'nyc',
      state: 'ny',
      postcode: '11216',
      country: 'usa'
    });
    done();
  });
});

test('should parse a Polish address', function (_, done) {
  parse('ul. Łopuszańska 38D 02-232 Warszawa Polska', function (err, parsed) {
    assert.ifError(err);
    assert.deepEqual(toObject(parsed), {
      house_number: '38d',
      road: 'ul. łopuszańska',
      city: 'warszawa',
      postcode: '02-232',
      country: 'polska'
    });
    done();
  });
});

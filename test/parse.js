const test = require('tape');
const { parse } = require('../');


function toObject(parsed) {
  const r = {};
  parsed.forEach(({ label, value }) => r[label] = value);
  return r;
}

test('should parse a US address', function(t) {
  parse('781 Franklin Ave Crown Heights Brooklyn NYC NY 11216 USA', function(err, parsed) {
    t.error(err);
    t.same(toObject(parsed), {
      house_number: '781',
      road: 'franklin ave',
      suburb: 'crown heights',
      city_district: 'brooklyn',
      city: 'nyc',
      state: 'ny',
      postcode: '11216',
      country: 'usa'
    });
    t.end();
  });
});

test('should parse a Polish address', function(t) {
  parse('ul. Łopuszańska 38D 02-232 Warszawa Polska', function(err, parsed) {
    t.error(err);
    t.same(toObject(parsed), {
      house_number: '38d',
      road: 'ul. łopuszańska',
      city: 'warszawa',
      postcode: '02-232',
      country: 'polska'
    });
    t.end();
  });
});

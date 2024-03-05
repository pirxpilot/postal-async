const test = require('node:test');
const assert = require('node:assert/strict');

const { expand } = require('../');

test('should correctly expand a US address', function (_, done) {
  expand('781 Franklin Ave Crown Hts Brooklyn NY', function (err, expansions) {
    assert.ifError(err);
    assert.ok(expansions.includes('781 franklin avenue crown heights brooklyn new york'));
    done();
  });
});

test('should correctly dedupe equivalent addresses', function (_, done) {
  const opts = { languages: ['en'] };

  expand('30 West Twenty-sixth Street Floor Number 7', opts, function (err, expansions1) {
    assert.ifError(err);
    expand('Thirty W 26th St Fl #7', opts, function (err, expansions2) {
      const intersection = expansions1.filter(item => expansions2.includes(item));
      assert.ok(intersection.length > 0);
      done();
    });
  });
});

test('should handle non-ASCII addresses correctly from Node', function (_, done) {
  expand('Friedrichstraße 128, Berlin, Germany', function (err, expansions) {
    assert.ifError(err);
    assert.ok(expansions.includes('friedrich strasse 128 berlin germany'));
    done();
  });
});

test('should transliterate', function (_, done) {
  expand('ул Каретный Ряд, д 4, строение 7', function (err, expansions) {
    assert.ifError(err);
    assert.ok(expansions.includes('ulitsa karetnyy ryad dom 4 stroyeniye 7'));
    done();
  });
});

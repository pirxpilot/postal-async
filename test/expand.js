const test = require('tape');
const { expand } = require('../');

test('should correctly expand a US address', function(t) {
  expand('781 Franklin Ave Crown Hts Brooklyn NY', function(err, expansions) {
    t.error(err);
    t.ok(expansions.includes('781 franklin avenue crown heights brooklyn new york'));
    t.end();
  });
});

test('should correctly dedupe equivalent addresses', function(t) {
  const opts = { languages: ['en'] };

  expand('30 West Twenty-sixth Street Floor Number 7', opts, function(err, expansions1) {
    t.error(err);
    expand('Thirty W 26th St Fl #7', opts, function(err, expansions2) {
      const intersection = expansions1.filter(item => expansions2.includes(item));
      t.ok(intersection.length > 0);
      t.end();
    });
  });
});

test('should handle non-ASCII addresses correctly from Node', function(t) {
  expand('Friedrichstraße 128, Berlin, Germany', function(err, expansions) {
    t.error(err);
    t.ok(expansions.includes('friedrich strasse 128 berlin germany'));
    t.end();
  });
});

test('should transliterate', function(t) {
  expand('ул Каретный Ряд, д 4, строение 7', function(err, expansions) {
    t.error(err);
    t.ok(expansions.includes('ulitsa karetnyy ryad dom 4 stroyeniye 7'));
    t.end();
  });
});
